cmake_minimum_required (VERSION 3.1)
project (Sonnet)

file(GLOB SonnetSources "*.cpp" "*.h")
set(SOURCES ${SonnetSources})

configure_file(CMakeLists.External.txt eastl-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/eastl-download )
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/eastl-download )

set(SonnetLibraries
    EASTL)
	
add_subdirectory(${CMAKE_BINARY_DIR}/eastl-src
                 ${CMAKE_BINARY_DIR}/eastl-build)
	
add_executable(Sonnet ${SonnetSources})

target_link_libraries(Sonnet ${SonnetLibraries} )