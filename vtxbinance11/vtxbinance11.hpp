
#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/time.hpp>
#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <vector>

using namespace eosio;
using std::vector;



class [[eosio::contract("vtxbinance11")]] vtxbinance11 : public contract {


	public:



 	using contract::contract; 



 	vtxbinance11(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
 	
	[[eosio::action]] 
	void txfds(name account, double amount);



};