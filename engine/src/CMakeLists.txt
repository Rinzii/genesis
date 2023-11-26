add_subdirectory(graphics)
add_subdirectory(gtl)
add_subdirectory(inputs)
add_subdirectory(io)
add_subdirectory(system)
add_subdirectory(windowing)

target_sources(${PROJECT_NAME} PRIVATE
        application.cpp
		engine.cpp
		time.cpp
        )
