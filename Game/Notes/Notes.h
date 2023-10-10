#pragma once
#include "Engine/WorldTransform/WorldTransform.h"
#include "Engine/Texture/Model.h"
#include <memory>
#include "math/Matrix4x4.h"
#include "Game/Player/Player.h"
#include "math/Vector3.h"

class Notes
{
public:

	Notes();
	~Notes() = default;

	enum NoteType {
		Normal, //通常ノーツ
		Long, //長押し
		Damage //敵の妨害ノーツ

	};

	virtual void Initialize();

	virtual void Update();

	virtual void Draw(const Matrix4x4& viewProjection);

	virtual void ModelLoad(Model* model);

	void SetPosition(const Vector3& position) { 
		worldTransform_.translation_ = position;
		worldTransform_.UpdateMatrix();
	}

	void SetPlayer(Player* player) { player_ = player; }

	void SetNoteType(NoteType type) { type_ = type; }

	Vector3 GetPosition() { return worldTransform_.translation_; }

	bool GetIsHit() { return isHit_; }

	bool GetIsMiss() { return isMiss_; }

	void SetSize(float size) { worldTransform_.scale_ = { size, size, size }; }

protected:

	NoteType type_ = Normal;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	Player* player_;

	//元のサイズ
	const float kNormalSize = 1.0f;

	//速度
	Vector3 velocity_;

	//ノーツを叩いたかどうか
	bool isHit_ = false;

	//プレイヤーがノーツに触れたか
	bool isTouch_ = false;

	//ノーツをスルー、または反応がずれすぎた場合ミス
	bool isMiss_ = false;

};

class NoteNormal : public Notes
{
public:
	NoteNormal();
	~NoteNormal() = default;

	void Initialize() override;

private:

};

class NoteLong : public Notes
{
public:
	NoteLong();
	~NoteLong() = default;

	static void StaticInitialize();

	enum LongNoteType {
		Start,
		Middle,
		End
	};

	void Update() override;

	void Initialize() override;

	void SetLongNoteType(LongNoteType type) { longNoteType_ = type; }

private:

	//始点、中点、終点のいずれか
	LongNoteType longNoteType_ = Start;

	//始点をタップしたかどうか
	static bool isHitStart_;

	//終点用のフラグ
	bool onHoldNote_ = false;

};

class NoteDamage : public Notes
{
public:
	NoteDamage();
	~NoteDamage() = default;

	void Update() override;

	void Initialize() override;

private:

};


