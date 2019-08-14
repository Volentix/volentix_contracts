# ledger
> A recording system for credits and debits on the blockchain

The purpose of the ledger is to record transactions between different accounts before VTX has been
created and distributed. After VTX has been created, the transactions recorded here can be replayed
as actual transfers.

Each of the accounts defined here is an EOS account. Our intial idea is that each transaction is in
a smart contract and consists of a debit and a credit, each stored in its own EOS database.

There are many accounts that are kept track of, but the ones mentioned here are:
- Distribution: Holds the VTX for the crowdsale
- Trust: Records how much VTX each person is owed

rcrdtfr(account_name s, string fromaccount, string toaccount, uint amount, string fromkey, string tokey)

*Fields are of string and float type only*

### rcrdtfr(account_name s, string fromaccount, string toaccount, float amount, string tokey, string nonce, string comment)
*account_name, fromaccount and toaccount are mandatory fields*
Specify a transfer from one account to another, and optionally with keys into that account (the keys
being used for the trust account). So for example, if Alice purchased 100 VTX, the call would be:

`rcrdtfr("vtxdistrib, "vtxtrust", 100.00, "", "Alice",string nonce, string comment)`

Internally some calls like this are written to the database:

```
Distribution : debit  100  // -> Trust[Alice]
Trust[Alice] : credit 100  // <- Distribution
```



