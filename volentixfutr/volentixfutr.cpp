#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/time.hpp>
#include <eosiolib/symbol.hpp>



using namespace eosio;

class [[eosio::contract]] vtxcontribut : public eosio::contract {

public:
  using contract::contract;



  vtxcontribut(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
  name accounts[10] = {"user11111111"_n, "user21111111"_n, "user31111111"_n, "user41111111"_n, "user51111111"_n, "user61111111"_n,
  "user71111111"_n, "user81111111"_n, "user91111111"_n, "user10111111"_n};
  

  //126227704 s = 4 YEARS
  //94670778 s = 3 YEARS
  //63113852 s = 2 YEARS
  //31556926 s = 1 YEAR

  time_t  end_time  =  1554091200 + 126227704;
  // Epoch timestamp: 1554091200
  // Timestamp in milliseconds: 1554091200000
  // Human time (GMT): Monday, April 1, 2019 4:00:00 AM
  // Human time (your time zone): Monday, April 1, 2019 12:00:00 AM GMT-04:00
  time_t  starttime =  1554091200;
  time_t  oneyear = starttime + 31556926;
  time_t  twoyears = starttime + 63113852 ;
  time_t  threeyears = starttime + 94670778;
  time_t  fouryears = starttime + 126227704;

  uint64_t total_tokens = 133000000;
  time_t  rawtime =  current_time();
 // struct tm * ptm = gmtime ( &rawtime );
  
[[eosio::action]]
  void getbalance(name account) {
    std::string sym = "VTX";
    symbol symbolvalue = symbol(symbol_code("VTX"),4);
    action getbalance = action(
      permission_level{ account,"active"_n},
      "currency"_n,
      "balance"_n,
      std::make_tuple("volentixgsys", account, symbolvalue)
    );
    getbalance.send();
  }





   //List of accounts
 // Array of dates
 //transfer a % of tokens to each account if date is passed 
[[eosio::action]]
  void transfer(name treasury, name account, double amount) {
    
    require_auth(treasury);
    require_auth(account);
    get_balance();
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
 

// 130000000 / 126227704 
// Facilitators
// Permissions: Facilitators, treasury
// Time lock 4 years.
// 	At the discretion of the development partners management committee.
// 	testnet:volentixfutr	

};

EOSIO_DISPATCH( vtxcontribut, (transfer))
