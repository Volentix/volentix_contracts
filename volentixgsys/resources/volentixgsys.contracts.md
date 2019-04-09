<h1 class="contract">
   create
</h1>
### Parameters
Input parameters:

* `issuer` (name of the party issuing tokens)

Implied parameters: 

* `account_name` (name of the party invoking and signing the contract)

### Intent
INTENT. The intention of the author and the invoker of this contract is to print output. It shall have no other effect.

### Term
TERM. This Contract expires at the conclusion of code execution.

<h1 class="contract">
   issue
</h1>
### Parameters
Input parameters:

* `to` (name of the party the tokens are issued to)
* `quantity` (the quantity of tokens issued)
* `memo` (a message displayed when the action executed)

Implied parameters: 

* `account_name` (name of the party invoking and signing the contract)

### Intent
INTENT. The intention of the author and the invoker of this contract is to print output. It shall have no other effect.

### Term
TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">
   arestriction
</h1>
### Parameters
Input parameters:

* `account` (name of the party that restriction added)
* `code` (name of the party that permission will be required for transfers from account balance)
* `permission` (code permission level that will be required for transfers from account balance)

Implied parameters: 

* `account_name` (name of the party invoking and signing the contract)

### Intent
INTENT. The intention of the author and the invoker of this contract is to add a restriction on transfers tokens from the account balance. It shall have no other effect.

### Term
TERM. This Contract expires at the conclusion of code execution.


<h1 class="contract">
   erestriction
</h1>
### Parameters
Input parameters:

* `account` (name of the party that restriction erased)

Implied parameters: 

* `account_name` (name of the party invoking and signing the contract)

### Intent
INTENT. The intention of the author and the invoker of this contract is to erase a restriction on transfers tokens from the account balance. It shall have no other effect.

### Term
TERM. This Contract expires at the conclusion of code execution.