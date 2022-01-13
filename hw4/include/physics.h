#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <string>
#include "Vertex.h"
#include "global.h"
#include "ModelStatus.h"

void resetGravity(void);
void reduceGravity(void);
void applyGravity(ModelStatus& st);
void restrictY(ModelStatus& st, float y);

void updateWithSpeed(float& value, float speed);

#endif