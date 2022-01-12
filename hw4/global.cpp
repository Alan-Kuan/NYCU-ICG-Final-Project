#include "global.h"

bool inspect_mode = false;
float scene_angle = 0.0f;

void updateAngle(float& angle, float delta_angle, const std::string& prefix) {
	angle += delta_angle;
	if (angle >= 360.0f)
		angle -= 360.0f;
	else if (angle < 0.0f)
		angle += 360.0f;
	if (inspect_mode)
		std::cout << prefix << "_angle: " << angle << std::endl;
}