mkdir -p build
eosio-cpp -o build/distribution.wasm src/distribution.cpp -abigen --contract=distribution -I .
rm -rf ~/Desktop/distribution
cp -R build ~/Desktop/distribution