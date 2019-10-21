#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/time.hpp>
#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <vector>

using namespace eosio;
using std::vector;


class [[eosio::contract]] volentixprir : public eosio::contract {

	public:
 		using contract::contract; 
 		volentixprir(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
 		const double monthly_percent = ((double)25 / 12) / 100;
 		const uint32_t month = 30*24*60*60;
  		
  		//Epoch timestamp: 1590710400
		//Timestamp in milliseconds: 1590710400000
		//Date and time (GMT): Friday, May 29, 2020 12:00:00 AM
		//Date and time (your time zone): Thursday, May 28, 2020 8:00:00 PM GMT-04:00
  		const uint32_t start_time = 1590710400;
  		const name txfds_treasury = name("staider11111");
  		const name facilitators_modify_treasury = name("volentixdev2");
  		const name vtxsys_contract = name("volentixgsys");
  		const symbol vtx_symbol = symbol(symbol_code("VTX"), 8);
		const float year_one_percentage = .05;
		const float year_two_percentage = .10;
		const float year_three_percentage = .15;
		const float year_four_percentage = .70;
		const float first_8_months_distribution_percentage = .05;
		const float last_4_months_distribution_percentage = .15;

  		asset calculate_allocation(uint32_t sse, asset total_allocation) 
  		{
        check(start_time < sse, "allocation hasn't started" );
		asset allocation;
		allocation.symbol = total_allocation.symbol;
		uint32_t months_count = (sse - start_time) / month;
		if(months_count <= 8){
			allocation.amount = total_allocation.amount * year_one_percentage * first_8_months_distribution_percentage;
		}
		else if(months_count > 8 && months_count <= 12){

			allocation.amount = total_allocation.amount  * year_one_percentage * last_4_months_distribution_percentage;

		}
		else if(months_count > 12 && months_count <= 20){

			allocation.amount = total_allocation.amount  * year_two_percentage * first_8_months_distribution_percentage;

		}
		else if(months_count > 20 && months_count <= 24)
		{
			allocation.amount = total_allocation.amount  * year_two_percentage * last_4_months_distribution_percentage;

		}
		else if(months_count > 24 && months_count <= 32)
		{
			allocation.amount = total_allocation.amount  * year_three_percentage * first_8_months_distribution_percentage;
		}
		else if(months_count > 32 && months_count <= 36)
		{
			allocation.amount = total_allocation.amount  * year_three_percentage * last_4_months_distribution_percentage;
		}
		else if(months_count > 36 && months_count <= 44)
		{
			allocation.amount = total_allocation.amount  * year_four_percentage * first_8_months_distribution_percentage;	
		}
		else if(months_count > 44 && months_count <= 48)
		{
			allocation.amount = total_allocation.amount  * year_four_percentage * last_4_months_distribution_percentage;	
		}
					
		return allocation;
  		}

  		[[eosio::action]] 
  		void txfds(name account);


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
// The volentixprir account shall make no distributions for a period of six (6) months commencing November 30, 2019 and ending May 28, 2020. 

// Commencing May 29, 2020, and during the period of four (4) years ending May 29, 2024, any volentixprir account distribution determined to be made to any individual (in each case, the “Total Quantity”) shall be made in annual and monthly fractional installments, specifically:

// YEAR 1
// (i) Five (5) percent (the “Year One Quantity”) of the Total Quantity, shall be distributed during the period commencing May 29, 2020 and ending May 28, 2021 (“Year One”) in fractional monthly installments, specifically:

// Five (5) percent of the Year One Quantity shall be distributed at 13:00 EDT on the thirteenth (13th) calendar day of each calendar month for the first eight (8) successive months of Year One, and fifteen (15) percent of the Year One Quantity shall be distributed at 13:00 EDT on the thirteenth (13th) calendar day of each calendar month for the remaining four (4) successive months of Year One.
// (if u start paying on the 13th of each month that bring first payment to June 13th 2020, and 
// And payment 12 to May 13th 2021 ,meaning u can only atrt year two , June 13th 2021 and not April !) please comment on this observation before we go to year two ) 
// YEAR 2
// (ii) Ten (10) percent (the “Year Two Quantity”) of the Total Quantity, shall be distributed during the period commencing April 1, 2021 and ending March 28, 2022 (“Year Two”) in fractional monthly installments, specifically:

// Five (5) percent of the Year Two Quantity shall be distributed at 13:00 EDT on the thirteenth (13th) calendar day of each calendar month for the first eight (8) successive months of Year Two, and fifteen (15) percent of the Year Two Quantity shall be distributed at 13:00 EDT on the thirteenth (13th) calendar day of each calendar month for the remaining four (4) successive months of Year Two.

// YEAR 3
// (iii) Fifteen (15) percent (the “Year Three Quantity”) of the Total Quantity, shall be distributed during the period commencing April 1, 2022 and ending May 28, 2023 (“Year Three”) in fractional monthly installments, specifically:

// Five (5) percent of the Year Three Quantity shall be distributed at 13:00 EDT on the thirteenth (13th) calendar day of each calendar month for the first eight (8) successive months of Year Three, and fifteen (15) percent of the Year Three Quantity shall be distributed at 13:00 EDT on the thirteenth (13th) calendar day of each calendar month for the remaining four (4) successive months of Year Three.

// YEAR 4
// (iv) Seventy (70) percent (the “Year Four Quantity”) of the Total Quantity, shall be distributed during the period commencing April 1, 2023 and ending May 29, 2024 (“Year Four”) in fractional monthly installments, specifically:

// Five (5) percent of the Year Four Quantity shall be distributed at 13:00 EDT on the thirteenth (13th) calendar day of each calendar month for the first eight (8) successive months of Year Four, and fifteen (15) percent of the Year Four Quantity shall be distributed at 13:00 EDT on the thirteenth (13th) calendar day of each calendar month for the remaining four (4) successive months of Year Four.
