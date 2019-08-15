import subprocess
import os
import platform
import re
import shutil
import requests
import json
import random
from time import sleep

home = os.environ['HOME']
os.environ['EOS_SOURCE'] = home + "/eos"
if platform.system() == 'Darwin':
    os.environ['NODEOS_DATA'] = home + "/Library/Application\ Support/eosio/nodeos/data"
elif platform.system() == 'Linux':
    os.environ['NODEOS_DATA'] = home + "/.local/share/eosio/nodeos/data/"
os.environ['EOS_NODEOS'] = "/usr/local/eosio/bin/nodeos"
os.environ['EOS_KEOSD'] = "/usr/local/eosio/bin/keosd"
os.environ['CLEOS'] = "/usr/local/eosio/bin/cleos"



class Table():

    def __init__(self):
        self.contract = ""
        self.table = ""
        self.body = []

class BlockChain():

    def __init__(self):
        
        self.producer = "http://api.kylin.alohaeos.com"
        #self.producer = "http://mainnet.eoscalgary.io:80"
        #self.producer = "http://127.0.0.1:8888"


class Account():

    def __init__(self):
        self.name = ""
        self.creator = ""
        self.receiver = ""
        self.creatorOwnerKey = ""
        self.creatorActiveKey = ""
        self.cpu = ""
        self.bandwidth = ""
        self.ram = ""

def getTable():
        out = ""
        accum = ""
        entries = []
        #if self.blockchain.testProducer == '' or self.account.name == '' or self.table.contract == '' or self.table.table == '':
         #   return
#         try:
        ub = 1000
        lb = 0
        count = 1
#                 
        while(1):
#                 
            print(ub)
            #print('********************************************************************************************************************')    
            print(lb)
            
            out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'get', 'table', account.name, table.contract, table.table, '-U', str(ub), '-L', str(lb)])
                        
            #print(str(out))   
            accum = accum + str(out) 
            o = json.loads(out)
            d = o['rows']
            if len(d) == 0:
                break
            ub = ub * (count + 1)
            lb = ub - 1000   
        print((str(accum)))    

 
if __name__ == '__main__':
    account = Account()
    blockchain = BlockChain()
    table = Table()
    table.table = 'entry'
    table.contract = 'volentixfrst'
    account.name = 'volentixfrst'       
                                                                                                      
     object = '["114vtxledger", "vtxdistrib", "vtxtrust", 10000000000.00000000, "EOS$$$$$$$$$$$$$$$$$$$$$$$$$$", "manual-SC", "fzcc30e5-5ac6-462c-b62d-4cd65f52893e"]'
     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 4800000.00000000 , "EOS7M4xu4AhD9VLWhEVEfpQgo9h34PiqYKLZY1dyuirWBA3K81B9k", "manual-SC", "eea0ac0d-0d98-4409-bdf3-aa1fdba026be"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 2000000.00000000 , "EOS8L9E8gzFuLnKKayiniaqa3HHBJktSfZmitkfVUqu2rwY6N2Mq", "manual-SC", "9c68afa5-fe93-4238-9450-51657608e9be"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 200000.00000000 , "EOS5VnXE3Hk299gLbbBbvWurYd1MTjaLKkq1dHoN838PejpxND8qo", "manual-SC", "d490d620-67b8-49c7-abf7-41b1e378a8d8"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 37693.00000000 , "EOS6GZhU2SPcAgS5WqvKgpF9xnaPQDrtRzueXD3Zr31UBkwN9yxp", "manual-SC", "52c1e3f1-b4f7-4af9-9560-42062ffc7a11"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 412000.00000000 , "EOS69zAH52Tx9D1zA9EXaaxnw8y3v61SwiwohHYUDXerLNGE89PAE", "manual-SC", "0575a794-49ed-4bfa-aac4-b66a0c750de4"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     ####
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 1182779.00000000 , "EOS6cfHkzvTUgEaAzMH6DA8i73VF2S2T4BYc4FXYqHwpACtKvzv4U", "manual-SC", "abd09226-616f-4124-90d7-f00c972d77d1"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 89370.00000000 , "EOS6cfHkzvTUgEaAzMH6DA8i73VF2S2T4BYc4FXYqHwpACtKvzv4U", "manual-SC", "87183121-94f0-4720-a52d-0ba8b996f2b9"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#     ####
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 1359375.00000000 , "EOS7LaNePyHTeTTHw9gC3iYGXsNXjuk5xd2WKLfKviSm791hRW51Q", "manual-SC", "511e845f-b446-4fa8-9b47-4e82daa9642f"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 200000.00000000 , "EOS7LaNePyHTeTTHw9gC3iYGXsNXjuk5xd2WKLfKviSm791hRW51Q", "manual-SC", "f0b7e7f3-a6db-4d0f-8d2e-fed87c606cfc"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 371875.00000000 , "EOS7LaNePyHTeTTHw9gC3iYGXsNXjuk5xd2WKLfKviSm791hRW51Q", "manual-SC", "9cd97bb1-3b77-419a-909a-31466d1622eb"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#     ### 
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 500000.00000000 , "EOS7aHprWaLzyv7BEZYxXx9GToJjtVXqD6tbn7EvvwhfMcuQpS15s", "manual-SC", "e21fe4d7-16f0-45be-879f-4fd7411a458b"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
#      
#     object = '["volentixfrst", "vtxdistrib", "vtxtrust", 204790.00000000 , "EOS73qspFY5jMFbHmzunNv3w4imXU3QsrCvQA3YsvsttxSeV9XMb1", "manual-SC", "c1aaa583-4172-4deb-a158-63522cb3e54c"]'
#     out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
    
    #object = '["volentixfrst", "vtxdistrib", "vtxtrust", 0.00000001 , "Decimal test", "manual-SC", "13245"]'
    #out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
    #object = '["volentixfrst", "vtxdistrib", "vtxtrust", 0.01234567 , "Decimal test", "manual-SC", "12345"]'
    #out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
    
    
    
    
    
    
    
    #getTable()
    #print(out)
    #print('************************************************************************************************************************************************************')
    #out = subprocess.check_output([os.environ['CLEOS'], '--url', blockchain.producer, 'get', 'table', account.name, account.name, 'entry', '-l', '10000' ])
    #print(out)
 
