#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>

using namespace eosio;

class [[eosio::contract]] volentixtrez : public eosio::contract {

public:
  using contract::contract;
  
  volentixtrez(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
//   2. 455 Million VTX 
//   Public treasury proposals
//   Permissions: proposers, treasury 
//  	Process: 
// Pay a small VtX fee to the public treasury to submit proposals.
// Unlock VtX request for proposal with sufficient votes or
// rejected. Fee stays in the public treasury. As transaction is non refundable. 
// testnet: volentixtrez

  [[eosio::action]]
  void payfee(name treasury, name account, double amount) {
    
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
      std::make_tuple(account, treasury, tosend ,std::string(""))
    );
    send.send();
  }




};

EOSIO_DISPATCH( volentixtrez, (payfee))
