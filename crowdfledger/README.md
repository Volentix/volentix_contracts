# crowdfledger contract

## Deploying
* Add the crowdfledger eosio.code permissions into active permissions for crowdfledger account
* Compile the contract
* Set contract into the crowdfledger account
* Add the pool account with crowdfledger code and active permissions into restriction table of volentixgsys

## Usage
* Cast crowdfledger::rcrdtfr action by providing the crowdfledger permission

## Test transactions

`crowdfledger::rcrdtfr` with permission `crowdfledger@active`

Conditions:
* `forbanpirate` and `forbanfizzle` **ADDED** into `volentixgsys` `restrictions` table.
* `anotherforba` is **NOT ADDED** into `volentixgsys` `restrictions` table.

|from         |to           |Status  |
|:-----------:|:------------|:------:|
|forbanfizzle |forbanpirate | passed |
|forbanfizzle |anotherforba | passed |
|forbanfizzle |volentixgsys | passed |
|forbanpirate |forbanfizzle | passed |
|forbanpirate |anotherforba | passed |
|forbanpirate |volentixgsys | passed |
|anotherforba |forbanfizzle | failed |
|anotherforba |forbanpirate | failed |
|anotherforba |volentixgsys | failed |

Conditions:
* `anotherforba` is **ADDED** into `volentixgsys` `restrictions` table.

|from         |to           |Status  |
|:-----------:|:------------|:------:|
|anotherforba |forbanfizzle | passed |
|anotherforba |forbanpirate | passed |
|anotherforba |volentixgsys | passed |

That the `crowdfledger` contract worked and did not require separate permission for each pool from which the deduction will be made. The pool must be added to the table of restrictions of the `volentixgsys` contract with the code for `crowdfledger` contract.

