#pragma once

#ifndef TF_QUAKE_DATA
#define TF_QUAKE_DATA

	typedef enum  {
		Red,
		Blue,
		Grey
	}ETeamColor;

	typedef enum  {
		Soldier,
		Sniper,
		Medic,
		Scout,
		Spy,
		Heavy,
		Demoman,
		Pyro,
		Engineer
	}EPlayerClass;

	typedef struct  {
		EPlayerClass Class;
		float MaxHealth;
		float MaxRunSpeed;	
	}FClassInfo;


class TFQuakeUtils {
public:
	TFQuakeUtils(void);
	~TFQuakeUtils(void);
};

#endif
