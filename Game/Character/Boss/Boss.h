#pragma once
#include "Game/Character/BaseCharacter/BaseCharacter.h"
#include "Game/Character/Boss/BossBullet/BossBullet.h"
#include <vector>

class Boss : public BaseCharacter
{
public:
	Boss() = default;
	~Boss() = default;

	void Initialize() override;

	void Update() override;

	void SetParent(const WorldTransform* parent) { parent_ = parent, transform_.parent_ = parent; }

	void SetFlag(const bool& flag) { attackFlag = flag; }

	void SetBulletModel(Model* model) { bulletModel_ = model; }

private:
	const WorldTransform* parent_;

	//	攻撃タイミングのフラグ
	bool attackFlag_ = false;
	//	アニメーション開始用フラグ
	bool animeFlag_ = false;
	//	playerが攻撃するフラグ
	bool bulletDieFlag_ = false;

	Model* bulletModel_;

	std::vector<BossBullet> bullets_;

	void Attack();

};
