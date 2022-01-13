#include "script.h"

#define PB_INIT_Y 0.7

void statusInit(ModelStatus& status_p, ModelStatus& status_e, ModelStatus& status_b) {
    status_p = ModelStatus(-1.0, 0.0,       0.0, "pikachu");
    status_e = ModelStatus( 1.0, 0.0,       0.0, "eevee");
    status_b = ModelStatus(-0.5, PB_INIT_Y, 0.0, "pokeball");
}

void jump(ModelStatus& status) {
    status.speed = Vertex(0, 0.05f, 0);
}

void toggleGlow(ModelStatus& status) {
    status.glow = !status.glow;
}

void toggleExpand(ModelStatus& status) {
    status.expand = !status.expand;
}

void ball2pikachu(ModelStatus& status) {
    status.speed = Vertex(-0.03f, 0.05f, 0);
}

void ball2eevee(ModelStatus& status) {
    status.speed = Vertex(0.03f, 0.05f, 0);
}

void calculatePhysics(ModelStatus& status_p, ModelStatus& status_e, ModelStatus& status_b) {
	applyGravity(status_p);
	applyGravity(status_e);
	applyGravity(status_b);
	restrictY(status_b, PB_INIT_Y);

    rotateWithSpeed(scene_angle, scene_speed);
    rotateWithSpeed(status_b.angle_h, status_b.ang_speed_h);
    rotateWithSpeed(status_b.angle_v, status_b.ang_speed_v);
    rotateWithSpeed(status_p.angle_h, status_p.ang_speed_h);
}

void runScript(ModelStatus& status_p, ModelStatus& status_e, ModelStatus& status_b) {
    switch (frame_num) {
    case 0:
        resetGravity();
        break;

    // passing ball
    case 1:
    case 101:
    case 201:
        jump(status_p);
        status_b.ang_speed_h = 0.0f;
        status_b.ang_speed_v = 0.0f;
        break;
    case 10:
    case 110:
    case 210:
        ball2eevee(status_b);
        status_b.ang_speed_h = 3.0f;
        status_b.ang_speed_v = 5.0f;
        break;
    case 51:
    case 151:
    case 251:
        jump(status_e);
        status_b.ang_speed_h = 0.0f;
        status_b.ang_speed_v = 0.0f;
        break;
    case 60:
    case 160:
    case 260:
        ball2pikachu(status_b);
        status_b.ang_speed_h = 3.0f;
        status_b.ang_speed_v = 5.0f;
        break;

    // scene rotation
    case 159:
        scene_speed = 0.6f;
        break;
    case 258:
        scene_speed = 0.0f;
        break;

    // iron tail
    case 300:
        toggleGlow(status_p);
        jump(status_p);
        status_p.ang_speed_h = 3.5f;
        break;
    case 320:
        status_p.ang_speed_h = 2.5f;
        break;
    case 350:
        // TODO: hit the ball
        status_p.ang_speed_h = 0.5f;
        reduceGravity();
        break;
    case 360:
        status_p.ang_speed_h = 0.0f;
        break;
    case 365:
        toggleGlow(status_p);
        resetGravity();
        break;
    case 370:
        status_p.speed = Vertex(0, 0.03f, 0);
        status_p.ang_speed_h = -7.45f;
        break;
    case 390:
        status_p.ang_speed_h = 0.0f;
        break;

    }

    if (playing) {
        calculatePhysics(status_p, status_e, status_b);
        frame_num++;
    }
}