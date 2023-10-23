#pragma once
#include "Game/Character/BaseCharacter/BaseCharacter.h"
#include "Game/Character/Boss/BossBullet/BossBullet.h"
#include <vector>
#include "Engine/Texture/Model.h"
#include "Game/Notes/Notes.h"

class Boss : public BaseCharacter
{
public:
	Boss();
	~Boss() = default;

	void Initialize() override;

	void Update() override;

	void Draw3D(const Matrix4x4& viewProjectionMat) override;

	void SetParent(const WorldTransform* parent) { parent_ = parent, transform_.parent_ = parent; }

	void SetFlag(const bool& flag) { attackFlag_ = flag; }

	void SetBulletModel(Model* model) { bulletModel_ = model; }

	void Attack(Notes* notes);

private:
	const WorldTransform* parent_;

	//	攻撃タイミングのフラグ
	bool attackFlag_ = false;
	//	アニメーション開始用フラグ
	bool animeFlag_ = false;
	//	playerが攻撃するフラグ
	bool bulletDieFlag_ = false;

	Model* bulletModel_;

	std::list<std::shared_ptr<BossBullet>> bullets_;

private:
	std::unique_ptr<Model> back_;
	WorldTransform backTrans_;

};
