#pragma once

#include <memory>
#include <string>
#include <algorithm>
#include <driver_types.h>

typedef float real;

namespace NBody
{
	class NBodySystem
	{
	private:
		bool m_initialized;

		// buffer memory.
		real* m_deviceVelocity;
		real* m_devicePosition[2];

		real m_damping;
		real m_softening;
		unsigned int m_numBodies;
		unsigned int m_blockSize;

		// double buffers.
		unsigned int m_pbo[2];
		unsigned int m_currentRead;
		unsigned int m_currentWrite;
		cudaGraphicsResource* m_pGRes[2];

	public:
		typedef std::shared_ptr<NBodySystem> ptr;

		NBodySystem(unsigned int nBodies, unsigned int blockSize);
		virtual ~NBodySystem();

		void setDamping(real damping);
		void setBlockSize(unsigned int size);
		void setSofteningFactor(real softening);
		void setParticlesPosition(real* data);
		void setParticlesVelocity(real* data);

		void loadDatFile(const std::string& path);
		void loadTabFile(const std::string& path);
		void loadSnapFile(const std::string& path);
		void loadTipsyFile(const std::string& path);

		real& getDamping() { return m_damping; }
		real& getSofteningFactor() { return m_softening; }
		unsigned int& getBlockSize() { return m_blockSize; }
		unsigned int& getNumBodyies() { return m_numBodies; }
		unsigned int getNumBodyies()const { return m_numBodies; }
		unsigned int getCurrentBuffer()const { return m_pbo[m_currentRead]; }

		void simulate(real deltaTime);

		void registerCudaGraphicsResource();

	public:
		void initialize(int numBodies);
		void finalize();

		void getLastCudaError(const char* errorMessage);
	};

}