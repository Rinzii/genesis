add_subdirectory(utils)
add_subdirectory(vk)

target_sources(${PROJECT_NAME} PRIVATE
        renderWindow.cpp
        buffer.cpp
        device.cpp
        fencePool.cpp
        semaphorePool.cpp
        bufferPool.cpp
        frame.cpp
        vk/devices/instance.cpp
        pipeline.cpp
        renderer.cpp
        swapchain.cpp
        )
