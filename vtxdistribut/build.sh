mkdir -p build
eosio-cpp -o build/vtxdistribut.wasm src/vtxdistribut.cpp --abigen --contract=vxtdistribut -I .
rm -rf ~/Desktop/vtxdistribut
cp -R build ~/Desktop/vtxdistribut