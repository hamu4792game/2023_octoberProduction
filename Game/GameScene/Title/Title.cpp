#include "Title.h"

Title::Title() {

}

void Title::Initialize() {

}

void Title::Update() {

	//	座標の更新
	modelTrans.UpdateMatrix();
}

void Title::Draw3D(const Matrix4x4& viewProjection) {
	Model::ModelDraw(modelTrans, viewProjection, 0xffffffff, model_.get());
}

void Title::Draw2D(const Matrix4x4& viewProjection) {
	Texture2D::TextureDraw(hudTrans, viewProjection, 0xffffffff, hud_.get());
}
