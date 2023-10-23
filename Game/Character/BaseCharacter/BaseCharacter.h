#pragma once
#include "math/Matrix4x4.h"
#include <vector>
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"

class BaseCharacter
{
public:
	BaseCharacter() = default;
	~BaseCharacter() = default;

	virtual void Initialize();

	virtual void Update();

	virtual void Draw3D(const Matrix4x4& viewProjectionMat);

	void SetModel(std::vector<std::shared_ptr<Model>> model) { parts_ = model; };

	const WorldTransform& GetTransform() { return transform_; }

	void TransformUpdate();

protected:
	
	//	中心座標
	WorldTransform transform_;
	std::vector<WorldTransform> partsTransform_;
	//	モデルのパーツ
	std::vector<std::shared_ptr<Model>> parts_;

};

