
        Nodes can be added to the contract in order to receive VTX
            addnode(name account);
    
        Nodes can be removed from the contract with the following action
            removenode(name account);

        The action to be called to determine how long a node has been up is:
            uptime(name account);
        
        Details of the rewards are set with this action:

        setrewardrule(uint32_t reward_id, 
                            uint32_t reward_id,
                            uint32_t reward_period, 
                            asset reward_amount,
                            asset standby_amount,
                            uint32_t rank_threshold,
                            uint32_t standby_rank_threshold, 
                            double votes_threshold, 
                            uint32_t uptime_threshold, 
                            uint32_t uptime_timeout,
                            string memo,
                            string standby_memo );