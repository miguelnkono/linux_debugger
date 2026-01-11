
cmake_minimum_required(VERSION 3.20)
project(linux_debugger)

add_subdirectory(ext)


add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_14)
target_link_libraries(${PROJECT_NAME} PRIVATE linenoise
    ${PROJECT_SOURCE_DIR}/ext/libelfin/dwarf/libdwarf++.so
    ${PROJECT_SOURCE_DIR}/ext/libelfin/elf/libelf++.so
)

add_custom_target(
    libelfin
    COMMAND make
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/ext/libelfin
)

add_dependencies(${PROJECT_NAME} libelfin)