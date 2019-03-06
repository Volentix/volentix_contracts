#include <eosiolib/eosio.hpp>


using namespace eosio;

class [[eosio::contract]] volentixtrez : public eosio::contract {

public:
  using contract::contract;
  
  volentixtrez(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  

};

EOSIO_DISPATCH( volentixtrez, (insert)(erase)(modify)(execute))
