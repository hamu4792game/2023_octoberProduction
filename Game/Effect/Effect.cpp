#include "Effect.h"
#include "externals/imgui/imgui.h"

Effect::Effect() {
	particleResources_ = std::make_unique<Particle>();
	ModelLoad();
	//particleTrans_->rotation_.x = AngleToRadian(90.0f);
}

void Effect::ModelLoad() {
	particleResources_->Texture("Resources/plane/plane.obj", "./Resources/Shader/Particle.VS.hlsl", "./Resources/Shader/Particle.PS.hlsl", "Resources/hud/bug.png", _countof(particleTrans_));
}

void Effect::Update(const Matrix4x4& cameraMat) {
	ImGui::Begin("te");
	ImGui::DragFloat3("rotate", &particleTrans_[0].rotation_.x, AngleToRadian(1.0f));
	ImGui::End();

	for (auto& i : particleTrans_) {
		i.translation_ = Vector3(RandNum(-16.0f, 16.0f), RandNum(-9.0f, 9.0f), 0.0f);
		i.UpdateMatrix();
		i.worldMatrix *= cameraMat;
	}
}

void Effect::Draw(const Matrix4x4& viewProjection) {

	Particle::ParticleDraw(particleTrans_, viewProjection, 0xffffffff, particleResources_.get());
}
