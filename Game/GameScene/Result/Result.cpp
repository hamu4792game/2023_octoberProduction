#include "Result.h"
#include "externals/imgui/imgui.h"
#include"Engine/Easing/Ease.h"
#include"Engine/Easing/Easing.h"
#include"Engine/Input/KeyInput/KeyInput.h"
#include"Game/GameScene/GameScene.h"

Result::Result(Camera* camera){
	camera_ = camera;

	dust_ = std::make_unique<Particle>();

	particleResources_ = std::make_unique<Texture2D>();
	titleResources_ = std::make_unique<Texture2D>();
	pressResources_= std::make_unique<Texture2D>();
	//BResources_= std::make_unique<Texture2D>();

	cloudResources_ = std::make_unique<Model>();
	loopBGM_[0] = std::make_unique<AudioInput>();
	loopBGM_[0]->SoundLoadWave("./Resources/loopBGM/tempo_06.wav");
	loopBGM_[1] = std::make_unique<AudioInput>();
	loopBGM_[1]->SoundLoadWave("./Resources/loopBGM/tempo_09.wav");

	ModelLoad();
	TextureLoad();
}

void Result::Initialize(){
	camera_->transform.Reset();
	camera_->transform.translation_ = Vector3(0.0f, 2.0f, -10.0f);
	camera_->transform.rotation_ = Vector3(AngleToRadian(10.0f), 0.0f, 0.0f);

	for (uint8_t i = 0; i < _countof(dustTrans_); i++) {
		//dustTrans_[i].translation_.x = -50.0f + (2.0f * i);
		//dustTrans_[i].translation_.y = 2.0f * i;
		dustTrans_[i].translation_.x = RandNum(-640.0f, 640.0f);
		dustTrans_[i].translation_.y = RandNum(-360.0f, 360.0f);
		//dustTrans_[i].scale_ = Vector3(0.5f, 0.5f, 1.0f);
	}
	particleResources_->SetBlend(BlendMode::Screen);
	cloudTrans_.scale_ = Vector3(8.0f, 8.0f, 1.0f);
	titleTrans_.scale_ = Vector3(2.0f, 2.0f, 1.0f);
	pressTrans_.scale_ = { 1.2f,1.2f,1.0f };
	pressTrans_.translation_ = { 0.0f,-236.0f,0.0f };
	//BTrans_.scale_ = { 1.0f,1.0f,1.0f };
	//BTrans_.translation_ = { 100.0f,-236.0f,0.0f };

	clearEaseStart_ = { 0.0f,600.0f,0.0f };
	clearEaseEnd_ = { 0.0f,250.0f,0.0f };
	clearEaseSpeed_ = 0.02f;
	isClearEase = true;

	pressEaseStart_ = 125;
	pressEaseEnd_ = 0x000000ff;
	pressEaseSpeed_ = 0.02f;
	pressMagnification = 1.0f;
	isPressEase = true;

	modelTrans_.resize(model_.size());
}

void Result::Update(){
	BackParticle();
	/*GUIの表示*/
	DrawImgui();

	if (GameScene::GetInstance()->sceneChangeFlag == false) {
		loopBGM_[1]->SoundPlayWave(true);
		loopBGM_[1]->SetVolume(0.3f);
	}

	/*イージング関連を記入する*/
	if (isClearEase){
		if (clearEaseNum_ < 1.0f) {
			clearEaseNum_ += clearEaseSpeed_;
		}
		else {
			isClearEase = false;
		}

		clearT_ = Easing::EaseInOutBack(clearEaseNum_);		
	}
	else {
		clearEaseNum_ = 0.0f;
	}
	titleTrans_.translation_ =
		Multiply((1 - clearT_), clearEaseStart_) +
		Multiply(clearT_, clearEaseEnd_);

	if (isPressEase) {
		pressEaseNum_ += pressEaseSpeed_ * pressMagnification;	
		if (pressEaseNum_ > 0.98f || pressEaseNum_ < 0.02f) {
			pressMagnification *= -1;
		}	

		pressT_ = Easing::EaseInSine(pressEaseNum_);
	}
	pressColor = static_cast<int>((1 - pressT_) * pressEaseStart_ + pressT_ * pressEaseEnd_);
	if (pressColor < pressEaseStart_) {
		pressColor = pressEaseStart_;

	}
	
	//	座標の更新
	for (auto& i : modelTrans_) {
		i.UpdateMatrix();
	}
	for (auto& i : dustTrans_) {
		i.UpdateMatrix();
	}
	cloudTrans_.UpdateMatrix();
	titleTrans_.UpdateMatrix();
	pressTrans_.UpdateMatrix();
	//BTrans_.UpdateMatrix();

	if ((KeyInput::PushKey(DIK_SPACE) || KeyInput::GetInstance()->GetPadButtonDown(XINPUT_GAMEPAD_B)) && GameScene::GetInstance()->sceneChangeFlag == false) {
		GameScene::GetInstance()->sceneChangeFlag = true;
		loopBGM_[1]->SoundStop();
		selectSE_->SoundStop();
		selectSE_->SoundPlayWave();
		selectSE_->SetVolume(0.3f);
	}
}

void Result::Draw3D(const Matrix4x4& viewProjection){
	for (uint8_t i = 0; i < model_.size(); i++) {
		Model::ModelDraw(modelTrans_[i], viewProjection, 0xffffffff, model_[i].get());
	}
	//Particle::ParticleDraw(dustTrans_, viewProjection, 0xffffffff, dust_.get());
	Model::ModelDraw(cloudTrans_, viewProjection, 0xffffffff, cloudResources_.get());
}

void Result::Draw2D(const Matrix4x4& viewProjection){
	Texture2D::TextureDraw(titleTrans_, viewProjection, 0xffffffff, titleResources_.get());
	Texture2D::TextureDraw(pressTrans_, viewProjection, pressColor, pressResources_.get());
	//Texture2D::TextureDraw(BTrans_, viewProjection, 0x00ff, BResources_.get());
	for (auto& i : dustTrans_) {
		Texture2D::TextureDraw(i, viewProjection, 0x0000ccaa, particleResources_.get());
	}
}

void Result::ModelLoad(){
	dust_->Texture("Resources/eatRamen/eatRamen.obj", "./Resources/Shader/Particle.VS.hlsl", "./Resources/Shader/Particle.PS.hlsl","Resources/eatRamen/ramen.png", _countof(dustTrans_));
	cloudResources_->Texture("Resources/plane/plane.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/hud/cloud.png");
}

void Result::TextureLoad(){
	particleResources_->Texture("Resources/hud/particle.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	titleResources_->Texture("Resources/hud/GameClear.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	pressResources_->Texture("Resources/hud/Press.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	//BResources_->Texture("Resources/hud/BButtom.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");

}

void Result::BackParticle(){
	for (uint8_t i = 0; i < _countof(dustTrans_); i++) {
		dustTrans_[i].translation_.x += 0.5f;
		dustTrans_[i].translation_.y -= 0.5f;

		if (dustTrans_[i].translation_.y < -360.0f) {
			dustTrans_[i].translation_.y = 360.0f;
		}
		if (dustTrans_[i].translation_.x > 640.0f) {
			dustTrans_[i].translation_.x = -640.0f;
		}
	}
}

void Result::DrawImgui(){
	ImGui::Begin("位置座標");
	ImGui::DragFloat3("ゲームクリア", &titleTrans_.translation_.x, 1.00f);
	ImGui::DragFloat3("Press", &pressTrans_.translation_.x, 1.0f);
	//ImGui::DragFloat3("B", &BTrans_.translation_.x, 1.0f);
	ImGui::End();

	ImGui::Begin("イージング関連");
	if (ImGui::TreeNode("ゲームクリア")) {
		ImGui::DragFloat3("イージングの始点", &clearEaseStart_.x, 1.0f);
		ImGui::DragFloat3("イージングの終点", &clearEaseEnd_.x, 1.0f);
		ImGui::DragFloat("イージングスピード", &clearEaseSpeed_, 0.01f, 0.01f, 1.0f);
		ImGui::Checkbox("イージング開始", &isClearEase);
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Press")) {
		ImGui::DragInt("イージングの始点", &pressEaseStart_, 1.0f);
		ImGui::DragInt("イージングの終点", &pressEaseEnd_, 1.0f);
		ImGui::DragFloat("イージングスピード", &pressEaseSpeed_, 0.01f, 0.01f, 1.0f);
		ImGui::Checkbox("イージング開始", &isPressEase);
		ImGui::DragInt("テキストの色", &pressColor);
		ImGui::TreePop();
	}
	ImGui::End();
}
