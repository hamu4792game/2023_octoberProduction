#include "Stage.h"

Stage::Stage() {
	for (uint8_t i = 0; i < 5; i++)	{
		buildings.push_back(std::make_shared<Model>());
	}
	transforms_.resize(buildings.size());
}

void Stage::Update() {

}

void Stage::Draw3D(const Matrix4x4& viewProjectionMat) {
	for (uint8_t i = 0; i < transforms_.size(); i++) {
		Model::ModelDraw(transforms_[i], viewProjectionMat, 0xffffffff, buildings[i].get());
	}
}

void Stage::ModelLoad() {
	buildings[0]->Texture("Resources/box/box.obj", "./Resources/Shader/Texture2D.VS.hlsl", "./Resources/Shader/Texture2D.PS.hlsl", "Resources/uvChecker.png");
	for (uint8_t i = 0; i < buildings.size(); i++) {
		buildings[i] = buildings[0];
	}
}
