#uncomment this if you have sub directories
#add_subdirectory()

target_sources(${PROJECT_NAME} PRIVATE
        fileAsync.cpp
        file.cpp
        fileUtils.cpp
        fileHelper.cpp
        )
