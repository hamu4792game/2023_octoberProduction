#include "Skydome.h"

Skydome::Skydome()
{
	skydome = std::make_unique<Model>();
	skydomeGameOver_ = std::make_unique<Model>();
	Initialize();
}

void Skydome::SetParent(const WorldTransform& world)
{
	transform.parent_ = &world;
}

void Skydome::NullParent()
{
	transform.parent_ = nullptr;
}

void Skydome::Initialize()
{
	transform.scale_ = Vector3(400.0f, 400.0f, 400.0f);
	transform.translation_ = Vector3(0.0f, 0.0f, 300.0f);
}

void Skydome::ModelLoad()
{
	skydome->Texture("Resources/skydome/skydome.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/skydome/skydome6.png");
	skydomeGameOver_->Texture("Resources/skydome/skydome.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/skydome/skydome3.png");
}

void Skydome::Draw(const Matrix4x4& viewProjection)
{
	transform.rotation_.y += AngleToRadian(0.1f);
	transform.UpdateMatrix();
	Model::ModelDraw(transform, viewProjection, 0xffffffff, skydome.get());
}

void Skydome::DrawGameOver(const Matrix4x4& viewProjection)
{
	transform.rotation_.y += AngleToRadian(0.1f);
	transform.UpdateMatrix();
	Model::ModelDraw(transform, viewProjection, 0xffffffff, skydomeGameOver_.get());
}

