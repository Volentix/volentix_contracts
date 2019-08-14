<h1 class="contract">
   paycore
</h1>
### Parameters
Input parameters:

* `accounts` (Array of keys to which corresponding EOS accounts are transferred tokens)


### Intent
INTENT.
- Validate all requirements have been met for registering as a node.<\br>
- Allows for registering in a campaign.<\br>
- Validates work has been done<\br>
- Transfers payment<\br>

1. In order to be a node you must:<br/>
   1. Possess over X VTX
   2. Run Linux
   3. Possess a valid EOS KeyPair
   4. Satisfy geolocation diversification criteria

2. In order to get paid you must:
   1. Fullfill campaign tasks
   2. Possess a valid EOS account



### Term
TERM. This Contract expires at the conclusion of code execution.

<h1 class="contract">
  Node
</h1>
### Parameters
Input parameters:

* `accounts` (List of accounts)
* `allocation` (the quantity of tokens that allocated to each account)

Implied parameters:

* `account_name` (name of the party invoking and signing the contract)

### Intent
INTENT. The intention of the author and the invoker of this contract pay vdexnodes and add/remove them from an active list

### Term
TERM. This Contract expires at the conclusion of code execution.
