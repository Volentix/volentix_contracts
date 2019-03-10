#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/time.hpp>
#include "/home/sylvain/eosio.contracts/contracts/eosio.token/include/eosio.token/eosio.token.hpp"




using namespace eosio;

class [[eosio::contract]] volentixfutr : public eosio::contract {

public:
  using contract::contract; 
  volentixfutr(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  name accounts[10] = {"useraaaaaaaa"_n, "userbbbbbbbb"_n, "usercccccccc"_n, "userdddddddd"_n, "usereeeeeeee"_n, "userffffffff"_n };

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
  uint64_t nowWithMicroseconds = time_point(current_time())
  uint64_t nowWithSeconds = time_point_sec(now())

  
 
//    //List of accounts
//  // Array of dates
//  //transfer a % of tokens to each account if date is passed 
[[eosio::action]] void txfds(name treasury, name account, double amount) { 
    require_auth(treasury);
    require_auth(account);
    double balance = getbalance(account);
    //if balance is less than 
    // if()
    // {



    // }
    // std::string sym = "VTX";
    // symbol symbolvalue = symbol(symbol_code("VTX"),4);
    // eosio::asset tosend;
    // tosend.amount = amount;
    // tosend.symbol = symbolvalue;
    // action send = action(
    //   permission_level{ treasury,"active"_n},
    //   "volentixgsys"_n,
    //   "transfer"_n,
    //   std::make_tuple(get_self(), account, tosend ,std::string(""))
    // );
    // send.send();
  }

   double getbalance(name account) {
    
    const symbol sym(symbol_code("EOS"), 4);
    const auto balance = eosio::token::get_balance("volentixgsys"_n, account, sym.code());
    return balance.amount;
  }


// 130000000 / 126227704 
// Facilitators
// Permissions: Facilitators, treasury
// Time lock 4 years.
// 	At the discretion of the development partners management committee.
// 	testnet:volentixfutr	
private:
  std::string balance;
};

EOSIO_DISPATCH(volentixfutr,(txfds))
