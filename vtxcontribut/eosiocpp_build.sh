mkdir -p build;
contract_name=$(ls src | grep cpp | cut -d. -f1);
docker run --rm  -v $(pwd):/tmp/eosfactory/contracts/ -t maxpetriev/eos_testing_env:1.0.1 /bin/bash -c "eosio-cpp -o build/${contract_name}.wasm src/${contract_name}.cpp -abigen --contract=${contract_name}"




