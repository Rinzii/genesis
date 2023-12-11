add_subdirectory(devices)
target_sources(${PROJECT_NAME} PRIVATE
        commandBuffer.cpp
        commandPool.cpp
        descriptorPool.cpp
        descriptorSet.cpp
        descriptorSetLayout.cpp
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
