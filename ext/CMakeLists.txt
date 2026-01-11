
# define the source
set(SOURCES linenoise/src/linenoise.c)

add_library(linenoise ${SOURCES})

target_include_directories(linenoise PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/linenoise/include)
