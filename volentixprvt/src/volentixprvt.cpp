#include "volentixprvt.hpp"

void volentixprvt::transfer(name account, asset quantity)
{
	require_auth(treasury);
    require_auth(account);
    
    vector<permission_level> p;
    p.push_back(permission_level{ get_self(), "active"_n });
    p.push_back(permission_level{ treasury, "active"_n });
    
    action(
        p, 
        vtxsys_contract, 
        "transfer"_n, 
        std::make_tuple( get_self(), account, quantity, std::string("") )
    ).send();
}



EOSIO_DISPATCH( volentixprvt, (transfer))

