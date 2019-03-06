#include <eosiolib/eosio.hpp>


using namespace eosio;

class [[eosio::contract]] volentixsale : public eosio::contract {

public:
  using contract::contract;
  
  volentixsale(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
};

EOSIO_DISPATCH( volentixsale, (insert)(erase)(modify)(execute))
