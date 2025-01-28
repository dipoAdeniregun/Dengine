#include "ParticleEngine2D.h"
#include "ParticleBatch2D.h"
#include "SpriteBatch.h"

namespace Dengine {
	ParticleEngine2D::ParticleEngine2D()
	{
	}

	ParticleEngine2D::~ParticleEngine2D()
	{
		for (auto& b : batches) {
			delete b;
		}
		batches.clear();
	}

	void ParticleEngine2D::addParticleBatch(ParticleBatch2D* particleBatch)
	{
		batches.push_back(particleBatch);
	}

	void ParticleEngine2D::update(float dt) {
		for (auto& b : batches)
		{
			b->update(dt);
		}
	}

	void ParticleEngine2D::draw(SpriteBatch* _spriteBatch) {
		for (auto& b : batches)
		{
			_spriteBatch->begin();
			b->draw(_spriteBatch);

			_spriteBatch->end();
			_spriteBatch->renderBatch();
		}
		
	}
}
