#pragma once

#include <provable/eos_api.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/time.hpp>
#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>
#include <string>

#define SYMBOL_PRE_DIGIT 8
#define TOKEN_SYMBOL "TEST"
#define BALANCE_ACC name("vtxcontract1")
#define MIN_STAKE_AMOUNT asset(100000000000, symbol(TOKEN_SYMBOL, SYMBOL_PRE_DIGIT))
#define MAX_STAKE_AMOUNT asset(1000000000000000, symbol(TOKEN_SYMBOL, SYMBOL_PRE_DIGIT))
#define MIN_STAKE_PERIOD 30
#define MAX_STAKE_PERIOD 300
#define STAKE_MULTIPLE_PERIOD 30
#define REWARD_PER 1.1
#define SYMBOL_PRE_F 100000000

using namespace eosio;
using std::string;

class[[eosio::contract]] volentixstak : public contract
{
public:
   using contract::contract;

   [[eosio::on_notify("vtxcontract1::transfer")]] void deposit(name from,
                                                               name to,
                                                               asset quantity,
                                                               string memo);

   void stake(name owner, const asset quantity, uint16_t stake_period);
   [[eosio::action]] void unstake(name owner, uint64_t stake_id);

   [[eosio::action]] void clearlock(name owner);

   [[eosio::action]] void addblacklist(const symbol &symbol, name account);
   [[eosio::action]] void rmblacklist(const symbol &symbol, name account);

   [[eosio::action]] void execquery();
   [[eosio::action]] void callback(const eosio::checksum256 queryId, const std::vector<uint8_t> result, const std::vector<uint8_t> proof);

private:
   struct [[eosio::table]] account
   {
      asset balance;

      uint64_t primary_key() const { return balance.symbol.code().raw(); }
   };

   struct [[eosio::table]] lock_account
   {
      uint64_t stake_id;
      asset stake_amount;
      uint32_t stake_time;
      uint16_t stake_period;

      uint64_t primary_key() const { return stake_id; }
   };

   struct [[eosio::table]] stake_blacklist
   {
      name account;

      uint64_t primary_key() const { return account.value; }
   };

   typedef eosio::multi_index<name("accounts"), account> accounts;
   typedef eosio::multi_index<name("lockaccounts"), lock_account> lock_accounts;
   typedef eosio::multi_index<name("blacklist"), stake_blacklist> blacklist_table;

   using transfer_action = action_wrapper<name("transfer"), &volentixstak::deposit>;

   void check_symbol(asset quantity)
   {
      auto sym = quantity.symbol.code();
      check(quantity.is_valid(), "invalid quantity");
      check(quantity.amount > 0, "must be positive quantity");
      check(quantity.symbol == MIN_STAKE_AMOUNT.symbol, "symbol precision mismatch");
   }

   uint32_t stake_period_into_sec(uint16_t stake_period)
   {
      return (60 * 60 * 24 * stake_period);
   }

   void check_blacklist(uint64_t sym_code_raw, name account);
};