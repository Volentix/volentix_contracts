#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/name.hpp>

#include <vector>

using namespace eosio;
using std::vector;

using namespace eosio;

class [[eosio::contract]] vtxcontribut : public eosio::contract {

    public:
        using contract::contract;
        vtxcontribut(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

        const name treasury = name("staider11111");
        const name vtxsys_contract = name("volentixgsys");
  

        [[eosio::action]]
        void transfer(name account, asset quantity);
};
