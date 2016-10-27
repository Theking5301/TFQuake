#pragma once
#include "TFQuakePCH.h"

typedef enum  {
	SCOUT,
	SOLDIER,
	PYRO,
	DEMOMAN,
	HEAVY,
	ENGINEER,
	MEDIC,
	SNIPER,
	SPY
} EPlayerClass;

struct FPlayerClassInfo {
	int maxHealth;
	int maxRunSpeed;
	int maxJumpHeight;
	int maxJumpCount;
};

class TFQPlayerClassInfo {
public:
	TFQPlayerClassInfo(void);
	~TFQPlayerClassInfo(void);
};

