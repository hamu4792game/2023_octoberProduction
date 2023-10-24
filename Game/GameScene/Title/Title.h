#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Texture/Particle/Particle.h"
#include <vector>
#include "Game/Effect/Effect.h"

class Title
{
public:
	Title(Camera* camera);
	~Title() = default;

	//	初期化
	void Initialize();
	//	更新処理
	void Update();
	//	描画処理
	void Draw3D(const Matrix4x4& viewProjection);
	void Draw2D(const Matrix4x4& viewProjection);

	void ModelLoad();

private: // メンバ変数
	
	std::vector <std::shared_ptr<Model>> model_;
	std::vector<WorldTransform> modelTrans_;
	//	タイトル
	Camera* camera_ = nullptr;

	std::unique_ptr<Model> cloudResources_;
	WorldTransform cloudTrans_;
	std::unique_ptr<Texture2D> particleResources_;
	std::vector<WorldTransform> particleTrans_;

	std::unique_ptr<Texture2D> titleResources_;

	std::unique_ptr<Particle> dust_;
	WorldTransform dustTrans_[30];

	std::unique_ptr<Effect> eff;

private: // メンバ関数
	void BackParticle();

public: // モデルのセット
	void SetModels(std::vector<std::shared_ptr<Model>> model) { model_ = model; };



};

