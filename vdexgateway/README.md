# Deposit and withdraw workflow

## Deposit
1. Register account to get deposit address

`cleos push action vdexgateway regaccount '["bob"]' -p bob@active
executed transaction: 77da176791a8404f71f730e90b584ac861a442a3359ac46aaa666fe2afcae1d0  104 bytes  288 us
vdexgateway <= vdexgateway::regaccount      {"account":"bob"}
`

Now Bob has deposit addresses.

`cleos get  table vdexgateway BVTX accntinfo
{
  "rows": [{
      "account": "bob",
      "deposit_address": "15Ez6SCQF7RA6pXUBbnPijaEmQahFkYdzE",
      "balance": "0.00000000 BVTX"
    }
  ],
  "more": false,
  "next_key": ""
}`

2. Then Bob send 0.05325221 BTC to 15Ez6SCQF7RA6pXUBbnPijaEmQahFkYdzE and submit transqction hash to gateway contract.
`cleos push action vdexgateway submitdep '[ "bob", "0.05325221 BVTX", "d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11fb445eb31d2"]' -p bob@active
executed transaction: ee63e509a522b2c4e6309c01946f13fccd62a51ce19309f0c2ba5870e6ba5ebe  152 bytes  1539 us
vdexgateway <= vdexgateway::submitdep       {"account":"bob","amount":"0.05325221 BVTX","tx_hash":"d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6...
bob <= vdexgateway::submitdep       {"account":"bob","amount":"0.05325221 BVTX","tx_hash":"d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6...
`

Bob can claim 0.05325221 BVTX once vdexnodes confirms tha transaction `d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11fb445eb31d2` become irreversible in bitcoin blockchain.

3. Vdex node confirm deposit using following action
`cleos push action vdexgateway confirmdep '[ "vdexnode1", "bob", "d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11fb445eb31d2" ]' -p vdexnode1@active
executed transaction: 280f8d923ef7a66c79bf988180d6aab1aa94ad55bdea77900601ecf3a06da91f  144 bytes  306 us
vdexgateway <= vdexgateway::confirmdep      {"node":"vdexnode1","account":"bob","tx_hash":"d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11..`

`cleos push action vdexgateway confirmdep '[ "vdexnode2", "bob", "d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11fb445eb31d2" ]' -p vdexnode2@active
executed transaction: 08c5642f374c59c7acd059658f3f3740b54f466a56829e492b092011a4612cb3  144 bytes  342 us
vdexgateway <= vdexgateway::confirmdep      {"node":"vdexnode2","account":"bob","tx_hash":"d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11...`

Now deposit confirmed and colateral token(VBTC) can be claimed.
`cleos get  table vdexgateway bob accntdeposit
{
  "rows": [{
      "id": 0,
      "amount": "0.05325221 BVTX",
      "tx_hash": "d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11fb445eb31d2",
      "is_confirmed": 1,
      "is_claimed": 0,
      "nodes_confirmed": 2
    }
  ],
  "more": false,
  "next_key": ""
}`

4. Bob can claim colateral token using claimdep action 
`cleos push action vdexgateway claimdep '[ "bob", "d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11fb445eb31d2"]' -p bob@active
executed transaction: 3bdfc60b7ec5d3fcae7aea4daff224450a549e6a8987f6ea9ebfaad2efd44e73  136 bytes  589 us
vdexgateway <= vdexgateway::claimdep        {"account":"bob","tx_hash":"d2cae97da959fc75d0b057765b1ec480b297f0d54d3d6c8c9bd11fb445eb31d2"}
volentixgsys <= volentixgsys::transfer       {"from":"vdexgateway","to":"bob","quantity":"0.05325221 BVTX","memo":"memo"}
vdexgateway <= volentixgsys::transfer       {"from":"vdexgateway","to":"bob","quantity":"0.05325221 BVTX","memo":"memo"}
bob <= volentixgsys::transfer       {"from":"vdexgateway","to":"bob","quantity":"0.05325221 BVTX","memo":"memo"}
vdexdposvote <= vdexdposvote::updatevotes    {"name_from":"vdexgateway","name_to":"bob"}`

## Withdraw
1. Register account to get deposit address 
`cleos push action vdexgateway regaccount '["alice"]' -p alice@active
executed transaction: e98fe209bba429d1bfe1ea6554c9721978bda1ea1c83b2e80337e0050c75187a  104 bytes  584 us
vdexgateway <= vdexgateway::regaccount      {"account":"alice"}`

2. Transfer desired amount of VBTC for gateway contract
`cleos push action volentixgsys transfer '[ "alice", "vdexgateway",  "1.00000000 BVTX", "memo" ]' -p alice@active
executed transaction: a096f2d6b01d76e5229f8acdd83abdcbc51f024818d092e48611678018ccbfba  136 bytes  821 us
volentixgsys <= volentixgsys::transfer       {"from":"alice","to":"vdexgateway","quantity":"1.00000000 BVTX","memo":"memo"}
alice <= volentixgsys::transfer       {"from":"alice","to":"vdexgateway","quantity":"1.00000000 BVTX","memo":"memo"}
vdexgateway <= volentixgsys::transfer       {"from":"alice","to":"vdexgateway","quantity":"1.00000000 BVTX","memo":"memo"}
vdexdposvote <= vdexdposvote::updatevotes    {"name_from":"alice","name_to":"vdexgateway"}`

Now Alice has liquid balance and can withdraw it
`cleos get  table vdexgateway BVTX accntinfo
{
  "rows": [{
      "account": "alice",
      "deposit_address": "16Ndc1WaDgqtFfKCq3fVQL5gRrDU2qkaas",
      "balance": "1.00000000 BVTX"
    },{
      "account": "bob",
      "deposit_address": "15Ez6SCQF7RA6pXUBbnPijaEmQahFkYdzE",
      "balance": "0.00000000 BVTX"
    }
  ],
  "more": false,
  "next_key": ""
}
`

3. Create withdraw request
`cleos push action vdexgateway withdraw '["alice", "1.00000000 BVTX", "1Fu3iBR2EMQWeYGi3XvrPmcPUkne8ZWj9h"]' -p alice@active
executed transaction: cb2ffefb0db34424e815f88af39356f4141ad6e72de7890be6f378fdb45c4668  152 bytes  397 us
vdexgateway <= vdexgateway::withdraw        {"account":"alice","amount":"1.00000000 BVTX","address":"1Fu3iBR2EMQWeYGi3XvrPmcPUkne8ZWj9h"}
vdexgateway <= vdexgateway::newwithdraw     {"account":"alice","amount":"1.00000000 BVTX","id":0}`

4. Now each vdexnode is able to process that withdrawal and get reward.
Firstly vdex node has to lock withdraw
`cleos push action vdexgateway lockwithdraw '["vdexnode1", "alice", "0"]' -p vdexnode1@active
executed transaction: dc8d33b78808008c67007f84e5270c5d61b9c376a2435ca239a8b4a3bdf62c7f  120 bytes  278 us
vdexgateway <= vdexgateway::lockwithdraw    {"node":"vdexnode1","account":"alice","withd_id":0}`

After locking vdexnode1 send BTC to Alice account and submit tx_hash to gateway contract
`cleos push action vdexgateway submitwithd '["vdexnode1", "alice", "0", "08de200a964d7597cf9e78a69ccfc276cf767c07f97949957ece4af61a1ac37e"]' -p vdexnode1@active
executed transaction: 3f1fd969f0434dd21e32a3e20220bf51c8e728149de32d2a3f005b95ec33598c  152 bytes  981 us
vdexgateway <= vdexgateway::submitwithd     {"node":"vdexnode1","account":"alice","withd_id":0,"tx_hash":"08de200a964d7597cf9e78a69ccfc276cf767c..`


5. Now vdexnode1 can get reward for performing withdrawal once other vdex nodes will confirm that BTC transaction becomes irreversible
`cleos push action vdexgateway confirmwithd '["vdexnode1", "alice", "08de200a964d7597cf9e78a69ccfc276cf767c07f97949957ece4af61a1ac37e"]' -p vdexnode1@active
executed transaction: d8d09931a691280499507c9c2dd94704940dd5cfbea848d77d30c97b766ca68f  144 bytes  490 us
vdexgateway <= vdexgateway::confirmwithd    {"node":"vdexnode1","account":"alice","tx_hash":"08de200a964d7597cf9e78a69ccfc276cf767c07f97949957ec...`
`cleos push action vdexgateway confirmwithd '["vdexnode2", "alice", "08de200a964d7597cf9e78a69ccfc276cf767c07f97949957ece4af61a1ac37e"]' -p vdexnode2@active
executed transaction: 06a7d8c6f00cc87dc46e4d997cb8abcee2bb0f85c3337929e64c21a2ff4c4aad  144 bytes  363 us
vdexgateway <= vdexgateway::confirmwithd    {"node":"vdexnode2","account":"alice","tx_hash":"08de200a964d7597cf9e78a69ccfc276cf767c07f97949957ec...` 

Now withdraw is confirmed, but vdexnode1 hasn't rewarded yes
`cleos get  table vdexgateway alice accntwithdra
{
  "rows": [{
      "id": 0,
      "amount": "1.00000000 BVTX",
      "tx_hash": "08de200a964d7597cf9e78a69ccfc276cf767c07f97949957ece4af61a1ac37e",
      "is_confirmed": 1,
      "is_rewarded": 0,
      "nodes_confirmed": 2,
      "proccesed_by": "vdexnode1",
      "address": "1Fu3iBR2EMQWeYGi3XvrPmcPUkne8ZWj9h"
    }
  ],
  "more": false,
  "next_key": ""
}`

To get reward vdexnode1 has to use getreward action
`cleos push action vdexgateway getreward '["vdexnode1", "alice", "08de200a964d7597cf9e78a69ccfc276cf767c07f97949957ece4af61a1ac37e"]' -p vdexnode1@active
executed transaction: df6e8473f768d1d7e2c501b3c09a3eac3366df7d159ac2fa9509eb0697fd1d87  144 bytes  682 us
vdexgateway <= vdexgateway::getreward       {"node":"vdexnode1","account":"alice","tx_hash":"08de200a964d7597cf9e78a69ccfc276cf767c07f97949957ec...
volentixgsys <= volentixgsys::transfer       {"from":"vdexgateway","to":"vdexnode1","quantity":"2.00000000 BVTX","memo":"memo"}
vdexgateway <= volentixgsys::transfer       {"from":"vdexgateway","to":"vdexnode1","quantity":"2.00000000 BVTX","memo":"memo"}
vdexnode1 <= volentixgsys::transfer       {"from":"vdexgateway","to":"vdexnode1","quantity":"2.00000000 BVTX","memo":"memo"}
vdexdposvote <= vdexdposvote::updatevotes    {"name_from":"vdexgateway","name_to":"vdexnode1"}`

Vdexnode got 2 BVTX cause:
a) Alice withdrawed 1 BTC(1 BTC = 1 VBTC)
b) reward for withdraw is set to 1 VBTC
`cleos get  table vdexgateway vdexgateway currency
{
  "rows": [{
      "currency_symbol": "8,BVTX",
      "colateral_token": "volentixgsys",
      "is_active": 1,
      "deposit_confirmations": 2,
      "withdraw_confirmations": 2,
      "withdraw_reward": "1.00000000 BVTX"
    }
  ],
  "more": false,
  "next_key": ""
}`
