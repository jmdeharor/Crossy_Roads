#ifndef BASECAMERA_H
#define BASECAMERA_H
#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class BaseCamera {
protected:
    glm::mat4 VM, PM;
public:
    glm::vec3 VRP;
    float zNear, zFar;

    BaseCamera();
    BaseCamera(glm::vec3 VRP, float zNear, float zFar);
    virtual ~BaseCamera();

    virtual void updateVM() = 0;
    virtual void updatePM() = 0;
    virtual void resize(int h, int w) = 0;
    const float *getViewMatrix() const;
    const float *getProjectionMatrix() const;
    const glm::mat4* getFullViewMatrix() const;
};

#endif // BASECAMERA_H
