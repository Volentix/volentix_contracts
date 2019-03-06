#include <eosiolib/eosio.hpp>

using namespace eosio;

class [[eosio::contract]]  : public eosio::contract {

public:
  using contract::contract;
  
  volentixpool(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}
  
//   1. 800 Million VTX 

// Support of the vDex network
// 	Permissions: vDex network supporters, treasury. Each transaction unlocks X amount of tokens, those tokens are redistributed among liquidity stakers and infrastructure providers:
// Supporting infrastructure staking VTX. (70%)
// Staking VTX for liquidity. (30%)
// Both are prorated to amount of stake and amount of resources.
// Testnet: volentixpool





};

EOSIO_DISPATCH( volentixpool, )
