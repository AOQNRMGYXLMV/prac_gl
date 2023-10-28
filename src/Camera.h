#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	Camera() = default;
	void SetCamera(const glm::vec3& pos, const glm::vec3& look_at, const glm::vec3& up);
	void SetPerspective(float fov_y, float aspect_ratio, float near, float far);
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

private:
	float DegreeToRadian(float deg) const;

	glm::vec3 pos_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::mat4 view_;
	glm::mat4 projection_;
	float fov_y_;
	float aspect_ratio_;
	float near_;
	float far_;

	static constexpr float M_PI = 3.14159265358979323846f;
};
