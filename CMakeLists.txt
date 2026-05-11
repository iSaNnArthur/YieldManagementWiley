cmake_minimum_required(VERSION 3.20)

project(YieldManagement)

set(CMAKE_CXX_STANDARD 17)

# =====================================================
# CPLEX ROOT
# =====================================================

set(CPLEX_ROOT
    "C:/Program Files/IBM/ILOG/CPLEX_Studio_Community2212"
)

# =====================================================
# INCLUDE
# =====================================================

include_directories(
    "${CPLEX_ROOT}/concert/include"
    "${CPLEX_ROOT}/cplex/include"
)

# =====================================================
# LIBRARIES
# =====================================================

link_directories(
    "${CPLEX_ROOT}/concert/lib/x64_windows_msvc14/stat_mda"
    "${CPLEX_ROOT}/cplex/lib/x64_windows_msvc14/stat_mda"
)

# =====================================================
# EXECUTABLE
# =====================================================

add_executable(yield_management

    YieldManagement.cpp
    model.cpp
    data.cpp
)

# =====================================================
# LINK
# =====================================================

target_link_libraries(yield_management

    concert
    ilocplex
    cplex2212
)

# =====================================================
# WINDOWS
# =====================================================

if(WIN32)

    target_compile_definitions(yield_management PRIVATE

        IL_STD
    )
endif()