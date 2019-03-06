#include <eosiolib/eosio.hpp>


using namespace eosio;

class [[eosio::contract]] volentixprir : public eosio::contract {

public:
  using contract::contract;
  
  volentixprir(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
 

// 5. 130 Million VTX
// Prior work
//  	Team and advisors
// 	Permissions: STAIDER 
//  		Time Locked account to be distributed over 4 years.
// 	testnet: volentixprir





};

EOSIO_DISPATCH( volentixprir, )
