cleos wallet unlock -n VTX --password $(cat key.txt)
apiEndpoint="http://jungle2.cryptolions.io"
cleos -u $apiEndpoint set contract volentixstak $PWD/../build/volentixstak/
cleos wallet lock -n VTX