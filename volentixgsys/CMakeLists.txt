cmake_minimum_required( VERSION 3.5 )
project ( "Test-App" VERSION 1.0 LANGUAGES NONE
) 

#include( CTest )

add_custom_target( wast ALL COMMAND python3 -c 'from eosfactory.shell.contract import ContractBuilder\; ContractBuilder(\"${CMAKE_SOURCE_DIR}\" ) .build() ' )
