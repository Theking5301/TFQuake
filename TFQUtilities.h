#pragma once
#include "TFQuakePCH.h"
#include "TFQPlayerClassInfo.h"


class TFQUtilities {

public:
	TFQUtilities(void);
	~TFQUtilities(void);

	static FPlayerClassInfo GetPlayerClassInfo(idDict SpawnArgs, EPlayerClass Class);
	static FPlayerClassInfo CreateClassInfo(idDict spawnArgs, const char* maxHealth, const char* maxRunSpeed, const char* maxJumpHeigh, const char* maxJumpCount);
};

