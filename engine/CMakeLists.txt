SET(THIRD_PARTY_LIBS
        Vulkan::Vulkan
        spdlog::spdlog
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
SET(COMPILE_DEFINITIONS
        DLLEXPORT
        VULKAN_HPP_NO_CONSTRUCTORS
        VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
        $<$<CONFIG:Debug>:_DBG>
        $<$<CONFIG:RelWithDebInfo>:_DBG>
        $<$<CONFIG:Release>:_REL>
        $<$<CONFIG:MinSizeRel>:_REL>)

add_library(WindVulkan SHARED ${Wind_Engine_Src})
target_link_libraries(WindVulkan PUBLIC ${THIRD_PARTY_LIBS})
if (APPLE)
    LIST(APPEND COMPILE_DEFINITIONS VK_USE_PLATFORM_MACOS_MVK)
endif ()
target_compile_definitions(WindVulkan PUBLIC ${COMPILE_DEFINITIONS})
# TODO
#target_compile_options(WindVulkan PUBLIC -Wall -Wextra -pedantic)

# TODO
#target_precompile_headers(WindVulkan PUBLIC include/pch.hpp)

add_subdirectory(src)
