 #include "vdexdposvote.hpp"

void vdexdposvote::regproducer(const name producer, const std::string &producer_name, const std::string &url,
                               const std::string &key, const std::string &node_id, const std::vector<uint32_t> &job_ids) {
    require_auth(producer);
    check(jobs_valid(job_ids), "job ids vector is invalid");

    auto prod = _producers.find(producer.value);

    if (prod != _producers.end()) {
        _producers.modify(prod, producer, [&](producer_info &info) {
            info.is_active = true;
            info.producer_name = producer_name;
            info.url = url;
            info.key = key;
            info.node_id = node_id;
            info.job_ids = job_ids;
        });
    } else {
        _producers.emplace(producer, [&](producer_info &info) {
            info.owner = producer;
            info.total_votes = 0;
            info.is_active = true;
            info.producer_name = producer_name;
            info.url = url;
            info.key = key;
            info.node_id = node_id;
            info.job_ids = job_ids;
        });
    }
}

void vdexdposvote::unregprod(const name producer) {
    require_auth(producer);

    const auto &prod = _producers.get(producer.value, "producer not found");

    _producers.modify(prod, same_payer, [&](producer_info &info) {
        info.is_active = false;
    });
}

void vdexdposvote::activateprod(const name producer) {
    require_auth(producer);

    const auto &prod = _producers.get(producer.value, "producer not found");

    _producers.modify(prod, same_payer, [&](producer_info &info) {
        info.is_active = true;
    });
}

void vdexdposvote::voteproducer(const name voter_name, const std::vector <name> &producers) {
    require_auth(voter_name);

    check(producers.size() <= 21, "attempt to vote for too many producers");

    const double balance_tokens = get_token_balance(voter_name);
    check(balance_tokens > 1, "need at least 1 VTX token for vote");

    uint64_t now = current_time_point().sec_since_epoch();

    vote(voter_name, producers, balance_tokens, now);
}

void vdexdposvote::vote(const name voter_name, const std::vector <name> &producers,
                    const double balance_tokens, const uint64_t vouting_time) {

    double votes = sqrt(balance_tokens);
    double votes_per_prod = votes / producers.size();

    //every time a vote is cast we must first "undo" the last vote weight, before casting the new vote weight.
    auto voter = _voters.find(voter_name.value);
    if (voter != _voters.end()) {
        update_producers(voter_name, voter->producers, voter->votes_per_prod, true);
    }

    update_producers(voter_name, producers, votes_per_prod, false);
    update_voters(voter_name, producers, votes_per_prod, balance_tokens, vouting_time);
}

void vdexdposvote::update_producers(const name voter_name, const std::vector <name> &producers,
                                const double votes_per_prod, const bool undo_prev_vote) {
    for (size_t i = 0; i < producers.size(); i++) {
        name producer = producers[i];

        check(producer.value != voter_name.value, "producer cannot vote for himself");

        const auto &prod = _producers.get(producer.value,
                                          ("producer " + producer.to_string() + " is not currently registered").data());
        double delta = undo_prev_vote ? -votes_per_prod : votes_per_prod;

        _producers.modify(prod, producer, [&](producer_info &info) {
            info.total_votes += delta;
        });
    }
}

void vdexdposvote::update_voters(const name voter_name, const std::vector <name> &producers,
                             const double votes_per_prod, const double tokens, const uint64_t vouting_time) {
    auto voter = _voters.find(voter_name.value);

    if (voter != _voters.end()) {
        _voters.modify(voter, voter_name, [&](voter_info &info) {
            info.producers = producers;
            info.votes_per_prod = votes_per_prod;
            info.tokens = tokens;
            if (vouting_time) info.date = vouting_time;
        });
    } else {
        _voters.emplace(voter_name, [&](voter_info &info) {
            info.owner = voter_name;
            info.producers = producers;
            info.votes_per_prod = votes_per_prod;
            info.tokens = tokens;
            if (vouting_time) info.date = vouting_time;
        });
    }
}

double vdexdposvote::get_token_balance(const name account) {
    accounts vtx_table(vtx_account, account.value);
    const auto it = vtx_table.find(symbol_code(vtx_symbol_code).raw());
    if (it != vtx_table.end()) {
        return it->balance.amount / vtx_precision;
    } else {
        return 0;
    }
}

void vdexdposvote::updatevotes(const name name_from, const name name_to) {
    require_auth(vtx_account);

    updatevote(name_from);
    updatevote(name_to);
}

void vdexdposvote::updatevote(const name voter_name) {
    require_auth(vtx_account);

    auto voter = _voters.find(voter_name.value);

    if (voter != _voters.end()) {
        const double balance_tokens = get_token_balance(voter_name);

        if (balance_tokens > 1) {
            vote(voter_name, voter->producers, balance_tokens, 0);
        }
    }
}

bool vdexdposvote::jobs_valid(const std::vector<uint32_t> &job_ids) {
    bool result = true;
    for (auto const &job_id : job_ids) {
        if ( job_id < job_id_bounds[0] || job_id > job_id_bounds[1] ) {
            result = false;
            break;
        }
    }

    return result;
}