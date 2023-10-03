#include "Notes.h"

Notes::Notes() {
	model_ = std::make_unique<Model>();
}

void Notes::Initialize() {

	

}

void Notes::Update() {

	worldTransform_.UpdateMatrix();

}

void Notes::Draw(const Matrix4x4& viewProjection) {

	model_->ModelDraw(worldTransform_, viewProjection, 0xffffffff, model_.get());

}

void Notes::LoadModel() {

	model_->Texture("Resources/notes/notes.obj", "./Shader/Texture2D.VS.hlsl", "./Shader/Texture2D.PS.hlsl", "notes/notes.png");

}