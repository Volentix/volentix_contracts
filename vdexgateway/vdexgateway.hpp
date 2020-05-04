#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <eosio/time.hpp>
#include <vector>
#include <string>

#define VOLENTIX_ORACLE name("vdexoracle1")
#define RANK 21
#define VOTING_CONTRACT name("vdexdposvote")
#define NULL_NAME name("null") // null name means that withdraw is free to lock

using std::string;
using std::vector;
using namespace eosio;

class[[eosio::contract]] vdexgateway : public contract
{
public:
   using contract::contract;
   vdexgateway(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
            currencies(receiver, receiver.value) {}

   // TODO:
   // 1. add limits for deposit and withdraw
   // 2. add blacklist
   // 3. payble actions for colateral token transfers
   // 4. withdraw expiration?

   [[eosio::action]]
   void regaccount(name account);

   [[eosio::action]]
   void addaddresses(string currency_name,
   					 uint8_t currency_precision,
   					 const vector<string> &addresses);

   [[eosio::action]]
   void addcurrency(string currency_name,
					uint8_t currency_precision,
					uint16_t deposit_confirmations,
					uint16_t withdraw_confirmations,
					name colateral_token,
					asset withdraw_reward);

   [[eosio::action]]
   void submitdep(name account, asset amount, checksum256 tx_hash);

   [[eosio::action]]
   void confirmdep(name node, asset amount, checksum256 tx_hash);

   [[eosio::action]]
   void claimdep(name account, checksum256 tx_hash);
   
   [[eosio::action]] 
   void withdraw(name account, asset amount, string address);

   [[eosio::action]]
   void lockwithdraw(name node, asset amount, uint64_t withd_id);

   [[eosio::action]]
   void submitwithd(name node, asset amount, uint64_t withd_id, checksum256 tx_hash);

   [[eosio::action]]
   void confirmwithd(name node, asset amount, checksum256 tx_hash);

   [[eosio::action]]
   void getreward(name node, asset amount, checksum256 tx_hash);

   [[eosio::on_notify("btcvdextoken::transfer")]]
   void onbtctrnsfer(name from, name to, asset quantity, string memo);

   [[eosio::on_notify("ethvdextoken::transfer")]]
   void onethtrnsfer(name from, name to, asset quantity, string memo);

private:
   // scope: currency_symbol
   struct [[eosio::table]] deposit_address
   {
   	uint64_t id;
   	string address;
   	uint64_t primary_key() const { return id; }
   };
   typedef eosio::multi_index<name("depaddresses"), deposit_address> deposit_addresses;

   // scope : account_name.value
   struct [[eosio::table]] account_deposit 
   {
   	uint64_t id;
      name account;
   	asset amount;
   	checksum256 tx_hash;
   	bool is_confirmed;
   	bool is_claimed;
   	uint16_t nodes_confirmed;
   	uint64_t primary_key() const { return id; }
   	checksum256 by_tx_hash() const { return tx_hash; }
   };
   typedef eosio::multi_index<name("accntdeposit"), account_deposit,
           indexed_by < name("bytxhash"), const_mem_fun < account_deposit, checksum256, &account_deposit::by_tx_hash>  >
    >
   account_deposits;


   // scope : node_name.value
   struct [[eosio::table]] confirmation
   {
   	uint64_t id;
   	checksum256 tx_hash;
   	uint64_t primary_key() const { return id; }
   	checksum256 by_tx_hash() const { return tx_hash; }
   };
   typedef eosio::multi_index<name("confirmation"), confirmation,
           indexed_by < name("bytxhash"), const_mem_fun < confirmation, checksum256, &confirmation::by_tx_hash>  >
    >
   confirmations;

   // scope: currency_symbol
   struct [[eosio::table]] account_info
   {
   	 name account;
   	 string deposit_address;
   	 // user can claim colateral tokens for balance
   	 asset balance;
   	 uint64_t primary_key() const { return account.value; }
   };
   typedef eosio::multi_index<name("accntinfo"), account_info> accounts_info;

   struct [[eosio::table]] currency
   {
   	 symbol currency_symbol;
   	 name colateral_token;
   	 bool is_active;
   	 uint16_t deposit_confirmations; // number of nodes required to confirm deposit
   	 uint16_t withdraw_confirmations; // number of nodes required to confirm withdraw
   	 asset withdraw_reward;
   	 uint64_t primary_key() const { return currency_symbol.code().raw(); }
   };
   typedef eosio::multi_index<name("currency"), currency> currency_index;
   currency_index currencies;

  // scope : currency_symbol
   struct [[eosio::table]] account_withdraw
   {
   	uint64_t id;
      name account;
   	asset amount;
   	checksum256 tx_hash;
   	bool is_confirmed;
   	bool is_rewarded;
   	uint16_t nodes_confirmed;
   	name proccesed_by;
      string address;
   	uint64_t primary_key() const { return id; }
   	checksum256 by_tx_hash() const { return tx_hash; }
   };
   typedef eosio::multi_index<name("accntwithdra"), account_withdraw,
           indexed_by < name("bytxhash"), const_mem_fun < account_withdraw, checksum256, &account_withdraw::by_tx_hash>  >
    >
   account_withdrawals;

   bool has_rank(name node, int rank);
   void assign_addresses(name account);
   void assign_address(name account, symbol currency_symbol);
   void confirm_by_hash(name node, checksum256 tx_hash);
   void ontransfer(name from, name to, asset quantity, string memo);
};
