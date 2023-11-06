set(SOURCE
	main.cpp
	Camera.h
	Camera.cpp
	Events.h
	Events.cpp
	Mesh.h
	Mesh.cpp
	Shader.h
	Shader.cpp
	stb_image.cpp)

add_executable(prac_gl ${SOURCE})

target_include_directories(prac_gl PUBLIC ${PROJECT_SOURCE_DIR}/third_party/stb)

target_link_libraries(prac_gl glad glfw spdlog::spdlog glm::glm assimp)

