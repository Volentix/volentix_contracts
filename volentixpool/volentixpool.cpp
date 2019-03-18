#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>

using namespace eosio;

class [[eosio::contract]] volentixpool : public eosio::contract {

public:
  using contract::contract;
  
  volentixpool(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

  
  [[eosio::action]]
  void payproducer(name treasury, name account, double amount) {
    
    require_auth(treasury);
    require_auth(account);
    if (treasury.to_string() != "staider11111")
      return;  

    //calculate prorata ressources
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

  [[eosio::action]]
  void payliquid(name treasury, name account, double amount) {
    
    require_auth(treasury);
    require_auth(account);
    if (treasury.to_string() != "staider11111")
      return;  

    //calculate prorata ressources
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

EOSIO_DISPATCH( volentixpool, (payproducer)(payliquid))
//   1. 800 Million VTX 

// Support of the vDex network
// 	Permissions: vDex network supporters, treasury. Each transaction unlocks X amount of tokens, those tokens are redistributed among liquidity stakers and infrastructure providers:
// Supporting infrastructure staking VTX. (70%)
// Staking VTX for liquidity. (30%)
// Both are prorated to amount of stake and amount of resources.
// Testnet: volentixpool