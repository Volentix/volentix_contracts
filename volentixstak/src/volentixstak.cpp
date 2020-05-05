#include <volentixstak.hpp>

void volentixstak::deposit(name from,
                           name to,
                           asset quantity,
                           string memo)
{

   if (from == BALANCE_ACC || from == _self)
   {
      return;
   }

   require_auth(from);

   check(to == _self, "Invalid notify");

   check(memo.size() <= 3, "memo has more than 2 bytes");

   uint16_t stake_period = stoi(memo);

   check_blacklist(quantity.symbol.code().raw(), from);

   stake(from, quantity, stake_period);
}

// STAKE ON PARTICULAR STATE
void volentixstak ::stake(name owner, const asset quantity, uint16_t stake_period)
{
   check_symbol(quantity);

   // stake amount checks
   check(quantity >= MIN_STAKE_AMOUNT, "stake amount is too low");
   check(quantity <= MAX_STAKE_AMOUNT, "stake amount is too high");

   // stake period checks
   check(stake_period >= MIN_STAKE_PERIOD, "stake period is too low");
   check(stake_period <= MAX_STAKE_PERIOD, "stake period is too high");
   check(stake_period % STAKE_MULTIPLE_PERIOD == 0, "stake period is incorrect");

   lock_accounts lock_to_acnts(_self, owner.value);

   uint32_t total_stake_period;

   auto lock_to = lock_to_acnts.begin();

   // while (lock_to != lock_to_acnts.end())
   // {
      // total_stake_period += lock_to->stake_period;
      // lock_to++;
   // }

   // check((stake_period + total_stake_period) <= MAX_STAKE_PERIOD, "Total staking period is too high");

   total_stake_amounts total_stake_amnt(_self, _self.value);

   auto stk_amt = total_stake_amnt.find(quantity.symbol.code().raw());

   if (stk_amt == total_stake_amnt.end())
   {
      total_stake_amnt.emplace(_self, [&](auto &a) {
         a.amount = quantity;
      });
   }
   else
   {
      check((stk_amt->amount + quantity) <= MAX_STAKE_AMOUNT, "Total stake amount exceed limit");

      total_stake_amnt.modify(stk_amt, same_payer, [&](auto &a) {
         a.amount += quantity;
      });
   }

   uint64_t new_stake_id = lock_to_acnts.available_primary_key();

   lock_to_acnts.emplace(_self, [&](auto &a) {
      a.stake_id = new_stake_id;
      a.stake_amount = quantity;
      a.stake_time = current_time_point().sec_since_epoch();
      a.stake_period = stake_period;
   });

   // Deferred transaction to unstake action
   eosio::transaction t{};

   t.actions.emplace_back(
       permission_level(_self, "active"_n),
       _self,
       "unstake"_n,
       std::make_tuple(owner, new_stake_id));

   t.delay_sec = stake_period_into_sec(stake_period);
   t.send(current_time_point().sec_since_epoch(), _self);
}

// UNSTAKE TOKEN FROM PARTICULATE STATE
void volentixstak ::unstake(name owner, uint64_t stake_id)
{

   // Must require auth of user or contract
   check(has_auth(owner) || has_auth(_self), "Auth failed");

   lock_accounts lock_from_acnts(_self, owner.value);

   // Pointer to the stake_id object if not found then assert error
   const auto &lock_from = lock_from_acnts.get(stake_id, "No such stake object found with provided stake_id");

   // Stake period days into second
   uint32_t stake_period_sec = stake_period_into_sec(lock_from.stake_period);

   // Current uinx epoch second time
   uint32_t current_time = current_time_point().sec_since_epoch();

   // Cannot unstake before stake period time
   check(current_time >= (lock_from.stake_time + stake_period_sec), "You cannot unstake now");

   // stake reward

   // Converting stake days to period
   uint8_t stake_pow = lock_from.stake_period / MIN_STAKE_PERIOD;

   // Calculating the Stake percentage based on Period i.e stake_period ^ MIN_STAKE_PERIOD
   double stake_per = 1;
   for (int i = 0; i < stake_pow; i++)
      stake_per *= REWARD_PER;

   // Converting stake percentage to double
   double stake_decimal = stake_per / 100;

   // Converting stake_amount uint64_t to double
   double stake_amount = lock_from.stake_amount.amount / SYMBOL_PRE_F;

   // Calculating Stake reward from amount
   double stake_reward = stake_decimal * stake_amount;

   // Calculating total unstake amount
   double total_unstake_amount = stake_amount + stake_reward;

   // Converting again to the asset form again
   double unstake_amount = total_unstake_amount * SYMBOL_PRE_F;

   // Casting float to uint64_t
   uint64_t decimal_amount = (uint64_t)unstake_amount;

   // Casting uint64_t to asset
   asset amount = asset(decimal_amount, symbol(TOKEN_SYMBOL, SYMBOL_PRE_DIGIT));

   // Transfer unstaking amount to user
   action(
       permission_level{_self, "active"_n},
       BALANCE_ACC,
       "transfer"_n,
       std::make_tuple(_self, owner, amount, std::string("unstaking fund")))
       .send();

   //Decrement in Total stake amount

   total_stake_amounts total_stake_amnt(_self, _self.value);

   const auto &stk_amt = total_stake_amnt.get(lock_from.stake_amount.symbol.code().raw());

   total_stake_amnt.modify(stk_amt, same_payer, [&](auto &a) {
      a.amount -= lock_from.stake_amount;
   });

   // Delete Lock state
   lock_from_acnts.erase(lock_from);

}

void volentixstak::addblacklist(const symbol &symbol, name account)
{
   auto sym_code_raw = symbol.code().raw();

   blacklist_table blacklist_tbl(_self, sym_code_raw);
   auto item = blacklist_tbl.find(account.value);
   if (item == blacklist_tbl.end())
   {
      blacklist_tbl.emplace(_self, [&](auto &i) {
         i.account = account;
      });
   }
}

void volentixstak::rmblacklist(const symbol &symbol, name account)
{
   auto sym_code_raw = symbol.code().raw();

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
   auto itr = blacklist_tbl.find(account.value);
   check(itr == blacklist_tbl.end(), "account is blacklisted.");
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

void volentixstak ::clearlock(name owner)
{
   require_auth(_self);

   lock_accounts lock_acnts(_self, owner.value);

   auto itr = lock_acnts.begin();

   while (itr != lock_acnts.end())
      lock_acnts.erase(itr++);
}
