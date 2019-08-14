#include "crowdfledger.hpp"

void crowdfledger::rcrdtfr(name from, name to, asset quantity, string tokey, string comment, string nonce)
{
    // Parameters validation
    check(from != to, "From and To fields should be different.");
    auto sym = quantity.symbol;
    check(sym.is_valid(), "Invalid symbol name");
    check(quantity.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Must transfer positive amount");
    check(!tokey.empty(), "You can not send to empty wallet");
    check(tokey.length() > 50, "Your public key is not valid");
    check(tokey.find("EOS") != string::npos, "It looks like you entered a non-public key");
    check(comment.size() <= 256, "Memo has more than 256 bytes");

    require_auth(get_self());
    deduction(from, to, quantity);

    transactions_index transactions(_self, _self.value);
    uint64_t timestamp = current_time();
    transactions.emplace(_self, [&](auto &row) {
        row.id = transactions.available_primary_key();
        row.from = from;
        row.to = to;
        row.quantity = quantity;
        row.tokey = tokey;
        row.comment = comment;
        row.nonce = nonce;
        row.timestamp = timestamp;
    });
}

void crowdfledger::updatetfr(uint64_t id, name from, name to, asset quantity, string tokey, string comment, string nonce)
{
    check(id >= 0, "ID should be positive");
    check(from != to, "From and To fields should be different.");
    auto sym = quantity.symbol;
    check(sym.is_valid(), "Invalid symbol name");
    check(quantity.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Must transfer positive amount");
    check(!tokey.empty(), "You can not send to empty wallet");
    check(tokey.length() > 50, "Your public key is not valid");
    check(tokey.find("EOS") != string::npos, "It looks like you entered a non-public key");
    check(comment.size() <= 256, "Memo has more than 256 bytes");

    require_auth(get_self());
    transactions_index transactions(_self, _self.value);
    uint64_t timestamp = current_time();
    auto toupdate = transactions.find(id);

    transactions.modify(toupdate, _self, [&](auto &row) {
        row.from = from;
        row.to = to;
        row.quantity = quantity;
        row.tokey = tokey;
        row.comment = "UPDATED: "+comment;
        row.nonce = nonce;
        row.timestamp = timestamp;
    });
}

void crowdfledger::deletetfr(uint64_t id) {
    check(id >= 0, "ID should be positive");

    require_auth(get_self());
    transactions_index transactions(_self, _self.value);
    auto todelete = transactions.find(id);
    check(todelete != transactions.end(), "ID does not exist");
    transactions.erase(todelete);
}

void crowdfledger::deduction(name from, name to, asset quantity) {
    // Parameters validation
    check(from != to, "From and To fields should be different.");
    auto sym = quantity.symbol;
    check(sym.is_valid(), "Invalid symbol name");
    check(quantity.is_valid(), "Invalid quantity");
    check(quantity.amount > 0, "Must transfer positive amount");

    std::vector<permission_level> p;
    p.push_back(permission_level{get_self(), "active"_n});
    action(p, "volentixgsys"_n, "transfer"_n, std::make_tuple(from, to, quantity, std::string("Pool deduction of VTX tokens"))).send();
}

EOSIO_DISPATCH(crowdfledger, (rcrdtfr)(updatetfr)(deletetfr))