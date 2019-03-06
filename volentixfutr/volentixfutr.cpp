#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>


using namespace eosio;

class [[eosio::contract]] vtxcontribut : public eosio::contract {

public:
  using contract::contract;
  
  vtxcontribut(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
   //List of accounts
 // Array of dates
 //transfer a % of tokens to each account if date is passed 
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


// 4. 130 Million VTX
// Facilitators
// Permissions: Facilitators, treasury
// Time lock 4 years.
// 	At the discretion of the development partners management committee.
// 	testnet:volentixfutr	

  



};

EOSIO_DISPATCH( vtxcontribut, (transfer))
