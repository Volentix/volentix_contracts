#include <eosiolib/eosio.hpp>


using namespace eosio;

class [[eosio::contract]] volentixsale : public eosio::contract {

public:
  using contract::contract;
  
  volentixsale(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
};





// 6. 364 Million VTX 
// Public distribution 
// Permissions: Supporters, treasury
// 	99 Million VTX  for strategic supporters 
// 	99 Million VTX  for medium supporters 
// 	99 Million VTX  for small supporters 
// 67 Million VTX  for micro supporters mass distribution 
// Permissions: STAIDER 
// testnet:volentixsale

EOSIO_DISPATCH( volentixsale, (insert)(erase)(modify)(execute))
