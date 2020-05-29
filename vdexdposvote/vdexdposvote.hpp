#include <eosio/eosio.hpp>
#include <eosio/asset.hpp>
#include <eosio/symbol.hpp>
#include <eosio/transaction.hpp>
#include <eosio/singleton.hpp>
#include <cmath>

using namespace eosio;
using namespace std;

class [[eosio::contract("vdexdposvote")]] vdexdposvote : public contract {
public:
    using contract::contract;

    name vtx_account = "volentixgsys"_n;
    std::string vtx_symbol_code = "VTX";
    int64_t vtx_precision = 100000000;

    // job ids:
    //   1 - daily reward
    //   2 - btc mpt
    //   3 - eth mpt
    //   4 - eos mpt
    //   5 - gateaway
    //   6 - db
    //   7 - ccxt
    //   8 - loopring
    const uint32_t job_id_bounds[2] = {1,8};


    vdexdposvote(name receiver, name code, datastream<const char *> ds) : contract(receiver, code, ds),
                                                                      _producers(receiver, receiver.value),
                                                                      _voters(receiver, receiver.value), 
                                                                      _ppweights(receiver, receiver.value) {}

    [[eosio::action]]
    void regproducer(const name producer, const std::string &producer_name,
                     const std::string &url, const std::string &key, const std::string &node_id,
                     const std::vector<uint32_t> &job_ids);

    [[eosio::action]]
    void unregprodall();
     [[eosio::action] ]
    void deluserinfo();

    [[eosio::action]]
    void unregprod(const name producer);

    [[eosio::action]]
    void activateprod(const name producer);

    [[eosio::action]]
    void voteproducer(const name voter_name, const std::vector <name> &producers);

    [[eosio::action]]
    void updatevotes(const name name_from, const name name_to);

    using updatevotes_action = action_wrapper<"updatevotes"_n, &vdexdposvote::updatevotes>;
    
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

    static std::vector<uint32_t> get_jobs(name voting_contract, name account) {
        producers_table producers(voting_contract, voting_contract.value);
        auto iterator = producers.find(account.value);
        check( iterator != producers.end(), "producer with this name doesn't exist" );
        return iterator->job_ids;
    }

    static int get_rank(name voting_contract, name account) {
        
        producers_table producers(voting_contract, voting_contract.value);
        auto m = producers.get_index<"prototalvote"_n>();
        int i = 0;
        for (auto &o : m) {
            if (o.owner.value == account.value)
                return i;
            i++;    
        }
        return 0;
    }

    static std::vector<name> get_top_nodes(name voting_contract, uint32_t top, uint32_t job_id) {
        producers_table producers(voting_contract, voting_contract.value);
        auto producers_iter = producers.get_index<"prototalvote"_n>();
        std::vector<name> top_producers;
        uint32_t i = 0;
        for (auto &producer : producers_iter) {
            auto job_ids = get_jobs(voting_contract, producer.owner);
            if (i < top && std::find(job_ids.begin(), job_ids.end(), job_id) != job_ids.end()) {
                top_producers.push_back(producer.owner);
                i++;
            } else if (i >= top) {
                break;        
            }
        }

        return top_producers;

    }

    struct [[eosio::table]] producer_info {
        name owner;
        double total_votes = 0;
        bool is_active = true;
        std::string producer_name;
        std::string url;
        std::string key;
        std::string node_id;
        std::vector<uint32_t> job_ids;

        uint64_t primary_key() const { return owner.value; }

        double by_votes() const { return is_active ? -total_votes : total_votes; }

        bool active() const { return is_active; }
    };

    typedef eosio::multi_index<"producers"_n, producer_info,
            indexed_by < "prototalvote"_n, const_mem_fun < producer_info, double, &producer_info::by_votes>  >
    >
    producers_table;

    producers_table _producers;


    struct [[eosio::table("ppweights"), eosio::contract("vdexdposvote")]] ppweights {
        map<asset, int> by_price = {};
    };

    typedef eosio::singleton< "ppweights"_n, ppweights> ppweights_table;

    [[eosio::action]]
    void setppweights(map<asset, int> by_price) {
        require_auth(_self);

        auto params = ppweights{
            .by_price = by_price
        };

        _ppweights.set( params, _self );
    };

    using setppweights_action = eosio::action_wrapper<"setppweights"_n, &vdexdposvote::setppweights>;
    ppweights_table              _ppweights;

private:
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
    
    struct account {
        asset    balance;
        uint64_t primary_key()const { return balance.symbol.code().raw(); }
    };

    typedef eosio::multi_index<"accounts"_n, account> accounts;

    void update_voters(const name voter_name, const std::vector <name> &producers,
                       const double votes_per_prod, const double tokens, const uint64_t vouting_time);

    void update_producers(const name voter_name, const std::vector <name> &producers,
                          const double votes_per_prod, const bool undo_prev_vote);

    void vote(const name voter_name, const std::vector <name> &producers,
              const double balance_tokens, const uint64_t vouting_time);

    double get_token_balance(const name account);

    void updatevote(const name voter_name);

    bool jobs_valid(const std::vector<uint32_t> &job_ids);
};
