add_executable(WindTest main.cpp)
target_include_directories(WindTest PUBLIC ../engine/src)
target_link_libraries(WindTest WindVulkan)
