#pragma once
#include "Game/Character/BaseCharacter/BaseCharacter.h"
#include<fstream>
#include<iostream>
#include<memory>
#include"externals/nlohmann/json.hpp"
#include"Engine/Easing/Ease.h"
#include"Engine/Easing/Easing.h"


class Hero : public BaseCharacter
{
public:
	Hero() = default;
	~Hero() = default;

	void Initialize() override;

	void Update() override;

	void Move();


public:
	void SetPosition(const Vector3& position);

private:
	enum Position {
		Start = 0,
		Middle,
		End,
		kMaxCount,
		Animation
	};

	struct HeroTransform {
		Vector3 translation_;
		Vector3 rotation_;
		Vector3 scale_;
	};

	using json = nlohmann::json;

	std::vector<std::string> partsName;

	std::vector<std::string> posName;

	std::vector<std::string> fileName;

	std::vector<HeroTransform> startPos;

	std::vector<HeroTransform> middlePos;

	std::vector<HeroTransform> EndPos;

	int chackOnlyNumber = 0;

	int fileNumber = 0;

	int AnimationCount = 0;

	int SetStart = Position::Start;

	//アニメーション関連
	bool isAnimation = false;
	//動きの速度に掛ける倍率
	float Magnification = 1.0f;
	//アニメーションスピード
	float AnimationSpeed;

	float AnimationNum = 0.0f;

	float T = 0.0f;

	float ArmT = 0.0f;

	float UpperLegT = 0.0f;

	float BottomLegT = 0.0f;

	const std::string kDirectoryPath = "Resources/Animation/";

	const std::string kDirectoryName = "Resources/Animation";

	const std::string kItemName_ = "Hero";

	std::string Name_ = "\0";

	char ItemName_[256]{};

	void DrawImgui();

	void SaveFile(const std::string& kItemName);

	void ChackFiles();

	void LoadFiles();

	void LoadFile(const std::string& groupName);

	void from_json(const json& j, Vector3& v);

	bool LoadChackItem(const std::string& directoryPath, const std::string& itemName);

};