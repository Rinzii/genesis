add_subdirectory(buffers)
add_subdirectory(commands)
add_subdirectory(descriptors)
add_subdirectory(devices)
add_subdirectory(images)
add_subdirectory(pipelines)

target_sources(${PROJECT_NAME} PRIVATE
        fence.cpp
        image.cpp
        imageView.cpp
        pipeline.cpp
        pipelineLayout.cpp
        queryPool.cpp
        queue.cpp
        sampler.cpp
        semaphore.cpp
        swapchain.cpp
        )
