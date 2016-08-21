#ifndef H_PARTICLE_GENERATOR
#define H_PARTICLE_GENERATOR
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "model.hpp"
#include "shader.hpp"

struct Particle {
	glm::vec3 position; 
	glm::vec3 velocity;
	glm::vec4 color;
	GLfloat life; // maleje z czasem

	Particle():position(0.0f), velocity(0.0f), color(1.0f), life(0.0f) { }
};

class ParticleGenerator
{
public:
	// konstruktor klasy
	ParticleGenerator(ShaderProgram shader, Model texture, GLuint amount);
	// odswiez wszystkie czastki
	void update(GLfloat dt, glm::vec3 position, glm::vec3 velocity, GLuint newParticles, glm::vec3 offset = glm::vec3(0.0f, 0.0f,0.0f)); // dt - delta time
	//renderowanie wszystkich czastek
	void draw();
private:
	// stan rendera
	ShaderProgram shader;
	Model texture;
	GLuint VAO;
	// stan
	std::vector<Particle> particles;
	GLuint amount;
	// inicjalizacja bufora i atrybutow vertexa
	void init();
	// zwraca pierwsza aktualnie nieuzywana czastke tzn taka ktorej life <= 0.0f lub 0 jesli nie ma nieaktywnej czastki
	GLuint firstUnusedParticle();
	// 
	void respawnParticle(Particle &particle, glm::vec3 position, glm::vec3 velocity, glm::vec3 offset = glm::vec3(0.0f, 0.0f,0.0f));
};

#endif
