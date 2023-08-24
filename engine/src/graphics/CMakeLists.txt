#uncomment this if you have sub directories
#add_subdirectory()

target_sources(${PROJECT_NAME} PRIVATE
        graphicsExceptions.cpp
        command.cpp
        device.cpp
        swapchain.cpp
        renderer.cpp
        vkHelpers.cpp
        )
