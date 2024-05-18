SET(THIRD_PARTY_LIBS
        spdlog::spdlog
        Vulkan::Vulkan
        glm::glm
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
SET(COMPILE_DEFINITIONS
        DLLEXPORT
        VULKAN_HPP_NO_CONSTRUCTORS
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
        SPDLOG_FMT_EXTERNAL
        $<$<CONFIG:Debug>:_DBG>
        $<$<CONFIG:RelWithDebInfo>:_DBG>
        $<$<CONFIG:Release>:_REL>
        $<$<CONFIG:MinSizeRel>:_REL>
        GLM_FORCE_RADIANS 
        GLM_FORCE_DEPTH_ZERO_TO_ONE)

file(GLOB_RECURSE WIND_ENGINE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src "*.cpp")
add_library(WindVulkan ${WIND_ENGINE_SRC})
target_link_libraries(WindVulkan PUBLIC ${THIRD_PARTY_LIBS})
target_include_directories(WindVulkan PUBLIC ../mirage-math/include)
if (APPLE)
    LIST(APPEND COMPILE_DEFINITIONS VK_USE_PLATFORM_MACOS_MVK )
endif ()
target_compile_definitions(WindVulkan PUBLIC ${COMPILE_DEFINITIONS})
set_target_properties(WindVulkan PROPERTIES CXX_EXTENSIONS OFF)

add_subdirectory(src)
