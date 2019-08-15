vDexNetwork Voting
----------

This contract provides multiple functionalities and operational requirements:
- Any host of a vDexNode can nominate itself as a vDexNode
- All vDexNodes are required to vote
- Users can vote for vDexNodes with a weight of the vote proportional to the amount of VTX tokens they hold
- vDexNode cannot vote for himself
- Votes are distributed according to [quadratic voting](https://en.wikipedia.org/wiki/Quadratic_voting) mechanics
- Need at least 1 VTX token for vote

Actions:
The naming convention is actionname followed by a list of paramters.

## regvDexNode vDexNode vDexNode_name url key node_id
   - Indicates that a particular account wishes to become a vDexNode or update his info
   - **vDexNode** account registering to be a vDexNode candidate
   - **vDexNode_name** the name of the vDexNode's owner account
   - **url** link to vDexNode posts where he explains his goals and tells Volentix community why this account deserves to be a vDexNode
   - **key** public key of the vDexNode that is used for generation of users deposit addresses and execution of key and signature generation protocols
   - **node_id** identifier of vDexNode Volentix node

## unregprod vDexNode
   - **vDexNode** vDexNode account to be deactivated

## votevDexNode voter_name vDexNodes
   - **voter** the account doing the voting
   - **vDexNodes** list of vDexNodes voted for. A maximum of 30 vDexNodes is allowed
   - The votes are distributed according to [quadratic voting](https://en.wikipedia.org/wiki/Quadratic_voting) mechanics