url='http://jungle2.cryptolions.io:80'


# balance
cleos -u $url get currency balance volentixgsys volentixfutr VTX



# add restriction
cleos -u $url push action volentixgsys arestriction '{"account": "volentixfutr", "code": "staider11111", "permission": "active"}' -p staider11111@active

# check restriction table
 cleos -u $url get table volentixgsys volentixgsys restriction
# {
#   "rows": [{
#       "account": "volentixfutr",
#       "code": "staider11111",
#       "permission": "active"
#     }
#   ],
#   "more": false
# }


# add faclitator
cleos -u $url push action volentixfutr afacilitator  '{"account": "maxpetriev11", "allocation": "100.00000000 VTX"}' -p vtxtstaccnt1@active


# check faclitators table
cleos -u $url get table volentixfutr volentixfutr facilitators

# {
#   "rows": [{
#       "key": "maxpetriev11",
#       "allocation": "100.0000 VTX",
#       "already_allocated": "0.0000 VTX"
#     }
#   ],
#   "more": false
# }


# invoke txfds
cleos -u $url multisig propose transfer12 '[{"actor": "staider11111", "permission": "active"},{"actor": "maxpetriev11", "permission": "active"}]' '[{"actor": "staider11111", "permission": "active"},{"actor": "maxpetriev11", "permission": "active"}]' volentixfutr txfdsmocked '{"account": "maxpetriev11", "sse_mocked": "1556683200"}' -p staider11111@active;
cleos -u $url multisig approve staider11111 transfer12 '{"actor": "staider11111", "permission": "active"}' -p staider11111@active;
cleos -u $url multisig approve staider11111 transfer12 '{"actor": "maxpetriev11", "permission": "active"}' -p maxpetriev11@active;
cleos -u http://jungle2.cryptolions.io:80 multisig exec staider11111 transfer12 -p staider11111@active;


#check balance 
cleos -u $url get currency balance volentixgsys maxpetriev11 VTX;
# 2.08333333 VTX

# check faclitators table
cleos -u $url get table volentixfutr volentixfutr facilitators;

# {
#   "rows": [{
#       "key": "maxpetriev11",
#       "allocation": "100.00000000 VTX",
#       "already_allocated": "2.08333333 VTX"
#     }
#   ],
#   "more": false
# }
