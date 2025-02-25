add_library(sorter_lib
    src/sorter.cpp
)

target_include_directories(sorter_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

