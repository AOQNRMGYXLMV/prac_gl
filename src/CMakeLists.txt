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

set (THIRD_PARTY ${PROJECT_SOURCE_DIR}/third_party)

file (GLOB IM_GUI_SOURCE
	${THIRD_PARTY}/imgui/*.h
	${THIRD_PARTY}/imgui/*.cpp
	${THIRD_PARTY}/imgui/backends/imgui_impl_opengl3.h
	${THIRD_PARTY}/imgui/backends/imgui_impl_opengl3.cpp
	${THIRD_PARTY}/imgui/backends/imgui_impl_glfw.h
	${THIRD_PARTY}/imgui/backends/imgui_impl_glfw.cpp)

source_group(ImGui FILES ${IM_GUI_SOURCE})
add_executable(prac_gl ${SOURCE} ${IM_GUI_SOURCE})

target_include_directories(prac_gl PUBLIC
	${THIRD_PARTY}
	${THIRD_PARTY}/imgui
	${THIRD_PARTY}/stb)

target_link_libraries(prac_gl glad glfw spdlog::spdlog glm::glm assimp)

