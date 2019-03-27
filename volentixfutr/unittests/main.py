import unittest
from eosfactory.eosf import *
from settings import *
verbosity([VERBOSITY])#, Verbosity.TRACE, Verbosity.DEBUG])

# Actors of the test:
MASTER = MasterAccount()
HOST = Account()
ALICE = Account()
BOB = Account()
CAROL = Account()

class Test(unittest.TestCase):
    '''Unittest class definition.
    '''
    @classmethod
    def setUpClass(cls):
        SCENARIO('''
        Create test accounts.
        ''')
        reset()
        create_master_account("MASTER")

        COMMENT('''
        Create test accounts:
        ''')
        create_account("host", MASTER)
        contract = Contract(host, CONTRACT_WORKSPACE)
        contract.build()
        create_account("ALICE", MASTER, 'alice123')
        create_account("BOB", MASTER)
        create_account("CAROL", MASTER)
    
    @classmethod
    def tearDownClass(cls):
        stop()


if __name__ == "__main__":
    unittest.main()
