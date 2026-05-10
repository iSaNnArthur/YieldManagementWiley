cmake_minimum_required(VERSION 3.20)

project(YieldManagement)

set(CMAKE_CXX_STANDARD 17)

set(CPLEX_ROOT
    "C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2212"
)

include_directories(
    "${CPLEX_ROOT}/concert/include"
    "${CPLEX_ROOT}/cplex/include"
    "${CPLEX_ROOT}/opl/include"
)

link_directories(
    "${CPLEX_ROOT}/opl/lib/x64_windows_msvc14/stat_mda"
)

add_executable(yield_management
    YieldManagement.cpp
)

target_compile_definitions(yield_management PRIVATE
    IL_STD
)

target_link_libraries(yield_management
    opl
    ilocplex
    cplex2212
    concert
    cp
    iljs
)