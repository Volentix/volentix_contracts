#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/name.hpp>

#include <vector>

using namespace eosio;
using std::vector;

using namespace eosio;

class [[eosio::contract]] volentixtrez : public eosio::contract {

    public:
        using contract::contract;
        volentixtrez(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

		//   2. 455 Million VTX 
		//   Public treasury proposals
		//   Permissions: proposers, treasury 
		//  	Process: 
		//   Pay a small VtX fee to the public treasury to submit proposals.
		//   Unlock VtX request for proposal with sufficient votes or
		//   rejected. Fee stays in the public treasury. As transaction is non refundable. 
		//   testnet: volentixtrez

        const name treasury = name("staider11111");
        const name vtxsys_contract = name("volentixgsys");
  

        [[eosio::action]]
        void payfee(name account, asset quantity);
};