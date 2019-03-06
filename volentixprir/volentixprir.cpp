#include <eosiolib/eosio.hpp>


using namespace eosio;

class [[eosio::contract]] volentixprir : public eosio::contract {

public:
  using contract::contract;
  
  volentixprir(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
 
};

EOSIO_DISPATCH( volentixprir, )
