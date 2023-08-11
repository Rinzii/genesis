add_subdirectory(graphics)
add_subdirectory(io)
#add_subdirectory(system)
add_subdirectory(logger)
add_subdirectory(windowing)

target_sources(${PROJECT_NAME} PRIVATE
        application.cpp
		time.cpp
		inputs/controller.cpp
		inputs/inputs.cpp
		inputs/pcPeripherals.cpp
        )
