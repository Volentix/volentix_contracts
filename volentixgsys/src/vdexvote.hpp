#include <eosio/symbol.hpp>
#include <eosio/transaction.hpp>
#include <cmath>
#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

using namespace eosio;

class [[eosio::contract("vdexvote")]] vdexvote : public contract {
public:
    using contract::contract;

    name vtx_account = "volentixgsys"_n;
    std::string vtx_symbol_code = "VTX";
    int64_t vtx_precision = 100000000;

    vdexvote(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                      _producers(receiver, receiver.value),
                                                                      _voters(receiver, receiver.value) {}

    [[eosio::action]]
    void regproducer(const name producer, const std::string &producer_name,
                     const std::string &url, const std::string &key, const std::string &node_id);

    [[eosio::action]]
    void unregprod(const name producer);

    [[eosio::action]]
    void activateprod(const name producer);

    [[eosio::action]]
    void voteproducer(const name voter_name, const std::vector <name> &producers);

    [[eosio::action]]
    void updatevotes(const name name_from, const name name_to);

    using updatevotes_action = action_wrapper<"updatevotes"_n, &vdexvote::updatevotes>;
    
    static std::vector <name> get_voters_by_time(const name vouting_account, const uint64_t from, const uint64_t to, const name user) {
        voters_table voters(vouting_account, vouting_account.value);

        std::vector <name> voters_need;

        for (auto itr = voters.begin(); itr != voters.end(); itr++) {
            if (itr->date >= from && itr->date <= to) {
                voters_need.push_back(itr->owner);
            }
        }

        return voters_need;
    }

    static double get_votes(name voting_contract, name account) {
        producers_table producers(voting_contract, voting_contract.value);
        
        auto iterator = producers.find(account.value);
        
        if (iterator != producers.end()) {
            return iterator->total_votes;
        } else {
            return 0;
        }
    }

private:

    struct [[eosio::table]] producer_info {
        name owner;
        double total_votes = 0;
        bool is_active = true;
        std::string producer_name;
        std::string url;
        std::string key;
        std::string node_id;

        uint64_t primary_key() const { return owner.value; }

        double by_votes() const { return is_active ? -total_votes : total_votes; }

        bool active() const { return is_active; }
    };

    typedef eosio::multi_index<"producers"_n, producer_info,
            indexed_by < "prototalvote"_n, const_mem_fun < producer_info, double, &producer_info::by_votes>  >
    >
    producers_table;

    producers_table _producers;

    struct [[eosio::table]] voter_info {
        name owner;
        std::vector <name> producers;
        uint64_t date;
        double votes_per_prod = 0;
        double tokens = 0;

        uint64_t primary_key() const { return owner.value; }
    };

    typedef eosio::multi_index<"voters"_n, voter_info> voters_table;

    voters_table _voters;

    void update_voters(const name voter_name, const std::vector <name> &producers,
                       const double votes_per_prod, const double tokens, const uint64_t vouting_time);

    void update_producers(const name voter_name, const std::vector <name> &producers,
                          const double votes_per_prod, const bool undo_prev_vote);

    void vote(const name voter_name, const std::vector <name> &producers,
              const double balance_tokens, const uint64_t vouting_time);

    double get_token_balance(const name account);

    void updatevote(const name voter_name);
};