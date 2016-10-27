#pragma once

typedef enum E_TeamColor {
	Red,
	Blue,
	Grey
} ETeamColor;

typedef enum E_Class {
	Soldier,
	Sniper,
	Medic,
	Scout,
	Spy,
	Heavy,
	Demoman,
	Pyro,
	Engineer
} EClass;

struct FClassInfo {
	EClass Class;
    float MaxHealth;
    float MaxRunSpeed;	
};
class TFQuakeUtils {
public:
	TFQuakeUtils(void);
	~TFQuakeUtils(void);
};

