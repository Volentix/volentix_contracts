#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/name.hpp>

using namespace eosio;


class [[eosio::contract]] test : public eosio::contract {
	public:
 		using contract::contract; 
 		test(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

 		const name vtxsys_contract = name("volentixgsys");

 		[[eosio::action]]
 		void transfer(name from, name to, asset quantity)
 		{
 			require_auth(from);
 		    action(
        	permission_level{ get_self(), "active"_n },
        	vtxsys_contract, 
        	"transfer"_n, 
        	std::make_tuple( get_self(), to, quantity, std::string("") )
    		).send();
 		}

 };

 EOSIO_DISPATCH( test, (transfer))