#pragma once

#include <glm/glm.hpp>
#include <functional>

#include "Vertex.h"
#include "SpriteBatch.h"
#include "GLTexture.h"

namespace Dengine {

	class Particle2D {
	public:
				glm::vec2 position = glm::vec2(0.0f);
		glm::vec2 velocity = glm::vec2(0.0f);
		ColorRGBA8 color = ColorRGBA8(255, 255, 255, 255);
		float life = 1.0f;
		float size = 0.0f;		//assuming square particles

	private:
		
		
	};

	inline void defaultParticleUpdate(Particle2D& _particle, float dt) {
		_particle.position += _particle.velocity * dt;
	}

	class ParticleBatch2D
	{
	public:
		ParticleBatch2D();
		~ParticleBatch2D();

		void init(int _maxParticles, float _decayRate,
				  GLTexture _texture,
				  std::function<void(Particle2D&, float)> _updateFunc = defaultParticleUpdate);

		void addParticle(const glm::vec2& _position,
						 const glm::vec2& _velocity,
						 float _size,
						 const ColorRGBA8& _color);

		void update(float dt);

		//assumes all particles in a particle batch share one sprite
		void draw(SpriteBatch* _spriteBatch);		 

		
	private:
		std::function<void(Particle2D&, float)> updateFunc;

		int findFreeParticle();

		float decayRate = 0.1f;
		Particle2D* particles = nullptr;
		int maxParticles = 0;
		
		GLTexture texture;

		int lastFreeParticle = 0;
	};

}