#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/time.hpp>
#include <eosio/contract.hpp>
#include "/home/sylvain/eosio.contracts/contracts/eosio.token/include/eosio.token/eosio.token.hpp"


using namespace eosio;

class [[eosio::contract]] volentixfutr : public eosio::contract {

public:
  using contract::contract; 
  volentixfutr(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  

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

  time_point_sec tps = time_point_sec();
  uint64_t sse;
  
   double getbalance(name account) {
    
    const symbol sym(symbol_code("VTX"), 4);
    const auto balance = eosio::token::get_balance("volentixgsys"_n, account, sym.code());
    return balance.amount;
  }

  uint64_t calc_allocation(uint64_t sse, uint64_t total_allocation){
    uint64_t allocation;
    if (sse > oneyear && sse < twoyears)
      allocation = .25 * total_allocation;
    else if (sse > twoyears && sse < threeyears)
      allocation = .50 * total_allocation;
    else if (sse > threeyears && sse < fouryears)
      allocation = .75 * total_allocation;
    else if (sse > fouryears)
      allocation = total_allocation;
    return allocation;
  }

  //transfer a % of tokens to each account if date is passed 
[[eosio::action]] void txfds(name treasury, name account, double amount) { 
    require_auth(treasury);
    require_auth(account);
    sse = tps.sec_since_epoch();
    double balance;
    facilitators_index facilitators(_self, _self.value);
    auto iterator = facilitators.find(account.value);
    if(iterator != facilitators.end())
    {
      balance = getbalance(account);
    }
    uint64_t total_allocation = iterator->allocation;
    uint64_t allocation = calc_allocation(sse, total_allocation);
    if (balance < allocation){
      amount = allocation - balance;  
      std::string sym = "VTX";
      symbol symbolvalue = symbol(symbol_code("VTX"),4);
      eosio::asset tosend;
      tosend.amount = amount;
      tosend.symbol = symbolvalue;
      action send = action(permission_level{ treasury,"active"_n}, "volentixgsys"_n, "transfer"_n, std::make_tuple(get_self(), account, tosend ,std::string("")));
      send.send();
    }


}

//add a facilitator
[[eosio::action]] void afacilitator(name treasury, name account, double allocation) { 
   // require_auth(treasury);  
     facilitators_index facindex(_self, _self.value);
      facindex.emplace(treasury, [&]( auto& row ) {
       row.ID = facindex.available_primary_key(); 
       row.account = account;
       row.allocation = allocation; 
      });
  }

  //add a facilitator
[[eosio::action]] void erase(uint64_t ID) { 
   
    facilitators_index facindex(_self, _self.value);
    auto iterator = facindex.find(ID);
    //eosio_assert(iterator != facindex.end(), "Record does not exist");
    facindex.erase(iterator);
   
  }

   struct [[eosio::table]] facilitators {
        uint64_t ID;
        name account;
        double allocation;
        uint64_t get_secondary_1() const { return account.value;}
        uint64_t primary_key() const { return ID;}
  };
 
  typedef eosio::multi_index<"facilitators"_n, facilitators, indexed_by<"byaccount"_n, const_mem_fun<facilitators, uint64_t, &facilitators::get_secondary_1>>> facilitators_index;

private:

};

EOSIO_DISPATCH(volentixfutr, (afacilitator) (txfds)(erase))

// 130000000 / 126227704 
// Facilitators
// Permissions: Facilitators, treasury
// Time lock 4 years.
// 	At the discretion of the development partners management committee.
// 	testnet:volentixfutr