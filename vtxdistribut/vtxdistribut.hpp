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
        const name pool_account = name("vtxtestpool1");
        const name treasury = name("vtxdistribut");
        const name vtxsys_contract = name("volentixgsys");
        const name voting_contract = name("vdexdposvote");
        const symbol vtx_symbol = symbol(symbol_code("VTX"), 8);
        const uint32_t one_day = 24 * 60 * 60;
        const uint32_t daily_reward_id = 0;
        const uint32_t standby_reward_id = 1;

        vtxdistribut(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
            rewards(receiver, receiver.value), usblacklist(receiver, receiver.value),
            inituptime(receiver, receiver.value) {}
         	
        
        [[eosio::action]]
        void paycore();

        [[eosio::action]]
        void paycampaign(string campaign);       
        
        [[eosio::action]]
        void uptime(name account, const std::vector<uint32_t> &job_ids);
        
        [[eosio::action]]
        void setrewardrule( uint32_t reward_id,
                            uint32_t reward_period, 
                            asset reward_amount,
                            asset standby_amount,
                            uint32_t rank_threshold,
                            uint32_t standby_rank_threshold, 
                            double votes_threshold, 
                            uint32_t uptime_threshold, 
                            uint32_t uptime_timeout,
                            string memo,
                            string standby_memo );

        [[eosio::action]]
        void addblacklist(name account, string ip);

        [[eosio::action]]
        void rmblacklist(name account);

        [[eosio::action]]
        void initup(name account);

        [[eosio::action]]
        void rmup(name account);

       
    

    private:
        struct [[eosio::table]] vdexnodes_uptime {
            uint32_t job_id;
            uint32_t period_num;
            uint32_t count;
            uint32_t last_timestamp;
            uint32_t primary_key() const { return job_id;}
        };


        typedef eosio::multi_index<"uptimes"_n, vdexnodes_uptime> uptime_index;

        struct [[eosio::table]] reward_info {
            uint32_t reward_id;
            uint32_t reward_period;
            asset reward_amount;
            asset standby_amount;
            uint32_t rank_threshold;
            // set standby_rank_threshold to 0 to disable it
            uint32_t standby_rank_threshold;
            double votes_threshold;
            uint32_t uptime_threshold;
            uint32_t uptime_timeout;
            string memo;
            string standby_memo;
            uint64_t primary_key() const { return reward_id;}
        };

        typedef eosio::multi_index<"rewards"_n, reward_info> reward_index;
        reward_index rewards;

        struct [[eosio::table]] blacklist {
            name account;
            string IP;
            uint64_t primary_key() const { return account.value;}
        };

        typedef eosio::multi_index<"usblacklist"_n, blacklist> blacklist_index;
        blacklist_index usblacklist;

        
        struct [[eosio::table]] init_uptime {
            name account;
            uint32_t init_uptime;
            uint64_t primary_key() const { return account.value;}
        };

        typedef eosio::multi_index<"inituptime"_n, init_uptime> inituptime_index;
        inituptime_index inituptime;

        void reward(name account, uint32_t job_id, uint32_t timestamp);
        void checkblaclist ( name account );

};














