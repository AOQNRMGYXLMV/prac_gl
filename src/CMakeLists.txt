set(SOURCE
	main.cpp
	Shader.h
	Shader.cpp)

add_executable(prac_gl ${SOURCE})

target_link_libraries(prac_gl glad glfw spdlog::spdlog glm::glm)

