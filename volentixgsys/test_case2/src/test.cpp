#include <eosiolib/asset.hpp>
#include <eosiolib/eosio.hpp>
#include <eosiolib/name.hpp>
#include <vector>

using namespace eosio;
using std::vector;


class [[eosio::contract]] test : public eosio::contract {
	public:
 		using contract::contract; 
 		test(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

 		const name vtxsys_contract = name("volentixgsys");
 		const name txfds_treasury = name("staider11111");

 		[[eosio::action]]
 		void transfer(name to, asset quantity)
 		{
 			require_auth(txfds_treasury);
 			vector<permission_level> p;
    		p.push_back(permission_level{ get_self(), "active"_n });
    		p.push_back(permission_level{ txfds_treasury, "active"_n });

 		    action(
        	p,
        	vtxsys_contract, 
        	"transfer"_n, 
        	std::make_tuple( get_self(), to, quantity, std::string("") )
    		).send();
 		}

 };

 EOSIO_DISPATCH( test, (transfer))