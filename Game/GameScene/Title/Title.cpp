#include "Title.h"
#include "externals/imgui/imgui.h"

Title::Title(Camera* camera) {
	camera_ = camera;

	dust_ = std::make_unique<Particle>();

	particleResources_ = std::make_unique<Texture2D>();
	titleResources_ = std::make_unique<Texture2D>();
	cloudResources_ = std::make_unique<Model>();
	
	ModelLoad();
	eff = std::make_unique<Effect>();
}

void Title::Initialize() {
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
	//titleTrans_.scale_ = Vector3(2.0f, 2.0f, 1.0f);
	modelTrans_.resize(model_.size());
}

void Title::Update() {

	BackParticle();

	//	座標の更新
	for (auto& i : modelTrans_) {
		i.UpdateMatrix();
	}
	for (auto& i : dustTrans_) {
		i.UpdateMatrix();
	}
	cloudTrans_.UpdateMatrix();
	//titleTrans_.UpdateMatrix();

}

void Title::Draw3D(const Matrix4x4& viewProjection) {
	eff->Update(camera_->billboardMatrix);
	//for (uint8_t i = 0; i < model_.size(); i++) {
	//	Model::ModelDraw(modelTrans_[i], viewProjection, 0xffffffff, model_[i].get());
	//}
	//Particle::ParticleDraw(dustTrans_, viewProjection, 0xffffffff, dust_.get());
	//Model::ModelDraw(cloudTrans_, viewProjection, 0xffffffff, cloudResources_.get());
	eff->Draw(viewProjection);
}

void Title::Draw2D(const Matrix4x4& viewProjection) {
	//Texture2D::TextureDraw(titleTrans_, viewProjection, 0xffffffff, titleResources_.get());
	/*for (auto& i : dustTrans_) {
		Texture2D::TextureDraw(i, viewProjection, 0x0000ccaa, particleResources_.get());
	}*/
}

void Title::ModelLoad() {
	dust_->Texture("Resources/eatRamen/eatRamen.obj", "./Resources/Shader/Particle.VS.hlsl", "./Resources/Shader/Particle.PS.hlsl", "Resources/eatRamen/ramen.png", _countof(dustTrans_));
	particleResources_->Texture("Resources/hud/particle.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	titleResources_->Texture("Resources/hud/bugRhythm.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	cloudResources_->Texture("Resources/plane/plane.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/hud/cloud.png");
}

void Title::BackParticle() {
	for (uint8_t i = 0; i < _countof(dustTrans_); i++) {
		dustTrans_[i].translation_.x += 0.5f;
		dustTrans_[i].translation_.y -= 0.5f;

		if (dustTrans_[i].translation_.y < -360.0f)	{
			dustTrans_[i].translation_.y = 360.0f;
		}
		if (dustTrans_[i].translation_.x > 640.0f)	{
			dustTrans_[i].translation_.x = -640.0f;
		}
	}
}
