cmake_minimum_required(VERSION 3.15)

project(
        clockwork
        VERSION 0.1
        DESCRIPTION "HCE Community Engine"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
include_directories(src)
add_compile_options(-march=native)
add_link_options(-march=native)

enable_testing()
set(CMAKE_SKIP_TEST_ALL_DEPENDENCY FALSE)
function(do_test name)
  add_executable(${name} tests/${name}.cpp ${srcs})
  add_test(${name} ${name})
endfunction()

# Keep list sorted
set(srcs
  src/util/vec/avx512.hpp
  src/util/vec/avx2.hpp
  src/util/types.hpp
  src/util/vec.hpp
  src/movegen.h
  src/movegen.cpp
  src/uci.cpp
  src/uci.hpp
  src/square.hpp
  src/position.hpp
  src/board.hpp
  src/common.hpp
  src/position.cpp
  src/geometry.hpp
)

add_executable(clockwork ${srcs} src/main.cpp)
set_target_properties(clockwork PROPERTIES
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO)

do_test(test_position)

# Warnings
# We don't support MSVC
target_compile_options(clockwork PRIVATE -Wall -Wextra -Wconversion)

#Optimizations
target_compile_options(clockwork PRIVATE -march=native)

# LTO
include(CheckIPOSupported)
check_ipo_supported(RESULT lto)
if(lto)
  set_target_properties(clockwork PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
