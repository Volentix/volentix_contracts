url='http://jungle2.cryptolions.io:80'

# propose create action
cleos -u $url multisig propose createvtx '[{"actor": "staider11111", "permission": "active"},{"actor": "volentixdev1", "permission": "active"}]' '[{"actor": "volentixgsys", "permission": "active"}]' volentixgsys create '{"issuer": "volentixgsys", "maximum_supply": "2100000000.00000000 VTX"}' -p volentixdev1@active;
cleos -u $url multisig approve volentixdev1 createvtx '{"actor": "staider11111", "permission": "active"}' -p staider11111@active;
cleos -u $url multisig approve volentixdev1 createvtx '{"actor": "volentixdev1", "permission": "active"}' -p volentixdev1@active;
cleos -u http://jungle2.cryptolions.io:80 multisig exec volentixdev1 createvtx -p volentixdev1@active;