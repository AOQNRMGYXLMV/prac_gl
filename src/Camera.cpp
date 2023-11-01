#include "Camera.h"

#include <spdlog/spdlog.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// calculate view & projection matrix, refer: https://sites.cs.ucsb.edu/~lingqi/teaching/resources/GAMES101_Lecture_04.pdf
void Camera::SetCamera(const glm::vec3& pos, const glm::vec3& look_at, const glm::vec3& up) {
	pos_ = pos;
	front_ = glm::normalize(look_at - pos);
	right_ = glm::normalize(glm::cross(up, front_));
	up_ = glm::cross(front_, right_);

	glm::mat4 T_view = glm::translate(glm::mat4(1.0), -pos);
	glm::mat4 R_view = glm::mat4(glm::transpose(glm::mat3{ right_, up_, front_ }));

	view_ = R_view * T_view;
}

double Camera::DegreeToRadian(double deg) const {
	return deg * M_PI / 180.0f;
}

void Camera::SetPerspective(double fov_y, double aspect_ratio, double near, double far) {
	// right hand coordinate
	near_ = near;
	far_ = far;
	fov_y_ = fov_y;
	aspect_ratio_ = aspect_ratio;

	// perspective to orthographic matrix
	glm::mat4 M_p_to_o(0.0);
	M_p_to_o[0][0] = near_;
	M_p_to_o[1][1] = near_;
	M_p_to_o[2][2] = near_ + far_;
	M_p_to_o[2][3] = 1.0;
	M_p_to_o[3][2] = -near_ * far_;

	double height = near * std::tan(DegreeToRadian(fov_y_ / 2.0f)) * 2.0f;
	double width = height * aspect_ratio_;

	glm::mat4 M_ortho_scale = glm::scale(glm::mat4(1.0), glm::vec3(2.0 / width, 2.0 / height, 2.0/(far_-near_)));
	glm::mat4 M_ortho_trans = glm::translate(glm::mat4(1.0), glm::vec3(0.0, 0.0, -(near_ + far_) / 2.0));

	projection_ = M_ortho_scale * M_ortho_trans * M_p_to_o;
}

glm::mat4 Camera::GetViewMatrix() const {
	return view_;
}

glm::mat4 Camera::GetProjectionMatrix() const {
	return projection_;
}
