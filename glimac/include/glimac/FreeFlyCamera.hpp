#pragma once
#include "glimac/glm.hpp"

class FreeFlyCamera{

public:
	FreeFlyCamera(float rayon);
	void computeDirectionVectors();
	void moveLeft(float t);
	void moveFront(float t);
	void rotateLeft(float degrees);
	void rotateUp(float degrees);
	void boundaryTest();

	glm::mat4 getViewMatrix() const;

private:
	glm::vec3 m_Position;
	float m_fPhi;
	float m_fTheta;
	float maxX;
	float maxY;
	float maxZ;
	float minX;
	float minY;
	float minZ;

	glm::vec3 m_FrontVector;
	glm::vec3 m_LeftVector;
	glm::vec3 m_UpVector;

};