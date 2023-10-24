#pragma once
#include "math/Matrix4x4.h"
#include "Engine/WorldTransform/WorldTransform.h"

class Camera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="farClip_">深度値</param>
	/// <param name="proType">false = 正射影 true = 透視投影</param>
	Camera(float farClip_ = 1000.0f, bool proType = false);
	~Camera() = default;

	//ワールドトランスフォームの親ポインタのセット
	void SetParent(WorldTransform* parent) { transform.parent_ = parent; }

private:
	//	深度値
	float farZ;
	//	正射影か透視投影か false = 正射影 true = 透視投影
	bool type;
	//	行列の生成
	Matrix4x4 cameraMatrix{};
	Matrix4x4 viewMatrix{};
	Matrix4x4 projectionMatrix{};
	Matrix4x4 viewProjectionMatrix{};

	void CreateBillboard();

public:
	WorldTransform transform;
	//	viewProjectionMatrixの取得
	Matrix4x4 GetViewProMat();

	Vector3 position;	//カメラの座標
	Matrix4x4 billboardMatrix;
	Vector3 offset;

public:
	
	/// <summary>
	/// 
	/// </summary>
	/// <param name="startTrans"></param>
	/// <param name="endTrans"></param>
	/// <param name="startRotate"></param>
	/// <param name="endRotate"></param>
	/// <param name="t"></param>
	/// <returns>true = 動いて入る false = 動いていない</returns>
	bool CameraWork(const Vector3& startTrans, const Vector3& endTrans,const Vector3& startRotate, const Vector3& endRotate, const float& t);

	void TargetRotate(const WorldTransform& target);

};

