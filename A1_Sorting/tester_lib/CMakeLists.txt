add_library(tester_lib STATIC
    src/correctness_tester.cpp
    src/load_tester.cpp
    src/testcase_generator.cpp
)

if(Debug)
    target_compile_definitions(tester_lib PRIVATE DEBUG)
endif()

target_include_directories(tester_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(tester_lib PRIVATE 
    sorter_lib
    pthread
)