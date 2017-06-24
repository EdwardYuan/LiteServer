cmake_minimum_required( VERSION 2.8.3 )

include( ../cmake/config.cmake )
include( ../cmake/macros.cmake )
include( ../cmake/common_path.cmake )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
clear_all_list()

add_include_path( ../../include )

add_include_path( ../../common )
add_include_path( ../../msgdefine )
add_include_path( /usr/local/ssl/include )
add_include_path( /usr/include/mysql )

add_link_path( ../../../libcmake)
add_link_path( /usr/local/ssl/lib)

add_link_lib( msgdefine )
add_link_lib( protobuf )
add_link_lib( common )
add_link_lib( lua )
add_link_lib( dl )
add_link_lib( event )
add_link_lib( pthread )
add_link_lib( ssl )
add_link_lib( crypto )
add_link_lib( mysqlpp )

add_source_files( ../../testlib/*.* )


build_executable( testlib ./ )





