#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	Camera() = default;
	void SetCamera(const glm::vec3& pos, const glm::vec3& look_at, const glm::vec3& up);
	void SetPerspective(double fov_y, double aspect_ratio, double near, double far);
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

private:
	double DegreeToRadian(double deg) const;

	glm::vec3 pos_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::mat4 view_;
	glm::mat4 projection_;
	double fov_y_;
	double aspect_ratio_;
	double near_;
	double far_;

	static constexpr double M_PI = 3.14159265358979323846f;
};
