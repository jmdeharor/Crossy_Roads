#pragma once
class RenderizableObject {
public:
	virtual void render() = 0;
	RenderizableObject();
	virtual ~RenderizableObject();
};

