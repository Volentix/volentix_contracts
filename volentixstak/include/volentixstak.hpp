#pragma once

#include <provable/eos_api.hpp>
#include <eosio/asset.hpp>
#include <eosio/system.hpp>
#include <eosio/time.hpp>
#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>
#include <string>

using namespace eosio;
using eosio::current_time_point;
using std::string;

class[[eosio::contract]] volentixstak : public contract
{
public:
   using contract::contract;

   [[eosio::action]] void create(name issuer,
                                 asset maximum_supply);

   [[eosio::action]] void issue(name to, asset quantity, string memo);

   [[eosio::action]] void retire(asset quantity, string memo);

   [[eosio::action]] void transfer(name from,
                                   name to,
                                   asset quantity,
                                   string memo);

   [[eosio::action]] void stake(name owner, const asset quantity, uint16_t stake_period);

   //### UNSTAKE TOKEN FROM UNIVERSAL STAKE STATE
   // [[eosio::action]] void unstake(name owner, const asset quantity);

   //##### UNSTAKE TOKEN FROM PARTICULATE STATE
    [[eosio::action]] void unstake(name owner, uint64_t stake_id);

   [[eosio::action]] void addblacklist(const symbol &symbol, name account);

   [[eosio::action]] void rmblacklist(const symbol &symbol, name account);

   [[eosio::action]] void open(const name &owner, const symbol &symbol, const name &ram_payer);

   [[eosio::action]] void close(const name &owner, const symbol &symbol);

   void check_symbol(asset quantity);

   static asset get_supply(name token_contract_account, symbol_code sym_code)
   {
      stats statstable(token_contract_account, sym_code.raw());
      const auto &st = statstable.get(sym_code.raw());
      return st.supply;
   }

   static asset get_balance(name token_contract_account, name owner, symbol_code sym_code)
   {
      accounts accountstable(token_contract_account, owner.value);
      const auto &ac = accountstable.get(sym_code.raw());
      return ac.balance;
   }
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

      //  uint64_t primary_key() const { return stake_amount.symbol.code().raw(); }
      uint64_t primary_key() const { return stake_id; }
   };

   struct [[eosio::table]] currency_stats
   {
      asset supply;
      asset max_supply;
      name issuer;

      uint64_t primary_key() const { return supply.symbol.code().raw(); }
   };

   // accounts who cannot be 'TO' in transfer type of 'FromLiquidTostake'
   // scope: sym_code_raw
   struct [[eosio::table]] stake_blacklist
   {
      name account;

      uint64_t primary_key() const { return account.value; }
   };

   typedef eosio::multi_index<name("accounts"), account> accounts;
   typedef eosio::multi_index<name("lockaccounts"), lock_account> lock_accounts;
   typedef eosio::multi_index<name("stat"), currency_stats> stats;
   typedef eosio::multi_index<name("blacklist"), stake_blacklist> blacklist_table;

   // void sub_balance(name owner, asset value, bool use_locked_balance = false);
   void sub_balance(const name &owner, const asset &value);
   void add_balance(const name &owner, const asset &value, const name &ram_payer);

   void check_blacklist(uint64_t sym_code_raw, name account);
};