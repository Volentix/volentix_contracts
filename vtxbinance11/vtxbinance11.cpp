#include "vtxbinance11.hpp"

void vtxbinance11::freeze(asset quantity ) {

    require_auth(get_self());    
    
    check( quantity.symbol.is_valid(), "must be VTX" );
    check(quantity.amount > 0, "amount must be bigger than 0");
    
    vector<permission_level> p;
    p.push_back(permission_level{ get_self(), "active"_n });
    p.push_back(permission_level{ "vdexmainpool"_n, "active"_n });
    
    //Send from vdexmainpool to token account volentixgsys
    action(
        p, 
        "volentixgsys"_n,  
        "transfer"_n, 
        std::make_tuple( "vdexmainpool"_n, "volentixgsys"_n, quantity, std::string("Send from vdexmainpool to token account volentixgsys, retiring for VTXB") )
    ).send();
    
    vector<permission_level> t;
    t.push_back(permission_level{ get_self(), "active"_n });
    t.push_back(permission_level{ "volentixgsys"_n, "active"_n });
    
    //Retire from circulating supply
    action(
        p, 
        "volentixgsys"_n, 
        "retire"_n, 
        std::make_tuple( get_self(), quantity, std::string("Freeze for VTXB") )
    ).send();
    
 }

 void vtxbinance11::unfreeze( name account, asset quantity ) {
    require_auth(get_self());

    check( quantity.symbol.is_valid(), "must be VTX" );
    check(quantity.amount > 0, "amount must be bigger than 0");
    vector<permission_level> p;
    p.push_back(permission_level{ get_self(), "active"_n });
    //Issue to back in circulating supply to user's account
    action(
        p, 
        "volentixgsys"_n,  
        "issue"_n, 
        std::make_tuple( account, quantity, std::string("Unfreeze from VTXB to VTX") )
    ).send();
 }


EOSIO_DISPATCH(vtxbinance11,(freeze)(unfreeze))
