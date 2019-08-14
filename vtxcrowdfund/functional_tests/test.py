import os, sys, subprocess, pexpect, random, fnmatch, ast
import random, string
import time


# def setPermissions():
#     ##set permissions
#     object = '{"threshold":2,"keys":[],"accounts":[{"permission":{"actor":"vtxtstaccnt1","permission":"active"},"weight":1},{"permission":{"actor":"vtxtstaccnt2","permission":"active"},"weight":1},{"permission":{"actor":"vtxtstaccnt3","permission":"active"},"weight":1}],"waits":[]}'

#     try:
#         out = subprocess.check_output(['cleos', '--url',  'http://api.kylin.alohaeos.com', 'set', 'account', 'permission', 'vtxmltisig11', 'active', object, 'owner', '-p', 'vtxmltisig11@owner'])
#     except Exception as e:
#         print('Could not transfer:' + str(e))
#         out = 'Could not transfer: ' + str(e)

#     print(out.decode("utf-8"))
#     time.sleep(1)
#     object = '{"threshold":2,"keys":[],"accounts":[{"permission":{"actor":"vtxtstaccnt1","permission":"active"},"weight":1},{"permission":{"actor":"vtxtstaccnt2","permission":"active"},"weight":1},{"permission":{"actor":"vtxtstaccnt3","permission":"active"},"weight":1}],"waits":[]}'

#     try:
#         out = subprocess.check_output(['cleos', '--url',  'http://api.kylin.alohaeos.com', 'set', 'account', 'permission', 'vtxmltisig11', 'owner', object, '-p', 'vtxmltisig11@owner'])
#     except Exception as e:
#         print('Could not transfer:' + str(e))
#         out = 'Could not transfer: ' + str(e)

#     print(out.decode("utf-8"))

# ##approve local
# def approve1(proposal_name):
#     try:
#         out = subprocess.check_output(['cleos', '--url', 'http://api.kylin.alohaeos.com', 'multisig','approve', 'vtxtstaccnt1',proposal_name,'{"actor": "vtxtstaccnt2", "permission": "active"}', '-p', 'vtxtstaccnt2@active'])
#     except Exception as e:
#         print('Could not transfer:' + str(e))
#         out = 'Could not transfer: ' + str(e)

#     print(out)

# ##approve local
# def approve2(proposal_name):
#     try:
#         out = subprocess.check_output(['cleos', '--url', 'http://api.kylin.alohaeos.com', 'multisig','approve', 'vtxtstaccnt1',proposal_name,'{"actor": "vtxtstaccnt3", "permission": "active"}', '-p', 'vtxtstaccnt3@active'])
#     except Exception as e:
#         print('Could not transfer:' + str(e))
#         out = 'Could not transfer: ' + str(e)

#     print(out)


# def proposal(proposal_name):
#     #proposal

#     try:
#         out = subprocess.check_output(['cleos', '--url', 'http://api.kylin.alohaeos.com', 'multisig', 'propose',  proposal_name , '[{"actor": "vtxtstaccnt2", "permission": "active"}, {"actor": "vtxtstaccnt3", "permission": "active"}]' , '[{"actor": "vtxmltisig11", "permission": "active"}]', 'volentixfutr', 'txfds', '{"treasury":"vtxtstaccnt1", "account":"vtxtstaccnt2"}', '-p',  'vtxtstaccnt1@active' ])
#         print(out)
#     except Exception as e:
#         print('Could not transfer:' + str(e))
#         out = 'Could not transfer: ' + str(e)
#         print(out)

#     print(out)


# def execute(proposal_name):
#     print('execute')
#     try:
#         out = subprocess.check_output(['cleos', '--url', 'http://api.kylin.alohaeos.com', 'multisig','exec', 'vtxtstaccnt1', proposal_name, '-p', 'vtxmltisig11@active'])
#     except Exception as e:
#         print('Could not transfer:' + str(e))
#         out = 'Could not transfer: ' + str(e)

#     print(out)

# def review(proposal_name):
#     try:
#         out = subprocess.check_output(['cleos', '--url', 'http://api.kylin.alohaeos.com', 'multisig','review', 'vtxtstaccnt1',  proposal_name  ])

#     except Exception as e:
#         print('Could not transfer:' + str(e))
#         out = 'Could not transfer: ' + str(e)
#     print(out.decode("UTF-8"))


# def randomString(stringLength=12):
#     """Generate a random string of fixed length """
#     letters = string.ascii_lowercase
#     return ''.join(random.choice(letters) for i in range(stringLength))

def main():
    # out = ''
    # setPermissions()
    # proposal_name = randomString(12)
    # time.sleep(2)
    # proposal(proposal_name)
    # time.sleep(2)
    # approve2(proposal_name)
    # time.sleep(2)
    # approve1(proposal_name)
    # time.sleep(2)
    # review(proposal_name)
    # time.sleep(5)
    # execute(proposal_name)

if __name__ == '__main__':
    main()
