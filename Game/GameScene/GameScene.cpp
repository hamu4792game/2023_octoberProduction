#include "GameScene.h"
#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Easing/Ease.h"
#include <algorithm>
#include "Game/PartsEnum.h"
#include "Engine/Base/MultipathRendering/MultipathRendering.h"

GameScene* GameScene::GetInstance() {
	static GameScene instance;
	return &instance;
}

void GameScene::Initialize() {
	//	カメラの読み込みと生成
	camera = std::make_shared<Camera>(2000.0f, true);
	camera->transform.translation_.y = 70.0f;
	camera->transform.translation_.z = -40.0f;
	camera->transform.rotation_ = { 1.0f,0.0f,0.0f };
	camera->transform.scale_.z = 0.5f;
	camera2d = std::make_shared<Camera>();
	//	カメラ行列の更新
	viewProjectionMatrix = camera->GetViewProMat();
	viewProjectionMatrix2d = camera2d->GetViewProMat();

	//	モデルの生成
	model_ = std::make_shared<Model>();
	notesModelNormal_ = std::make_unique<Model>();
	notesModelLong_ = std::make_unique<Model>();
	notesModelDamage_ = std::make_unique<Model>();
	notesModelHitLine_ = std::make_unique<Model>();
	notesModelEffect_ = std::make_unique<Model>();
	hud_ = std::make_shared<Texture2D>();
	hitLine_ = std::make_unique<Texture2D>();
	currentStage_ = std::make_unique<Texture2D>();
	totalStage_ = std::make_unique<Texture2D>();
	gauge_ = std::make_unique<Texture2D>();
	gaugeFrame_ = std::make_unique<Texture2D>();
	gaugeMax_ = std::make_unique<Texture2D>();
	gaugeMinus_ = std::make_unique<Texture2D>();
	gaugeOver_ = std::make_unique<Texture2D>();

	box = std::make_shared<Texture2D>();

	boxModel_ = std::make_shared<Model>();

	//	主人公のモデルの生成
	heroModel_.resize(static_cast<uint8_t>(HeroParts::kMaxCount));
	for (uint8_t i = 0; i < static_cast<uint8_t>(HeroParts::kMaxCount); i++) {
		heroModel_[i] = std::make_shared<Model>();
	}
	//	ボスのモデルの生成
	bossModel_.resize(static_cast<uint8_t>(BossParts::kMaxCount));
	for (uint8_t i = 0; i < static_cast<uint8_t>(BossParts::kMaxCount); i++) {
		bossModel_[i] = std::make_shared<Model>();
	}

	//	ステージモデルの生成
	for (uint8_t i = 0; i < 6; i++) {
		stageModel_.push_back(std::make_shared<Model>());
	}

	//	タイトルテクスチャの生成
	titleResources_ = std::make_shared<Texture2D>();
	
	//Bボタン関連の生成
	BButtomResources_ = std::make_shared<Texture2D>();
	pushBButtomResources_ = std::make_shared<Texture2D>();

	//	モデルのロード
	ModelLoad();

	//	音源の生成とセット
	bgm_ = std::make_unique<AudioInput>();
	////	ロード
	//bgm_->SoundLoadWave("./Resources/loopBGM/drumloop1.wav");
	////	音の再生
	//bgm_->SoundPlayWave();
	////	音量の設定
	//bgm_->SetVolume(0.2f);
	
	//	シーンの生成
	title = std::make_unique<Title>(camera.get());
	battle = std::make_unique<Battle>(camera);
	result = std::make_unique<Result>(camera.get());

	std::vector<Model*> noteModels{ notesModelNormal_.get(), notesModelLong_.get(), notesModelDamage_.get(), notesModelHitLine_.get() };
	std::vector<Texture2D*> noteTextures{ hitLine_.get() };
	title->SetModels(heroModel_);

	battle->ModelLoad(noteModels, noteTextures);
	battle->SetHeroModels(heroModel_);
	battle->SetBossModels(bossModel_);
	battle->SetStageModels(stageModel_);
	battle->SetBossBulletModel(noteModels.at(0));
	battle->SetNotesEffectModel(notesModelEffect_.get());
	battle->SetBoxTexture(box.get());
	battle->SetTitleTexture(titleResources_.get());
	std::vector<Texture2D*> textures = { currentStage_.get(), totalStage_.get(), gauge_.get(), gaugeMax_.get(),
		gaugeMinus_.get(),gaugeOver_.get(), gaugeFrame_.get() };
	battle->SetUI(textures);
	battle->SetBButtomTexture(BButtomResources_.get());
	battle->SetPushBButtomTexture(pushBButtomResources_.get());

	//	シーンの初期化
	title->Initialize();
	battle->Initialize();
	result->Initialize();

	//	変数の初期化
	scene = Scene::BATTLE;
	oldscene = Scene::RESULT;

}

void GameScene::Update() {
#ifdef _DEBUG
	ImGui::Begin("camera");
	ImGui::DragFloat3("translate", &camera->transform.translation_.x, 1.0f);
	ImGui::DragFloat3("rotate", &camera->transform.rotation_.x, AngleToRadian(1.0f));
	Vector3 handle = Vector3(RadianToAngle(camera->transform.rotation_.x), RadianToAngle(camera->transform.rotation_.y), RadianToAngle(camera->transform.rotation_.z));
	ImGui::Text("%f : %f : %f", handle.x, handle.y, handle.z);
	ImGui::DragFloat3("scale", &camera->transform.scale_.x, 0.1f);
	ImGui::End();

	ImGui::Begin("aaaaa");
	ImGui::DragFloat2("pos", &MultipathRendering::GetInstance()->cEffectParameters->centerPosition.x);
	ImGui::DragFloat("rate", &MultipathRendering::GetInstance()->cEffectParameters->parameterRate);
	ImGui::SliderInt("type", &MultipathRendering::GetInstance()->cEffectParameters->type, 0, 10);

	ImGui::End();

#endif // _DEBUG

	//	シーン切替わり時の初期化
	if (oldscene != scene) {
		switch (scene) {
		case GameScene::Scene::TITLE:
			camera->SetParent(nullptr);
			title->Initialize();
			break;
		case GameScene::Scene::BATTLE:
			camera->SetParent(nullptr);
			battle->Initialize();
			break;
		case GameScene::Scene::RESULT:
			camera->SetParent(nullptr);
			result->Initialize();
			break;
		}
	}
	oldscene = scene;

	switch (scene) {
	case GameScene::Scene::TITLE:
		title->Update();
		break;
	case GameScene::Scene::BATTLE:
		battle->Update();
		break;
	case GameScene::Scene::RESULT:
		result->Update();
		break;
	}

	if (KeyInput::PushKey(DIK_S) && KeyInput::GetKey(DIK_LSHIFT)) {
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

void GameScene::Draw() {

	//	3D描画
	switch (scene) {
	case GameScene::Scene::TITLE:
		title->Draw3D(viewProjectionMatrix);
		break;
	case GameScene::Scene::BATTLE:
		battle->Draw3D(viewProjectionMatrix);
		break;
	case GameScene::Scene::RESULT:
		result->Draw3D(viewProjectionMatrix);
		break;
	}

	//	2D描画
	switch (scene) {
	case GameScene::Scene::TITLE:
		title->Draw2D(viewProjectionMatrix2d);
		break;
	case GameScene::Scene::BATTLE:
		battle->Draw2D(viewProjectionMatrix2d);
		break;
	case GameScene::Scene::RESULT:
		result->Draw2D(viewProjectionMatrix2d);
		break;
	}

	if (sceneChangeFlag) {
		Texture2D::TextureDraw(boxtransform, viewProjectionMatrix2d, 0x000000ff, box.get());
	}
	
}

void GameScene::Finalize() {
	Line::Finalize();
	Model::Finalize();
	Texture2D::Finalize();
}


void GameScene::ModelLoad() {
	model_->Texture("Resources/eatRamen/eatRamen.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	hud_->Texture("Resources/uvChecker.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	hitLine_->Texture("Resources/hitline/hitline.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	box->Texture("Resources/block.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	notesModelNormal_->Texture("Resources/notes/notes.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/notes/normal.png");
	notesModelLong_->Texture("Resources/notes/notes.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/notes/long.png");
	notesModelDamage_->Texture("Resources/plane/plane.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/notes/damage.png");
	notesModelHitLine_->Texture("Resources/hitline/hitline.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/hitline/hitline.png");
	notesModelEffect_->Texture("Resources/effectnotes/effectnotes.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/effectnotes/effectnotes.png");

	boxModel_->Texture("Resources/box/box.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/uvChecker.png");

	heroModel_[static_cast<uint8_t>(HeroParts::Body)]->Texture("Resources/player/body.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/player/body.png");
	heroModel_[static_cast<uint8_t>(HeroParts::Head)]->Texture("Resources/player/head.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	heroModel_[static_cast<uint8_t>(HeroParts::Waist)]->Texture("Resources/player/waist.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/player/leg.png");
	heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)]->Texture("Resources/player/legs.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	heroModel_[static_cast<uint8_t>(HeroParts::LeftUpperArm)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	heroModel_[static_cast<uint8_t>(HeroParts::RightBottomArm)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	heroModel_[static_cast<uint8_t>(HeroParts::LeftBottomArm)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	heroModel_[static_cast<uint8_t>(HeroParts::RightUpperLeg)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	heroModel_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	heroModel_[static_cast<uint8_t>(HeroParts::RightBottomLeg)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	heroModel_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];

	bossModel_[static_cast<uint8_t>(BossParts::Body)] = heroModel_[static_cast<uint8_t>(HeroParts::Body)];
	bossModel_[static_cast<uint8_t>(BossParts::Head)] = heroModel_[static_cast<uint8_t>(HeroParts::Head)];
	bossModel_[static_cast<uint8_t>(HeroParts::Waist)] = heroModel_[static_cast<uint8_t>(HeroParts::Waist)];
	bossModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	bossModel_[static_cast<uint8_t>(HeroParts::LeftUpperArm)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	bossModel_[static_cast<uint8_t>(HeroParts::RightBottomArm)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	bossModel_[static_cast<uint8_t>(HeroParts::LeftBottomArm)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	bossModel_[static_cast<uint8_t>(HeroParts::RightUpperLeg)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	bossModel_[static_cast<uint8_t>(HeroParts::LeftUpperLeg)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	bossModel_[static_cast<uint8_t>(HeroParts::RightBottomLeg)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];
	bossModel_[static_cast<uint8_t>(HeroParts::LeftBottomLeg)] = heroModel_[static_cast<uint8_t>(HeroParts::RightUpperArm)];

	//	ステージモデル
	stageModel_[0]->Texture("Resources/floor/floor.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/stage/Road.png");
	stageModel_[1]->Texture("Resources/box/box.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/stage/building1.png");
	stageModel_[2] = stageModel_[1];
	stageModel_[3] = stageModel_[1];
	stageModel_[4] = stageModel_[1];
	stageModel_[5] = stageModel_[1];

	//	タイトルテキスト
	titleResources_->Texture("Resources/hud/bugRhythm.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	//インゲームのUI
	currentStage_->Texture("Resources/UI/currentstage.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	totalStage_->Texture("Resources/UI/totalstage.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	gauge_->Texture("Resources/UI/gauge.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	gaugeFrame_->Texture("Resources/UI/gaugeframe.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	gaugeMax_->Texture("Resources/UI/gaugemax.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	gaugeMinus_->Texture("Resources/UI/gaugeminus.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	gaugeOver_->Texture("Resources/UI/gaugeover.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

	//ボタン
	BButtomResources_->Texture("Resources/hud/BButtom.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	pushBButtomResources_->Texture("Resources/hud/PushBButtom.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
}


void GameScene::SceneChange() {
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
				scene = GameScene::Scene::BATTLE;
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

void GameScene::CameraUpdate() {



}
