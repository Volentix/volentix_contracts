#pragma once

#include <eosio/asset.hpp>
#include <eosio/system.hpp> 
#include <eosio/time.hpp>
#include <eosio/eosio.hpp>
#include <eosio/transaction.hpp>
//#include "provable/eos_api.hpp"
#include <string>
using namespace eosio;
using std::string;

class [[eosio::contract]] volentixlist: public contract {
   public:
      using contract::contract;

      [[eosio::action]]
      void checkcountry(name account, std::string country);
      
      void addblacklist(name account);

      void rmblacklist(name account);

      
      struct [[eosio::table]] blacklist {
      name account;
      uint64_t  primary_key()const { return account.value; }
      };

      struct [[eosio::table]] whitelist {
      name account;
      uint64_t  primary_key()const { return account.value; }
      };

      typedef eosio::multi_index< name("whitelist"), whitelist > whitelist_table;
      
      typedef eosio::multi_index< name("blacklist"), blacklist > blacklist_table;

      
};

