#pragma once
#include "Engine/Camera/Camera.h"

#include "Engine/Texture/Model.h"
#include "Engine/Texture/Texture2D.h"
#include "Engine/Input/AudioInput/AudioInput.h"

#include "Engine/WorldTransform/WorldTransform.h"
#include "Game/GameScene/Title/Title.h"
#include "Game/GameScene/Battle/Battle.h"
#include"Game/GameScene/Result/Result.h"

#include "Game/Stage/Stage.h"

#include<vector>

class GameScene
{
public:
	GameScene() = default;
	~GameScene() = default;

	enum class Scene {
		TITLE,
		BATTLE,
		RESULT
	};
	Scene scene = Scene::TITLE;
	Scene oldscene = Scene::TITLE;

	bool sceneChangeFlag = false;

	//	シングルトンインスタンス
	static GameScene* GetInstance();
	
private:
	
	//	カメラの生成
	std::shared_ptr<Camera> camera;
	std::shared_ptr<Camera> camera2d;
	//	カメラ行列の生成
	Matrix4x4 viewProjectionMatrix{};
	Matrix4x4 viewProjectionMatrix2d{};

	//	シーン用インスタンス
	std::unique_ptr<Title> title;
	std::unique_ptr<Battle> battle;
	std::unique_ptr<Result> result;

public: // 基本処理関数

	void Initialize();

	void Update();
	
	void Draw();

	void Finalize();

private: // メンバ関数
	//	モデルのロード
	void ModelLoad();

	//カメラ更新
	void CameraUpdate();

	//	シーンチェンジ用
	std::shared_ptr<Texture2D> box;
	WorldTransform boxtransform;
	float boxScale = 0.0f;
	float easeNum = 0.0f;
	bool flag = false;
	void SceneChange();

private: // モデル変数
	std::shared_ptr<Model> model_;
	std::unique_ptr<Model> notesModelNormal_;
	std::unique_ptr<Model> notesModelLong_;
	std::unique_ptr<Model> notesModelDamage_;
	std::unique_ptr<Model> notesModelHitLine_;
	std::shared_ptr<Texture2D> hud_;
	std::unique_ptr<Texture2D> hitLine_;

	std::shared_ptr<Model> boxModel_;

	//	主人公のモデル
	std::vector<std::shared_ptr<Model>> heroModel_;
	//	ボスのモデル
	std::vector<std::shared_ptr<Model>> bossModel_;
	//	ステージ用のモデル
	std::vector<std::shared_ptr<Model>> stageModel_;

private: // 音源
	std::unique_ptr<AudioInput> bgm_;

};