#include "volentixfutr.hpp"

ACTION volentixfutr::txfds( name account ) {
    require_auth( txfds_treasury );
    require_auth( account );
    
    facilitators_index facilitators(_self, _self.value);
    auto iterator = facilitators.find(account.value);
    eosio_assert(iterator != facilitators.end(), "facilitator doesn't exist");

    time_point_sec tps = time_point_sec();
    uint32_t sse = tps.sec_since_epoch();
    asset already_allocated = iterator->already_allocated;
    asset total_allocation = iterator->allocation;
    eosio_assert(already_allocated.amount < total_allocation.amount, "all available VTX already allocated");
    asset allocation = calculate_allocation(sse, total_allocation);
    
    asset to_send = allocation - already_allocated;
    eosio_assert(to_send.amount > 0, "liquid allocation is 0, try later");

    action(
        permission_level{ txfds_treasury, "active"_n }, 
        vtxsys_contract, 
        "transfer"_n, 
        std::make_tuple( get_self(), account, to_send, std::string("") )
    ).send();

    facilitators.modify(iterator, txfds_treasury, [&]( auto& row ) {
            row.already_allocated += to_send;
        });
}

void volentixfutr::afacilitator(name account, asset allocation) {
    require_auth(facilitators_modify_treasury);
    eosio_assert(allocation.symbol == vtx_symbol, "only VTX symbol allowed");
    eosio_assert(allocation.amount > 0, "allocation should be greater than 0");

    facilitators_index facilitators(_self, _self.value);
    auto iterator = facilitators.find( account.value );
    if ( iterator == facilitators.end() ) {
        facilitators.emplace(facilitators_modify_treasury, [&] ( auto& row ) {
            row.key = account;
            row.allocation = allocation;
            row.already_allocated.symbol = vtx_symbol;
            row.already_allocated.amount = 0;
        });
    } else {
        facilitators.modify(iterator, facilitators_modify_treasury, [&]( auto& row ) {
            row.allocation = allocation;
        });
    }

}

void volentixfutr::erase(name account) {
    require_auth(facilitators_modify_treasury);
    facilitators_index facilitators(_self, _self.value);
    auto iterator = facilitators.find( account.value );
    eosio_assert(iterator != facilitators.end(), "facilitator does not exist");
    facilitators.erase(iterator);

}

EOSIO_DISPATCH(volentixfutr, (afacilitator)(txfds)(erase))

// 130000000 / 126227704 
// Facilitators
// Permissions: Facilitators, treasury
// Time lock 4 years.
// 	At the discretion of the development partners management committee.
// 	testnet:volentixfutr