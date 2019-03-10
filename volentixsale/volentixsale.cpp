#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>

using namespace eosio;

class [[eosio::contract]] volentixsale : public eosio::contract {

public:
  using contract::contract;
  
  volentixsale(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

//Issue these at the same time as the others and make their own contract
//Maybe have this contract call those contracts
  // 	99 Million VTX  for strategic supporters 
// 	99 Million VTX  for medium supporters 
// 	99 Million VTX  for small supporters 
// 67 Million VTX  for micro supporters mass distribution 

   [[eosio::action]]
  void transfer(name treasury, name debitaccount, name account, double amount) {
    
    require_auth(treasury);
    require_auth(account);
    if ((getbalance(debitaccount) < 99000000 && debitaccount.to_string() ==  "vtxstsupport") &&
       (getbalance(debitaccount) < 99000000 && debitaccount.to_string() ==  "vtxmesupport") &&
       (getbalance(debitaccount) < 99000000 && debitaccount.to_string() ==  "vtxsmsupport") &&
        (getbalance(debitaccount) < 67000000 && debitaccount.to_string() ==  "vtxsmsupport")) {
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
  }

  uint64_t getbalance(name account) {
    // auto balance = eosio::token::get_balance(token_contract, token_holder_name, symbol_code);
    return 1111;
  }
  
};

// Permissions: STAIDER 
// testnet:volentixsale

EOSIO_DISPATCH( volentixsale, (transfer))
