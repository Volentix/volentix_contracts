url='https://api.eosnewyork.io:443'

# RENAME PROPOSAL NAME, TO AND QUANTITY
#cleos -u $url multisig propose issue '[{"actor": "staider11111", "permission": "active"},{"actor": "volentixdev2", "permission": "active"}]' '[{"actor": "volentixgsys", "permission": "active"}]' volentixgsys issue '{"to": "volentixprir", "quantity": "130000000.00000000 VTX", "memo": ""}' -p volentixdev2@active;
cleos -u https://api.eosnewyork.io:443 multisig propose transfer3 '[{"actor": "staider11111", "permission": "active"},{"actor": "volentixdev2", "permission": "active"}]' '[{"actor": "volentixsale", "permission": "active"}]' volentixgsys transfer '{"from":"volentixsale", to": "vtxtransfer1", "quantity": "251230.6040256800 VTX", "memo": "for accounts holding under 1000 VTX on the volentix ledger"}' -p volentixdev2@active
cleos -u https://api.eosnewyork.io:443 multisig approve volentixdev2 vsmartbotm2 '{"actor": "staider11111", "permission": "active"}' -p staider11111@active
cleos -u https://api.eosnewyork.io:443 multisig approve volentixdev2 issue '{"actor": "volentixdev2", "permission": "active"}' -p volentixdev2@active;
cleos -u https://api.eosnewyork.io:443 multisig exec volentixdev2 issue -p volentixdev2@active;
