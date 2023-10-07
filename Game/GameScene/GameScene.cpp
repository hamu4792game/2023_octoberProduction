#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Easing/Ease.h"
#include <algorithm>

GameScene* GameScene::GetInstance()
{
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize()
{
	//	カメラの読み込みと生成
	camera = std::make_shared<Camera>(2000.0f, true);
	camera->transform.translation_.y = 15.0f;
	camera->transform.translation_.z = -100.0f;
	camera->transform.rotation_.x = 0.0f;
	camera2d = std::make_shared<Camera>();
	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

	//	モデルの生成
	model_ = std::make_shared<Model>();
	notesModelNormal_ = std::make_unique<Model>();
	notesModelLong_ = std::make_unique<Model>();
	notesModelDamage_ = std::make_unique<Model>();
	hud_ = std::make_shared<Texture2D>();

	box = std::make_shared<Texture2D>();

	ModelLoad();
	
	//	シーンの生成と初期化
	title = std::make_unique<Title>();
	title->Initialize();
	title->SetModels(model_);
	title->SetHud(hud_);

	battle = std::make_unique<Battle>();
	std::vector<Model*> noteModels{ notesModelNormal_.get(), notesModelLong_.get(), notesModelDamage_.get() };

	battle->ModelLoad(noteModels);
	battle->Initialize();

	//	変数の初期化
	scene = Scene::BATTLE;
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

	if (KeyInput::PushKey(DIK_S)) {
		sceneChangeFlag = true;
	}

	//	シーンチェンジの処理
	if (sceneChangeFlag) {
		SceneChange();
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

	if (sceneChangeFlag) {
		Texture2D::TextureDraw(boxtransform, viewProjectionMatrix2d, 0x000000ff, box.get());
	}

}

void GameScene::Finalize()
{
	Line::Finalize();
	Model::Finalize();
	Texture2D::Finalize();
}


void GameScene::ModelLoad()
{
	model_->Texture("Resources/eatRamen/eatRamen.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	hud_->Texture("Resources/uvChecker.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");


	box->Texture("Resources/block.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	notesModelNormal_->Texture("Resources/notes/notes.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "notes/normal.png");
	notesModelLong_->Texture("Resources/notes/notes.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "notes/long.png");
	notesModelDamage_->Texture("Resources/notes/notes.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "notes/damage.png");

}


void GameScene::SceneChange()
{
	//80
	boxtransform.rotation_.z += AngleToRadian(15.0f);
	easeNum += 0.02f;
	easeNum = std::clamp(easeNum, 0.0f, 1.0f);
	if (!flag) {
		boxScale = Ease::UseEase(0.0f, 100.0f, easeNum, 1.0f, Ease::EaseType::EaseOutCirc);

		if (easeNum == 1.0f) {
			flag = true;
			easeNum = 0.0f;
			boxScale = 100.0f;
			//	あくまでもここはシーン切り替え処理のみ
			switch (scene)
			{
			case GameScene::Scene::TITLE:
				scene = GameScene::Scene::BATTLE;
				break;
			case GameScene::Scene::BATTLE:
				scene = GameScene::Scene::RESULT;
				break;
			case GameScene::Scene::RESULT:
				scene = GameScene::Scene::TITLE;
				break;
			}
		}
	}
	else if (flag) {
		boxScale = Ease::UseEase(100.0f, 0.0f, easeNum, 1.0f, Ease::EaseType::EaseOutCirc);
		if (easeNum == 1.0f) {
			flag = false;
			sceneChangeFlag = false;
			easeNum = 0.0f;
			boxScale = 0.0f;
			boxtransform.rotation_.z = 0.0f;
		}
	}

	boxtransform.scale_ = Vector3(boxScale, boxScale, 1.0f);

	boxtransform.UpdateMatrix();
}
