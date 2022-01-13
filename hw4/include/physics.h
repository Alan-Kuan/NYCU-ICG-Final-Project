#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <string>
#include "Vertex.h"
#include "global.h"
#include "ModelStatus.h"

void applyGravity(ModelStatus& st);
void restrictY(ModelStatus& st, float y);

void rotateWithSpeed(float& angle, float speed);

#endif