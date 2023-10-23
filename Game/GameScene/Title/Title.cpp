#include "Title.h"
#include "externals/imgui/imgui.h"

Title::Title(Camera* camera) {
	camera_ = camera;

	dust_ = std::make_unique<Particle>();

	particleResources_ = std::make_unique<Texture2D>();
	cloudResources_ = std::make_unique<Model>();
	
	ModelLoad();
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
	}
}

void Title::Update() {

	//BackParticle();

	//	座標の更新
	modelTrans.UpdateMatrix();
	for (auto& i : dustTrans_) {
		i.UpdateMatrix();
	}
	cloudTrans_.UpdateMatrix();
	cloudTrans_.scale_ = Vector3(8.0f, 8.0f, 1.0f);
}

void Title::Draw3D(const Matrix4x4& viewProjection) {
	Model::ModelDraw(modelTrans, viewProjection, 0xffffffff, model_.get());
	//Particle::ParticleDraw(dustTrans_, viewProjection, 0xffffffff, dust_.get());
	Model::ModelDraw(cloudTrans_, viewProjection, 0xffffffff, cloudResources_.get());
}

void Title::Draw2D(const Matrix4x4& viewProjection) {
	//Texture2D::TextureDraw(hudTrans, viewProjection, 0xffffffff, hud_.get());
	//Texture2D::TextureDraw(hudTrans, viewProjection, 0xffffffff, particleResources_.get());
	for (auto& i : dustTrans_) {
		Texture2D::TextureDraw(i, viewProjection, 0xffffffff, particleResources_.get());
	}
}

void Title::ModelLoad() {
	dust_->Texture("Resources/eatRamen/eatRamen.obj", "./Resources/Shader/Particle.VS.hlsl", "./Resources/Shader/Particle.PS.hlsl", _countof(dustTrans_));
	particleResources_->Texture("Resources/hud/particle.png", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl");
	cloudResources_->Texture("Resources/plane/plane.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/hud/cloud.png");
}

void Title::BackParticle() {
	for (uint8_t i = 0; i < _countof(dustTrans_); i++) {
		dustTrans_[i].translation_.x += 0.1f;
		dustTrans_[i].translation_.y -= 0.1f;

		if (dustTrans_[i].translation_.y < -10.0f)	{
			dustTrans_[i].translation_.x = -10.0f;
			dustTrans_[i].translation_.y = 10.0f;

		}
	}
}
