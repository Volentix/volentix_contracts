url='https://api.eosnewyork.io:443'

# RENAME PROPOSAL NAME, TO AND QUANTITY
cleos -u $url multisig propose issue '[{"actor": "staider11111", "permission": "active"},{"actor": "volentixdev2", "permission": "active"}]' '[{"actor": "volentixgsys", "permission": "active"}]' volentixgsys issue '{"to": "volentixpool", "quantity": "800000000.00000000 VTX", "memo": ""}' -p volentixdev2@active;
cleos -u $url multisig approve volentixdev2 issue '{"actor": "staider11111", "permission": "active"}' -p staider11111@active;
cleos -u $url multisig approve volentixdev2 issue '{"actor": "volentixdev2", "permission": "active"}' -p volentixdev2@active;
cleos -u http://jungle2.cryptolions.io:80 multisig exec volentixdev2 issue -p volentixdev2@active;