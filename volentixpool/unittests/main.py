import unittest
from eosfactory.eosf import *
from settings import *
import sys
verbosity(VERBOSITY)

CONTRACT_WORKSPACE = sys.path[0] + "/../"

class Test(unittest.TestCase):
    '''
    volentixpool smart-contract unittests.
    '''
    @classmethod
    def setUpClass(cls):
        reset()
        create_master_account("MASTER")
        create_account("volentixpool", MASTER, "volentixpool" )
        create_account("treasury", MASTER, "staider11111")
        create_account("volentixgsys", MASTER, "volentixgsys")
        create_account("tester1", MASTER, "tester1")
        create_account("tester2", MASTER, "tester2")
        
        volentixpool_contract = Contract(volentixpool, CONTRACT_WORKSPACE)
        volentixpool_contract.build(force=False)
        volentixpool_contract.deploy()

        token_contract = Contract(volentixgsys, "/usr/local/eosfactory/contracts/eosio_token")
        token_contract.build(force=False)
        token_contract.deploy()

        volentixgsys.push_action(
            "create", 
            {
                "issuer": volentixgsys,
                "maximum_supply": "500000000.00000000 VTX"
            }, [volentixgsys])

        volentixgsys.push_action(
            "issue", 
            {
                "to": volentixpool,
                "quantity": "500000000.00000000 VTX",
                "memo": ""
            }, [volentixgsys])
    

        volentixpool.set_account_permission(
            permission_name="active",
            parent_permission_name="owner",
            authority = {
                "threshold": 1,
                "keys": [
                        {
                            "key": volentixpool.active(),
                            "weight": 1        
                        }
                ],
                "accounts": [
                        {
                            "permission": 
                                {
                                    "actor": "volentixpool",
                                    "permission": "eosio.code"
                                },
                            "weight": 1
                        }
                ]
            }
        )

        treasury.set_account_permission(
            permission_name="active",
            parent_permission_name="owner",
            authority = {
                "threshold": 1,
                "keys": [
                        {
                            "key": treasury.active(),
                            "weight": 1        
                        }
                ],
                "accounts": [
                        {
                            "permission": 
                                {
                                    "actor": "volentixpool",
                                    "permission": "eosio.code"
                                },
                            "weight": 1
                        }
                ]
            }
        )    

    @staticmethod
    def get_vtx_balance(account):
            table = volentixgsys.table("accounts", account)
            return table.json["rows"][0]["balance"]

    def test_payproducer_action(self):
        COMMENT('''
            Transfer 100 VTX to tester1.
        ''')

        volentixpool.push_action(
            "payproducer",
            {
                "account": tester1,
                "quantity": "100.00000000 VTX"
            },
            [treasury, tester1])
        
        self.assertEqual(
            self.get_vtx_balance(tester1),
            "100.00000000 VTX",
            "wrong balance"
            )

        COMMENT(f'''
        Trying to invoke payproducer action without treasury authority. 
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixpool.push_action(
                "payproducer",
                {
                    "account": tester1,
                    "quantity": "100.00000000 VTX"
                },
                [tester1])

    def test_payliquid_action(self):
        COMMENT('''
            Transfer 100 VTX to tester2.
        ''')

        volentixpool.push_action(
            "payliquid",
            {
                "account": tester2,
                "quantity": "100.00000000 VTX"
            },
            [treasury, tester2])
        
        self.assertEqual(
            self.get_vtx_balance(tester2),
            "100.00000000 VTX",
            "wrong balance"
            )

        COMMENT(f'''
        Trying to invoke payliquid action without treasury authority. 
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixpool.push_action(
                "payliquid",
                {
                    "account": tester2,
                    "quantity": "100.00000000 VTX"
                },
                [tester2])

    @classmethod
    def tearDownClass(cls):
        stop()


if __name__ == "__main__":
    unittest.main()
