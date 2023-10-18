#pragma once
#include "Game/Character/BaseCharacter/BaseCharacter.h"

class Hero : public BaseCharacter
{
public:
	Hero() = default;
	~Hero() = default;

	void Initialize() override;

	void Update() override;

private:

};
