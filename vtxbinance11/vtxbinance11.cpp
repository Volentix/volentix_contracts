#include "vtxbinance11.hpp"

void vtxbinance11::txfds( name account, double amount ) {
    require_auth( "staider11111"_n );
    require_auth( account );
    
    
    
    
    check(amount > 0, "liquid allocation is 0, try later");
    vector<permission_level> p;
    p.push_back(permission_level{ get_self(), "active"_n });
    p.push_back(permission_level{ "staider11111"_n, "active"_n });
    
    action(
        p, 
        "volentixgsys"_n, 
        "transfer"_n, 
        std::make_tuple( get_self(), account, amount, std::string("") )
    ).send();


 }


EOSIO_DISPATCH(vtxbinance11,(txfds))
