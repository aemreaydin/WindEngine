SET(THIRD_PARTY_LIBS
        Vulkan::Vulkan
        spdlog::spdlog
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)


add_library(WindVulkan SHARED core/logger.cpp core/assert.cpp
        engine.cpp
        core/window.cpp)
target_link_libraries(WindVulkan PUBLIC ${THIRD_PARTY_LIBS})
target_compile_definitions(WindVulkan PUBLIC DLLEXPORT)