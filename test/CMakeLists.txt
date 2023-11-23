add_executable(TEST main.cpp)
target_include_directories(TEST PUBLIC ../engine/src)
target_link_libraries(TEST WindVulkan)