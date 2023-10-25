#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Texture/Particle/Particle.h"
#include "Engine/WorldTransform/WorldTransform.h"

class Effect
{
public:
	Effect();
	~Effect()= default;

	void ModelLoad();

	void Update(const Matrix4x4& cameraMat);

	void Draw(const Matrix4x4& viewProjection);

	void SetParent(const WorldTransform& parent);

private:
	std::unique_ptr<Particle> particleResources_;
	//std::vector<WorldTransform> particleTrans_;
	WorldTransform particleTrans_[100];

};

