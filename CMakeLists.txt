cmake_minimum_required(VERSION 2.8)
project(cpsl)

find_package(BISON)
find_package(FLEX)

bison_target(CpslParser FrontEnd/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
flex_target(CpslScanner FrontEnd/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
add_flex_bison_dependency(CpslScanner CpslParser)

enable_testing()
include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/FrontEnd
  ${CMAKE_CURRENT_SOURCE_DIR}/Internal
#  ${Boost_INCLUDE_DIRS}
#  ${WINDOWS_GNU}
)

#set(EXTRA_COMPILE_FLAGS "-g -std=c++0x")
set(EXTRA_COMPILE_FLAGS "-g -std=gnu++0x")
#set(LCOV_FLAGS "--coverage")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_COMPILE_FLAGS} ${LCOV_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LCOV_FLAGS}")

set(main_srcs main.cpp)
source_group("Main" FILES ${cpsl_srcs})

set(frontend_srcs
  FrontEnd/FrontEnd.cpp
  ${BISON_CpslParser_OUTPUTS}
  ${FLEX_CpslScanner_OUTPUTS}
)
source_group("FrontEnd" FILES ${frontend_srcs})
set(cpsl_srcs ${frontend_srcs})

set(internal_srcs
  Internal/ControlFlow.cpp
  Internal/ExpressionType.hpp
  Internal/Expressions.cpp
  Internal/Functions.cpp
  Internal/HighMIPS.hpp
  Internal/LinkedList.hpp
  Internal/Locations.hpp
  Internal/MIPS.hpp
  Internal/Register.hpp
  Internal/RegisterPool.hpp
  Internal/SymbolTable.hpp
  Internal/UserDefinedDataTypes.cpp
)
source_group("Internal" FILES ${internal_srcs})
list(APPEND cpsl_srcs ${internal_srcs})

set(CMAKE_INSTALL_RPATH ".")
add_library(cpsl_common STATIC ${cpsl_srcs})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_target_properties(cpsl_common PROPERTIES MACOSX_RPATH "@loader_path/../lib")
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

add_executable(cpsl ${main_srcs})

target_link_libraries(cpsl_common ${FLEX_LIBRARIES} ${BISON_LIBRARIES})
target_link_libraries(cpsl cpsl_common)

find_program(BASH_PROGRAM bash)
if (BASH_PROGRAM)
    add_test(NAME IntegrationTest COMMAND ${BASH_PROGRAM} ${CMAKE_CURRENT_BINARY_DIR}/tester/tester.sh)
endif (BASH_PROGRAM)

