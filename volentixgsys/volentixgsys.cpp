#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] volentixgsys : public eosio::contract {

public:
  using contract::contract;
  
  volentixgsys(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

//Total VtX 2.1 Billion  volentixzero    
//reimplement eosio.token so its generates all accounts and subsequently distribute amounts
//Owner is Foundation account created on Crypto Kylin test net: volentixgsys


  
};

EOSIO_DISPATCH( volentixgsys, )