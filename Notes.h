#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include <memory>
#include "math/Matrix4x4.h"
#include "Player.h"
#include "math/Vector3.h"

class Notes
{
public:

	Notes();
	~Notes() = default;

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void LoadModel();

	void SetPosition(const Vector3& position) { 
		worldTransform_.translation_ = position;
		worldTransform_.UpdateMatrix();
	}

	void SetPlayer(Player* player) { player_ = player; }

	Vector3 GetPosition() { return worldTransform_.translation_; }

	bool GetIsHit() { return isHit_; }

private:

	std::unique_ptr<Model> model_;

	WorldTransform worldTransform_;

	Player* player_;

	//ノーツを叩いたかどうか
	bool isHit_ = false;

	/*bool isTouch_ = false;*/

	//ノーツをスルー、または反応がずれすぎた場合ミス
	bool isMiss_ = false;

};

