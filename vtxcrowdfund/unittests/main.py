import unittest
from eosfactory.eosf import *
from settings import *
import sys
verbosity(VERBOSITY)

CONTRACT_WORKSPACE = sys.path[0] + "/../"

class Test(unittest.TestCase):
    '''
    volentixfutr smart-contract unittests.
    '''
    @classmethod
    def setUpClass(cls):
        reset()
        create_master_account("MASTER")
        create_account("volentixfutr", MASTER, "volentixfutr" )
        create_account("txfds_treasury", MASTER, "staider11111")
        create_account("facilitators_modify_treasury", MASTER, "vtxtstaccnt1")
        create_account("facilitator1", MASTER, "facilitator1")
        create_account("facilitator2", MASTER, "facilitator2")
        create_account("facilitator3", MASTER, "facilitator3")
        create_account("volentixgsys", MASTER, "volentixgsys")
        volentixfutr_contract = Contract(volentixfutr, CONTRACT_WORKSPACE)
        volentixfutr_contract.build(force=False)
        volentixfutr_contract.deploy()
    
    def test_afacilitator_action(self):
        COMMENT('''
        Add facilitator1 with allocation 100.0000 VTX.
        ''')
        COMMENT('''
        Add facilitator1:
            {
                "account": "facilitator1",
                "allocation": "100.00000000 VTX"
            }
        ''')
        volentixfutr.push_action(
            "afacilitator",
            {
                "account": "facilitator1",
                "allocation": "100.00000000 VTX"
            },
            facilitators_modify_treasury
        )
        table = volentixfutr.table("facilitators", volentixfutr, 
                lower="facilitator1", upper="facilitator1", key_type="name")
        self.assertEqual(
            table.json["rows"][0]["key"], "facilitator1",
            f'wrong key: expected - facilitator1, got {table.json["rows"][0]["key"]}')
        self.assertEqual(
            table.json["rows"][0]["allocation"], "100.00000000 VTX",
            f'wrong allocation: expected 100.0000 VTX, got: {table.json["rows"][0]["allocation"]}')
        self.assertEqual(
            table.json["rows"][0]["already_allocated"], "0.00000000 VTX",
            f'wrong already allocated: expected 0.0000 VTX, got: {table.json["rows"][0]["allocation"]}')

        COMMENT('''
        Set facilitator1 allocation to 150.0000 VTX.
        ''')        
        COMMENT('''
        Modify facilitator1 record:
            {
                "account": "facilitator1",
                "allocation": "150.0000 VTX"
            }
        ''')
        volentixfutr.push_action(
            "afacilitator",
            {
                "account": "facilitator1",
                "allocation": "150.00000000 VTX"
            },
            facilitators_modify_treasury
        )
        table = volentixfutr.table("facilitators", volentixfutr, 
            lower="facilitator1", upper="facilitator1", key_type="name")
        self.assertEqual(
            table.json["rows"][0]["key"], "facilitator1",
            f'wrong key: expected - facilitator1, got {table.json["rows"][0]["key"]}')
        self.assertEqual(
            table.json["rows"][0]["allocation"], "150.00000000 VTX",
            f'wrong allocation: expected 150.0000 VTX, got: {table.json["rows"][0]["allocation"]}')
        self.assertEqual(
            table.json["rows"][0]["already_allocated"], "0.00000000 VTX",
            f'wrong already allocated: expected 0.0000 VTX, got: {table.json["rows"][0]["allocation"]}')

        COMMENT(f'''
        Trying to invoke afacilitator action with wrong autority. 
        Only vtxtstaccnt1 allowed to add/modify facilitator records.
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixfutr.push_action(
                "afacilitator",
                {
                    "account": "facilitator2",
                    "allocation": "200.00000000 VTX"
                },
                txfds_treasury
            )

    def test_erase_action(self):
        COMMENT('''
        Add facilitator2 with allocation 200.0000 VTX and than erase it.
        ''')
        volentixfutr.push_action(
            "afacilitator",
            {
                "account": "facilitator2",
                "allocation": "200.00000000 VTX"
            },
            facilitators_modify_treasury
        )
        volentixfutr.push_action(
            "erase",
            {
                "account": "facilitator2"
            },
            facilitators_modify_treasury
        )
        table = volentixfutr.table("facilitators", volentixfutr, 
                lower="facilitator2", upper="facilitator2", key_type="name")
        self.assertEqual(table.json["rows"], [], "facilitator2 record wasn't erased")
        
        COMMENT(f'''
        Trying to invoke erase action with wrong autority. 
        Only vtxtstaccnt1 allowed to erase facilitator records.
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixfutr.push_action(
                "erase",
                {
                    "account": "facilitator2"
                },
                txfds_treasury
                )

    def test_txfds(self):
        COMMENT('''
            Add facilitator3 with 100.0000 VTX alloaction and
            test txfds in following cases:
                - missing txfds_treasury autority(should raise MissingRequiredAuthorityError)
                - half-month after start time (should raise error "liquid allocation is 0, try later")
                - one month after start time (should transfer 2.0833 VTX to facilitator3)
                - one month and two weeks aster start time (should raise error "liquid allocation is 0, try later")
                - two month after start time (should 2.0833 VTX to facilitator3)
                - two month and two weeks after start time (should raise error "liquid allocation is 0, try later")
                - 47 month after start time (should tranfer 93.75 VTX)
                - 48,5 month after start time (should tranfer remaining 2.0834 VTX)
                - 49 month after start time (should raise "all available VTX already allocated" error)

             ''')
        token_contract = Contract(volentixgsys, "/usr/local/eosfactory/contracts/eosio_token")
        token_contract.build(force=False)
        token_contract.deploy()
        volentixgsys.push_action(
            "create", 
            {
                "issuer": volentixgsys,
                "maximum_supply": "10000.00000000 VTX",
                "can_freeze": "0",
                "can_recall": "0",
                "can_whitelist": "0"
            }, [volentixgsys])

        volentixgsys.push_action(
            "issue",
            {
                "to": volentixfutr, 
                "quantity": "1000.00000000 VTX", 
                "memo": ""
            },
            [volentixgsys])

        volentixfutr.push_action(
            "afacilitator",
            {
                "account": "facilitator3",
                "allocation": "100.00000000 VTX"
            },
            facilitators_modify_treasury
        )

        start_time = 1554091200
        month = 30*24*60*60

        volentixfutr.set_account_permission(
            permission_name="active",
            parent_permission_name="owner",
            authority = {
                "threshold": 1,
                "keys": [
                        {
                            "key": volentixfutr.active(),
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

        txfds_treasury.set_account_permission(
            permission_name="active",
            parent_permission_name="owner",
            authority = {
                "threshold": 1,
                "keys": [
                        {
                            "key": txfds_treasury.active(),
                            "weight": 1        
                        }
                ],
                "accounts": [
                        {
                            "permission": 
                                {
                                    "actor": "volentixfutr",
                                    "permission": "eosio.code"
                                },
                            "weight": 1
                        }
                ]
            }
        )

        def invoke_txfdsmocked(account, sse_mocked, authority):
            volentixfutr.push_action(
                "txfdsmocked",
                {
                    "account": account,
                    "sse_mocked": sse_mocked
                },
                authority
            )

        def get_already_allocated(account):
            table = volentixfutr.table("facilitators", volentixfutr, 
                              lower=account, upper=account, key_type="name")
            return table.json["rows"][0]["already_allocated"]

        def get_vtx_balance(account):
            table = volentixgsys.table("accounts", account)
            return table.json["rows"][0]["balance"]

        # Check missing txfds_treasury autotity
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            invoke_txfdsmocked(facilitator3, f"{ start_time + month}", [facilitator3])

        # Check liquid allocation is 0, try later
        with self.assertRaises(errors.Error):
            invoke_txfdsmocked(facilitator3, f"{ start_time + month // 2}", [txfds_treasury, facilitator3])

        invoke_txfdsmocked(facilitator3, f"{start_time + month}", [txfds_treasury, facilitator3])
        self.assertEqual(get_already_allocated("facilitator3"), "2.08333333 VTX", "wrong already_allocated")
        self.assertEqual(get_vtx_balance(facilitator3), "2.08333333 VTX", "wrong vtx balance")

        with self.assertRaises(errors.Error):
            invoke_txfdsmocked(facilitator3, f"{ start_time + month + month // 2}", [txfds_treasury, facilitator3])

        invoke_txfdsmocked(facilitator3, f"{ start_time + 2*month}", [txfds_treasury, facilitator3])

        self.assertEqual(get_already_allocated("facilitator3"), "4.16666666 VTX", "wrong already_allocated")
        self.assertEqual(get_vtx_balance(facilitator3), "4.16666666 VTX", "wrong vtx balance")

        with self.assertRaises(errors.Error):
            invoke_txfdsmocked(facilitator3, f"{ start_time + 2*month + month // 2}", [txfds_treasury, facilitator3])

        invoke_txfdsmocked(facilitator3, f"{ start_time + 47*month}", [txfds_treasury, facilitator3]) 
        self.assertEqual(get_already_allocated("facilitator3"), "97.91666666 VTX", "wrong already_allocated")
        self.assertEqual(get_vtx_balance(facilitator3), "97.91666666 VTX", "wrong vtx balance")

        invoke_txfdsmocked(facilitator3, f"{ start_time + 48*month + month // 2}", [txfds_treasury, facilitator3]) 
        self.assertEqual(get_already_allocated("facilitator3"), "100.00000000 VTX", "wrong already_allocated")
        self.assertEqual(get_vtx_balance(facilitator3), "100.00000000 VTX", "wrong vtx balance")

        # check all available VTX already allocated error        
        with self.assertRaises(errors.Error):
            invoke_txfdsmocked(facilitator3, f"{ start_time + 49*month}", [txfds_treasury, facilitator3])



    @classmethod
    def tearDownClass(cls):
        stop()


if __name__ == "__main__":
    unittest.main()
