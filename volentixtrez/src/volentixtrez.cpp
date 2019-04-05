#include "volentixtrez.hpp"

void volentixtrez::payfee(name account, asset quantity)
{
	require_auth(treasury);
    require_auth(account);

    vector<permission_level> p;
    p.push_back(permission_level{ account, "active"_n });
    p.push_back(permission_level{ treasury, "active"_n });
    
    action(
        p, 
        vtxsys_contract, 
        "transfer"_n, 
        std::make_tuple( account, treasury, quantity, std::string("") )
    ).send();

}

EOSIO_DISPATCH( volentixtrez, (payfee))