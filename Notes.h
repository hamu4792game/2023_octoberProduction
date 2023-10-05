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

protected:

	NoteType type_ = Normal;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	Player* player_;

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

	void Initialize() override;

private:

};

class NoteDamage : public Notes
{
public:
	NoteDamage();
	~NoteDamage() = default;

	void Initialize() override;

private:

};


