# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (lab-2)

file(GLOB_RECURSE FRAMEWORK_HEADERS  include/*.hpp include/*.h)
file(GLOB_RECURSE FRAMEWORK_SOURCES  source/*.cpp source/*.c)

# The main executable
add_executable(main
        ${FRAMEWORK_SOURCES}
        ${FRAMEWORK_HEADERS}
)

set_property(TARGET main PROPERTY CXX_STANDARD 11)
target_compile_options(main PRIVATE -Wall)

# glfw
add_subdirectory(lib/glfw EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glfw)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(lib/glew-cmake EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE libglew_static)

# glm
add_subdirectory(lib/glm EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glm)
