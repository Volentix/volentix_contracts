
        Nodes can be added to the contract in order to receive VTX
            addnode(name account);
    
        Nodes can be removed from the contract with the following action
            removenode(name account);

        The action to be called to determine how long a node has been up is:
            uptime(name account);
        
        Details of the rewards are set with this action:

            setrewardrule(uint32_t reward_id, asset reward_amount, double votes_threshold, uint32_t uptime_threshold, uint32_t uptime_timeout);
