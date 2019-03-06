#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]] vtxcontribut : public eosio::contract {

public:
  using contract::contract;
  
  vtxcontribut(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  
  [[eosio::action]]
  void transfer(name treasury, name account, double amount) {
    
    require_auth(treasury);
    require_auth(account);
    std::string sym = "VTX";
    symbol symbolvalue = symbol(symbol_code("VTX"),4);
    eosio::asset tosend;
    tosend.amount = amount;
    tosend.symbol = symbolvalue;
    action send = action(
      permission_level{ user1,"active"_n},
      "volentixgsys"_n,
      "transfer"_n,
      std::make_tuple(get_self(), user2, tosend ,std::string(""))
    );
    send.send();
  }

};

EOSIO_DISPATCH( vtxcontribut, (transfer))
