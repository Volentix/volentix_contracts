#include "volentixlist.hpp"


void volentixlist::checkcountry(name account, std::string country){


}

void volentixlist::addblacklist(name account)
{
//   auto sym_code_raw = symbol.code().raw();

   
//    require_auth( st.issuer );

//    blacklist_table blacklist_tbl( _self, sym_code_raw );
//    auto item = blacklist_tbl.find( account.value );
//    if (item == blacklist_tbl.end()) {
//       blacklist_tbl.emplace( st.issuer, [&](auto& i) {
//          i.account = account;
//       });
//    } 
}

void volentixlist::rmblacklist(name account)
{
//    set staider? permissions
//    require_auth( st.issuer );

   blacklist_table blacklist_tbl( _self, sym_code_raw );
   auto item = blacklist_tbl.find( account.value );
   if (item != blacklist_tbl.end()) {
   blacklist_tbl.erase(item);
}




EOSIO_DISPATCH( volentixlist, (checkcountry))