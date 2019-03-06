#include <eosiolib/eosio.hpp>


using namespace eosio;

class [[eosio::contract]] volentixprvt : public eosio::contract {

public:
  using contract::contract;
  
  volentixprvt(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
};

EOSIO_DISPATCH( volentixprvt, (insert)(erase)(modify)(execute))
