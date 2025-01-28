#pragma once

#include <vector>



namespace Dengine {

	class ParticleBatch2D;
	class SpriteBatch;

	class ParticleEngine2D
	{
	public:
		ParticleEngine2D();
		~ParticleEngine2D();

		//after adding a particle batch, ParticleEngine2D is responsible for deallocation
		void addParticleBatch(ParticleBatch2D* particleBatch);

		void update(float dt);

		void draw(SpriteBatch* _spriteBatch);

	private:
		std::vector<ParticleBatch2D*> batches;
	};

}
