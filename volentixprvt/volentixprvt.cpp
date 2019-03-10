#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>


using namespace eosio;

class [[eosio::contract]] volentixprvt : public eosio::contract {

public:
  using contract::contract;
  
  volentixprvt(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
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
      permission_level{ treasury,"active"_n},
      "volentixgsys"_n,
      "transfer"_n,
      std::make_tuple(get_self(), account, tosend ,std::string(""))
    );
    send.send();
  }

};

// 65 Million VTX 
// Angels and early backers
// Permissions: STAIDER 
// Testnet: volentixprvt	



EOSIO_DISPATCH( volentixprvt, (transfer))
