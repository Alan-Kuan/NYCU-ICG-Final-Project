#include "script.h"

static float y_limit_b = 0.7;

void statusInit(ModelStatus& status_p, ModelStatus& status_e, ModelStatus& status_b) {
    resetGravity();
    y_limit_b = 0.7f;

    status_p = ModelStatus(-1.0, 0.0, 0.0, "pikachu");
    status_e = ModelStatus( 1.0, 0.0, 0.0, "eevee");
    status_b = ModelStatus(-0.5, 0.7, 0.0, "pokeball");
    status_p.expand_ratio = 0.1f;
    status_e.expand_ratio = 2.0f;
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

void toggleEmitRay(ModelStatus& status) {
    status.emit_ray = !status.emit_ray;
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
	restrictY(status_b, y_limit_b);

    updateWithSpeed(scene_dist, scene_speed);
    updateWithSpeed(scene_angle, scene_ang_speed);
    updateWithSpeed(status_b.angle_h, status_b.ang_speed_h);
    updateWithSpeed(status_b.angle_v, status_b.ang_speed_v);
    updateWithSpeed(status_p.angle_h, status_p.ang_speed_h);
    updateWithSpeed(status_e.angle_h, status_e.ang_speed_h);
}

void runScript(ModelStatus& status_p, ModelStatus& status_e, ModelStatus& status_b) {
    switch (frame_num) {
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
        scene_ang_speed = 0.6f;
        break;
    case 258:
        scene_ang_speed = 0.0f;
        break;

    // iron tail
    case 300:
        toggleGlow(status_p);
        jump(status_p);
        status_p.ang_speed_h = 3.5f;
        scene_speed = -0.05f;
        break;
    case 320:
        status_p.ang_speed_h = 2.5f;
        break;
    case 350:
        ball2eevee(status_b);
        toggleEmitRay(status_b);
        scene_speed = 0.0f;
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

    // Eevee fail to catch
    case 400:
        toggleEmitRay(status_b);
        jump(status_e);
        break;
    case 409:
        ball2eevee(status_b);
        y_limit_b = 0.0f;
        break;
    case 460:
        status_b.ang_speed_h = 0.0f;
        status_b.ang_speed_v = 0.0f;
        break;

    // Eevee turn around
    case 500:
        status_e.speed = Vertex(0, 0.03f, 0);
        status_e.ang_speed_h = -7.45f;
        break;
    case 525:
        status_e.ang_speed_h = 0.0f;
        break;
    case 580:
        status_e.speed = Vertex(0, 0.03f, 0);
        status_e.ang_speed_h = -7.45f;
        break;
    case 605:
        status_e.ang_speed_h = 0.0f;
        break;

    // Eevee laugh & Pikachu be angry
    case 680:
        toggleExpand(status_e);
        break;
    case 730:
        scene_ang_speed = -2.0f;
        scene_speed = -0.035f;
        break;
    case 790:
        scene_speed = 0.0f;
        scene_ang_speed = 0.0f;
        break;
    case 800:
        toggleExpand(status_p);
        break;

    }

    if (playing) {
        calculatePhysics(status_p, status_e, status_b);
        frame_num++;
    }
}