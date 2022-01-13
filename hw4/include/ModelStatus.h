#ifndef MODEL_STATUS_H
#define MODEL_STATUS_H

#include <string>
#include "Vertex.h"

struct ModelStatus {
    ModelStatus(void) {
        this->position = Vertex(0, 0, 0);
        this->speed = Vertex(0, 0, 0);
        this->angle_h = this->angle_v = 0;
        this->ang_speed_h = this->ang_speed_v = 0;
        this->glow = false;
        this->expand = false;
        this->emit_ray = false;
        this->name = "";
    }
    ModelStatus(float x, float y, float z, std::string name) {
        this->position = Vertex(x, y, z);
        this->speed = Vertex(0, 0, 0);
        this->angle_h = this->angle_v = 0;
        this->ang_speed_h = this->ang_speed_v = 0;
        this->glow = false;
        this->expand = false;
        this->emit_ray = false;
        this->name = name;
    }

    Vertex position;
    Vertex speed;
    float angle_h, angle_v;
    float ang_speed_h, ang_speed_v;
    bool glow, expand, emit_ray;
    std::string name;
};

#endif