SET(THIRD_PARTY_LIBS
        Vulkan::Vulkan
        spdlog::spdlog
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)

SET(CORE_FILES
        core/logger.cpp
        core/assert.cpp
        core/window.cpp)

add_library(WindVulkan SHARED ${CORE_FILES}
        engine.cpp)

target_link_libraries(WindVulkan PUBLIC ${THIRD_PARTY_LIBS})
target_compile_definitions(WindVulkan PUBLIC DLLEXPORT)