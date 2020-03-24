cleos wallet unlock -n VTX --password $(cat key.txt)
apiEndpoint="http://jungle2.cryptolions.io"

# Create VTX contract token
# cleos -u $apiEndpoint push action vtxcontract1 create '{"issuer": "vtxcontract1", "maximum_supply": "10000000000.00000000 ADD"}' -p vtxcontract1

# Issue VTX contract token
# cleos -u $apiEndpoint push action vtxcontract1 issue '{"to": "vtxcontract1", "quantity": "10000000000.00000000 ADD", "memo": ""}' -p vtxcontract1

# Transfer from VTX contract to User account
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from": "vtxcontract1", "to": "vtxtestacc11", "quantity": "1000000.00000000 ADD", "memo": ""}' -p vtxcontract1

# Buy RAM
# cleos -u $apiEndpoint system buyram volentixstak volentixstak "80.0000 EOS"

# Stake Bandwidth to contract
# cleos -u $apiEndpoint system delegatebw volentixstak volentixstak "50.0000 EOS" "50.0000 EOS"

# Set permission code
# cleos -u $apiEndpoint set account permission volentixstak active volentixstak --add-code

# clear lock balance
# cleos -u $apiEndpoint push action volentixstak clearlock '{"owner":"vtxtestacc11"}' -p volentixstak

# Add account to blocklist
# cleos -u $apiEndpoint push action volentixstak addblacklist '{"symbol": "8,VTX", "account": "vtxtestacc11"}' -p volentixstak

# Remove account from blocklist
# cleos -u $apiEndpoint push action volentixstak rmblacklist '{"symbol": "8,VTX", "account": "vtxtestacc11"}' -p volentixstak

########################### STAKING ##################################

# stake token using stake action
cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"1000.00000000 VTX", "memo":"30"}' -p vtxtestacc11

######################### STAKING TESTS #########################

######## Auth check ########
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"1000.00000000 VTX", "memo": 30}' -p vtxtestacc12

######## Must be positive amount ########
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"0.00000000 VTX", "memo": 30 }' -p vtxtestacc11

####### Check Incorrect Symbol ######
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"1000.00000000 ADD", "memo": 30 }' -p vtxtestacc11

####### Check incorrect symbol precision ######
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"1000.0000 VTX", "memo": 30 }' -p vtxtestacc11

######## Check Amount Exceeding require Limit ########
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxcontract1", "to":"volentixstak", "quantity":"10000000001.00000000 VTX", "memo": 30 }' -p vtxcontract1

######### Check account overdraw stake #########
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"10000000000.00000000 VTX", "memo": 30 }' -p vtxtestacc11

####### no balance object found in lock_accounts (Must test with new account) #######
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc13", "to":"volentixstak", "quantity":"10000000000.00000000 VTX", "memo": 30 }' -p vtxtestacc13

############################ Staking amount checks ##########################

# staking amount should be greater than 1000
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"100.00000000 VTX", "memo": 30 }' -p vtxtestacc11

######### staking amount should be less than 10000000 ###########
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"10000001.00000000 VTX", "memo": 30 }' -p vtxtestacc11

############ Staking period checks ##############

######## staking period should be multiple of 30 ######
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"1000000.00000000 VTX", "memo": 80 }' -p vtxtestacc11

######### staking period should be greater than 30 ########
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"1000000.00000000 VTX", "memo": 0 }' -p vtxtestacc11

########## staking period should be less than 300 #########
# cleos -u $apiEndpoint push action vtxcontract1 transfer '{"from":"vtxtestacc11", "to":"volentixstak", "quantity":"1000000.00000000 VTX", "memo": 600 }' -p vtxtestacc11

########### UNSTAKING ####################


# unstake token using unstaketest action
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"1000.00000000 VTX", "stake_id": 0 }' -p vtxtestacc11

############# UNSTAKING VTX ##############

######## Auth check #######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"10.0000 VTX"}'

###### Must be positive amount ########
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"0.0000 VTX" }' -p vtxtestacc11

######## Check Incorrect Symbol ########
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"10.0000 ADD"}' -p vtxtestacc11

######## Check incorrect symbol precision #######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"10.00 VTX"}' -p vtxtestacc11

######## Check Amount Exceeding require Limit #######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"2000.0000 VTX"}' -p vtxtestacc11

######## Check account overdraw stake #######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc12", "quantity":"10.0000 VTX" }' -p vtxtestacc12


####### no balance object found in lock_accounts (Must test with new account) ######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc13", "quantity":"10.0000 VTX" }' -p vtxtestacc13


echo "done"
cleos wallet lock -n VTX 
