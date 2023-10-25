#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/Camera/Camera.h"
#include "Engine/Texture/Particle/Particle.h"
#include <vector>

class Result
{
public:
	Result(Camera* camera);
	~Result() = default;

	//	初期化
	void Initialize();
	//	更新処理
	void Update();
	//	描画処理
	void Draw3D(const Matrix4x4& viewProjection);
	void Draw2D(const Matrix4x4& viewProjection);

	void ModelLoad();
	void TextureLoad();

private: // メンバ変数

	std::vector <std::shared_ptr<Model>> model_;
	std::vector<WorldTransform> modelTrans_;
	//	タイトル
	std::shared_ptr<Texture2D> hud_;
	WorldTransform hudTrans_;

	Camera* camera_ = nullptr;

	std::unique_ptr<Model> cloudResources_;
	WorldTransform cloudTrans_;
	std::unique_ptr<Texture2D> particleResources_;
	std::vector<WorldTransform> particleTrans_;

	std::unique_ptr<Texture2D> titleResources_;
	WorldTransform titleTrans_;

	std::unique_ptr<Texture2D> pressResources_;
	WorldTransform pressTrans_;

	/*std::unique_ptr<Texture2D> BResources_;
	WorldTransform BTrans_;*/

	std::unique_ptr<Particle> dust_;
	WorldTransform dustTrans_[30];

	//イージング用
	Vector3 clearEaseStart_;
	Vector3 clearEaseEnd_;
	float clearEaseNum_;
	float clearT_;
	float clearEaseSpeed_;
	bool isClearEase;
	//Bを押せー
	int pressColor;
	float pressMagnification;
	int pressEaseStart_;
	int pressEaseEnd_;
	float pressEaseNum_;
	float pressT_;
	float pressEaseSpeed_;
	bool isPressEase;

private: // メンバ関数
	void BackParticle();
	void DrawImgui();

public: // モデルのセット
	void SetModels(std::vector<std::shared_ptr<Model>> model) { model_ = model; };
	void SetHud(std::shared_ptr<Texture2D> hud) { hud_ = hud; };



};


