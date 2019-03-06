#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]]  : public eosio::contract {

public:
  using contract::contract;
  
  volentixpool(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  typedef eosio::multi_index<"orders"_n, order> order_index;

};

EOSIO_DISPATCH( volentixpool, )
