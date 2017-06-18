cmake_minimum_required( VERSION 2.8.3 )

macro( clear_include_list )
	set( INCLUDE_LIST "" )
endmacro( clear_include_list )


macro( clear_source_list )
	set( SOURCE_LIST "" )
endmacro( clear_source_list )

macro( clear_link_list )
	set( LINK_LIST "" )
endmacro( clear_link_list )

macro( clear_link_lib_list )
	set( LINK_LIBS "" )
endmacro( clear_link_lib_list )

macro( clear_all_list )
	clear_include_list()
	clear_source_list()
	clear_link_list()
	clear_link_lib_list()
endmacro( clear_all_list )

macro( add_include_path include_path )
	list( APPEND INCLUDE_LIST ${include_path} )
endmacro( add_include_path )

macro( add_source_files source_path )
	file( GLOB_RECURSE file_list ${source_path} )
	list( APPEND SOURCE_LIST ${file_list} )
endmacro( add_source_files )

macro( add_link_path link_path )
	list( APPEND LINK_LIST ${link_path} )
endmacro( add_link_path )

macro( add_link_lib lib_file )
	list( APPEND LINK_LIBS ${lib_file} )
endmacro( add_link_lib )

macro( build_static_lib libname output_path )
	set( LIBRARY_OUTPUT_PATH ${output_path} )
	include_directories( ${INCLUDE_LIST} )
	add_library( ${libname} STATIC ${SOURCE_LIST} )
endmacro( build_static_lib )

macro( build_shared_lib libname output_path )
	set( LIBRARY_OUTPUT_PATH ${output_path} )
	include_directories( ${INCLUDE_LIST} )
	add_library( ${libname} SHARED ${SOURCE_LIST} )
endmacro( build_shared_lib )

macro( build_executable execname output_path )
	set( EXECUTABLE_OUTPUT_PATH ${output_path} )
	include_directories( ${INCLUDE_LIST} )
	link_directories( ${LINK_LIST} )
	add_executable( ${execname} ${SOURCE_LIST} )
	target_link_libraries( ${execname} ${LINK_LIBS} )
endmacro( build_executable )


























