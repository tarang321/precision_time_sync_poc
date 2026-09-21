# Define minimal required version of CMake.
cmake_minimum_required(VERSION "3.25")

# Project definition
project(
	pts_master_si917_p4
	VERSION 1.0
	LANGUAGES C CXX ASM
)

# Include the definition of the slc_pts_master_si917_p4 target,
# which contains the content of the SLC project
include(pts_master_si917_p4.cmake)

add_executable(pts_master_si917_p4
    # Add additional sources here
)

target_include_directories(pts_master_si917_p4 PUBLIC
    # Add additional include paths here
)

target_compile_definitions(pts_master_si917_p4 PUBLIC
    # Add additional macros here
)

target_compile_options(pts_master_si917_p4 PUBLIC
    # Set additional compiler flags here
)

target_link_options(pts_master_si917_p4 PUBLIC
    # Set additional linker flags here
)

# Link with the content defined in the SLC project
target_link_libraries(pts_master_si917_p4 PRIVATE
    slc
)

# Include managed project content if available
include(pts_master_si917_p4_project.cmake OPTIONAL RESULT_VARIABLE managed_project)
if(managed_project)
    message(STATUS "Using managed project content from ${managed_project}")
endif()

# Force the gcc linker command
set_target_properties(pts_master_si917_p4 PROPERTIES LINKER_LANGUAGE C)

# Create .bin, .hex and .s37 artifacts after building the project
add_custom_command(TARGET pts_master_si917_p4
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_SREC_CMD} "$<TARGET_FILE:pts_master_si917_p4>" "$<TARGET_FILE_DIR:pts_master_si917_p4>/$<TARGET_FILE_BASE_NAME:pts_master_si917_p4>.s37"
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_IHEX_CMD} "$<TARGET_FILE:pts_master_si917_p4>" "$<TARGET_FILE_DIR:pts_master_si917_p4>/$<TARGET_FILE_BASE_NAME:pts_master_si917_p4>.hex"
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_BIN_CMD}  "$<TARGET_FILE:pts_master_si917_p4>" "$<TARGET_FILE_DIR:pts_master_si917_p4>/$<TARGET_FILE_BASE_NAME:pts_master_si917_p4>.bin" 
)

# Run post-build pipeline to perform additional post-processing
if(post_build_command)
add_custom_command(TARGET pts_master_si917_p4
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
    COMMAND ${post_build_command}
)
endif()