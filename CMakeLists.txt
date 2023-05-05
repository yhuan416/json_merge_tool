cmake_minimum_required(VERSION 2.8.5)

project(json_merge_tool)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(BUILD_SHARED_AND_STATIC_LIBS On)
set(ENABLE_CJSON_UTILS On)

add_subdirectory(cJSON)

add_executable(json_merge_tool src/main.c)

target_link_libraries(json_merge_tool cjson-static)
target_link_libraries(json_merge_tool cjson_utils-static)

include_directories(json_merge_tool cJSON)
