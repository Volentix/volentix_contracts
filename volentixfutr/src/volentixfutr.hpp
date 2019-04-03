#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/name.hpp>
#include <vector>

using namespace eosio;
using std::vector;


class [[eosio::contract]] volentixfutr : public eosio::contract {

	public:
 		using contract::contract; 
 		volentixfutr(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
 		const double monthly_percent = ((double)25 / 12) / 100;
 		const uint32_t month = 30*24*60*60;
  		
  		// Epoch timestamp: 1554091200
 		// Timestamp in milliseconds: 1554091200000
 		// Human time (GMT): Monday, April 1, 2019 4:00:00 AM
 		// Human time (your time zone): Monday, April 1, 2019 12:00:00 AM GMT-04:00
  		const uint32_t start_time = 1554091200;
  		const name txfds_treasury = name("staider11111");
  		const name facilitators_modify_treasury = name("vtxtstaccnt1");
  		const name vtxsys_contract = name("volentixgsys");
  		const symbol vtx_symbol = symbol(symbol_code("VTX"), 8);

  		asset calculate_allocation(uint32_t sse, asset total_allocation) 
  		{
        eosio_assert(start_time < sse, "allocation hasn't started" );
  			asset allocation;
  			allocation.symbol = total_allocation.symbol;
  			uint32_t mounts_count = (sse - start_time) / month;
  			double total_part = mounts_count * monthly_percent;
  			if ( total_part < 1 ) {
  				allocation.amount = total_allocation.amount * total_part;
  			} else {
  				allocation.amount = total_allocation.amount;
  			}
  			return allocation;
  		}

  		[[eosio::action]] 
  		void txfds(name account);

        [[eosio::action]] 
        void txfdsmocked(name account, uint32_t sse_mocked);
        // WARNING: txfds_mocked_time NEEDS ONLY FOR TESTING
        // DO NOT FORGET TO DELETE IT BEFORE PRODUCTION DEPLOY

  		[[eosio::action]] 
  		void afacilitator(name account, asset allocation);

  		[[eosio::action]]
  		void erase(name account);

  	private:
  		struct [[eosio::table]] facilitators {
        	name key;
        	asset allocation;
        	asset already_allocated;
        	uint64_t primary_key() const { return key.value;}
  		};

  		typedef eosio::multi_index< "facilitators"_n, facilitators> facilitators_index;	

};