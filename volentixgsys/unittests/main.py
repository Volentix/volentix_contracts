import unittest
from eosfactory.eosf import *
from settings import *
import sys
verbosity(VERBOSITY)

CONTRACT_WORKSPACE = sys.path[0] + "/../"

class Test(unittest.TestCase):
    '''
    volentixgsys smart-contract unittests.
    '''
    @classmethod
    def setUpClass(cls):
        reset()
        create_master_account("MASTER")
        create_account("volentixgsys", MASTER, "volentixgsys")
        create_account("test", MASTER, "test")
        create_account("test2", MASTER, "test2")
        create_account("treasury", MASTER, "staider11111")
        create_account("tester1", MASTER, "tester1")
        create_account("tester2", MASTER, "tester2")
        create_account("tester3", MASTER, "tester3")
        
        volentixgsys_contract = Contract(volentixgsys, CONTRACT_WORKSPACE)
        volentixgsys_contract.build(force=False)
        volentixgsys_contract.deploy()

        test_contract = Contract(test, CONTRACT_WORKSPACE + "/test_case1/")
        test_contract.build(force=False)
        test_contract.deploy()

        test_contract2 = Contract(test2, CONTRACT_WORKSPACE + "/test_case2/")
        test_contract2.build(force=False)
        test_contract2.deploy()

        # create VTX token
        volentixgsys.push_action(
            "create", 
            {
                "issuer": volentixgsys,
                "maximum_supply": "10000.0000 VTX",
                "can_freeze": "0",
                "can_recall": "0",
                "can_whitelist": "0"
            }, [volentixgsys])

        # issue 100 VXT to tester1
        volentixgsys.push_action(
            "issue",
            {
                "to": tester1, 
                "quantity": "100.0000 VTX", 
                "memo": ""
            },
            [volentixgsys])

        volentixgsys.push_action(
            "issue",
            {
                "to": test, 
                "quantity": "100.0000 VTX", 
                "memo": ""
            },
            [volentixgsys])

        volentixgsys.push_action(
            "issue",
            {
                "to": test2, 
                "quantity": "100.0000 VTX", 
                "memo": ""
            },
            [volentixgsys])


        test.set_account_permission(
            permission_name="active",
            parent_permission_name="owner",
            authority = {
                "threshold": 1,
                "keys": [
                        {
                            "key": test.active(),
                            "weight": 1        
                        }
                ],
                "accounts": [
                        {
                            "permission": 
                                {
                                    "actor": "test",
                                    "permission": "eosio.code"
                                },
                            "weight": 1
                        }
                ]
            }
        )

        test2.set_account_permission(
            permission_name="active",
            parent_permission_name="owner",
            authority = {
                "threshold": 1,
                "keys": [
                        {
                            "key": test2.active(),
                            "weight": 1        
                        }
                ],
                "accounts": [
                        {
                            "permission": 
                                {
                                    "actor": "staider11111",
                                    "permission": "active"
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
                                    "actor": "test2",
                                    "permission": "eosio.code"
                                },
                            "weight": 1
                        }
                ]
            }
        )        
    
    def test_arestriction_action(self):
        COMMENT(''' 
            Add restriction for account tester_1:
               name: test123,
               permission: test123
        ''')
        volentixgsys.push_action(
            "arestriction",
            {
                "account": tester1, 
                "code": "test123", 
                "permission": "test123"
            },
            [treasury])

        table = volentixgsys.table("restriction", volentixgsys, 
                 lower="tester1", upper="tester1", key_type="name")

        self.assertEqual(
            table.json["rows"][0]["account"], "tester1",
            'wrong key account')
        self.assertEqual(
            table.json["rows"][0]["code"], "test123",
            'wrong code')
        self.assertEqual(
            table.json["rows"][0]["permission"], "test123",
            'wrong permission')


        COMMENT(''' 
            Modify restriction for account tester_1:
               name: test,
               permission: eosio.code
        ''')

        volentixgsys.push_action(
            "arestriction",
            {
                "account": tester1, 
                "code": "test", 
                "permission": "active"
            },
            [treasury])


        table = volentixgsys.table("restriction", volentixgsys, 
                 lower="tester1", upper="tester1", key_type="name")

        self.assertEqual(
            table.json["rows"][0]["account"], "tester1",
            f'wrong key account')
        self.assertEqual(
            table.json["rows"][0]["code"], "test",
            f'wrong code')
        self.assertEqual(
            table.json["rows"][0]["permission"], "active",
            f'wrong permission')

        COMMENT(f'''
        Trying to invoke arestriction action with wrong autority. 
        Only staider11111 allowed to add/modify restrictions.
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixgsys.push_action(
                "arestriction",
                {
                    "account": tester1, 
                    "code": "test111", 
                    "permission": "eosio.code"
                },
                [tester1])

    
    @staticmethod
    def get_vtx_balance(account):
            table = volentixgsys.table("accounts", account)
            return table.json["rows"][0]["balance"]

    def test_arestriction_case1(self):
        COMMENT(f'''
        Test restriction in case 1(see ./test_case1)
        Trying to invoke VTX transfer directly. 
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixgsys.push_action(
                "transfer",
                {
                    "from": tester1,
                    "to": tester2, 
                    "quantity": "50.0000 VTX", 
                    "memo": ""
                },
                [tester1])
        
        COMMENT(f'''
        Transfer VTX to tester2 using test smart contract(see source at ./test_case1/src/)
        ''')
        test.push_action(
            "transfer",
            {
                "from": tester1,
                "to": tester2, 
                "quantity": "50.0000 VTX", 
                "memo": ""
            },
            [tester1])
        self.assertEqual(self.get_vtx_balance(test), "50.0000 VTX", "wrong balance")
        self.assertEqual(self.get_vtx_balance(tester2), "50.0000 VTX", "wrong balance")


    def test_arestriction_case2(self):
        COMMENT('''
        Test restriction in case 2(see ./test_case2)
        ''')
        volentixgsys.push_action(
            "arestriction",
            {
                "account": test2, 
                "code": "staider11111", 
                "permission": "active"
            },
            [treasury])

        COMMENT(f'''
        Transfer VTX to tester3 using test2 smart contract(see source at ./test_case2/src)
        ''')
        test2.push_action(
            "transfer",
            {
                "to": tester3, 
                "quantity": "50.0000 VTX", 
                "memo": ""
            },
            [treasury])

        self.assertEqual(self.get_vtx_balance(test2), "50.0000 VTX", "wrong balance")
        self.assertEqual(self.get_vtx_balance(tester3), "50.0000 VTX", "wrong balance")

        COMMENT('''
            Trying to invoke VTX transfer directly. 
        ''')

        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixgsys.push_action(
                "transfer",
                {
                    "from": test2, 
                    "to": tester3, 
                    "quantity": "5.0000 VTX", 
                    "memo": ""
                },
                [test2])


    def test_erestriction(self):
        COMMENT('''
        Trying to invoke erestriction action with wrong autority. 
        Only staider11111 allowed to add/modify restrictions.
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixgsys.push_action(
                "erestriction",
                {
                    "account": test2,
                },
                [tester1])

        COMMENT('''
        Erase restriction to test2 smart-contract.
        ''')
        volentixgsys.push_action(
            "erestriction",
            {
                "account": test2,
            },
            [treasury])

        table = volentixgsys.table("restriction", volentixgsys, 
                 lower="test2", upper="test2", key_type="name")

        self.assertEqual(
            table.json["rows"], [],
            "erestriction doesn't work")

    @classmethod
    def tearDownClass(cls):
        stop()


if __name__ == "__main__":
    unittest.main()
