project(volentixstak)

set(EOSIO_WASM_OLD_BEHAVIOR "Off")
find_package(eosio.cdt)

add_contract( volentixstak volentixstak volentixstak.cpp )
target_include_directories( volentixstak PUBLIC ${CMAKE_SOURCE_DIR}/../include/ )
# target_ricardian_directory( volentixstak ${CMAKE_SOURCE_DIR}/../ricardian )
