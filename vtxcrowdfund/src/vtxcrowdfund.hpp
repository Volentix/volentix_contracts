#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/name.hpp>
// #include "eosio.token.hpp"
#include "volentixgsys.hpp"
#include <vector>

using namespace eosio;
using std::vector;


class [[eosio::contract]] vtxcrowdfund : public eosio::contract {

    public:
        using contract::contract;
        vtxcrowdfund(name receiver, name code,  datastream<const char*> ds): contract(receiver, code, ds) {}

        const name pool_account = name("vtxmssupport");
        const name trust_account = name("crowdfdtrust"); // TestNet
        // const name trust_account = name("???????????"); // MainNet
        const name system_account = name("volentixgsys");
        const name ledger_account = name("volentix1111"); // TestNet
        // const name ledger_account = name("volentixfrst"); // MainNet

        const symbol vtx_symbol = symbol(symbol_code("VTX"), 8);

        [[eosio::action]]
        void transfer(string key, asset amount, string comment, string nonce);

    private:
        double get_balance(name account);
        void sub_balance(asset amount);
        void add_balance(string key, asset amount, string comment, string nonce);
};