cmake_minimum_required(VERSION 2.8.5)

project(json_merge_tool)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(CJSON_OVERRIDE_BUILD_SHARED_LIBS On)
set(CJSON_BUILD_SHARED_LIBS Off)
set(ENABLE_CJSON_UTILS On)
set(ENABLE_CJSON_TEST Off)

add_subdirectory(cJSON)

add_executable(json_merge_tool src/main.c src/utils.c)

target_link_libraries(json_merge_tool cjson)
target_link_libraries(json_merge_tool cjson_utils)

include_directories(json_merge_tool cJSON)
