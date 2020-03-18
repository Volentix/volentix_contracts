include(ExternalProject)
# if no cdt root is given use default path
if(EOSIO_CDT_ROOT STREQUAL "" OR NOT EOSIO_CDT_ROOT)
   find_package(eosio.cdt)
endif()

ExternalProject_Add(
   vtx_project
   SOURCE_DIR ${CMAKE_SOURCE_DIR}/src
   BINARY_DIR ${CMAKE_BINARY_DIR}/volentixstak
   CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${EOSIO_CDT_ROOT}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake
   UPDATE_COMMAND ""
   PATCH_COMMAND ""
   TEST_COMMAND ""
   INSTALL_COMMAND ""
   BUILD_ALWAYS 1
)

# ExternalProject_Add(
#    vtx_test_project
#    SOURCE_DIR ${CMAKE_SOURCE_DIR}/tests
#    BINARY_DIR ${CMAKE_BINARY_DIR}/tests
#    CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${EOSIO_CDT_ROOT}/lib/cmake/eosio.cdt/EosioWasmToolchain.cmake
#    UPDATE_COMMAND ""
#    PATCH_COMMAND ""
#    TEST_COMMAND ""
#    INSTALL_COMMAND ""
#    BUILD_ALWAYS 1
# )

### add a new test
# include (CTest)
# enable_testing()
# add_test(vtx_test ${CMAKE_BINARY_DIR}/tests/vtx_test)
