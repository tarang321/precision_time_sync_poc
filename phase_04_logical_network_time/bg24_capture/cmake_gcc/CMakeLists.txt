# Define minimal required version of CMake.
cmake_minimum_required(VERSION "3.25")

# Project definition
project(
	pts_bg24_network_time
	VERSION 1.0
	LANGUAGES C CXX ASM
)

# Include the definition of the slc_pts_bg24_network_time target,
# which contains the content of the SLC project
include(pts_bg24_network_time.cmake)

add_executable(pts_bg24_network_time
    # Add additional sources here
)

target_include_directories(pts_bg24_network_time PUBLIC
    # Add additional include paths here
)

target_compile_definitions(pts_bg24_network_time PUBLIC
    # Add additional macros here
)

target_compile_options(pts_bg24_network_time PUBLIC
    # Set additional compiler flags here
)

target_link_options(pts_bg24_network_time PUBLIC
    # Set additional linker flags here
)

# Link with the content defined in the SLC project
target_link_libraries(pts_bg24_network_time PRIVATE
    slc
)

# Include managed project content if available
include(pts_bg24_network_time_project.cmake OPTIONAL RESULT_VARIABLE managed_project)
if(managed_project)
    message(STATUS "Using managed project content from ${managed_project}")
endif()

# Force the gcc linker command
set_target_properties(pts_bg24_network_time PROPERTIES LINKER_LANGUAGE C)

# Create .bin, .hex and .s37 artifacts after building the project
add_custom_command(TARGET pts_bg24_network_time
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_SREC_CMD} "$<TARGET_FILE:pts_bg24_network_time>" "$<TARGET_FILE_DIR:pts_bg24_network_time>/$<TARGET_FILE_BASE_NAME:pts_bg24_network_time>.s37"
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_IHEX_CMD} "$<TARGET_FILE:pts_bg24_network_time>" "$<TARGET_FILE_DIR:pts_bg24_network_time>/$<TARGET_FILE_BASE_NAME:pts_bg24_network_time>.hex"
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_BIN_CMD}  "$<TARGET_FILE:pts_bg24_network_time>" "$<TARGET_FILE_DIR:pts_bg24_network_time>/$<TARGET_FILE_BASE_NAME:pts_bg24_network_time>.bin" 
)

# Run post-build pipeline to perform additional post-processing
if(post_build_command)
add_custom_command(TARGET pts_bg24_network_time
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
    COMMAND ${post_build_command}
)
endif()