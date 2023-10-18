#pragma once
#include "Game/Character/BaseCharacter/BaseCharacter.h"
#include<fstream>
#include<iostream>
#include<memory>
#include"externals/nlohmann/json.hpp"


class Hero : public BaseCharacter
{
public:
	Hero() = default;
	~Hero() = default;

	void Initialize() override;

	void Update() override;

private:
	enum Position {
		Start = 0,
		End,
		kMaxCount
	};

	struct HeroTransform {
		Vector3 translation_;
		Vector3 rotation_;
		Vector3 scale_;
	};

	using json = nlohmann::json;

	std::vector<std::string> partsName;

	std::vector<std::string> posName;

	std::vector<HeroTransform> startPos;

	std::vector<HeroTransform> EndPos;

	int chackOnlyNumber = 0;

	const std::string kDirectoryPath = "Resources/Animation/";

	const std::string kDirectoryName = "Resources/Animation";

	const std::string kItemName = "Hero";

	void SaveFile();

	void LoadFiles();

	void LoadFile(const std::string& groupName);

	void from_json(const json& j, Vector3& v);

	bool LoadChackItem(const std::string& directoryPath, const std::string& itemName);

};
