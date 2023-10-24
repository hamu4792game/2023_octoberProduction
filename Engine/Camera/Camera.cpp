#include "Engine/Camera/Camera.h"
#include "Engine/Engine.h"
//#include "externals/imgui/imgui.h"
#include "Engine/Input/KeyInput/KeyInput.h"
#include "Engine/Easing/Easing.h"
#include <numbers>

Camera::Camera(float farClip_, bool proType) {
	farZ = farClip_;
	type = proType;
	if (type) {
		projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(Engine::GetInstance()->WindowWidth) / float(Engine::GetInstance()->WindowHeight), 0.1f, farZ);
	}
	else {
		projectionMatrix = MakeOrthographicMatrix(-float(Engine::GetInstance()->WindowWidth / 2), float(Engine::GetInstance()->WindowHeight / 2),
			float(Engine::GetInstance()->WindowWidth / 2), -float(Engine::GetInstance()->WindowHeight / 2), 0.01f, farZ);
	}
	

	if (false)
	{
		transform.translation_ = { 0.0f,0.0f,-5.0f };
		transform.rotation_ = { 0.0f,0.0f,0.0f };
		transform.scale_ = { 1.0f,1.0f,1.0f };

		transform.translation_.x = 0.0f;
		transform.translation_.y = 250.0f;
		transform.translation_.z = 0.0f;
		transform.rotation_.x = AngleToRadian(90.0f);
		transform.rotation_.y = AngleToRadian(0.0f);
		transform.rotation_.z = AngleToRadian(0.0f);
	}
	//cameraMatrix = MakeAffineMatrix(transform.scale_, transform.rotation_, transform.translation_);

}

void Camera::CreateBillboard() {
	//	180度回転させる
	Matrix4x4 backToFrontMatrix = MakeRotateYMatrix(std::numbers::pi_v<float>);
	//	行列の生成
	billboardMatrix = backToFrontMatrix * cameraMatrix;
	billboardMatrix.m[3][0] = 0.0f;
	billboardMatrix.m[3][1] = 0.0f;
	billboardMatrix.m[3][2] = 0.0f;
}

Matrix4x4 Camera::GetViewProMat() {
	//	行列の計算
	cameraMatrix = transform.UpdateMatrix();
	viewMatrix = Inverse(cameraMatrix);
	viewProjectionMatrix = viewMatrix * projectionMatrix;
	//	ビルボードも作る
	CreateBillboard();

	return viewProjectionMatrix;
}

bool Camera::CameraWork(const Vector3& startTrans, const Vector3& endTrans, const Vector3& startRotate, const Vector3& endRotate, const float& t) {
	if (t >= 1.0f) {
		transform.translation_ = endTrans;
		transform.rotation_ = endRotate;
		return false;
	}

	float T = Easing::EaseInSine(t);
	transform.translation_.x = (1 - T) * startTrans.x + T * endTrans.x;
	transform.translation_.y = (1 - T) * startTrans.y + T * endTrans.y;
	transform.translation_.z = (1 - T) * startTrans.z + T * endTrans.z;
	transform.rotation_.x = (1 - T) * startRotate.x + T * endRotate.x;
	transform.rotation_.y = (1 - T) * startRotate.y + T * endRotate.y;
	transform.rotation_.z = (1 - T) * startRotate.z + T * endRotate.z;
	return true;
}

void Camera::TargetRotate(const WorldTransform& target) {
	if (KeyInput::GetKey(DIK_RIGHTARROW)) {
		transform.rotation_.y += AngleToRadian(1.0f);
	}
	if (KeyInput::GetKey(DIK_LEFTARROW)) {
		transform.rotation_.y -= AngleToRadian(1.0f);
	}
	if (KeyInput::GetInstance()->GetPadConnect()) {
		transform.rotation_.x = KeyInput::GetInstance()->GetPadRStick().x;
		transform.rotation_.y = KeyInput::GetInstance()->GetPadRStick().y;
	}


	Vector3 offset(0.0f, 2.0f, -20.0f);
	Matrix4x4 rotMat = MakeRotateMatrix(transform.rotation_);
	offset = TransformNormal(offset, rotMat);

	transform.translation_ = target.translation_ + offset;
	transform.UpdateMatrix();
}
