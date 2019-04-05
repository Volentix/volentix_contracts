#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/name.hpp>
#include <vector>

using namespace eosio;
using std::vector;

class [[eosio::contract]] volentixsale : public eosio::contract {

	public:
  		using contract::contract;
  
  		volentixsale(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  		const name treasury = name("staider11111");
  		const symbol vtx_symbol = symbol(symbol_code("VTX"), 8);
  		const name vtxsys_contract = name("volentixgsys");


  		[[eosio::action]]
  		void transfer(name to, asset quantity);

   		[[eosio::action]]
  		void init();
  	
   	private:
  		struct [[eosio::table]] limits {
        	name account;
        	asset limit;
        	asset already_transfered;
        	uint64_t primary_key() const { return account.value;}
  		};

  		typedef eosio::multi_index< "limits"_n, limits> limits_index;

  		void add_limit(name account, asset limit);

};