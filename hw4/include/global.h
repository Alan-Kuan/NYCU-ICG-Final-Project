#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <string>

extern bool inspect_mode;
extern float scene_angle;
extern float scene_ang_speed;
extern unsigned int frame_num;
extern bool playing;

void updateAngle(float& angle, float delta_angle, const std::string& prefix);

#endif