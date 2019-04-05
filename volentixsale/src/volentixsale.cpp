#include "volentixsale.hpp"

void volentixsale::add_limit(name account, asset limit)
{
    limits_index limits(_self, _self.value);
    auto iterator = limits.find(account.value);
    if (iterator == limits.end()) {
        limits.emplace( treasury, [&] (auto& row) {
            row.account = account;
            row.limit = limit;
            row.already_transfered.symbol = vtx_symbol;
            row.already_transfered.amount = 0;
        });
    }
}

    
void volentixsale::init(){
    require_auth(treasury);
    
    name strategic_supporters = name("vtxstsupport");
    name medium_supporters = name("vtxmesupport");
    name small_supporters = name("vtxsmsupport");
    name micro_supporters = name("vtxmssupport");
    
    asset limit;
    asset micro_limit;
    limit.symbol = vtx_symbol;
    micro_limit.symbol = vtx_symbol;
    limit.amount = 9900000000000000;
    micro_limit.amount = 6700000000000000;

    // 99 Million VTX  for strategic supporters 
    add_limit(strategic_supporters, limit);
    
    // 99 Million VTX  for medium supporters 
    add_limit(medium_supporters, limit);
    
    // 99 Million VTX  for small supporters 
    add_limit(small_supporters, limit);
    
    // 67 Million VTX  for micro supporters mass distribution 
    add_limit(micro_supporters, micro_limit);



}

void volentixsale::transfer(name to, asset quantity)
{
    require_auth(treasury);
    require_auth(to);
    eosio_assert(quantity.symbol == vtx_symbol, "only VTX symbol allowed");

    limits_index limits(_self, _self.value);
    auto iterator = limits.find(to.value);
    eosio_assert(iterator != limits.end(), "there is no available VTX for to account");

    eosio_assert(iterator->already_transfered + quantity <= iterator->limit, "limit exceeded");

    vector<permission_level> p;
    p.push_back(permission_level{ get_self(), "active"_n });
    p.push_back(permission_level{ treasury, "active"_n });

    action(
        p,
        vtxsys_contract, 
        "transfer"_n, 
        std::make_tuple( get_self(), to, quantity, std::string("") )
    ).send();

    limits.modify(iterator, treasury, [&]( auto& row ) {
            row.already_transfered += quantity;
        });

}

EOSIO_DISPATCH( volentixsale, (transfer)(init))
