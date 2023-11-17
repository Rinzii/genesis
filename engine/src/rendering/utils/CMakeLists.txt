#uncomment this if you have sub directories
#add_subdirectory()

target_sources(${PROJECT_NAME} PRIVATE
        debugUtils.cpp
        exceptions.cpp
        vkHelpers.cpp
        vulkanResource.cpp
        )
