#pragma once
#include "Engine/WorldTransform/WorldTransform.h"

class Battle {
public:
	Battle();
	~Battle() = default;

	//	初期化
	void Initialize();
	//	更新処理
	void Update();
	//	描画処理
	void Draw3D(const Matrix4x4& viewProjection);
	void Draw2D(const Matrix4x4& viewProjection);

private:



};
