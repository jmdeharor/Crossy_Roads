#pragma once
#include "obscamera.h"
#include "orthocamera.h"

class OBSOrthoCamera : public OBSCamera, public OrthoCamera{
public:
	OBSOrthoCamera();
	~OBSOrthoCamera();
};

