# Define minimal required version of CMake.
cmake_minimum_required(VERSION "3.25")

# Project definition
project(
	pts_receiver_si917_p5
	VERSION 1.0
	LANGUAGES C CXX ASM
)

# Include the definition of the slc_pts_receiver_si917_p5 target,
# which contains the content of the SLC project
include(pts_receiver_si917_p5.cmake)

add_executable(pts_receiver_si917_p5
    # Add additional sources here
)

target_include_directories(pts_receiver_si917_p5 PUBLIC
    # Add additional include paths here
)

target_compile_definitions(pts_receiver_si917_p5 PUBLIC
    # Add additional macros here
)

target_compile_options(pts_receiver_si917_p5 PUBLIC
    # Set additional compiler flags here
)

target_link_options(pts_receiver_si917_p5 PUBLIC
    # Set additional linker flags here
)

# Link with the content defined in the SLC project
target_link_libraries(pts_receiver_si917_p5 PRIVATE
    slc
)

# Include managed project content if available
include(pts_receiver_si917_p5_project.cmake OPTIONAL RESULT_VARIABLE managed_project)
if(managed_project)
    message(STATUS "Using managed project content from ${managed_project}")
endif()

# Force the gcc linker command
set_target_properties(pts_receiver_si917_p5 PROPERTIES LINKER_LANGUAGE C)

# Create .bin, .hex and .s37 artifacts after building the project
add_custom_command(TARGET pts_receiver_si917_p5
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_SREC_CMD} "$<TARGET_FILE:pts_receiver_si917_p5>" "$<TARGET_FILE_DIR:pts_receiver_si917_p5>/$<TARGET_FILE_BASE_NAME:pts_receiver_si917_p5>.s37"
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_IHEX_CMD} "$<TARGET_FILE:pts_receiver_si917_p5>" "$<TARGET_FILE_DIR:pts_receiver_si917_p5>/$<TARGET_FILE_BASE_NAME:pts_receiver_si917_p5>.hex"
    COMMAND ${CMAKE_OBJCOPY} ${OBJCOPY_BIN_CMD}  "$<TARGET_FILE:pts_receiver_si917_p5>" "$<TARGET_FILE_DIR:pts_receiver_si917_p5>/$<TARGET_FILE_BASE_NAME:pts_receiver_si917_p5>.bin" 
)

# Run post-build pipeline to perform additional post-processing
if(post_build_command)
add_custom_command(TARGET pts_receiver_si917_p5
    POST_BUILD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/..
    COMMAND ${post_build_command}
)
endif()