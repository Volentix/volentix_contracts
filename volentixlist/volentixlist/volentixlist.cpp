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
   //system permissions
//    set staider? permissions
//    require_auth( st.issuer );

   blacklist_table blacklist_tbl( _self, sym_code_raw );
   auto item = blacklist_tbl.find( account.value );
   if (item != blacklist_tbl.end()) {
   blacklist_tbl.erase(item);
}

void volentixstak::execquery()
{
   print("Sending query to Provable...");
   //curl http://localhost:8000/GetCountryIp?ip=x.x.x.x
   //{"Country":"..."}
   oraclize_query("URL", "http://144.217.34.70:8000/", (proofType_NONE));
};

void volentixstak::callback(
    const eosio::checksum256 queryId,
    const std::vector<uint8_t> result,
    const std::vector<uint8_t> proof)
{
   require_auth(provable_cbAddress());
   const std::string result_str = vector_to_string(result);
   print(" Result: ", result_str);
   print(" Proof length: ", proof.size());
};


EOSIO_DISPATCH( volentixlist, (checkcountry))