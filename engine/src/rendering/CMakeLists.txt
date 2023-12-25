add_subdirectory(utils)
add_subdirectory(vk)

target_sources(${PROJECT_NAME} PRIVATE
        bootstrap.hpp
)

target_sources(${PROJECT_NAME} PRIVATE
        bootstrap.cpp
        renderWindow.cpp
        renderFrame.cpp
        renderContext.cpp
        renderPipeline.cpp
        renderTarget.cpp
        renderer.cpp
        )
