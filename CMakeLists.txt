cmake_minimum_required(VERSION 3.14)
project(Lab10_1 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/utf-8)
endif()

# ---------- Пошук файлів програм ----------
# Для кожної програми пробуємо латинську і кириличну букву в імені файлу.
function(find_prog out_src out_name latin cyr)
    foreach(candidate "10.1${latin}.cpp" "10.1${cyr}.cpp")
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${candidate}")
            set(${out_src} "${CMAKE_CURRENT_SOURCE_DIR}/${candidate}" PARENT_SCOPE)
            set(${out_name} "${candidate}" PARENT_SCOPE)
            return()
        endif()
    endforeach()
    message(FATAL_ERROR "Не знайдено файл програми 10.1${latin}.cpp (ні з латинською, ні з кириличною буквою)")
endfunction()

find_prog(SRC_A NAME_A "A" "А")   # у другому аргументі кирилична А
find_prog(SRC_B NAME_B "B" "В")   # у другому аргументі кирилична В
message(STATUS "Програма A: ${NAME_A}")
message(STATUS "Програма B: ${NAME_B}")

# ---------- Програми ----------
add_executable(prog_A "${SRC_A}")
add_executable(prog_B "${SRC_B}")

# ---------- Google Test ----------
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

# ---------- Юніт-тести ----------
# Тести підключають файл програми через #include PROG_SOURCE,
# ім'я файлу передається сюди з CMake.
add_executable(test_A Test_10_1_A.cpp)
add_executable(test_B Test_10_1_B.cpp)

target_compile_definitions(test_A PRIVATE PROG_SOURCE="${NAME_A}")
target_compile_definitions(test_B PRIVATE PROG_SOURCE="${NAME_B}")

set_source_files_properties(Test_10_1_A.cpp PROPERTIES OBJECT_DEPENDS "${SRC_A}")
set_source_files_properties(Test_10_1_B.cpp PROPERTIES OBJECT_DEPENDS "${SRC_B}")

foreach(t test_A test_B)
    target_include_directories(${t} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(${t} PRIVATE GTest::gtest_main)
endforeach()

enable_testing()
include(GoogleTest)
gtest_discover_tests(test_A)
gtest_discover_tests(test_B)