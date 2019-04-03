url='http://jungle2.cryptolions.io:8888'

# RENAME PROPOSAL NAME, TO AND QUANTITY
cleos -u $url multisig propose issue '[{"actor": "staider11111", "permission": "active"},{"actor": "volentixdev1", "permission": "active"}]' '[{"actor": "volentixgsys", "permission": "active"}]' volentixgsys issue '{"to": "volentixfutr", "quantity": "1000.0000 VTX", "memo": ""}' -p volentixdev1@active;
cleos -u $url multisig approve volentixdev1 issue '{"actor": "staider11111", "permission": "active"}' -p staider11111@active;
cleos -u $url multisig approve volentixdev1 issue '{"actor": "volentixdev1", "permission": "active"}' -p volentixdev1@active;
cleos -u http://jungle2.cryptolions.io:80 multisig exec volentixdev1 issue -p volentixdev1@active;