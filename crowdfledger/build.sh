mkdir -p build
eosio-cpp -o build/crowdfledger.wasm src/crowdfledger.cpp -abigen --contract=crowdfledger
rm -rf ~/Desktop/crowdfledger
cp -R build ~/Desktop/crowdfledger