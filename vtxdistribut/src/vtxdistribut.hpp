#pragma once

#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/contract.hpp>
#include <eosio/name.hpp>
#include <vector>
#include <string>

using std::string;
using namespace eosio;

class[[eosio::contract]] vtxdistribut : public eosio::contract {
    public:
        using contract::contract;
        const name pool_account = name("vdexmainpool");
        const name treasury = name("staider11111");
        const name vtxsys_contract = name("volentixgsys");
        const name voting_contract = name("vtxvoting555");
        const uint32_t one_day = 24 * 60 * 60;
        const uint32_t daily_reward_id = 0;

        vtxdistribut(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
            vdexnodes(receiver, receiver.value), uptimes(receiver, receiver.value), rewards(receiver, receiver.value) {}
         	
        
        [[eosio::action]]
        void paycore();

        [[eosio::action]]
        void paycampaign(string campaign);

         [[eosio::action]]
        void addnode(name account);
    
        [[eosio::action]]
        void removenode(name account);

        [[eosio::action]]
        void uptime(name account);

        // [[eosio::action]]
        // void uptimemocked(name account, uint32_t time_);
        
        [[eosio::action]]
        void setrewardrule(uint32_t reward_id, asset reward_amount, double votes_threshold, uint32_t uptime_threshold, uint32_t uptime_timeout);

    

    private:
        struct [[eosio::table]] vdexnodes {
            name account;
            uint64_t primary_key() const { return account.value; }
        };

        typedef eosio::multi_index<"vdexnodes"_n, vdexnodes> vdexnodes_index;
        vdexnodes_index vdexnodes;
        
        struct [[eosio::table]] vdexnodes_uptime {
            name account;
            uint32_t day;
            uint32_t count;
            uint32_t last_timestamp;
            uint64_t primary_key() const { return account.value;}
        };


        typedef eosio::multi_index<"uptimes"_n, vdexnodes_uptime> uptime_index;
        uptime_index uptimes;

        struct [[eosio::table]] reward_info {
            uint32_t reward_id;
            asset reward_amount;
            double votes_threshold;
            uint32_t uptime_threshold;
            uint32_t uptime_timeout;
            uint64_t primary_key() const { return reward_id;}
        };

        typedef eosio::multi_index<"rewards"_n, reward_info> reward_index;
        reward_index rewards;

};














