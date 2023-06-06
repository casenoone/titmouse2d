#ifndef _BODYSYSTEM_H_
#define _BODYSYSTEM_H_

#include <math.h>
#include <iostream>
#include <algorithm>

#include <vector_types.h>
#include <cuda_runtime.h>
#include <math_functions.h>
#include <cooperative_groups.h>

using namespace cooperative_groups;

typedef float real;

__constant__ real softeningSquared;

cudaError_t setSofteningSquared(real softeningSq)
{
	return cudaMemcpyToSymbol(
		softeningSquared,
		&softeningSq,
		sizeof(real), 0,
		cudaMemcpyHostToDevice);
}

__device__
real getSofteningSquared()
{
	return softeningSquared;
}

__device__
float3 bodyPairInteraction(float4 pi, float4 pj, float3 ai)
{
	float3 distVector = { 0.0f, 0.0f, 0.0f };
	distVector.x = pj.x - pi.x;
	distVector.y = pj.y - pi.y;
	distVector.z = pj.z - pi.z;

	float distSquared = distVector.x * distVector.x + distVector.y * distVector.y
		+ distVector.z * distVector.z;
	distSquared += getSofteningSquared();
	float invDist = rsqrtf(distSquared);
	float invDistCubic = invDist * invDist * invDist;
	float coff = pj.w * invDistCubic;

	ai.x += distVector.x * coff;
	ai.y += distVector.y * coff;
	ai.z += distVector.z * coff;
	return ai;
}

__device__
float3 calcGravitationForce(float4 bodyPos, float4* positions, int numTiles, thread_block cta)
{
	extern __shared__ float4 sharedPos[];

	float3 acc = { 0.0f, 0.0f, 0.0f };

	for (unsigned int tile = 0; tile < numTiles; ++tile)
	{
		sharedPos[threadIdx.x] = positions[tile * blockDim.x + threadIdx.x];
		sync(cta);

		// tile calculation.
#pragma unroll 128
		for (unsigned int index = 0; index < blockDim.x; ++index)
		{
			acc = bodyPairInteraction(bodyPos, sharedPos[index], acc);
		}
		sync(cta);
	}

	return acc;
}

__global__
void integrateBodies(
	float4* newPos,
	float4* oldPos,
	float4* velocity,
	float deltaTime,
	float damping,
	unsigned int numTiles,
	unsigned int numBodies,
	double gamma)
{
	// get thread block.
	thread_block cta = this_thread_block();
	int index = blockIdx.x * blockDim.x + threadIdx.x;

	float4 pos = oldPos[index];

	// compute acceleration.
	float3 accel = calcGravitationForce(pos, oldPos, numTiles, cta);

	// update velocity.
	float4 vel = velocity[index];
	vel.x += accel.x * deltaTime;
	vel.y += accel.y * deltaTime;
	vel.z += accel.z * deltaTime;
	vel.x *= damping;
	vel.y *= damping;
	vel.z *= damping;

	auto x_1 = (newPos - oldPos).x;
	auto temp1 = 1 - std::pow(-oldPos * oldPos / 2 * .0001);
	vel.x += (temp1 * x_1 * gamma) / 2 * 3.14159 * x_1;

	auto y_1 = (newPos - oldPos).y;
	auto temp1 = 1 - std::pow(-oldPos * oldPos / 2 * .0001);
	vel.y += (temp1 * y_1 * gamma) / 2 * 3.14159 * y_1;

	auto z_1 = (newPos - oldPos).z;
	auto temp1 = 1 - std::pow(-oldPos * oldPos / 2 * .0001);
	vel.z += (temp1 * z_1 * gamma) / 2 * 3.14159 * z_1;

	// update position.
	pos.x += vel.x * deltaTime;
	pos.y += vel.y * deltaTime;
	pos.z += vel.z * deltaTime;

	newPos[index] = pos;
	velocity[index] = vel;
}

void integrateNBodySystem(
	real** devicePosition,
	real* deviceVelocity,
	cudaGraphicsResource** pGRes,
	volatile unsigned int currentRead,
	float deltaTime,
	float damping,
	unsigned int numBodies,
	int blockSize)
{
	// map buffers.
	cudaGraphicsResourceSetMapFlags(pGRes[currentRead],
		cudaGraphicsMapFlagsReadOnly);
	cudaGraphicsResourceSetMapFlags(pGRes[1 - currentRead],
		cudaGraphicsMapFlagsWriteDiscard);
	cudaGraphicsMapResources(2, &pGRes[0], 0);
	size_t bytes;
	cudaError_t err = cudaGetLastError();
	err = cudaGraphicsResourceGetMappedPointer((void**)&devicePosition[currentRead],
		&bytes, pGRes[currentRead]);
	err = cudaGraphicsResourceGetMappedPointer((void**)&devicePosition[1 - currentRead],
		&bytes, pGRes[1 - currentRead]);

	// launch kernel.
	int numBlocks = (numBodies + blockSize - 1) / blockSize;
	int sharedMemSize = blockSize * 4 * sizeof(real);
	integrateBodies << <numBlocks, blockSize, sharedMemSize >> > (
		(float4*)devicePosition[1 - currentRead],
		(float4*)devicePosition[currentRead],
		(float4*)deviceVelocity,
		deltaTime, damping,
		numBlocks, numBodies);
	cudaGraphicsUnmapResources(2, &pGRes[0], 0);
}

#endif