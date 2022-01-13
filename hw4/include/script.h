#ifndef SCRIPT_H
#define SCRIPT_H

#include "ModelStatus.h"
#include "global.h"
#include "physics.h"

void statusInit(ModelStatus& status_p, ModelStatus& status_e, ModelStatus& status_b);
void runScript(ModelStatus& status_p, ModelStatus& status_e, ModelStatus& status_b);
void jump(ModelStatus& status);
void toggleGlow(ModelStatus& status);
void toggleExpand(ModelStatus& status);
void ball2pikachu(ModelStatus& status);
void ball2eevee(ModelStatus& status);

#endif