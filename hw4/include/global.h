#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>

extern bool inspect_mode;
extern float scene_angle;

void updateAngle(float& angle, float delta_angle, const std::string& prefix);

#endif