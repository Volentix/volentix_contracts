import subprocess
import os
import platform
import re
import shutil
import requests
import json
import random
import psutil

home = os.environ['HOME']
os.environ['EOS_SOURCE'] = '~/eos'
if platform.system() == 'Darwin':
    os.environ['NODEOS_DATA'] = home + "/Library/Application\ Support/eosio/nodeos/data"
elif platform.system() == 'Linux':
    os.environ['NODEOS_DATA'] = home + "/.local/share/eosio/nodeos/data/"
os.environ['EOS_NODEOS'] = "/usr/local/eosio/bin/nodeos"
os.environ['EOS_KEOSD'] = "/usr/local/eosio/bin/keosd"
os.environ['CLEOS'] = "/usr/local/eosio/bin/cleos"


class BlockChain():

    def __init__(self):
        #self.producer = "https://api.eosnewyork.io:443"
        self.producer = "http://api.kylin.alohaeos.com"


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


class Wallet():

    def __init__(self):
        self.name = ""
        self.key = ""
        self.ownerPrivateKey = ''
        self.ownerPublicKey = ''
        self.activePrivateKey = ''
        self.activePublicKey = ''

    def erasePrivateKeys(self):
        self.ownerPrivateKey = ""
        self.activePrivateKey = ""


class Order():

    def __init__(self):
        self.to = ""
        self.amount = 0.0000
        self.contract = ""
        self.currency = ""
        self.contractAccountName = ""
        self.stakeCPU = ""
        self.stakeBandWidth = ""
        self.buyRam = 0

    def reset(self):
        self.to = ""
        self.amount = 0.0000
        self.contract = ""
        self.currency = ""
        self.contractAccountName = ""


def setOwnerKeys():
    out = subprocess.check_output(["/usr/local/eosio/bin/cleos", "create", "key", "--to-console"])
    key = out[13:]
    key = key[:-67]
    key2 = out[77:]
    key2 = key2[:-1]
    wallet.ownerPrivateKey = key
    wallet.ownerPublicKey = key2
    print('Owner keys set')
    print(wallet.ownerPrivateKey)


def createEosioWallet():
    createWallet()
    setOwnerKeys()
    out = subprocess.check_output([os.environ['CLEOS'], 'wallet', 'import', '-n', 'eosio', '--private-key', '5Kduj7gFp4jUSEssfiDJcdpJyoKKiquA1r6VQPASpgviPK89mjb'])
    out = subprocess.check_output([os.environ['CLEOS'], 'wallet', 'import', '-n', 'eosio', '--private-key', wallet.ownerPrivateKey])
    print(str(out))


def createvolentixfrstWallet():
    createWallet()
    out = subprocess.check_output([os.environ['CLEOS'], 'wallet', 'import', '-n', 'volentixfrst', '--private-key', '5K3ssx6BdK3pgWAYzAJuUfxaHQs8zFZKdfiSAuSBdm83Sr6pKKn'])
    out = subprocess.check_output([os.environ['CLEOS'], 'wallet', 'import', '-n', 'volentixfrst', '--private-key', '5KDqp7FnHmfHmrXmWx1vA9g7Kt9JXuecUNzTyrZHCDSEGjvoaJ6'])
    print(str(out))


def setContractSteps():
    out = ''
    try:
        #cleos --url http://api.kylin.alohaeos.com  set code eostitandocs eostitandocs.wasm
        #cleos --url http://api.kylin.alohaeos.com  set abi eostitandocs eostitandocs.abi

        out = subprocess.check_output([os.environ['CLEOS'], '--url', blockchain.producer, 'set', 'code', account.name,   os.environ['HOME']+'/eclipse-workspace/ledger/volentixfrst/volentixfrst.wasm'])
        out = subprocess.check_output([os.environ['CLEOS'], '--url', blockchain.producer, 'set', 'abi', account.name,  os.environ['HOME']+'/eclipse-workspace/ledger/volentixfrst/volentixfrst.abi'])
    except:
        out = 'Cannot set contract steps'
    print(str(out))
    print('set contract steps')


def setupContract():
    compileContract()
    order.contract = os.environ['HOME']+'/ledger/volentixfrst'
    setContractSteps()


def compileContract():
    out = subprocess.check_output(['/usr/local/eosio.cdt/bin/eosio-cpp', '-o', os.environ['HOME']+'/eclipse-workspace/ledger/volentixfrst/volentixfrst.wasm' , os.environ['HOME']+'/eclipse-workspace/ledger/volentixfrst/volentixfrst.cpp', '--abigen' ])
    #out = subprocess.check_output(['/usr/local/eosio.cdt/bin/eosio-cpp', '-o', os.environ['HOME'] + '/eclipse-workspace/ledger/-/volentixfrst.wast' , os.environ['HOME'] + '/eclipse-workspace/ledger/volentixfrst/volentixfrst.cpp' ])
    print(str(out))


def createAccount():
    print('creating *************************************************************************************************', account.name, ':account')
    out = ''
    out = subprocess.check_output([os.environ['CLEOS'], '--url', blockchain.producer, 'create', 'account', 'eosio', account.name, "EOS6ywGxmLeBEpt6ko5dZvFwH79Ggyw7k7y36yR5bMQK6Y9pZ6WA4", "EOS6AwgmB2zR9BcCvJCekgQA6c5tVSvQuuKxYYfuSVKJDjDr8U7KA", '-p', 'eosio'])
    print(str(out))


def createWallet():
    print('creating *************************************************************************************************', wallet.name, ':wallet')
    out = subprocess.check_output([os.environ['CLEOS'], 'wallet', 'create', '-n', wallet.name, '-f', wallet.name])
    print(out)


def killKeosd():
    for p in psutil.process_iter(attrs=['pid', 'name']):
        if 'keosd' in p.info['name']:
            pid = str(p.info['pid'])
            out = subprocess.check_output(['kill', pid])


if __name__ == '__main__':
    account = Account()
    order = Order()
    wallet = Wallet()
    blockchain = BlockChain()
    #killKeosd()
    #out = subprocess.check_output(['rm', '-rf', os.environ['HOME'] + '/eosio-wallet/volentixfrst.wallet'])
    #print(out)
    #wallet.name = 'volentixfrst'
    #createvolentixfrstWallet()
    account.name = 'volentixldgr'
    compileContract()
    setupContract()
    #object = f'["volentixfrst", "godaccount", "vtxdistrib", 364000000, "", "", "test","nonce"]'
    #out = subprocess.check_output([os.environ['CLEOS'],'--url', blockchain.producer, 'push', 'action', account.name, 'rcrdtfr', object, '-p', 'volentixldgr' + '@active'])

