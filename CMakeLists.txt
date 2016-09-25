cmake_minimum_required (VERSION 3.1)
project (Sonnet)

file(GLOB SonnetSources "*.cpp" "*.h")
set(SOURCES ${SonnetSources})

set(SonnetLibraries
    EASTL)
	
add_subdirectory(EASTL)

add_executable(Sonnet ${SonnetSources})

target_link_libraries(Sonnet ${SonnetLibraries} )