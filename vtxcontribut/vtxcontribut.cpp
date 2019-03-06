#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] vtxcontribut : public eosio::contract {

public:
  using contract::contract;
  
  vtxcontribut(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
};

EOSIO_DISPATCH( vtxcontribut, (insert)(erase)(modify)(execute))
