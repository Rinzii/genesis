#uncomment this if you have sub directories
#add_subdirectory()

target_sources(${PROJECT_NAME} PRIVATE
        graphicsExceptions.cpp
        device.cpp
        swapchain.cpp
        renderer.cpp
        renderFrame.cpp
        vkHelpers.cpp
        )
