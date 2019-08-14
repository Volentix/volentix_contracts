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
        
        #self.producer = "http://api.kylin.alohaeos.com"
        self.producer = "http://mainnet.eoscalgary.io:80"
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
                                                                                                      
    
    #object = '["volentixfrst", "vtxdistrib", "vtxtrust", 0.00000001 , "Decimal test", "manual-SC", "13245"]'
    #out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
    #object = '["volentixfrst", "vtxdistrib", "vtxtrust", 0.01234567 , "Decimal test", "manual-SC", "12345"]'
    #out = subprocess.check_output(['cleos', '--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixfrst' + '@active'])
    
    
    
    
    
    
    
    #getTable()
    #print(out)
    #print('************************************************************************************************************************************************************')
    #out = subprocess.check_output([os.environ['CLEOS'], '--url', blockchain.producer, 'get', 'table', account.name, account.name, 'entry', '-l', '10000' ])
    #print(out)
 
