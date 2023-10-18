#pragma once
#include "math/Matrix4x4.h"
#include "Engine/Texture/Model.h"

class Stage
{
public:
	Stage(std::vector<std::shared_ptr<Model>> model);
	~Stage() = default;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position">初期位置</param>
	void Initialize(const Vector3& position);

	void Update();

	void Draw3D(const Matrix4x4& viewProjectionMat);

	const WorldTransform& GetTransform() { return worldTrans_; }

	void SetPosition(const Vector3& position);

private: // モデルデータ
	std::vector<std::shared_ptr<Model>> models_;

private: // 座標データ
	std::vector<WorldTransform> transforms_;
	//	中心座標
	WorldTransform worldTrans_;

};
