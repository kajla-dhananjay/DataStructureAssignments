add_executable(somefile_exe
    src/somefile.cpp
)
add_executable(main_exe
    src/main.cpp
)

target_link_libraries(main_exe PRIVATE tester_lib)

set_target_properties(main_exe PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)