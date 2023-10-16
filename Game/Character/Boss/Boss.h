#pragma once
#include "Game/Character/BaseCharacter/BaseCharacter.h"

class Boss : public BaseCharacter
{
public:
	Boss() = default;
	~Boss() = default;

	void Initialize() override;

	void Update() override;

private:

};
