#pragma once

#include <glm/glm.hpp>

class Camera {
public:
	Camera() = default;
	void SetCamera(const glm::vec3& pos, const glm::vec3& look_at, const glm::vec3& up);
	void SetPerspective(double fov_y, double aspect_ratio, double near, double far);
	void UpdateAspectRatio(double aspect_ratio);
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;
	void Move(const glm::vec3& vec);
	void ProcessMouseMove(float dx, float dy);
	void SetPosition(const glm::vec3& pos);
	glm::vec3 GetPosition() const;
	void RotateZ(float angle);

private:
	void UpdateCameraVectors();

	glm::vec3 pos_;
	glm::vec3 front_;
	glm::vec3 up_;
	glm::vec3 right_;
	glm::vec3 initial_up_;

	glm::mat4 view_trans_;
	glm::mat4 view_rotate_;
	glm::mat4 view_;
	glm::mat4 projection_;
	double fov_y_;
	double aspect_ratio_;
	double near_;
	double far_;
};
