#pragma once
#include "Engine/Texture/Model.h"
#include "Engine/WorldTransform/WorldTransform.h"
#include <memory>

class NotesEffect
{
public:
	NotesEffect();
	~NotesEffect();

	void Initialize();

	void Update();

	void Draw(const Matrix4x4& viewProjection);

	void ModelLoad(Model* model) {
		modelNotes_ = model;
	}

	void SetPosition(const Vector3& pos) { worldTransform_.translation_ = pos; }

	void SubPosition(const Vector3& sub) { worldTransform_.translation_ -= sub; }

	const Vector3& GetPosition() { return worldTransform_.translation_; }

	bool GetIsDead() { return isDead_; }

	void SetParent(const WorldTransform* worldTransform) { worldTransform_.parent_ = worldTransform; }

	void SetColor(uint32_t color) { color_ = color; }

	void SetIsMiss(bool flag) { isMiss_ = flag; }

private:

	Model* modelNotes_;

	WorldTransform worldTransform_;

	uint32_t color_ = 0xffffffff;

	bool isDead_ = false;

	bool isMiss_ = false;

	uint32_t count_ = 10;

};
