#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] vtxcontribut : public eosio::contract {

public:
  using contract::contract;
  
  vtxcontribut(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}




// 4. 130 Million VTX
// Facilitators
// Permissions: Facilitators, treasury
// Time lock 4 years.
// 	At the discretion of the development partners management committee.
// 	testnet:volentixfutr	

  



};

EOSIO_DISPATCH( vtxcontribut, (insert)(erase)(modify)(execute))
