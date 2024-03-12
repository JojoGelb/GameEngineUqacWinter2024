cmake_minimum_required(VERSION 3.19)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Évitez de construire votre projet directement dans les sources!")
endif()

project(SetupCheck VERSION 0.0.1 LANGUAGES CXX)

include(cmake/common.cmake)

FetchContent_Declare(glfw
  GIT_REPOSITORY  https://github.com/glfw/glfw
  GIT_TAG         3.3.8
)

FetchContent_Declare(glm
  GIT_REPOSITORY  https://github.com/g-truc/glm
  GIT_TAG         0.9.9.8
)

FetchContent_Declare(json
  GIT_REPOSITORY  https://github.com/ArthurSonzogni/nlohmann_json_cmake_fetchcontent
  GIT_TAG         v3.10.0
)

FetchContent_Declare(libpng
  GIT_REPOSITORY  https://github.com/glennrp/libpng.git
  GIT_TAG         v1.6.37
)

FetchContent_Declare(yamlcpp
  GIT_REPOSITORY  https://github.com/jbeder/yaml-cpp.git
  GIT_TAG         yaml-cpp-0.7.0
)

FetchContent_Declare(zlib
  GIT_REPOSITORY  https://github.com/madler/zlib.git
  GIT_TAG         v1.2.11
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_GetProperties(glm)
if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_GetProperties(json)
if(NOT json_POPULATED)
  FetchContent_Populate(json)
  add_subdirectory(${json_SOURCE_DIR} ${json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_GetProperties(yamlcpp)
if(NOT yamlcpp_POPULATED)
  FetchContent_Populate(yamlcpp)
  add_subdirectory(${yamlcpp_SOURCE_DIR} ${yamlcpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

SET(SKIP_INSTALL_EXPORT TRUE)

FetchContent_GetProperties(zlib)
if(NOT zlib_POPULATED)
    FetchContent_Populate(zlib)

    set(ZLIB_INCLUDE_DIR ${zlib_SOURCE_DIR} ${zlib_BINARY_DIR})
    set(ZLIB_LIBRARY zlibstatic)

    execute_process(
      COMMAND           ${GIT_EXECUTABLE} apply ${CMAKE_CURRENT_LIST_DIR}/cmake/zlib.patch
      WORKING_DIRECTORY ${zlib_SOURCE_DIR}
      OUTPUT_QUIET
      ERROR_QUIET
    )

    set(CMAKE_PROJECT_INCLUDE_BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/enable_cmp0048.cmake")
    add_subdirectory(${zlib_SOURCE_DIR} ${zlib_BINARY_DIR} EXCLUDE_FROM_ALL)
    unset(CMAKE_PROJECT_INCLUDE_BEFORE)
endif()

FetchContent_GetProperties(libpng)
if(NOT libpng_POPULATED)
    FetchContent_Populate(libpng)

    set(PNG_SHARED OFF)

    set(CMAKE_PROJECT_INCLUDE_BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/enable_cmp0077.cmake")
    add_subdirectory(${libpng_SOURCE_DIR} ${libpng_BINARY_DIR} EXCLUDE_FROM_ALL)
    unset(CMAKE_PROJECT_INCLUDE_BEFORE)
endif()

find_package(Vulkan REQUIRED)
file(GLOB_RECURSE shader_SOURCES CONFIGURE_DEPENDS data/*.frag data/*.vert)
foreach(shader ${shader_SOURCES})
  set(shader_SOURCE "${shader}")
  set(shader_OUTPUT "${shader}.spv")
  add_custom_command(
    OUTPUT ${shader_OUTPUT}
    COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${shader_SOURCE} -g -O0 -o ${shader_OUTPUT}
    DEPENDS ${shader_SOURCE}
  )

  list(APPEND shader_OUTPUTS ${shader_OUTPUT})
endforeach()
add_custom_target(setupcheck_shaders DEPENDS ${shader_OUTPUTS})

set(TARGET_NAME setupcheck)

file(GLOB_RECURSE setupcheck_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${TARGET_NAME} ${setupcheck_SOURCES})

target_precompile_headers(${TARGET_NAME} PRIVATE include/setupcheck/pch/precomp.h)
target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)

target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${libpng_SOURCE_DIR}/ ${libpng_BINARY_DIR}/)
target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${glfw_SOURCE_DIR}/include)
target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${glm_SOURCE_DIR})
target_include_directories(${TARGET_NAME} SYSTEM PRIVATE ${zlib_SOURCE_DIR}/)

add_dependencies(${TARGET_NAME} setupcheck_shaders)

find_package(Threads REQUIRED)
find_package(Vulkan REQUIRED)
target_link_libraries(${TARGET_NAME} PRIVATE Threads::Threads)
target_link_libraries(${TARGET_NAME} PRIVATE fmt::fmt)
target_link_libraries(${TARGET_NAME} PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(${TARGET_NAME} PRIVATE png_static)
target_link_libraries(${TARGET_NAME} PRIVATE glfw)
target_link_libraries(${TARGET_NAME} PRIVATE yaml-cpp)
target_link_libraries(${TARGET_NAME} PRIVATE Vulkan::Vulkan)

if (WIN32)
  target_link_libraries(${TARGET_NAME} PRIVATE wsock32 ws2_32)
endif()

add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E create_symlink
${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:setupcheck>/data)
