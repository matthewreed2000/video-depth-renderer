#pragma once

class Shader {
public:
	Shader(const char* filename, unsigned int shaderType);
	~Shader();

	void destroy();

	inline unsigned int getID() const { return id; };
private:
	unsigned int id;
	bool alive;
};