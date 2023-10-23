add_executable(prac_gl main.cpp)

# target_include_directories(prac_gl PUBLIC ${PROJECT_SOURCE_DIR}/third_party/glad/include)
target_include_directories(prac_gl PUBLIC ${PROJECT_SOURCE_DIR}/third_party/glfw/include)
target_link_libraries(prac_gl glad glfw)

