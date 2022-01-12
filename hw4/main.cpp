#define GLM_ENABLE_EXPERIMENTAL
#define _USE_MATH_DEFINES

#include "Object.h"
#include "FreeImage.h"
#include "glew.h"
#include "freeglut.h"
#include "shader.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <stb_image.h>
#include <ctime>
#include <chrono>
#include <thread>
#include <vector>

#include "Vertex.h"
#include "ModelStatus.h"
#include "global.h"
#include "physics.h"

using namespace std;

void keyboard(unsigned char key, int x, int y);
void shaderInit();
void generateVAO(vector<VertexAttribute>& data, GLuint* vao);
void bufferModel(Object* model, GLuint* vao);
void bindbufferInit();
void textureInit();
void display();
void idle();
void reshape(GLsizei w, GLsizei h);
void drawSphere(vector<VertexAttribute>& ball, float radius, float slice, float stack);
void drawModel(ModelStatus model_status, float expand_ratio, GLsizei vertex_num, GLuint vao, GLuint texture_, glm::mat4 &M, GLenum DrawingMode);
void loadTexture(const char* tFileName, GLuint* texture_id);
void Sleep(int ms);
glm::mat4 getV();
glm::mat4 getP();
void calculatePhysics();
void demo();

// feeling free to adjust below value to fit your computer efficacy.
#define MAX_FPS 120
// timer for FPS control
clock_t Start, End;

GLuint toon_program, expand_program;
GLuint vao_e, vao_p, vao_b;
GLuint texture_e, texture_p, texture_b;

ModelStatus status_p(-1.0, 0.0, 0.0, "pikachu");
ModelStatus status_e( 1.0, 0.0, 0.0, "eevee");
ModelStatus status_b(-1.0, 1.5, 0.0, "pokeball");

float windowSize[2] = { 600, 600 };
glm::vec3 WorldLightPos = glm::vec3(0.0, 8.0, 5.0);
glm::vec3 WorldCamPos 	= glm::vec3(0.0, 2.0, 7.5);

Object* Pikachu = new Object("Pikachu.obj");
Object* Eevee = new Object("Eevee.obj");
vector<VertexAttribute> Pokeball;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(windowSize[0], windowSize[1]);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("hw4");

	drawSphere(Pokeball, 0.3, 60, 30);
	glewInit();
	shaderInit();
	bindbufferInit();
	textureInit();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}

void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	// inspect mode
	case 'i':
		inspect_mode = !inspect_mode;
		cout << "Inspect Mode: " << (inspect_mode ? "On" : "Off") << endl;
		break;

	// rotation
	case 's':
		updateAngle(scene_angle, 10.0f, "scene");
		break;
	case 'S':
		updateAngle(scene_angle, -10.0f, "scene");
		break;
	case 'P':
		updateAngle(status_p.angle_h, 10.0f, "pikachu_h");
		break;
	case 'b':
		updateAngle(status_b.angle_h, 10.0f, "pokeball_h");
		break;
	case 'B':
		updateAngle(status_b.angle_v, 10.0f, "pokeball_v");
		break;

	// jump
	case 'p':
		status_p.speed = Vertex(0, 0.1f, 0);
		break;
	case 'e':
		status_e.speed = Vertex(0, 0.1f, 0);
		break;

	// edge glow
	case 'g':
		status_p.glow = !status_p.glow;
		break;

	// expansion
	case 'E':
		status_p.expand = !status_p.expand;
		status_e.expand = !status_e.expand;
		break;

	// projectile motion
	case 'a':
		status_b.speed = Vertex(-0.1f, 0.1f, 0);
		break;
	case 'd':
		status_b.speed = Vertex(0.1f, 0.1f, 0);
		break;
	}
}

void shaderInit() {
	// ### hint
	// create your shader in here
	// below two shader "expand3" & "expand4" only differ in "input type of primitive" of geometry shader (one for triangle , one for quad)
	// what kind of primitive is sent to geometry shader is depend on what model you call in "glDrawArrays" function
	GLuint _vert, _geom, _frag;

	_vert = createShader("Shaders/toon.vert", "vertex");
	_frag = createShader("Shaders/toon.frag", "fragment");
	toon_program = createProgram(_vert, 0, _frag);

	_vert = createShader("Shaders/expand.vert", "vertex");
	_geom = createShader("Shaders/expand.geom", "geometry");
	_frag = createShader("Shaders/toon.frag", "fragment");
	expand_program = createProgram(_vert, _geom, _frag);
}

void generateVAO(vector<VertexAttribute>& data, GLuint* vao) {
	GLuint _vbo;
	glGenVertexArrays(1, vao);
	glGenBuffers(1, &_vbo);
	glBindVertexArray(*vao);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexAttribute) * data.size(), &data[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)offsetof(VertexAttribute, position));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)offsetof(VertexAttribute, normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAttribute), (void*)offsetof(VertexAttribute, texcoord));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void bufferModel(Object* model, GLuint* vao) {
	vector<VertexAttribute> data;
	VertexAttribute temp;
	model->max_y = INT_MIN;
	model->min_y = INT_MAX;
	model->max_z = INT_MIN;
	model->min_z = INT_MAX;
	for (int i = 0; i < model->positions.size() / 3; i++) {
		int idx = i * 3;
		Vertex pos(model->positions[idx], model->positions[idx + 1], model->positions[idx + 2]);
		temp.setPosition(pos);
		Vertex norm(model->normals[idx], model->normals[idx + 1], model->normals[idx + 2]);
		temp.setNormal(norm);
		idx = i * 2;
		temp.setTexcoord(model->texcoords[idx], model->texcoords[idx + 1]);
		data.push_back(temp);
		model->max_y = max(model->max_y, model->positions[idx + 1]);
		model->min_y = min(model->min_y, model->positions[idx + 1]);
		model->max_z = max(model->max_z, model->positions[idx + 2]);
		model->min_z = min(model->min_z, model->positions[idx + 2]);
	}
	generateVAO(data, vao);
}

// ### hint
// load any object model you want with texture below
// be careful of some obect model may need ".mtl file" which file name shouldn't be changed and the size of model & model's default orientaion may be different.
// it's not all of the model compoesd by triangle. you can know how to draw the model by drop the object file in to visual studio & you can see what kind of polygon makes up the model
// this demo using the two kind of model. one is "Eevee" composed of triangle another one is "Umbreon" composed of quads. (by the way Pikachu is also composed of triangle)
void bindbufferInit() {
	bufferModel(Eevee, &vao_e);
	bufferModel(Pikachu, &vao_p);
	generateVAO(Pokeball, &vao_b);
}

void textureInit() {
	glEnable(GL_TEXTURE_2D);
	loadTexture("Eevee.jpg", &texture_e);
	loadTexture("Pikachu.png", &texture_p);
	loadTexture("Pokeball.png", &texture_b);
}

void display() {
	Start = clock();
	//Clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glClearDepth(1.0f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	calculatePhysics();
	demo();
	End = clock();
	glutSwapBuffers();
}

void reshape(GLsizei w, GLsizei h) {
	windowSize[0] = w;
	windowSize[1] = h;
}

void loadTexture(const char* tFileName, GLuint* texture_id) {
	GLuint _texture;
	static GLuint idx = 0;
	*texture_id = idx;
	glActiveTexture(GL_TEXTURE0 + idx++);
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	} else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

void idle() {
	// FPS control
	clock_t CostTime = End - Start;
	float PerFrameTime = 1000.0 / MAX_FPS;
	if (CostTime < PerFrameTime) {
		Sleep(ceil(PerFrameTime) - CostTime);
	}
	glutPostRedisplay();
}

void drawSphere(vector<VertexAttribute>& ball, float radius, float slice, float stack) {
	float theta, phi, xy_step = 360 / slice, z_step = 180 / stack;
	Vertex vert;
	float u, v;
	for (phi = -90; phi <= 90; phi += z_step) {
		VertexAttribute temp;
		for (theta = 0; theta <= 360; theta += xy_step) {
			vert.x = radius * sin(theta * M_PI / 180) * cos(phi * M_PI / 180);
			vert.y = radius * cos(theta * M_PI / 180) * cos(phi * M_PI / 180);
			vert.z = radius * sin(phi * M_PI / 180);
			temp.setPosition(vert);

			temp.setNormal(vert);

			if (vert.z >= 0) {
				u = (vert.x + radius) / (4 * radius) + 0.25;
				v = (vert.y + radius) / (2 * radius);
			} else if (vert.x >= 0) {
				u = (vert.x - radius) * 0.25 / -radius + 0.75;
				v = (vert.y + radius) / (2 * radius);
			} else {
				u = vert.x * 0.25 / -radius;
				v = (vert.y + radius) / (2 * radius);
			}
			temp.setTexcoord(u, v);
			ball.push_back(temp);

			vert.x = radius * sin(theta * M_PI / 180) * cos((phi + z_step) * M_PI / 180);
			vert.y = radius * cos(theta * M_PI / 180) * cos((phi + z_step) * M_PI / 180);
			vert.z = radius * sin((phi + z_step) * M_PI / 180);
			temp.setPosition(vert);

			temp.setNormal(vert);

			if (vert.z >= 0) {
				u = (vert.x + radius) / (4 * radius) + 0.25;
				v = (vert.y + radius) / (2 * radius);
			} else if (vert.x >= 0) {
				u = (vert.x - radius) * 0.25 / -radius + 0.75;
				v = (vert.y + radius) / (2 * radius);
			} else {
				u = vert.x * 0.25 / -radius;
				v = (vert.y + radius) / (2 * radius);
			}
			temp.setTexcoord(u, v);
			ball.push_back(temp);
		}
	}
}

void drawModel(ModelStatus status, float expand_ratio, GLsizei vertex_num, GLuint vao, GLuint texture_ID, glm::mat4& M, GLenum DrawingMode) {
	GLuint _location, program;

	if (status.expand) {
		program = expand_program;
		glUseProgram(program);
		GLuint location = glGetUniformLocation(program, "ratio");
		glUniform1f(location, expand_ratio);
		glUseProgram(0);
	} else {
		program = toon_program;
	}

	glUseProgram(program);

	_location = glGetUniformLocation(program, "M");
	glUniformMatrix4fv(_location, 1, GL_FALSE, &M[0][0]);

	glm::mat4 V = getV();
	_location = glGetUniformLocation(program, "V");
	glUniformMatrix4fv(_location, 1, GL_FALSE, &V[0][0]);

	glm::mat4 P = getP();
	_location = glGetUniformLocation(program, "P");
	glUniformMatrix4fv(_location, 1, GL_FALSE, &P[0][0]);

	_location = glGetUniformLocation(program, "texture");
	glUniform1i(_location, texture_ID);

	_location = glGetUniformLocation(program, "WorldLightPos");
	glUniform3f(_location, WorldLightPos.x, WorldLightPos.y, WorldLightPos.z);

	_location = glGetUniformLocation(program, "WorldCamPos");
	glUniform3f(_location, WorldCamPos.x, WorldCamPos.y, WorldCamPos.z);

	_location = glGetUniformLocation(program, "edge_glow");
	glUniform1i(_location, status.glow);

	glBindVertexArray(vao);
	glDrawArrays(DrawingMode, 0, vertex_num);
	glBindVertexArray(0);
	glActiveTexture(0);
	glUseProgram(0);
}

void Sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

glm::mat4 getV() {
	// set camera position and configuration
	return glm::lookAt(glm::vec3(WorldCamPos.x, WorldCamPos.y, WorldCamPos.z), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

glm::mat4 getP() {
	// set perspective view
	float fov = 45.0f;
	float aspect = windowSize[0] / windowSize[1];
	float nearDistance = 1.0f;
	float farDistance = 1000.0f;
	return glm::perspective(glm::radians(fov), aspect, nearDistance, farDistance);
}

void calculatePhysics() {
	applyGravity(status_p);
	applyGravity(status_e);
	applyGravity(status_b);
	restrictY(status_b, 1.5);
}

// ### hint
// Don't be afraid of this hw. It looks like a lot of code because I need to make sure tha all effect (3 kind of shader) can show with two model in different drawing way .
// You can just focus on one of effect(specifically reading expand.geom & knowing how it works ) to write your shader and create the effect you want to be displaied on video. 
// Reminding again : Eevee model & Umbreon model are composed of different polygon . (Don't forget Pikachu which is also provided in this homework. you can ues it.)
void demo() {
	Vertex _pos;
	glm::mat4 M_base(1.0f), M;

	M_base = glm::rotate(M_base, glm::radians(scene_angle), glm::vec3(0, 1, 0));

	// draw Pikachu
	_pos = status_p.position;
	M = M_base;
	M = glm::scale(M, glm::vec3(2, 2, 2));
	M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0, 1, 0));
	M = glm::translate(M, glm::vec3(-1 * _pos.z, _pos.y, _pos.x));
	M = glm::rotate(M, glm::radians(status_p.angle_h), glm::vec3(0, 1, 0));
	M = glm::rotate(M, glm::radians(status_p.angle_v), glm::vec3(-1, 0, 0));
	drawModel(status_p, 0.1f, Pikachu->positions.size() / 3 , vao_p, texture_p, M, GL_TRIANGLES);

	// draw Eevee
	_pos = status_e.position;
	M = M_base;
	M = glm::scale(M, glm::vec3(0.1, 0.1, 0.1));
	M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0, 0, 1));
	M = glm::rotate(M, glm::radians(90.0f), glm::vec3(0, -1, 0));
	M = glm::translate(M, glm::vec3(20 * _pos.z, -20 * _pos.x, -20 * _pos.y));
	M = glm::rotate(M, glm::radians(status_e.angle_h), glm::vec3(0, 0, -1));
	M = glm::rotate(M, glm::radians(status_e.angle_v), glm::vec3(1, 0, 0));
	drawModel(status_e, 2.0f, Eevee->positions.size() / 3 , vao_e, texture_e, M, GL_TRIANGLES);

	// draw Pokeball
	_pos = status_b.position;
	M = M_base;
	M = glm::translate(M, glm::vec3(_pos.x, _pos.y, _pos.z));
	M = glm::rotate(M, glm::radians(status_b.angle_h), glm::vec3(0, 1, 0));
	M = glm::rotate(M, glm::radians(status_b.angle_v), glm::vec3(0, 0, 1));
	drawModel(status_b, 0, Pokeball.size(), vao_b, texture_b, M, GL_TRIANGLE_STRIP);
}