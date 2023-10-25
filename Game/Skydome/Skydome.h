#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"

class Skydome
{
public:
	Skydome();
	~Skydome() = default;

	void SetParent(const WorldTransform& world);
	void NullParent();
	void Initialize();

	void ModelLoad();

	void Draw(const Matrix4x4& viewProjection);

	void DrawGameOver(const Matrix4x4& viewProjection);

private:

	std::unique_ptr<Model> skydome;
	std::unique_ptr<Model> skydomeGameOver_;
	WorldTransform transform;
};
