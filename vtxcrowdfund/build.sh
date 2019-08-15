mkdir -p build
eosio-cpp -o build/vtxcrowdfund.wasm src/vtxcrowdfund.cpp -abigen --contract=vtxcrowdfund
rm -rf ~/Desktop/vtxcrowdfund
cp -R build ~/Desktop/vtxcrowdfund