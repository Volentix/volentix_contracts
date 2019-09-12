#include "vtxdistribut.hpp"
#include "vdexvote.hpp"


void vtxdistribut::paycore() {}
void vtxdistribut::paycampaign(string campaign) {}


void vtxdistribut::setrewardrule(uint32_t reward_id, asset reward_amount, double votes_threshold, uint32_t uptime_threshold, uint32_t uptime_timeout){
  require_auth( treasury );
	auto reward_iterator = rewards.find(reward_id);
	
	if (reward_iterator == rewards.end()) {
	  rewards.emplace(treasury, [&] ( auto& row ) { 
		  row.reward_id = reward_id;
			row.reward_amount = reward_amount;
      row.votes_threshold = votes_threshold;
			row.uptime_threshold = uptime_threshold;
			row.uptime_timeout = uptime_timeout;
		});
	} else {
		rewards.modify(reward_iterator, treasury, [&] ( auto& row ) {
			row.reward_amount = reward_amount;
			row.votes_threshold = votes_threshold;
			row.uptime_threshold = uptime_threshold;
			row.uptime_timeout = uptime_timeout;
		});		
	}	
}


void vtxdistribut::addnode(name account) {
    require_auth( account );
    auto iterator = vdexnodes.find(account.value);
    
    check(iterator == vdexnodes.end(), "node already registired");
    
    vdexnodes.emplace(account, [&] ( auto& row ) {
    	row.account = account;
    });
}

void vtxdistribut::removenode(name account) {
    require_auth( account );
    
    auto iterator = vdexnodes.find(account.value);
    check(iterator != vdexnodes.end(), "node not found");
    
    vdexnodes.erase(iterator);
}



void vtxdistribut::uptime(name account) {
	require_auth( account );
  
  time_point_sec tps = current_time_point();
  uint32_t now = tps.sec_since_epoch();
	
	auto iterator = vdexnodes.find(account.value);
	check(iterator != vdexnodes.end(), "node not found");

	auto uptime_iterator = uptimes.find(account.value);
	uint32_t day = now / one_day;
	
	if (uptime_iterator != uptimes.end()) {
		auto reward_iterator = rewards.find(daily_reward_id);

		if (day > uptime_iterator->day ) {
			
			double votes = vdexvote::get_votes(voting_contract, account);
			if (votes >= reward_iterator->votes_threshold && uptime_iterator->count >= reward_iterator->uptime_threshold) {
			  
			  action(
          { get_self(), "active"_n }, 
        		pool_account, 
        		"payreward"_n, 
        		std::make_tuple(account, reward_iterator->reward_amount, std::string("daily reward") )
    		).send();
			}
			
			uptimes.modify(uptime_iterator, account, [&] ( auto& row ) {
				row.day = day;
				row.count = 1;
				row.last_timestamp = now;
			}); 

		} else {
			check(now > uptime_iterator->last_timestamp + reward_iterator->uptime_timeout, "too often uptime");
			
			uptimes.modify(uptime_iterator, account, [&] ( auto& row ) {
				row.count += 1;
				row.last_timestamp = now;
			}); 
		}

	} else {
    uptimes.emplace(account, [&] ( auto& row ) {
      row.account = account;
    	row.day = day;
    	row.count = 1;
    	row.last_timestamp = now;
    });		
	}
}


EOSIO_DISPATCH(vtxdistribut, (setrewardrule)(addnode)(removenode)(uptime))