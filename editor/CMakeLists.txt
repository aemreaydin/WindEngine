add_executable(WindEditor main.cpp)
target_include_directories(WindEditor PUBLIC ../engine/src)
target_link_libraries(WindEditor WindVulkan)
