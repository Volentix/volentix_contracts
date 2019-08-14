#pragma once

#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/symbol.hpp>
#include <eosiolib/contract.hpp>
#include <eosiolib/name.hpp>
#include <vector>
#include <string>

using std::string;
using namespace eosio;

class[[eosio::contract]] crowdfledger : public eosio::contract {
    public:
        using contract::contract;

        crowdfledger(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds) {}

        [[eosio::action]]
        void rcrdtfr(name from, name to, asset quantity, string tokey, string comment, string nonce);

        [[eosio::action]]
        void updatetfr(uint64_t id, name from, name to, asset quantity, string tokey, string comment, string nonce);

        [[eosio::action]]
        void deletetfr(uint64_t id);

    private:
        void deduction(name from, name to, asset quantity);

        struct [[eosio::table]] transactions {
            uint64_t id = 0;
            name from;
            name to;
            asset quantity;
            string tokey;
            string comment;
            string nonce;
            uint64_t timestamp;
            uint64_t primary_key() const { return id; }
        };

        typedef eosio::multi_index<"transactions"_n, transactions> transactions_index;
};