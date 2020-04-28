#include <volentixstak.hpp>

void volentixstak::registrglobl(name owner, uint64_t stake_id, asset quantity ){

   lck_accounts lock_from_acnts(_self, owner.value);
   const auto &lock_from = lock_from_acnts.get(stake_id, "No such stake object found with provided stake_id");

   global_amounts global_amnt(_self, _self.value);
   
   auto itr_amt = global_amnt.begin();
   if (itr_amt == global_amnt.end()){

      global_amnt.emplace(_self, [&](auto &b) {
            b.stake = lock_from.stake_amount;
            b.subsidy = lock_from.subsidy;
      });
   }else{
   
      auto &itr_amt2 = global_amnt.get(0, "No such stake object found with provided stake_id");
      if (quantity.amount > 0){
         global_amnt.modify(itr_amt2, same_payer, [&](auto &b) {
            b.stake.amount += lock_from.stake_amount.amount;
            b.subsidy.amount += lock_from.subsidy.amount;
         });
         }else{
            global_amnt.modify(itr_amt2, same_payer, [&](auto &b) {
            b.stake.amount -= lock_from.stake_amount.amount;
            b.subsidy.amount -= lock_from.subsidy.amount;
         });
      }
   }
}


 void volentixstak::registersubs(name owner,uint64_t stake_id ){
   
   // Must require auth of user or contract
   check(has_auth(owner) || has_auth(_self), "Auth failed");

   lck_accounts lock_from_acnts(_self, owner.value);

   // Pointer to the stake_id object if not found then assert error
   const auto &lock_from = lock_from_acnts.get(stake_id, "No such stake object found with provided stake_id");

   // Stake period days into second
   uint32_t stake_period_sec = stake_period_into_sec(lock_from.stake_period);

   // Current uinx epoch second time
   uint32_t current_time = current_time_point().sec_since_epoch();

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
   auto &itr = lock_from_acnts.get(stake_id, "No such stake object found with provided stake_id");
   lock_from_acnts.modify(itr, same_payer, [&](auto &a) {
      a.subsidy.amount =  amount.amount - a.stake_amount.amount;
   });
   
 }

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

   lck_accounts lock_to_acnts(_self, owner.value);

   uint32_t total_stake_period;

   auto lock_to = lock_to_acnts.begin();

   while (lock_to != lock_to_acnts.end())
   {
      total_stake_period += lock_to->stake_period;
      lock_to++;
   }

   check((stake_period + total_stake_period) <= MAX_STAKE_PERIOD, "Total staking period is too high");

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
      a.subsidy = quantity;
      a.account = owner;   
      a.stake_time = current_time_point().sec_since_epoch();
      a.stake_period = stake_period;   
   });
   
   //register subsidy
   registersubs(owner, new_stake_id);
//register global quantity 
   registrglobl(owner, new_stake_id, quantity);
   
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

   lck_accounts lock_from_acnts(_self, owner.value);

   // Pointer to the stake_id object if not found then assert error
   const auto &lock_from = lock_from_acnts.get(stake_id, "No such stake object found with provided stake_id");

   
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
   
   registrglobl(owner, stake_id, (-1) * lock_from.subsidy);
   // // Delete Lock state
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


void volentixstak ::clearamnts(name owner)
{
   require_auth(_self);
   total_stake_amounts total_stake_amnt(_self, _self.value);
   
  // const auto &stk_amt = total_stake_amnt.get(lock_from.stake_amount.symbol.code().raw());
  auto stk_amt = total_stake_amnt.begin();

   while (stk_amt != total_stake_amnt.end())
      total_stake_amnt.erase(stk_amt++);
}


void volentixstak ::clearlck(name owner)
{
   require_auth(_self);
   lck_accounts lock_acnts(_self, owner.value);
   auto itr = lock_acnts.begin();
   while (itr != lock_acnts.end())
      lock_acnts.erase(itr++);
   
}
void volentixstak ::clearglobal()
{
   require_auth(_self);
   global_amounts global_amnts(_self, _self.value);
   const auto &global = global_amnts.get(0, "No such stake object found with provided stake_id");
   global_amnts.erase(global);
   
}