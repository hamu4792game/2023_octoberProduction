#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize()
{
	//	カメラの読み込みと生成
	camera = std::make_shared<Camera>(2000.0f, true);
	camera2d = std::make_shared<Camera>();
	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

	//	モデルの生成
	model_ = std::make_shared<Model>();
	hud_ = std::make_shared<Texture2D>();

	ModelLoad();
	
	//	シーンの生成と初期化
	title = std::make_unique<Title>();
	title->Initialize();
	title->SetModels(model_);
	title->SetHud(hud_);

	battle = std::make_unique<Battle>();
	battle->Initialize();

	//	変数の初期化
	scene = Scene::TITLE;
	oldscene = Scene::RESULT;

}

void GameScene::Update()
{
#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("translate", &camera->transform.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &camera->transform.rotation_.x, AngleToRadian(1.0f));
	ImGui::DragFloat3("scale", &camera->transform.scale_.x, 0.1f);
	ImGui::End();
#endif // _DEBUG

	

	//	シーン切替わり時の初期化
	if (oldscene != scene) {
		switch (scene)
		{
		case GameScene::Scene::TITLE:
			title->Initialize();
			break;
		case GameScene::Scene::BATTLE:
			battle->Initialize();
			break;
		case GameScene::Scene::RESULT:
			break;
		}
	}
	oldscene = scene;

	switch (scene)
	{
	case GameScene::Scene::TITLE:
		title->Update();
		break;
	case GameScene::Scene::BATTLE:
		battle->Update();
		break;
	case GameScene::Scene::RESULT:
		break;
	}

	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

}

void GameScene::Draw()
{

	//	3D描画
	switch (scene)
	{
	case GameScene::Scene::TITLE:
		title->Draw3D(viewProjectionMatrix);
		break;
	case GameScene::Scene::BATTLE:
		battle->Draw3D(viewProjectionMatrix);
		break;
	case GameScene::Scene::RESULT:
		break;
	}

	//	2D描画
	switch (scene)
	{
	case GameScene::Scene::TITLE:
		title->Draw2D(viewProjectionMatrix2d);
		break;
	case GameScene::Scene::BATTLE:
		battle->Draw2D(viewProjectionMatrix2d);
		break;
	case GameScene::Scene::RESULT:
		break;
	}

}


void GameScene::ModelLoad()
{
	model_->Texture("Resources/eatRamen/eatRamen.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
	hud_->Texture("Resources/uvChecker.png", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl");
}
