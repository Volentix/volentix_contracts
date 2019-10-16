#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/name.hpp>

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
        void payreward(name account, asset quantity);
};
