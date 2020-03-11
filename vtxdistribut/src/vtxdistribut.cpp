#include "vtxdistribut.hpp"
#include "../../vdexdposvote/vdexdposvote.hpp"


void vtxdistribut::paycore() {}
void vtxdistribut::paycampaign(string campaign) {}

void vtxdistribut::setrewardrule( uint32_t reward_id,
                                  uint32_t reward_period, 
                                  asset reward_amount,
                                  asset standby_amount,
                                  uint32_t rank_threshold,
                                  uint32_t standby_rank_threshold, 
                                  double votes_threshold, 
                                  uint32_t uptime_threshold, 
                                  uint32_t uptime_timeout,
                                  string memo,
                                  string standby_memo )
{
  require_auth( treasury );
  auto reward_iterator = rewards.find(reward_id);
  
  if (reward_iterator == rewards.end()) {
    rewards.emplace(treasury, [&] ( auto& row ) { 
      row.reward_id = reward_id;
      row.reward_period = reward_period;
      row.reward_amount = reward_amount;
      row.standby_amount = standby_amount;
      row.rank_threshold = rank_threshold;
      row.standby_rank_threshold = standby_rank_threshold;
      row.votes_threshold = votes_threshold;
      row.uptime_threshold = uptime_threshold;
      row.uptime_timeout = uptime_timeout;
      row.memo = memo;
      row.standby_memo = standby_memo;
    });
  } else {
    rewards.modify(reward_iterator, treasury, [&] ( auto& row ) {
      row.reward_id = reward_id;
      row.reward_period = reward_period;
      row.reward_amount = reward_amount;
      row.standby_amount = standby_amount;
      row.rank_threshold = rank_threshold;
      row.standby_rank_threshold = standby_rank_threshold;
      row.votes_threshold = votes_threshold;
      row.uptime_threshold = uptime_threshold;
      row.uptime_timeout = uptime_timeout;
      row.memo = memo;
      row.standby_memo = standby_memo;
    });   
  } 
}



void vtxdistribut::uptime( name account, const std::vector<uint32_t> &job_ids ) {
  require_auth( account );
  checkblaclist( account );

  auto producer_job_ids = vdexdposvote::get_jobs( voting_contract, account );
  //DIFFER CONTRACT
  time_point_sec tps = current_time_point();
  uint32_t now = tps.sec_since_epoch();

    for (auto const &job_id : producer_job_ids) {
        // check whether job_id on registered job_ids
        if ( std::find(producer_job_ids.begin(), producer_job_ids.end(), job_id) != producer_job_ids.end() ) {
            reward(account, job_id, now);
        }
    }
}

void vtxdistribut::reward(name account, uint32_t job_id, uint32_t timestamp) {
  auto reward_iter = rewards.find(job_id);
  check( reward_iter != rewards.end(), "unknown job_id" );
  uint32_t current_period = reward_iter->reward_period / one_day;

  uptime_index uptimes( get_self(), account.value );
  auto uptime_iter = uptimes.find(job_id);
  
  if ( uptime_iter == uptimes.end() ) {
    // init uptime for account and job_id
    uptimes.emplace(account, [&] ( auto& row ) {
      row.job_id = job_id;
      row.period_num = current_period;
      row.count = 1;
      row.last_timestamp = timestamp;
    });

    return;

  }

  // send reward for prev period at start of current period
  if (current_period > uptime_iter->period_num ) {
    double votes = vdexdposvote::get_votes(voting_contract, account);
    int rank = vdexdposvote::get_rank(voting_contract, account);
    asset reward(0, vtx_symbol);
    string memo;
    //calcilate reward amount
    if ( rank <= reward_iter->rank_threshold ) {
        reward = reward_iter->reward_amount;
        memo = reward_iter->memo;
    } else if (rank <= reward_iter->standby_rank_threshold ) {
        reward = reward_iter->standby_amount;
        memo = reward_iter->standby_memo;
    }

    if ( reward.amount > 0 ) {
      action(
        { get_self(), "active"_n }, 
        pool_account, 
        "payreward"_n, 
        std::make_tuple(account, reward, memo )
      ).send();      
    }

    return;

  } 
  
  // update uptime count and timestamp
  check(timestamp > uptime_iter->last_timestamp + reward_iter->uptime_timeout, "too often uptime");
  uptimes.modify(uptime_iter, account, [&] ( auto& row ) {
    row.count += 1;
    row.last_timestamp = timestamp;
  });     
}


// void vtxdistribut::uptime(name account) {
//   require_auth( account );
//   //DIFFER CONTRACT
//     time_point_sec tps = current_time_point();
//     uint32_t now = tps.sec_since_epoch();
  
//   auto iterator = vdexnodes.find(account.value);
//   check(iterator != vdexnodes.end(), "node not found");

//   // TODO: mpve to checkblaclist
//   auto refuse = usblacklist.find(account.value);
//     check(refuse == usblacklist.end(), "Your IP is from the United States, cannot proceed with reward");

//   auto uptime_iterator = uptimes.find(account.value);
//   uint32_t day = now / one_day;
  
//   if (uptime_iterator != uptimes.end()) {
//     auto reward_iterator = rewards.find(daily_reward_id);
//     auto reward_iterator2 = rewards.find(standby_reward_id);

//     // бабки отсылаются на первый uptime в след периоде
//     if (day > uptime_iterator->day ) {
      
//       double votes = vdexdposvote::get_votes(voting_contract, account);
//       int rank = vdexdposvote::get_rank(voting_contract, account);
//       if (votes >= reward_iterator->votes_threshold && uptime_iterator->count >= reward_iterator->uptime_threshold) {
//         if(rank<= 21){
//               action(
//                 { get_self(), "active"_n }, 
//                 pool_account, 
//                 "payreward"_n, 
//                 std::make_tuple(account, reward_iterator->reward_amount, std::string("node reward") )
//               ).send();
//         }
//         else if (rank > 21 && rank <= 42){
//               action(
//                 { get_self(), "active"_n }, 
//                 pool_account, 
//                 "payreward"_n, 
//                 std::make_tuple(account, reward_iterator2->reward_amount, std::string("standby reward") )
//               ).send();
//         }
//       }
      
//       uptimes.modify(uptime_iterator, account, [&] ( auto& row ) {
//         row.day = day;
//         row.count = 1;
//         row.last_timestamp = now;
//       }); 

//     } else {
//       check(now > uptime_iterator->last_timestamp + reward_iterator->uptime_timeout, "too often uptime");
      
//       uptimes.modify(uptime_iterator, account, [&] ( auto& row ) {
//         row.count += 1;
//         row.last_timestamp = now;
//       }); 
//     }

//   } else {
//     uptimes.emplace(account, [&] ( auto& row ) {
//       row.account = account;
//       row.day = day;
//       row.count = 1;
//       row.last_timestamp = now;
//     });   
//   }
// }

void vtxdistribut::addblacklist(name account, string ip){
  require_auth( account );
    auto iterator = usblacklist.find(account.value);
    
    check(iterator == usblacklist.end(), "node already registered");
    
    usblacklist.emplace(account, [&] ( auto& row ) {
      row.account = account;
    });
}
void vtxdistribut::rmblacklist(name account){
  require_auth( account );
    
    auto iterator = usblacklist.find(account.value);
    check(iterator != usblacklist.end(), "node not found");
    
    usblacklist.erase(iterator);
}
void vtxdistribut::initup(name account){
  require_auth( account );
    auto iterator = inituptime.find(account.value);
    
    check(iterator == inituptime.end(), "node already registered");
    
    inituptime.emplace(account, [&] ( auto& row ) {
      row.account = account;
    });
}
void vtxdistribut::rmup(name account){
  require_auth( account );
    
    auto iterator =  inituptime.find(account.value);
    check(iterator !=  inituptime.end(), "node not found");
    
    inituptime.erase(iterator);
}

void vtxdistribut::checkblaclist ( name account ){ 
  auto refuse = usblacklist.find( account.value );
  check( refuse == usblacklist.end(), "Your IP is from the United States, cannot proceed with reward" );
}

EOSIO_DISPATCH(vtxdistribut, (setrewardrule)(uptime)(addblacklist)(rmblacklist)(initup)(rmup))