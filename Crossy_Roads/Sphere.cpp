#include "Sphere.h"
using namespace glm;

#define PI 3.14159f

void Sphere::init(int numCircles, int pointsPerCircle, float diameter, ShaderProgram& program) {
	vector<float> vertices;
	vector<float> colors;
	vector<float> normals((numCircles*pointsPerCircle + 2) * 3);
	vector<vec3> norms(numCircles * pointsPerCircle + 2, vec3(0));
	vector<int> occurrences(numCircles * pointsPerCircle + 2, 0);
	colors.reserve((numCircles*pointsPerCircle + 2) * 3);
	vertices.reserve((numCircles*pointsPerCircle + 2) * 3);
	vector<unsigned int> indices;
	indices.reserve(numCircles*pointsPerCircle);
	float hstep = diameter / (numCircles+1);
	float angleStep = 2*PI / pointsPerCircle;
	vertices.push_back(0);
	vertices.push_back(hstep*(numCircles / 2 + 1));
	vertices.push_back(0);
	colors.push_back(1);
	colors.push_back(1);
	colors.push_back(1);

	for (int i = numCircles/2; i >= -numCircles/2; --i) {
		float radius = cos(asin((float)i/(numCircles/2 + 1)))*(diameter/2);
		float h = i*hstep;
		float angle = 0;
		for (int j = 0; j < pointsPerCircle; ++j) {
			vertices.push_back(cos(angle)*radius);
			colors.push_back(1);

			vertices.push_back(h);
			colors.push_back(1);

			vertices.push_back(sin(angle)*radius);
			colors.push_back(1);

			angle += angleStep;
			
		}
	}
	vertices.push_back(0);
	vertices.push_back(-hstep*(numCircles / 2 + 1));
	vertices.push_back(0);
	colors.push_back(1);
	colors.push_back(1);
	colors.push_back(1);

	for (int i = 0; i < pointsPerCircle; ++i) {
		indices.push_back(0);
		indices.push_back(i+1);
		indices.push_back((i + 1) % pointsPerCircle + 1);
	}
	int nVert = vertices.size() / 3;
	int startPoint = nVert - pointsPerCircle - 1;
	for (int i = 0; i < pointsPerCircle; ++i) {
		indices.push_back(nVert -1);
		indices.push_back(i + startPoint);
		indices.push_back((i+1) % pointsPerCircle + startPoint);
	}
	for (int i = 0; i < numCircles - 1; ++i) {
		int startPoint = i*pointsPerCircle + 1;
		for (int j = 0; j < pointsPerCircle; ++j) {
			indices.push_back(j + startPoint);
			indices.push_back((j + 1) % pointsPerCircle + startPoint);
			indices.push_back(j + pointsPerCircle + startPoint);
		}
		int startPoint2 = (i + 1)*pointsPerCircle + 1;
		for (int j = 0; j < pointsPerCircle; ++j) {
			indices.push_back(j + startPoint2);
			indices.push_back((j + 1) % pointsPerCircle + startPoint2);
			indices.push_back((j + 1) % pointsPerCircle + startPoint);
		}
	}
	for (int i = 0; i < indices.size(); i += 3) {
		vec3 point1 = vec3(vertices[indices[i] * 3], vertices[indices[i] * 3 + 1], vertices[indices[i] * 3 + 2]);
		vec3 point2 = vec3(vertices[indices[i+1] * 3], vertices[indices[i + 1] * 3 + 1], vertices[indices[i + 1] * 3 + 2]);
		vec3 point3 = vec3(vertices[indices[i+2] * 3], vertices[indices[i + 2] * 3 + 1], vertices[indices[i + 2] * 3 + 2]);

		vec3 norm = cross(point3 - point1, point2 - point1);
		norms[indices[i]] += norm;
		occurrences[indices[i]] += 1;
		norms[indices[i+1]] += norm;
		occurrences[indices[i+1]] += 1;
		norms[indices[i+2]] += norm;
		occurrences[indices[i+2]] += 1;
	}
	for (int i = 0; i < norms.size(); ++i) {
		vec3 finalNorm = normalize(norms[i] / (float)occurrences[i]);
		normals[i * 3] = finalNorm.x;
		normals[i * 3 + 1] = finalNorm.y;
		normals[i * 3 + 2] = finalNorm.z;
	}
	size = indices.size();
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBOvert);
	glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = program.bindVertexAttribute("position", 3, 0, 0);
	glGenBuffers(1, &VBOcols);
	glBindBuffer(GL_ARRAY_BUFFER, VBOcols);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), &colors[0], GL_STATIC_DRAW);
	colorLocation = program.bindVertexAttribute("color", 3, 0, 0);
	glGenBuffers(1, &VBOnorm);
	glBindBuffer(GL_ARRAY_BUFFER, VBOnorm);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);
	normalLocation = program.bindVertexAttribute("normal", 3, 0, 0);
	glGenBuffers(1, &VBOind);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOind);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Sphere::render() {
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(colorLocation);
	glEnableVertexAttribArray(normalLocation);
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
}

Sphere::Sphere()
{
}


Sphere::~Sphere()
{
}
