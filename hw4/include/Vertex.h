#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include "glew.h"

class Vertex {
public:
	float x;
	float y;
	float z;

	// default constructor
	Vertex(): x(0), y(0), z(0) {}
	// constructor
	Vertex(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
	// copy constructor
	Vertex(Vertex const& obj) {
		this->x = obj.x;
		this->y = obj.y;
		this->z = obj.z;
	}

	Vertex& operator = (Vertex const& obj) {
		this->x = obj.x;
		this->y = obj.y;
		this->z = obj.z;
		return *this;
	}
	Vertex operator + (Vertex const& obj) {
		Vertex temp;
		temp.x = this->x + obj.x;
		temp.y = this->y + obj.y;
		temp.z = this->z + obj.z;
		return temp;
	}
	Vertex operator - (Vertex const& obj) {
		Vertex temp;
		temp.x = this->x - obj.x;
		temp.y = this->y - obj.y;
		temp.z = this->z - obj.z;
		return temp;
	}
	Vertex operator / (float const& num) {
		Vertex temp;
		temp.x = this->x / num;
		temp.y = this->y / num;
		temp.z = this->z / num;
		return temp;
	}

	Vertex& operator += (Vertex const& obj) {
		*this = *this + obj;
		return *this;
	}

	bool operator == (Vertex const& obj) {
		return this->x == obj.x && this->y == obj.y && this->z == obj.z;
	}
	bool operator != (Vertex const& obj) {
		return this->x != obj.x || this->y != obj.y || this->z != obj.z;
	}

	void normalize() {
		float magnitude;
		magnitude = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		*this = *this / magnitude;
	}
	Vertex cross(Vertex v2) {
		Vertex temp;
		temp.x = this->y * v2.z - this->z * v2.y;
		temp.y = this->z * v2.x - this->x * v2.z;
		temp.z = this->x * v2.y - this->y * v2.x;
		return temp;
	}
	Vertex GetNormalize() {
		float magnitude;
		magnitude = sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		return *this / magnitude;
	}

	friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
		os << v.x << "," << v.y << "," << v.z;
		return os;
	}
};

//Storing vertex datas that will be sent to shader
class VertexAttribute {
public:
	Vertex position;
	GLfloat texcoord[2];
	Vertex normal;
	void setPosition(float x, float y, float z) {
		position.x = x;
		position.y = y;
		position.z = z;
	};
	void setPosition(Vertex v) {
		position.x = v.x;
		position.y = v.y;
		position.z = v.z;
	};
	void setTexcoord(float u, float v) {
		texcoord[0] = u;
		texcoord[1] = v;
	}
	void setNormal(float x, float y, float z) {
		normal.x = x;
		normal.y = y;
		normal.z = z;
	}
	void setNormal(Vertex v) {
		normal.x = v.x;
		normal.y = v.y;
		normal.z = v.z;
	};
};

#endif