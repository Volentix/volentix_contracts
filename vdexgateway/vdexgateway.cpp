#include "vdexgateway.hpp"
#include "../vdexdposvote/vdexdposvote.hpp"

void vdexgateway::regaccount(name account) 
{
	require_auth(account);
	assign_addresses(account);
}

void vdexgateway::assign_addresses(name account)
{
	auto iter = currencies.begin();
	while (iter != currencies.end()) {
		if (iter->is_active == true) {
			assign_address(account, iter->currency_symbol);
		}
	}
}

void vdexgateway::assign_address(name account, symbol currency_symbol)
{
	deposit_addresses dep_addrs(get_self(), currency_symbol.code().raw());
	accounts_info acnt_info(get_self(), currency_symbol.code().raw());
	auto dep_itr = dep_addrs.find(account.value);
	auto acnt_itr = acnt_info.begin();
	check(acnt_itr == acnt_info.end(), "Address has already assigned for that account.");
	check(dep_itr != dep_addrs.end(), "No free deposit address.");
	// asign deposit address
	acnt_info.emplace(account, [&](auto& row){
        row.account = account;
        row.deposit_address = dep_itr->address;
        row.balance = asset(0, currency_symbol);
      });
	// delete assigned address from free addresses list
	dep_addrs.erase(dep_itr);
}

void vdexgateway::addaddresses(string currency_name, uint8_t currency_precision,	const vector<string> &addresses)
{
	require_auth(VOLENTIX_ORACLE);
	auto currency_symbol = symbol(symbol_code(currency_name), currency_precision);
	auto itr = currencies.find(currency_symbol.code().raw());
	deposit_addresses dep_addrs(get_self(), currency_symbol.code().raw());
	check(itr != currencies.end(), "Unknown currency.");
	for (auto const &address : addresses) {
	  dep_addrs.emplace(VOLENTIX_ORACLE, [&](auto& row){ 
	  	row.id = dep_addrs.available_primary_key();
	  	row.address = address;
		});
	}
}

void vdexgateway::addcurrency(string currency_name, 
								  						uint8_t currency_precision,
								  						uint16_t deposit_confirmations,
								  						uint16_t withdraw_confirmations,
								  						name colateral_token,
								  						asset withdraw_reward)
{
	require_auth(get_self());
	auto currency_symbol = symbol(symbol_code(currency_name), currency_precision);
	auto itr = currencies.find(currency_symbol.code().raw());
	check(itr == currencies.end(), "Currency already exists.");
	currencies.emplace(get_self(), [&](auto& row){ 
		row.currency_symbol = currency_symbol;
		row.is_active = true;
		row.deposit_confirmations = deposit_confirmations;
		row.withdraw_confirmations = withdraw_confirmations;
		row.colateral_token = colateral_token;
		row.withdraw_reward = withdraw_reward;
	});

}

// depoist amount can be set by Dapp using volentix node or one of producres nodes
// deposit amount will be obtained from blockchain using tx_hash 
void vdexgateway::submitdep(name account, asset amount, checksum256 tx_hash)
{
	require_auth(account);
	//TODO check amount precision
	require_recipient(account);
	account_deposits acnt_deps(get_self(), account.value);
	auto hash_index = acnt_deps.get_index<name("bytxhash")>();
	auto itr = hash_index.find(tx_hash);
	check(itr == hash_index.end(), "Deposit with tha same tx_hash already exists.");
	acnt_deps.emplace(account, [&](auto& row){
		row.id = acnt_deps.available_primary_key();
		row.amount = amount;
		row.tx_hash = tx_hash;
		row.is_confirmed = false;
		row.is_claimed = false;
		row.nodes_confirmed = 0;
	});
	// TODO: check limits
}


void vdexgateway::confirmdep(name node, name account, checksum256 tx_hash)
{
	require_auth(node);
	// check whether deposit already confirmed by node
	confirm_by_hash(node, tx_hash);
	// check permissions
	check(has_rank(node, RANK), "You don't have permission to confirm deposits.");
	// confirm deposit
	account_deposits acnt_deps(get_self(), account.value);
	auto deps_hash_index = acnt_deps.get_index<name("bytxhash")>();
	auto itr_dep = deps_hash_index.find(tx_hash);
	check(itr_dep != deps_hash_index.end(), "Deposit with that tx_hash doesn't exist.");
	if (!itr_dep->is_confirmed) {
		auto itr_curr = currencies.find(itr_dep->amount.symbol.code().raw());
		bool is_confirmed = itr_dep->nodes_confirmed + 1 == itr_curr->deposit_confirmations ? true : false;
		deps_hash_index.modify(itr_dep, node, [&]( auto& row ) {
			row.is_confirmed = is_confirmed;
			row.nodes_confirmed += 1;
		});
	}
}

void vdexgateway::confirm_by_hash(name node, checksum256 tx_hash)
{
	confirmations confrms(get_self(), node.value);
	auto confrm_hash_index = confrms.get_index<name("bytxhash")>();
	auto itr_confrm = confrm_hash_index.find(tx_hash);
	check(itr_confrm == confrm_hash_index.end(), "Transaction has already confirmed.");
	confrms.emplace(node, [&](auto& row){
		row.id = confrms.available_primary_key();
		row.tx_hash = tx_hash;
	});
}

bool vdexgateway::has_rank(name node, int rank)
{
	auto node_rank = vdexdposvote::get_rank(VOTING_CONTRACT, node);
	return node_rank < rank ? true : false;
}

void vdexgateway::claimdep(name account, checksum256 tx_hash)
{
	require_auth(account);
	account_deposits acnt_deps(get_self(), account.value);
	auto hash_index = acnt_deps.get_index<name("bytxhash")>();
	auto itr = hash_index.find(tx_hash);
	check(itr != hash_index.end(), "Deposit with that tx_hash doesn't exist.");
	check(itr->is_claimed == false, "Deposit has already claimed.");
	hash_index.modify(itr, account, [&]( auto& row ) {
		row.is_claimed = true;
	});
	auto itr_curr = currencies.find(itr->amount.symbol.code().raw());
	check(itr_curr != currencies.end(), "Unknown currency.");
	action(
  	{ get_self(), name("active") }, 
  	itr_curr->colateral_token,
  	name("transfer"), 
  	std::make_tuple(account, itr->amount, "memo") // TODO: add deposit tx_hash to memo
  ).send();
  // TODO: erase deposit record when tx_hash will be added to memo
  // TODO: erase confirmations
}

void vdexgateway::withdraw(name account, asset amount)
{
	require_auth(account);
	accounts_info acnt_info(get_self(), amount.symbol.code().raw());
	auto itr_acc = acnt_info.find(account.value);
	check(itr_acc != acnt_info.end(), "Account doesn't have balance.");
	check(itr_acc->balance.symbol == amount.symbol, "symbol precision mismatch");
	check(itr_acc->balance.amount >= amount.amount, "Account doesn't have enought funds.");
	// sub balance
  acnt_info.modify(itr_acc, account, [&]( auto& row ) {
		row.balance -= amount;
	});
	// create withdraw request
	account_withdrawals acnt_withd(get_self(), account.value);
	// сгенерить айдишник и создать вывод
	auto withd_id = acnt_withd.available_primary_key();
	acnt_withd.emplace(account, [&](auto& row){
		row.id = withd_id;
		row.amount = amount;
   	row.tx_hash = checksum256(); // zero hash
   	row.is_confirmed = false;
   	row.is_rewarded = false;
   	row.proccesed_by = NULL_NAME;
   	row.nodes_confirmed = 0;
	});

	// notify network about new withdraw id
	action(
  	{ get_self(), name("active") },
  	get_self(),
  	name("newwithdraw"), 
  	std::make_tuple(account, amount, withd_id)
  ).send();
}

void vdexgateway::newwithdraw(name account, asset amount, uint64_t id)
{
	require_auth(get_self());
}

void vdexgateway::lockwithdraw(name node, name account, uint64_t withd_id)
{
	require_auth(node);
	account_withdrawals acnt_withd(get_self(), account.value);
	auto itr = acnt_withd.find(withd_id);
	check(itr != acnt_withd.end(), "Withdraw doesn't exist.");
	check(itr->proccesed_by != NULL_NAME, "WIthdraw has already locked.");
  acnt_withd.modify(itr, node, [&]( auto& row ) {
		row.proccesed_by = node;
	});
}

// submit withdraw tx hash
void vdexgateway::submitwithd(name node, name account, uint64_t withd_id, checksum256 tx_hash)
{
	require_auth(node);
	account_withdrawals acnt_withd(get_self(), account.value);
	auto itr = acnt_withd.find(withd_id);
	check(itr != acnt_withd.end(), "Withdraw doesn't exist.");
	check(itr->proccesed_by == node, "Missing permission to modify withdraw record.");
  acnt_withd.modify(itr, node, [&]( auto& row ) {
		row.tx_hash = tx_hash;
	});	
}

// confirm withdraw by tx hash
void vdexgateway::confirmwithd(name node, name account, checksum256 tx_hash)
{
	require_auth(node);
	confirm_by_hash(node, tx_hash);
		// check permissions
	check(has_rank(node, RANK), "You don't have permission to confirm withdrawals.");
	// confirm withdraw
	account_withdrawals acnt_withd(get_self(), account.value);
	auto withd_hash_index = acnt_withd.get_index<name("bytxhash")>();
	auto itr_withd = withd_hash_index.find(tx_hash);
	check(itr_withd != withd_hash_index.end(), "Withdrawal with that tx_hash doesn't exist.");
	if (!itr_withd->is_confirmed) {
		auto itr_curr = currencies.find(itr_withd->amount.symbol.code().raw());
		bool is_confirmed = itr_withd->nodes_confirmed + 1 == itr_curr->withdraw_confirmations ? true : false;
		withd_hash_index.modify(itr_withd, node, [&]( auto& row ) {
			row.is_confirmed = is_confirmed;
			row.nodes_confirmed += 1;
		});
	}
} // TODO: unify deposit and withdraw confirmation

void vdexgateway::getreward(name node, name account, checksum256 tx_hash)
{
	require_auth(node);
	account_withdrawals acnt_withd(get_self(), account.value);
	auto hash_index = acnt_withd.get_index<name("bytxhash")>();
	auto itr = hash_index.find(tx_hash);
	check(itr != hash_index.end(), "Withdrawal with that tx_hash doesn't exist.");
	check(itr->is_rewarded == false, "Already rewarded.");
	hash_index.modify(itr, node, [&]( auto& row ) {
		row.is_rewarded = true;
	});

	auto itr_curr = currencies.find(itr->amount.symbol.code().raw());
	check(itr_curr != currencies.end(), "Unknown currency.");
	action(
  	{ get_self(), name("active") }, 
  	itr_curr->colateral_token,
  	name("transfer"), 
  	std::make_tuple(account, itr->amount + itr_curr->withdraw_reward, "memo") // TODO: add withdraw tx_hash to memo
  ).send();
  // TODO: erase withdraw record when tx_hash will be added to memo
  // TODO: erase confirmations
}

EOSIO_DISPATCH(vdexgateway, (regaccount)(addaddresses)(addcurrency)(submitdep)(confirmdep)(claimdep)(withdraw)(newwithdraw)(lockwithdraw)(submitwithd)(confirmwithd)(getreward))