# VTX Staking Contract
## Usage
cleos -u $apiEndpoint push action volentixgsys transfer '{"from":"volentixtst2", "to":"vtxstake1111", "quantity":"1000.00000000 VTX", "memo":"30"}' -p volentixtst2

cleos -u $apiEndpoint push action vtxstake1111 unstake '{"owner":"volentixtst2", "quantity":"10.0000 VTX" }' -p volentixtst2


1. Stake / Unstake
Staking tokens takes effect immediately. Unstaking tokens automatically takes effect with delay chosen at staking time in a multiple of 30 days. 
In case the deferred transaction does not execute when time is due, anyone can manually execute it by calling 'unstake' method.
2. Rewards for staking
The Staking formula would be 1.1% per 30 day interval.


For example: 

1.1% 30 days
1.21% 60 days
1.331% 90 days

3. Claim
Claiming of the rewards happens when anyone manually executes 'claim' method if the period has elapsed. This triggers all payments for all whitelisted users.

4. Minimums and Maximums
1000 VTX minimum to stake
10000000 VTX to stake per account
30 days minimum and 300 days maximum
