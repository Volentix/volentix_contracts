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
        create_account("treasury", MASTER, "staider11111")
        create_account("tester1", MASTER, "tester1")
        create_account("tester2", MASTER, "tester2")
        
        volentixgsys_contract = Contract(volentixgsys, CONTRACT_WORKSPACE)
        volentixgsys_contract.build(force=False)
        volentixgsys_contract.deploy()

        test_contract = Contract(test, CONTRACT_WORKSPACE + "/test_contract/")
        test_contract.build(force=False)
        test_contract.deploy()

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

        # tester1.set_account_permission(
        #     permission_name="active",
        #     parent_permission_name="owner",
        #     authority = {
        #         "threshold": 1,
        #         "keys": [
        #                 {
        #                     "key": tester1.active(),
        #                     "weight": 1        
        #                 }
        #         ],
        #         "accounts": [
        #                 {
        #                     "permission": 
        #                         {
        #                             "actor": "test",
        #                             "permission": "eosio.code"
        #                         },
        #                     "weight": 1
        #                 }
        #         ]
        #     }
        # )

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
            f'wrong key account')
        self.assertEqual(
            table.json["rows"][0]["code"], "test123",
            f'wrong code')
        self.assertEqual(
            table.json["rows"][0]["permission"], "test123",
            f'wrong permission')


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


    def test_arestriction_functionality(self):
        COMMENT(f'''
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
        Transfer VTX from to tester2 using test smart contract(see source at ./test_contract)
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



    @classmethod
    def tearDownClass(cls):
        stop()


if __name__ == "__main__":
    unittest.main()
