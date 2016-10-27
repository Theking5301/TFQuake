#include "../idlib/precompiled.h"
#include "TFQUtilities.h"


TFQUtilities::TFQUtilities(void) {}
TFQUtilities::~TFQUtilities(void) {}

FPlayerClassInfo TFQUtilities::GetPlayerClassInfo(idDict SpawnArgs, EPlayerClass Class) {
switch(Class) {
	case EPlayerClass::SCOUT:
		return 	CreateClassInfo(SpawnArgs, "scoutMaxHealth", "scoutRunSpeed", "scoutJumpHeight", "scoutJumpHeight");
	case EPlayerClass::SOLDIER:
		return 	CreateClassInfo(SpawnArgs, "soldierMaxHealth", "soldierRunSpeed", "soldierJumpHeight", "soldierJumpHeight");
	case EPlayerClass::PYRO:
		return 	CreateClassInfo(SpawnArgs, "pyroMaxHealth", "pyroRunSpeed", "pyroJumpHeight", "pyroJumpHeight");
	case EPlayerClass::DEMOMAN:
		return 	CreateClassInfo(SpawnArgs, "demomanMaxHealth", "demomanRunSpeed", "demomanJumpHeight", "demomanJumpHeight");
	case EPlayerClass::HEAVY:
		return 	CreateClassInfo(SpawnArgs, "heavyMaxHealth", "heavyRunSpeed", "heavyJumpHeight", "heavyJumpHeight");
	case EPlayerClass::ENGINEER:
		return 	CreateClassInfo(SpawnArgs, "engineerMaxHealth", "engineerRunSpeed", "engineerJumpHeight", "engineerJumpHeight");
	case EPlayerClass::MEDIC:
		return 	CreateClassInfo(SpawnArgs, "medicMaxHealth", "medicRunSpeed", "medicJumpHeight", "medicJumpHeight");
	case EPlayerClass::SNIPER:
		return 	CreateClassInfo(SpawnArgs, "sniperMaxHealth", "sniperRunSpeed", "sniperJumpHeight", "sniperJumpHeight");
	case EPlayerClass::SPY:
		return 	CreateClassInfo(SpawnArgs, "spyMaxHealth", "spyRunSpeed", "spyJumpHeight", "spyJumpHeight");
	default:
		FPlayerClassInfo defaultInfo;
		return defaultInfo;
	}
}
FPlayerClassInfo TFQUtilities::CreateClassInfo(idDict spawnArgs, const char* maxHealth, const char* maxRunSpeed, const char* maxJumpHeigh, const char* maxJumpCount) {
	FPlayerClassInfo tempClassInfo;

	tempClassInfo.maxHealth = spawnArgs.GetInt(maxHealth, "100" );
	tempClassInfo.maxRunSpeed = spawnArgs.GetInt(maxRunSpeed, "200" );
	tempClassInfo.maxJumpHeight = spawnArgs.GetInt(maxJumpHeigh, "100" );
	tempClassInfo.maxJumpCount = spawnArgs.GetInt(maxJumpCount, "1" );

	return tempClassInfo;
}