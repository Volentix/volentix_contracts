#include <volentixstak.hpp>

#define SENDER_ID(X, Y) (((uint128_t)X << 64) | Y)

#define MIN_STAKE_AMOUNT asset(10000000, symbol("TEST", 4))
#define MAX_STAKE_AMOUNT asset(100000000000, symbol("TEST", 4))
#define MIN_STAKE_PERIOD 30
#define MAX_STAKE_PERIOD 300
#define STAKE_MULTIPLE_PERIOD 30

void volentixstak::create(name issuer,
                          asset maximum_supply)
{
   require_auth(_self);

   auto sym = maximum_supply.symbol;
   check(sym.is_valid(), "invalid symbol name");
   check(maximum_supply.is_valid(), "invalid supply");
   check(maximum_supply.amount > 0, "max-supply must be positive");

   stats statstable(_self, sym.code().raw());
   auto existing = statstable.find(sym.code().raw());
   check(existing == statstable.end(), "token with symbol already exists");

   statstable.emplace(_self, [&](auto &s) {
      s.supply.symbol = maximum_supply.symbol;
      s.max_supply = maximum_supply;
      s.issuer = issuer;
   });
}

void volentixstak::issue(name to, asset quantity, string memo)
{
   auto sym = quantity.symbol;
   check(sym.is_valid(), "invalid symbol name");
   check(memo.size() <= 256, "memo has more than 256 bytes");

   stats statstable(_self, sym.code().raw());
   auto existing = statstable.find(sym.code().raw());
   check(existing != statstable.end(), "token with symbol does not exist, create token before issue");
   const auto &st = *existing;

   require_auth(st.issuer);
   check(quantity.is_valid(), "invalid quantity");
   check(quantity.amount > 0, "must issue positive quantity");

   check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
   check(quantity.amount <= st.max_supply.amount - st.supply.amount, "quantity exceeds available supply");

   statstable.modify(st, same_payer, [&](auto &s) {
      s.supply += quantity;
   });

   add_balance(st.issuer, quantity, st.issuer);

   if (to != st.issuer)
   {
      SEND_INLINE_ACTION(*this, transfer, {{st.issuer, "active"_n}},
                         {st.issuer, to, quantity, memo});
   }
}

void volentixstak::retire(asset quantity, string memo)
{
   auto sym = quantity.symbol;
   check(sym.is_valid(), "invalid symbol name");
   check(memo.size() <= 256, "memo has more than 256 bytes");

   stats statstable(_self, sym.code().raw());
   auto existing = statstable.find(sym.code().raw());
   check(existing != statstable.end(), "token with symbol does not exist");
   const auto &st = *existing;

   require_auth(st.issuer);
   check(quantity.is_valid(), "invalid quantity");
   check(quantity.amount > 0, "must retire positive quantity");

   check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");

   statstable.modify(st, same_payer, [&](auto &s) {
      s.supply -= quantity;
   });

   sub_balance(st.issuer, quantity);
}

void volentixstak::transfer(name from,
                            name to,
                            asset quantity,
                            string memo)
{
   check(from != to, "cannot transfer to self");
   require_auth(from);
   check(is_account(to), "to account does not exist");
   check_symbol(quantity);

   require_recipient(from);
   require_recipient(to);

   check(memo.size() <= 256, "memo has more than 256 bytes");

   auto payer = has_auth(to) ? to : from;

   // default transfer
   sub_balance(from, quantity);
   add_balance(to, quantity, payer);
}

//##### STAKE ON GLOBAL STATE
// void volentixstak ::stake(name owner, const asset quantity, uint16_t stake_period)
// {
//    require_auth(owner);
//    check_symbol(quantity);

//    uint64_t amount = quantity.amount / 10000;

//    //##### stake amount checks
//    check(amount >= MIN_STAKE_AMOUNT, "stake amount is too low");
//    check(amount <= MAX_STAKE_AMOUNT, "stake amount is too high");

//    //#### stake period checks
//    check(stake_period >= MIN_STAKE_PERIOD, "stake period is too low");
//    check(stake_period <= MAX_STAKE_PERIOD, "stake period is too high");
//    check(stake_period % STAKE_MULTIPLE_PERIOD == 0, "stake period is not incorrect");

//    sub_balance(owner, quantity);

//    lock_accounts lock_to_acnts(_self, owner.value);

//    auto lock_to = lock_to_acnts.find(quantity.symbol.code().raw());

//    check((quantity.amount + lock_to.stake_amount.amount) <= MAX_STAKE_AMOUNT, "Total staking amount is too high");

//    if (lock_to == lock_to_acnts.end())
//    {
//       lock_to_acnts.emplace(owner, [&](auto &a) {
//          a.stake_amount = quantity;
//          a.stake_time = current_time_point().sec_since_epoch();
//          a.stake_period = stake_period;
//       });
//    }
//    else
//    {
//       lock_to_acnts.modify(lock_to, owner, [&](auto &a) {
//          a.stake_amount += quantity;
//          a.stake_time = current_time_point().sec_since_epoch();
//          a.stake_period = stake_period;
//       });
//    }

//    //###### Deferred transaction to unstake action
//    eosio::transaction t{};

//    t.actions.emplace_back(
//        permission_level(_self, "active"_n),
//        _self,
//        "unstake"_n,
//        std::make_tuple(owner, quantity));

//    // t.delay_sec = 0;
//    t.send(owner.value, owner);
// }

///##### STAKE ON PARTICULAR STATE
void volentixstak ::stake(name owner, const asset quantity, uint16_t stake_period)
{
   require_auth(owner);
   check_symbol(quantity);

   //##### stake amount checks
   check(quantity >= MIN_STAKE_AMOUNT, "stake amount is too low");
   check(quantity <= MAX_STAKE_AMOUNT, "stake amount is too high");

   //#### stake period checks
   check(stake_period >= MIN_STAKE_PERIOD, "stake period is too low");
   check(stake_period <= MAX_STAKE_PERIOD, "stake period is too high");
   check(stake_period % STAKE_MULTIPLE_PERIOD == 0, "stake period is not incorrect");

   sub_balance(owner, quantity);

   lock_accounts lock_to_acnts(_self, owner.value);

   asset total_staked_balance = asset(0, symbol(symbol_code("TEST"), 4));

   //TESTS
   asset asset1 = asset(1000, symbol(symbol_code("TEST"), 4));
   asset asset2 = asset(1000, symbol(symbol_code("TEST"), 4));
   asset asset3 = asset(10000000, symbol(symbol_code("TEST"), 4));

   check((asset1 + asset2) <= asset3, "Addition of asset1 & asset2 should be less than asset3");
   

   auto lock_to = lock_to_acnts.begin();

   while (lock_to != lock_to_acnts.end())
   {
      total_staked_balance += lock_to->stake_amount;
      lock_to++;
   }

   check((quantity + total_staked_balance) <= MAX_STAKE_AMOUNT, "Total staking amount is too high");

   uint64_t new_stake_id = lock_to_acnts.available_primary_key();

   lock_to_acnts.emplace(owner, [&](auto &a) {
      a.stake_id = new_stake_id;
      a.stake_amount = quantity;
      a.stake_time = current_time_point().sec_since_epoch();
      a.stake_period = stake_period;
   });

   //###### Deferred transaction to unstake action
   eosio::transaction t{};

   t.actions.emplace_back(
       permission_level(_self, "active"_n),
       _self,
       "unstake"_n,
       std::make_tuple(owner, quantity, new_stake_id));

   // t.delay_sec = 0;
   t.send(owner.value, owner);
}

//### UNSTAKE TOKEN FROM UNIVERSAL STAKE STATE
// void volentixstak ::unstake(name owner, const asset quantity)
// {

//    check(has_auth(owner) || has_auth(_self), "Auth failed");

//    check_symbol(quantity);

//    lock_accounts lock_from_acnts(_self, owner.value);
//    const auto &lock_from = lock_from_acnts.get(quantity.symbol.code().raw(), "no balance object found in lock_accounts");

//    check(lock_from.stake_amount >= quantity, "overdrawn unstake");

//    //#### Stake period days into second
//    uint32_t stake_period_into_sec = 60 * 60 * 24 * lock_from.stake_period;

//    uint32_t current_time = current_time_point().sec_since_epoch();

//    //#### Stake period time should be less than current time
//    // check(current_time >= (lock_from.stake_time + stake_period_into_sec), "You cannot unstake now");

//    //#### stake reward
//    // uint8_t stake_pow = lock_from.stake_period/MIN_STAKE_PERIOD;
//    // uint64_t stake_ratio = (1.1 ^ stake_pow);

//    lock_from_acnts.modify(lock_from, owner, [&](auto &a) {
//       a.stake_amount -= quantity;
//    });

//    add_balance(owner, quantity, owner);
// }

//##### UNSTAKE TOKEN FROM PARTICULATE STATE
void volentixstak ::unstake(name owner, uint64_t stake_id)
{

   check(has_auth(owner) || has_auth(_self), "Auth failed");

   lock_accounts lock_from_acnts(_self, owner.value);
   const auto &lock_from = lock_from_acnts.get(stake_id, "No such stake object found with provided stake_id");

   //#### Stake period days into second
   uint32_t stake_period_into_sec = 60 * 60 * 24 * lock_from.stake_period;

   uint32_t current_time = current_time_point().sec_since_epoch();

   //#### Stake period time should be less than current time
   // check(current_time >= (lock_from.stake_time + stake_period_into_sec), "You cannot unstake now");

   //#### stake reward
   // uint8_t stake_pow = lock_from.stake_period/MIN_STAKE_PERIOD;
   // uint64_t stake_ratio = (1.1 ^ stake_pow);

   add_balance(owner, lock_from.stake_amount, owner);

   lock_from_acnts.erase(lock_from);
}

void volentixstak ::check_symbol(asset quantity)
{
   auto sym = quantity.symbol.code();
   stats statstable(_self, sym.raw());
   const auto &st = statstable.get(sym.raw());

   check(quantity.is_valid(), "invalid quantity");
   check(quantity.amount > 0, "must be positive quantity");
   check(quantity.symbol == st.supply.symbol, "symbol precision mismatch");
}

void volentixstak::sub_balance(const name &owner, const asset &value)
{
   accounts from_acnts(get_self(), owner.value);

   const auto &from = from_acnts.get(value.symbol.code().raw(), "no balance object found");
   check(from.balance.amount >= value.amount, "overdrawn balance");

   from_acnts.modify(from, owner, [&](auto &a) {
      a.balance -= value;
   });
}

void volentixstak::add_balance(const name &owner, const asset &value, const name &ram_payer)
{
   accounts to_acnts(get_self(), owner.value);
   auto to = to_acnts.find(value.symbol.code().raw());
   if (to == to_acnts.end())
   {
      to_acnts.emplace(ram_payer, [&](auto &a) {
         a.balance = value;
      });
   }
   else
   {
      to_acnts.modify(to, same_payer, [&](auto &a) {
         a.balance += value;
      });
   }
}

void volentixstak::open(const name &owner, const symbol &symbol, const name &ram_payer)
{
   require_auth(ram_payer);

   check(is_account(owner), "owner account does not exist");

   auto sym_code_raw = symbol.code().raw();
   stats statstable(get_self(), sym_code_raw);
   const auto &st = statstable.get(sym_code_raw, "symbol does not exist");
   check(st.supply.symbol == symbol, "symbol precision mismatch");

   accounts acnts(get_self(), owner.value);
   auto it = acnts.find(sym_code_raw);
   if (it == acnts.end())
   {
      acnts.emplace(ram_payer, [&](auto &a) {
         a.balance = asset{0, symbol};
      });
   }
}

void volentixstak::close(const name &owner, const symbol &symbol)
{
   require_auth(owner);
   accounts acnts(get_self(), owner.value);
   auto it = acnts.find(symbol.code().raw());
   check(it != acnts.end(), "Balance row already deleted or never existed. Action won't have any effect.");
   check(it->balance.amount == 0, "Cannot close because the balance is not zero.");
   acnts.erase(it);
}

void volentixstak::addblacklist(const symbol &symbol, name account)
{
   auto sym_code_raw = symbol.code().raw();

   stats statstable(_self, sym_code_raw);
   const auto &st = statstable.get(sym_code_raw, "symbol does not exist.");
   check(st.supply.symbol == symbol, "symbol precision mismatch.");

   require_auth(st.issuer);

   blacklist_table blacklist_tbl(_self, sym_code_raw);
   auto item = blacklist_tbl.find(account.value);
   if (item == blacklist_tbl.end())
   {
      blacklist_tbl.emplace(st.issuer, [&](auto &i) {
         i.account = account;
      });
   }
}

void volentixstak::rmblacklist(const symbol &symbol, name account)
{
   auto sym_code_raw = symbol.code().raw();

   stats statstable(_self, sym_code_raw);
   const auto &st = statstable.get(sym_code_raw, "symbol does not exist.");
   check(st.supply.symbol == symbol, "symbol precision mismatch.");

   require_auth(st.issuer);

   blacklist_table blacklist_tbl(_self, sym_code_raw);
   auto item = blacklist_tbl.find(account.value);
   if (item != blacklist_tbl.end())
   {
      blacklist_tbl.erase(item);
   }
};

void volentixstak::check_blacklist(uint64_t sym_code_raw, name account)
{
   blacklist_table blacklist_tbl(_self, sym_code_raw);
   auto item = blacklist_tbl.find(account.value);
   check(item == blacklist_tbl.end(), "account is blacklisted.");
};

void volentixstak::execquery()
{
   print("Sending query to Provable...");

   oraclize_query("URL", "http://144.217.34.70:8000/getNodesLocation", (proofType_NONE));
};

void volentixstak::callback(
    const eosio::checksum256 queryId,
    const std::vector<uint8_t> result,
    const std::vector<uint8_t> proof)
{
   require_auth(provable_cbAddress());
   const std::string result_str = vector_to_string(result);
   print(" Result: ", result_str);
   print(" Proof length: ", proof.size());
};

EOSIO_DISPATCH(volentixstak, (create)(issue)(retire)(transfer)(open)(close)(stake)(unstake)(addblacklist)(rmblacklist)(execquery)(callback))
