#include "volentixpool.hpp"


void volentixpool::payproducer(name account, asset quantity)
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


void volentixpool::payliquid(name account, asset quantity)
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


void volentixpool::payreward(name account, asset quantity, std::string memo)
{
	require_auth(vtxdstr_contract);

  vector<permission_level> p;
  p.push_back(permission_level{ get_self(), "active"_n });
  action(
    p, 
    vtxsys_contract, 
    "transfer"_n, 
    std::make_tuple( get_self(), account, quantity, memo )
  ).send();
}


EOSIO_DISPATCH( volentixpool, (payproducer)(payliquid)(payreward))

//   1. 800 Million VTX 

// Support of the vDex network
// 	Permissions: vDex network supporters, treasury. Each transaction unlocks X amount of tokens, those tokens are redistributed among liquidity stakers and infrastructure providers:
// Supporting infrastructure staking VTX. (70%)
// Staking VTX for liquidity. (30%)
// Both are prorated to amount of stake and amount of resources.
// Testnet: volentixpool