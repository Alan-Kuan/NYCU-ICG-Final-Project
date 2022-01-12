#include "physics.h"

static Vertex gravity(0, -0.01f, 0);

inline void inspect(ModelStatus& st) {
	if (inspect_mode) {
		std::cout << st.name << "_position: " << st.position << std::endl;
		std::cout << st.name << "_speed: " << st.speed << std::endl;
	}
}

void applyGravity(ModelStatus& st) {
	if (st.speed != Vertex(0, 0, 0)) {
		st.position += st.speed;
		st.speed += gravity;
		if (st.position.y <= 1.0e-3f) {
			st.position.y = 0;
			st.speed = Vertex(0, 0, 0);
		}
		inspect(st);
	}
}

void restrictY(ModelStatus& st, float y) {
	if (st.position.y <= y + 1.0e-3f && st.position.y != y) {
		st.position.y = y;
		st.speed = Vertex(0, 0, 0);
		inspect(st);
	}
}