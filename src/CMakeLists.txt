set(SOURCE
	main.cpp
	Camera.h
	Camera.cpp
	Events.h
	Events.cpp
	Mesh.h
	Mesh.cpp
	Shader.h
	Shader.cpp)

add_executable(prac_gl ${SOURCE})

target_link_libraries(prac_gl glad glfw spdlog::spdlog glm::glm assimp)

