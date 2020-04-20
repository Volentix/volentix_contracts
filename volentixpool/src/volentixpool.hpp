#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/name.hpp>
#include <vector>

using namespace eosio;
using std::vector;

using namespace eosio;

class [[eosio::contract]] volentixpool : public eosio::contract {

    public:
        using contract::contract;
        volentixpool(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

        const name treasury = name("staider1111"); //staider11111
        const name vtxsys_contract = name("volentixgsys");
        const name vtxdstr_contract = name("vtxdistribut");

        [[eosio::action]]
        void payproducer(name account, asset quantity);

        [[eosio::action]]
        void payliquid(name account, asset quantity);
        
        [[eosio::action]]
        void payreward(name account, asset quantity, std::string memo);
};
