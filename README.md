# Account-Reconsiliation
Given a csv of transactions states, 
in the transaction csv we can find:
- an account number
- an operation (withdraw or deposit)
- an amount
in the state scs we have
- an account number
- an amount

The program will analyse the transactions and compare them to the state of the account.
It will print out to the command line which accounts are not the same.

Provide test files as command line argument 
.tv testFiles/state.csv testFiles/transactions.csv
