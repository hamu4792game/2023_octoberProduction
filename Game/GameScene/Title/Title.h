#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"


class Title
{
public:
	Title();
	~Title() = default;

	//	初期化
	void Initialize();
	//	更新処理
	void Update();
	//	描画処理
	void Draw3D(const Matrix4x4& viewProjection);
	void Draw2D(const Matrix4x4& viewProjection);

private: // メンバ変数
	
	std::shared_ptr<Model> model_;
	std::shared_ptr<Texture2D> hud_;
	WorldTransform modelTrans;
	WorldTransform hudTrans;
public: // モデルのセット
	void SetModels(std::shared_ptr<Model> model) { model_ = model; };
	void SetHud(std::shared_ptr<Texture2D> hud) { hud_ = hud; };



};

