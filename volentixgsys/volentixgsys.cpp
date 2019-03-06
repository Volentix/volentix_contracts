#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] volentixgsys : public eosio::contract {

public:
  using contract::contract;
  
  volentixgsys(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
  
};

EOSIO_DISPATCH( volentixgsys, )