#include "ParticleBatch2D.h"

namespace Dengine {

	ParticleBatch2D::ParticleBatch2D()
	{
	}

	ParticleBatch2D::~ParticleBatch2D()
	{
		delete[] particles;
	}

	void ParticleBatch2D::init( int _maxParticles, float _decayRate,
								GLTexture _texture,
								std::function<void(Particle2D&, float)> _updateFunc /* = defaultFuncUpdate*/)

	{
		maxParticles = _maxParticles;
		particles = new Particle2D[maxParticles];
		decayRate = _decayRate;
		texture = _texture;
		updateFunc = _updateFunc;
	}

	void ParticleBatch2D::addParticle(const glm::vec2& _position, 
									  const glm::vec2& _velocity, 
									  float _size,
									  const ColorRGBA8& _color)
	{
		int particleIndex = findFreeParticle();
		auto& p = particles[particleIndex];

		p.life = 1.0f;
		p.position = _position;
		p.velocity = _velocity;
		p.size = _size;
		p.color = _color;
	}

	void ParticleBatch2D::update(float dt)
	{
		for (size_t i = 0; i < maxParticles; i++)
		{
			//only update active particles
			if (particles[i].life > 0.0f)
			{
				updateFunc(particles[i], dt);
				//particles[i].update(dt);
				particles[i].life -= decayRate * dt;
			}
		}
	}

	void ParticleBatch2D::draw(SpriteBatch* _spriteBatch)
	{
		static glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);

		for (size_t i = 0; i < maxParticles; i++)
		{
			auto& p = particles[i];

			//only update active particles
			if (particles[i].life > 0.0f)
			{
				glm::vec4 destRect(p.position.x, p.position.y, p.size, p.size);
				_spriteBatch->draw(destRect, uvRect, texture.id, 0, p.color);
			}
		}
	}

	int ParticleBatch2D::findFreeParticle()
	{
		for (int i = lastFreeParticle; i < maxParticles; i++) {
			if (particles[i].life <= 0.0f) {
				lastFreeParticle = i;
				return i;
			}
		}
		
		for (int i = 0; i < lastFreeParticle; i++) {
			if (particles[i].life <= 0.0f) {
				lastFreeParticle = i;
				return i;
			}
		}

		//overwrite 1st particle if nothing is free
		return 0;
	}
	
}
