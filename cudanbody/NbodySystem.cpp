#include "NBodySystem.h"

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <cuda_gl_interop.h>

#include "Tipsy.h"

void integrateNBodySystem(
	real** devicePosition,
	real* deviceVelocity,
	cudaGraphicsResource** pGRes,
	unsigned int currentRead,
	float deltaTime,
	float damping,
	unsigned int numBodies,
	int blockSize);

cudaError_t setSofteningSquared(real softeningSq);

namespace NBody
{
	NBodySystem::NBodySystem(unsigned int nBodies, unsigned int blockSize)
		:
		m_numBodies(nBodies),
		m_blockSize(blockSize),
		m_initialized(false),
		m_currentRead(0),
		m_currentWrite(1)
	{
		m_deviceVelocity = nullptr;
		m_devicePosition[0] = nullptr;
		m_devicePosition[1] = nullptr;

		initialize(nBodies);

		setDamping(1.0f);
		setSofteningFactor(0.00125f);
	}

	NBodySystem::~NBodySystem()
	{
		finalize();
		m_numBodies = 0;
	}

	void NBodySystem::setDamping(real damping)
	{
		m_damping = damping;
	}

	void NBodySystem::setBlockSize(unsigned int size)
	{
		m_blockSize = size;
	}

	void NBodySystem::setSofteningFactor(real softening)
	{
		m_softening = softening;
		real softeningSq = softening * softening;
		setSofteningSquared(softeningSq);
	}

	void NBodySystem::setParticlesPosition(real* data)
	{
		// set particles' initial position.
		if (!m_initialized)
			return;
		m_currentRead = 0;
		m_currentWrite = 1;
		glBindBuffer(GL_ARRAY_BUFFER, m_pbo[m_currentRead]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(real) * m_numBodies, data);
		int size = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, (GLint*)&size);
		if ((unsigned)size != 4 * (sizeof(real) * m_numBodies))
			fprintf(stderr, "WARNING: Pixel Buffer Object download failed!\n");
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//glBindBuffer(GL_ARRAY_BUFFER, m_pbo[m_currentWrite]);
		//glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(real) * m_numBodies, data);
		//size = 0;
		//glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, (GLint *)&size);
		//if ((unsigned)size != 4 * (sizeof(real) * m_numBodies))
		//	fprintf(stderr, "WARNING: Pixel Buffer Object download failed!\n");
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void NBodySystem::setParticlesVelocity(real* data)
	{
		// set particles' initial velocity.
		if (!m_initialized)
			return;
		m_currentRead = 0;
		m_currentWrite = 1;

		cudaMemcpy(m_deviceVelocity, data, m_numBodies * 4 * sizeof(real),
			cudaMemcpyHostToDevice);
	}

	void NBodySystem::loadDatFile(const std::string& path)
	{
		// load the bodies from a dat file.
		if (m_initialized)
			finalize();
		std::vector<real> positions;
		std::vector<real> velocities;

		int nBodies = 0;
		readDatFile(path, positions, velocities, nBodies);

		initialize(nBodies);
		setParticlesPosition(&positions[0]);
		setParticlesVelocity(&velocities[0]);
	}

	void NBodySystem::loadTabFile(const std::string& path)
	{
		// load the bodies from a tab file.
		if (m_initialized)
			finalize();
		std::vector<real> positions;
		std::vector<real> velocities;

		int nBodies = 0;
		readTabFile(path, positions, velocities, nBodies);

		initialize(nBodies);
		setParticlesPosition(&positions[0]);
		setParticlesVelocity(&velocities[0]);
	}

	void NBodySystem::loadSnapFile(const std::string& path)
	{
		// load the bodies from a tab file.
		if (m_initialized)
			finalize();
		std::vector<real> positions;
		std::vector<real> velocities;

		int nBodies = 0;
		readSnapFile(path, positions, velocities, nBodies);

		initialize(nBodies);
		setParticlesPosition(&positions[0]);
		setParticlesVelocity(&velocities[0]);
	}

	void NBodySystem::loadTipsyFile(const std::string& path)
	{
		// load the bodies from a tipsy file.
		if (m_initialized)
			finalize();
		std::vector<int> ids;
		std::vector<real> positions;
		std::vector<real> velocities;

		int nBodies = 0;
		int nFirst = 0, nSecond = 0, nThird = 0;

		readTipsyFile(path, positions, velocities, ids,
			nBodies, nFirst, nSecond, nThird);

		initialize(nBodies);
		setParticlesPosition(&positions[0]);
		setParticlesVelocity(&velocities[0]);
	}

	void NBodySystem::simulate(real deltaTime)
	{
		if (!m_initialized)
		{
			std::cout << "Haven't been initialized.\n";
			return;
		}

		integrateNBodySystem(
			m_devicePosition,
			m_deviceVelocity,
			m_pGRes,
			m_currentRead,
			deltaTime,
			m_damping,
			m_numBodies,
			m_blockSize);
		cudaDeviceSynchronize();
		getLastCudaError("Kernel execution failed");

		// swap buffer.
		std::swap(m_currentRead, m_currentWrite);
	}

	void NBodySystem::registerCudaGraphicsResource()
	{
		cudaGraphicsGLRegisterBuffer(&m_pGRes[0], m_pbo[0], cudaGraphicsMapFlagsNone);
		cudaGraphicsGLRegisterBuffer(&m_pGRes[1], m_pbo[1], cudaGraphicsMapFlagsNone);
	}

	void NBodySystem::initialize(int numBodies)
	{
		// allocate buffers.
		if (m_initialized)
		{
			std::cout << "Already initialized.\n";
			return;
		}
		m_numBodies = numBodies;
		unsigned int memSize = m_numBodies * 4 * sizeof(real);
		std::vector<real> position(m_numBodies * 4, 0);

		// generate double buffers.
		glGenBuffers(2, m_pbo);

		// initilize devices' buffers.
		glBindBuffer(GL_ARRAY_BUFFER, m_pbo[0]);
		glBufferData(GL_ARRAY_BUFFER, memSize, &position[0], GL_DYNAMIC_DRAW);
		int size = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, (GLint*)&size);
		if ((unsigned)size != memSize)
			fprintf(stderr, "WARNING: Pixel Buffer Object allocation failed!\n");
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_pbo[1]);
		glBufferData(GL_ARRAY_BUFFER, memSize, &position[0], GL_DYNAMIC_DRAW);
		size = 0;
		glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, (GLint*)&size);
		if ((unsigned)size != memSize)
			fprintf(stderr, "WARNING: Pixel Buffer Object allocation failed!\n");
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// allocate velocity buffer.
		cudaMalloc((void**)&m_deviceVelocity, memSize);
		cudaGraphicsGLRegisterBuffer(&m_pGRes[0], m_pbo[0], cudaGraphicsMapFlagsNone);
		cudaGraphicsGLRegisterBuffer(&m_pGRes[1], m_pbo[1], cudaGraphicsMapFlagsNone);

		m_initialized = true;
		getLastCudaError("Initialize failed");
	}

	void NBodySystem::finalize()
	{
		// destory.
		if (!m_initialized)
		{
			std::cout << "No need to finalize.\n";
			return;
		}
		cudaFree(m_deviceVelocity);
		cudaGraphicsUnregisterResource(m_pGRes[0]);
		cudaGraphicsUnregisterResource(m_pGRes[1]);
		glDeleteBuffers(2, m_pbo);
		m_deviceVelocity = nullptr;
		m_devicePosition[0] = nullptr;
		m_devicePosition[1] = nullptr;
		m_initialized = false;
	}

	void NBodySystem::getLastCudaError(const char* errorMessage)
	{
		// check cuda last error.
		cudaError_t err = cudaGetLastError();
		if (cudaSuccess != err) {
			std::cout << "getLastCudaError() CUDA error : "
				<< errorMessage << " : " << "(" << static_cast<int>(err) << ") "
				<< cudaGetErrorString(err) << ".\n";
		}
	}
}