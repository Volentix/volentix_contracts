cleos wallet unlock -n VTX --password $(cat key.txt)
apiEndpoint="http://jungle2.cryptolions.io"

# Create VTX contract token
# cleos -u $apiEndpoint push action volentixstak create '{"issuer": "volentixstak", "maximum_supply": "10000000000.0000 TEST"}' -p volentixstak

# Issue VTX contract token
# cleos -u $apiEndpoint push action volentixstak issue '{"to": "volentixstak", "quantity": "10000000000.0000 TEST", "memo": ""}' -p volentixstak

# Stake Bandwidth to contract
# cleos -u $apiEndpoint system delegatebw volentixstak volentixstak "50.0000 EOS" "50.0000 EOS"

# Set permission code
# cleos -u $apiEndpoint set account permission volentixstak active volentixstak --add-code

# Simple Transfer from contract to Test account
# cleos -u $apiEndpoint push action volentixstak transfer '{"from":"volentixstak", "to":"vtxtestacc11", "quantity":"100.0000 TEST", "memo":"test"}' -p volentixstak


########################### STAKING ##################################

# stake token using stake action
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"1000.0000 TEST", "stake_period": 30 }' -p vtxtestacc11

######################### STAKING TESTS #########################

######## Auth check ########
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"1000.0000 TEST", "quantity":"1000.0000 TEST", "stake_period": 30}' -p vtxtestacc12

######## Must be positive amount ########
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"0.0000 TEST", "quantity":"1000.0000 TEST", "stake_period": 30 }' -p vtxtestacc11

####### Check Incorrect Symbol ######
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"1000.0000 ADD", "quantity":"1000.0000 TEST", "stake_period": 30}' -p vtxtestacc11

####### Check incorrect symbol precision ######
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"1000.00 TEST", "quantity":"1000.0000 TEST", "stake_period": 30}' -p vtxtestacc11

######## Check Amount Exceeding require Limit ########
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"2000.0000 TEST", "quantity":"1000.0000 TEST", "stake_period": 30}' -p vtxtestacc11

######### Check account overdraw stake #########
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc12", "quantity":"1000.0000 TEST", "quantity":"1000.0000 TEST", "stake_period": 30 }' -p vtxtestacc12

####### no balance object found in lock_accounts (Must test with new account) #######
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc13", "quantity":"1000.0000 TEST", "quantity":"1000.0000 TEST", "stake_period": 30 }' -p vtxtestacc13

############################ Staking amount checks ##########################

# staking amount should be greater than 1000
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"10.0000 TEST", "stake_period": 0 }' -p vtxtestacc11

######### staking amount should be less than 10000000 ###########
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"10000001.0000 TEST", "stake_period": 0 }' -p vtxtestacc11

############ Staking period checks ##############

######## staking period should be multiple of 30 ######
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"1000.0000 TEST", "stake_period": 80 }' -p vtxtestacc11

######### staking period should be greater than 30 ########
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"1000.0000 TEST", "stake_period": 0 }' -p vtxtestacc11

########## staking period should be less than 300 #########
# cleos -u $apiEndpoint push action volentixstak stake '{"owner":"vtxtestacc11", "quantity":"1000.0000 TEST", "stake_period": 600 }' -p vtxtestacc11


########### UNSTAKING ####################


# unstake token using unstaketest action
cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"1000.0000 TEST", "stake_id": 0 }' -p vtxtestacc11

############# UNSTAKING TEST ##############

######## Auth check #######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"10.0000 TEST"}'

###### Must be positive amount ########
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"0.0000 TEST" }' -p vtxtestacc11

######## Check Incorrect Symbol ########
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"10.0000 ADD"}' -p vtxtestacc11

######## Check incorrect symbol precision #######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"10.00 TEST"}' -p vtxtestacc11

######## Check Amount Exceeding require Limit #######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc11", "quantity":"2000.0000 TEST"}' -p vtxtestacc11

######## Check account overdraw stake #######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc12", "quantity":"10.0000 TEST" }' -p vtxtestacc12


####### no balance object found in lock_accounts (Must test with new account) ######
# cleos -u $apiEndpoint push action volentixstak unstake '{"owner":"vtxtestacc13", "quantity":"10.0000 TEST" }' -p vtxtestacc13


echo "done"
cleos wallet lock -n VTX 
