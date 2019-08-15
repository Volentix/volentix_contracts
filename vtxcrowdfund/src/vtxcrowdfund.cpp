#include "vtxcrowdfund.hpp"

double vtxcrowdfund::get_balance(name account) {
    // Getting the VTX balance.
    const symbol sym(symbol_code("VTX"), 8);
    // const auto balance = eosio::token::get_balance("volentixgsys"_n, account, sym.code());
    const auto balance = volentixgsys::get_balance("volentixgsys"_n, account, sym.code());
    return balance.amount;
}

void vtxcrowdfund::transfer(string key, asset amount, string comment, string nonce) {
    // Parameters validation
    check( amount.is_valid(), "Invalid quantity" );
    check( amount.amount > 0, "Must transfer positive amount" );
    // Authority validation
    require_auth( pool_account );

    // Balance checking
    double balance = get_balance(pool_account);
    eosio_assert( balance > 0.00000000, "Insufficient balance: exhausted pool");
    eosio_assert( balance <= amount.amount, "Сannot send more than is on the balance");

    // Transactions
    sub_balance(amount);
    add_balance(key, amount, comment, nonce);
}

void vtxcrowdfund::sub_balance(asset amount) {
    // Deduction from vtxmssupport to trust account:
    // TestNet: crowdfdtrust
    // MainNet: ????????????

    // Permissions
    vector<permission_level> p;
    p.push_back(permission_level{ pool_account, "active"_n });

    action(
        p,
        system_account,
        "transfer"_n,
        std::make_tuple(pool_account, // from
                        trust_account, // to
                        amount, // amount
                        std::string("") // comment
                        )
    ).send();
}

void vtxcrowdfund::add_balance(string key, asset amount, string comment, string nonce) {
    // Add the transaction into entry table of ledger account

    // Permissions
    vector<permission_level> p;
    p.push_back(permission_level{ pool_account, "active"_n });

    action(
        p,
        ledger_account,
        "rcrdtfr"_n,
        std::make_tuple(ledger_account, // s
                        pool_account, // from
                        trust_account, // to
                        amount.amount, // amount
                        key, // tokey
                        comment, // comment
                        nonce // nonce
                        )
    ).send();
}

EOSIO_DISPATCH(vtxcrowdfund, (transfer))