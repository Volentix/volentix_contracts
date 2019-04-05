import unittest
from eosfactory.eosf import *
from settings import *
import sys
verbosity(VERBOSITY)

CONTRACT_WORKSPACE = sys.path[0] + "/../"

class Test(unittest.TestCase):
    '''
    volentixsale smart-contract unittests.
    '''
    @classmethod
    def setUpClass(cls):
        reset()
        create_master_account("MASTER")
        create_account("volentixsale", MASTER, "volentixsale" )
        create_account("treasury", MASTER, "staider11111")
        create_account("vtxstsupport", MASTER, "vtxstsupport")
        create_account("vtxmesupport", MASTER, "vtxmesupport")
        create_account("vtxsmsupport", MASTER, "vtxsmsupport")
        create_account("vtxmssupport", MASTER, "vtxmssupport")
        create_account("volentixgsys", MASTER, "volentixgsys")
        create_account("tester", MASTER, "tester")
        
        volentixfutr_contract = Contract(volentixsale, CONTRACT_WORKSPACE)
        volentixfutr_contract.build(force=False)
        volentixfutr_contract.deploy()

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
                "to": volentixsale,
                "quantity": "500000000.00000000 VTX",
                "memo": ""
            }, [volentixgsys])
    

        volentixsale.set_account_permission(
            permission_name="active",
            parent_permission_name="owner",
            authority = {
                "threshold": 1,
                "keys": [
                        {
                            "key": volentixsale.active(),
                            "weight": 1        
                        }
                ],
                "accounts": [
                        {
                            "permission": 
                                {
                                    "actor": "volentixsale",
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
                                    "actor": "volentixsale",
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

    @staticmethod
    def get_limits_record(account):
        table = volentixsale.table("limits", volentixsale,
                lower=account, upper=account, key_type="name")
        return table.json["rows"][0]["limit"], table.json["rows"][0]["already_transfered"]

    def test_init_action(self):
        SCENARIO('''
        Test init action. Init acction should init limits for vtxstsupport, vtxmesupport, vtxmssupport and vtxmssupport.
        ''')
        volentixsale.push_action(
            "init",
            {"foo": "bar"},
            [treasury])

        COMMENT('''
            Test limit for vtxstsupport
        ''')

        self.assertEqual(
            self.get_limits_record("vtxstsupport"), 
            ("99000000.00000000 VTX", "0.00000000 VTX"),
            "wrong limit")

        COMMENT('''
            Test limit for vtxmesupport
        ''')

        self.assertEqual(
            self.get_limits_record("vtxmesupport"), 
            ("99000000.00000000 VTX", "0.00000000 VTX"),
            "wrong limit")

        COMMENT('''
            Test limit for vtxsmsupport
        ''')

        self.assertEqual(
            self.get_limits_record("vtxsmsupport"), 
            ("99000000.00000000 VTX", "0.00000000 VTX"),
            "wrong limit")


        COMMENT('''
            Test limit for vtxmssupport
        ''')

        self.assertEqual(
            self.get_limits_record("vtxmssupport"), 
            ("67000000.00000000 VTX", "0.00000000 VTX"),
            "wrong limit") 


        COMMENT(f'''
        Trying to invoke init action with wrong autority. 
        Only staider11111 allowed to init state.
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixsale.push_action(
            "init",
            {"foo": "bar"},
            [tester])

    def test_transfer_action(self):
        SCENARIO('''
        Test transfer action. Transfer VTX to vtxstsupport, vtxmesupport, vtxsmsupport and vtxmssupport.
        ''')
        def transfer_to(account, limit, name):
            COMMENT(f'''
                Transfer VTX to {account}
            ''')
            volentixsale.push_action(
            "transfer",
            {
                "to": account, 
                "quantity": "1000000.00000000 VTX"
            },
            [treasury, account])
            
            self.assertEqual(
                self.get_limits_record(name), 
                (f"{limit}.00000000 VTX", "1000000.00000000 VTX"),
                "wrong limit")
            
            self.assertEqual(
                self.get_vtx_balance(account),
                "1000000.00000000 VTX",
                "wrong balance")

            volentixsale.push_action(
            "transfer",
            {
                "to": account, 
                "quantity": f"{limit - 1000000}.00000000 VTX"
            },
            [treasury, account])

            self.assertEqual(
                self.get_limits_record(name), 
                (f"{limit}.00000000 VTX", f"{limit}.00000000 VTX"),
                "wrong limit")
            
            self.assertEqual(
                self.get_vtx_balance(account),
                f"{limit}.00000000 VTX",
                "wrong balance")

            COMMENT(f'''
                Transfer VTX to {account} over limit
            ''')

            with self.assertRaises(errors.Error):
                volentixsale.push_action(
                "transfer",
                {
                    "to": account, 
                    "quantity": "2000000.00000000 VTX"
                },
            [treasury, account])
        
        transfer_to(vtxstsupport, 99000000, "vtxstsupport")
        transfer_to(vtxmesupport, 99000000, "vtxmesupport")
        transfer_to(vtxsmsupport, 99000000, "vtxsmsupport")
        transfer_to(vtxmssupport, 67000000, "vtxmssupport")

        COMMENT(f'''
        Trying to invoke transfer action without staider11111 autority.
        ''')
        with self.assertRaises(errors.MissingRequiredAuthorityError):
            volentixsale.push_action(
            "transfer",
            {
                "to": tester, 
                "quantity": "1000000.00000000 VTX"
            },
            [tester])

        COMMENT(f'''
        Trying to invoke transfer action with wrong account.
        ''')
        with self.assertRaises(errors.Error):
            volentixsale.push_action(
            "transfer",
            {
                "to": tester, 
                "quantity": "2000000.00000000 VTX"
            },
            [treasury, tester])

    #def test_afacilitator_action(self):
    #     COMMENT('''
    #     Add facilitator1 with allocation 100.0000 VTX.
    #     ''')
    #     COMMENT('''
    #     Add facilitator1:
    #         {
    #             "account": "facilitator1",
    #             "allocation": "100.00000000 VTX"
    #         }
    #     ''')
    #     volentixfutr.push_action(
    #         "afacilitator",
    #         {
    #             "account": "facilitator1",
    #             "allocation": "100.00000000 VTX"
    #         },
    #         facilitators_modify_treasury
    #     )
    #     table = volentixfutr.table("facilitators", volentixfutr, 
    #             lower="facilitator1", upper="facilitator1", key_type="name")
    #     self.assertEqual(
    #         table.json["rows"][0]["key"], "facilitator1",
    #         f'wrong key: expected - facilitator1, got {table.json["rows"][0]["key"]}')
    #     self.assertEqual(
    #         table.json["rows"][0]["allocation"], "100.00000000 VTX",
    #         f'wrong allocation: expected 100.0000 VTX, got: {table.json["rows"][0]["allocation"]}')
    #     self.assertEqual(
    #         table.json["rows"][0]["already_allocated"], "0.00000000 VTX",
    #         f'wrong already allocated: expected 0.0000 VTX, got: {table.json["rows"][0]["allocation"]}')

    #     COMMENT('''
    #     Set facilitator1 allocation to 150.0000 VTX.
    #     ''')        
    #     COMMENT('''
    #     Modify facilitator1 record:
    #         {
    #             "account": "facilitator1",
    #             "allocation": "150.0000 VTX"
    #         }
    #     ''')
    #     volentixfutr.push_action(
    #         "afacilitator",
    #         {
    #             "account": "facilitator1",
    #             "allocation": "150.00000000 VTX"
    #         },
    #         facilitators_modify_treasury
    #     )
    #     table = volentixfutr.table("facilitators", volentixfutr, 
    #         lower="facilitator1", upper="facilitator1", key_type="name")
    #     self.assertEqual(
    #         table.json["rows"][0]["key"], "facilitator1",
    #         f'wrong key: expected - facilitator1, got {table.json["rows"][0]["key"]}')
    #     self.assertEqual(
    #         table.json["rows"][0]["allocation"], "150.00000000 VTX",
    #         f'wrong allocation: expected 150.0000 VTX, got: {table.json["rows"][0]["allocation"]}')
    #     self.assertEqual(
    #         table.json["rows"][0]["already_allocated"], "0.00000000 VTX",
    #         f'wrong already allocated: expected 0.0000 VTX, got: {table.json["rows"][0]["allocation"]}')

    #     COMMENT(f'''
    #     Trying to invoke afacilitator action with wrong autority. 
    #     Only vtxtstaccnt1 allowed to add/modify facilitator records.
    #     ''')
    #     with self.assertRaises(errors.MissingRequiredAuthorityError):
    #         volentixfutr.push_action(
    #             "afacilitator",
    #             {
    #                 "account": "facilitator2",
    #                 "allocation": "200.00000000 VTX"
    #             },
    #             txfds_treasury
    #         )

    # def test_erase_action(self):
    #     COMMENT('''
    #     Add facilitator2 with allocation 200.0000 VTX and than erase it.
    #     ''')
    #     volentixfutr.push_action(
    #         "afacilitator",
    #         {
    #             "account": "facilitator2",
    #             "allocation": "200.00000000 VTX"
    #         },
    #         facilitators_modify_treasury
    #     )
    #     volentixfutr.push_action(
    #         "erase",
    #         {
    #             "account": "facilitator2"
    #         },
    #         facilitators_modify_treasury
    #     )
    #     table = volentixfutr.table("facilitators", volentixfutr, 
    #             lower="facilitator2", upper="facilitator2", key_type="name")
    #     self.assertEqual(table.json["rows"], [], "facilitator2 record wasn't erased")
        
    #     COMMENT(f'''
    #     Trying to invoke erase action with wrong autority. 
    #     Only vtxtstaccnt1 allowed to erase facilitator records.
    #     ''')
    #     with self.assertRaises(errors.MissingRequiredAuthorityError):
    #         volentixfutr.push_action(
    #             "erase",
    #             {
    #                 "account": "facilitator2"
    #             },
    #             txfds_treasury
    #             )

    # def test_txfds(self):
    #     COMMENT('''
    #         Add facilitator3 with 100.0000 VTX alloaction and
    #         test txfds in following cases:
    #             - missing txfds_treasury autority(should raise MissingRequiredAuthorityError)
    #             - half-month after start time (should raise error "liquid allocation is 0, try later")
    #             - one month after start time (should transfer 2.0833 VTX to facilitator3)
    #             - one month and two weeks aster start time (should raise error "liquid allocation is 0, try later")
    #             - two month after start time (should 2.0833 VTX to facilitator3)
    #             - two month and two weeks after start time (should raise error "liquid allocation is 0, try later")
    #             - 47 month after start time (should tranfer 93.75 VTX)
    #             - 48,5 month after start time (should tranfer remaining 2.0834 VTX)
    #             - 49 month after start time (should raise "all available VTX already allocated" error)

    #          ''')
    #     token_contract = Contract(volentixgsys, "/usr/local/eosfactory/contracts/eosio_token")
    #     token_contract.build(force=False)
    #     token_contract.deploy()
    #     volentixgsys.push_action(
    #         "create", 
    #         {
    #             "issuer": volentixgsys,
    #             "maximum_supply": "10000.00000000 VTX",
    #             "can_freeze": "0",
    #             "can_recall": "0",
    #             "can_whitelist": "0"
    #         }, [volentixgsys])

    #     volentixgsys.push_action(
    #         "issue",
    #         {
    #             "to": volentixfutr, 
    #             "quantity": "1000.00000000 VTX", 
    #             "memo": ""
    #         },
    #         [volentixgsys])

    #     volentixfutr.push_action(
    #         "afacilitator",
    #         {
    #             "account": "facilitator3",
    #             "allocation": "100.00000000 VTX"
    #         },
    #         facilitators_modify_treasury
    #     )

    #     start_time = 1554091200
    #     month = 30*24*60*60



    #     def invoke_txfdsmocked(account, sse_mocked, authority):
    #         volentixfutr.push_action(
    #             "txfdsmocked",
    #             {
    #                 "account": account,
    #                 "sse_mocked": sse_mocked
    #             },
    #             authority
    #         )

    #     def get_already_allocated(account):
    #         table = volentixfutr.table("facilitators", volentixfutr, 
    #                           lower=account, upper=account, key_type="name")
    #         return table.json["rows"][0]["already_allocated"]

    #     def get_vtx_balance(account):
    #         table = volentixgsys.table("accounts", account)
    #         return table.json["rows"][0]["balance"]

    #     # Check missing txfds_treasury autotity
    #     with self.assertRaises(errors.MissingRequiredAuthorityError):
    #         invoke_txfdsmocked(facilitator3, f"{ start_time + month}", [facilitator3])

    #     # Check liquid allocation is 0, try later
    #     with self.assertRaises(errors.Error):
    #         invoke_txfdsmocked(facilitator3, f"{ start_time + month // 2}", [txfds_treasury, facilitator3])

    #     invoke_txfdsmocked(facilitator3, f"{start_time + month}", [txfds_treasury, facilitator3])
    #     self.assertEqual(get_already_allocated("facilitator3"), "2.08333333 VTX", "wrong already_allocated")
    #     self.assertEqual(get_vtx_balance(facilitator3), "2.08333333 VTX", "wrong vtx balance")

    #     with self.assertRaises(errors.Error):
    #         invoke_txfdsmocked(facilitator3, f"{ start_time + month + month // 2}", [txfds_treasury, facilitator3])

    #     invoke_txfdsmocked(facilitator3, f"{ start_time + 2*month}", [txfds_treasury, facilitator3])

    #     self.assertEqual(get_already_allocated("facilitator3"), "4.16666666 VTX", "wrong already_allocated")
    #     self.assertEqual(get_vtx_balance(facilitator3), "4.16666666 VTX", "wrong vtx balance")

    #     with self.assertRaises(errors.Error):
    #         invoke_txfdsmocked(facilitator3, f"{ start_time + 2*month + month // 2}", [txfds_treasury, facilitator3])

    #     invoke_txfdsmocked(facilitator3, f"{ start_time + 47*month}", [txfds_treasury, facilitator3]) 
    #     self.assertEqual(get_already_allocated("facilitator3"), "97.91666666 VTX", "wrong already_allocated")
    #     self.assertEqual(get_vtx_balance(facilitator3), "97.91666666 VTX", "wrong vtx balance")

    #     invoke_txfdsmocked(facilitator3, f"{ start_time + 48*month + month // 2}", [txfds_treasury, facilitator3]) 
    #     self.assertEqual(get_already_allocated("facilitator3"), "100.00000000 VTX", "wrong already_allocated")
    #     self.assertEqual(get_vtx_balance(facilitator3), "100.00000000 VTX", "wrong vtx balance")

    #     # check all available VTX already allocated error        
    #     with self.assertRaises(errors.Error):
    #         invoke_txfdsmocked(facilitator3, f"{ start_time + 49*month}", [txfds_treasury, facilitator3])



    @classmethod
    def tearDownClass(cls):
        stop()


if __name__ == "__main__":
    unittest.main()
