#include "../../../idlib/precompiled.h"
#pragma hdrstop

#include "../../Game_local.h"
#include "../../Weapon.h"

class TFDemoGrenadeLauncher : public rvWeapon {
public:

	CLASS_PROTOTYPE( TFDemoGrenadeLauncher );

	TFDemoGrenadeLauncher ( void );

	virtual void			Spawn				( void );
	void					PreSave				( void );
	void					PostSave			( void );

#ifdef _XENON
	virtual bool		AllowAutoAim			( void ) const { return false; }
#endif

private:

	stateResult_t		State_Idle		( const stateParms_t& parms );
	stateResult_t		State_Fire		( const stateParms_t& parms );
	stateResult_t		State_Reload	( const stateParms_t& parms );

	const char*			GetFireAnim() const { return (!AmmoInClip()) ? "fire_empty" : "fire"; }
	const char*			GetIdleAnim() const { return (!AmmoInClip()) ? "idle_empty" : "idle"; }
	
	CLASS_STATES_PROTOTYPE ( TFDemoGrenadeLauncher );
};

CLASS_DECLARATION( rvWeapon, TFDemoGrenadeLauncher )
END_CLASS

/*
================
TFDemoGrenadeLauncher::TFDemoGrenadeLauncher
================
*/
TFDemoGrenadeLauncher::TFDemoGrenadeLauncher ( void ) {
}

/*
================
TFDemoGrenadeLauncher::Spawn
================
*/
void TFDemoGrenadeLauncher::Spawn ( void ) {
	SetState ( "Raise", 0 );	
}

/*
================
TFDemoGrenadeLauncher::PreSave
================
*/
void TFDemoGrenadeLauncher::PreSave ( void ) {
}

/*
================
TFDemoGrenadeLauncher::PostSave
================
*/
void TFDemoGrenadeLauncher::PostSave ( void ) {
}

/*
===============================================================================

	States 

===============================================================================
*/

CLASS_STATES_DECLARATION ( TFDemoGrenadeLauncher )
	STATE ( "Idle",		TFDemoGrenadeLauncher::State_Idle)
	STATE ( "Fire",		TFDemoGrenadeLauncher::State_Fire )
	STATE ( "Reload",	TFDemoGrenadeLauncher::State_Reload )
END_CLASS_STATES

/*
================
TFDemoGrenadeLauncher::State_Idle
================
*/
stateResult_t TFDemoGrenadeLauncher::State_Idle( const stateParms_t& parms ) {
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			if ( !AmmoAvailable ( ) ) {
				SetStatus ( WP_OUTOFAMMO );
			} else {
				SetStatus ( WP_READY );
			}
		
			PlayCycle( ANIMCHANNEL_ALL, GetIdleAnim(), parms.blendFrames );
			return SRESULT_STAGE ( STAGE_WAIT );
		
		case STAGE_WAIT:			
			if ( wsfl.lowerWeapon ) {
				SetState ( "Lower", 4 );
				return SRESULT_DONE;
			}		
			if ( !clipSize ) {
				if ( wsfl.attack && AmmoAvailable ( ) ) {
					SetState ( "Fire", 0 );
					return SRESULT_DONE;
				}
			} else { 
				if ( gameLocal.time > nextAttackTime && wsfl.attack && AmmoInClip ( ) ) {
					SetState ( "Fire", 0 );
					return SRESULT_DONE;
				}  
						
				if ( wsfl.attack && AutoReload() && !AmmoInClip ( ) && AmmoAvailable () ) {
					SetState ( "Reload", 4 );
					return SRESULT_DONE;			
				}
				if ( wsfl.netReload || (wsfl.reload && AmmoInClip() < ClipSize() && AmmoAvailable()>AmmoInClip()) ) {
					SetState ( "Reload", 4 );
					return SRESULT_DONE;			
				}
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}

/*
================
TFDemoGrenadeLauncher::State_Fire
================
*/
stateResult_t TFDemoGrenadeLauncher::State_Fire ( const stateParms_t& parms ) {
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			nextAttackTime = gameLocal.time + (fireRate * owner->PowerUpModifier ( PMOD_FIRERATE ));
			Attack ( false, 1, spread, 0, 1.0f );
			PlayAnim ( ANIMCHANNEL_ALL, GetFireAnim(), 0 );	
			return SRESULT_STAGE ( STAGE_WAIT );
	
		case STAGE_WAIT:		
			if ( wsfl.attack && gameLocal.time >= nextAttackTime && AmmoInClip() && !wsfl.lowerWeapon ) {
				SetState ( "Fire", 0 );
				return SRESULT_DONE;
			}
			if ( AnimDone ( ANIMCHANNEL_ALL, 0 ) ) {
				SetState ( "Idle", 0 );
				return SRESULT_DONE;
			}		
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}

/*
================
TFDemoGrenadeLauncher::State_Reload
================
*/
stateResult_t TFDemoGrenadeLauncher::State_Reload ( const stateParms_t& parms ) {
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			if ( wsfl.netReload ) {
				wsfl.netReload = false;
			} else {
				NetReload ( );
			}
			
			SetStatus ( WP_RELOAD );
			PlayAnim ( ANIMCHANNEL_ALL, "reload", parms.blendFrames );
			return SRESULT_STAGE ( STAGE_WAIT );
			
		case STAGE_WAIT:
			if ( AnimDone ( ANIMCHANNEL_ALL, 4 ) ) {
				//AddToClip ( ClipSize() );
				AddToClip (1);
				SetState ( "Idle", 4 );
				return SRESULT_DONE;
			}
			if ( wsfl.lowerWeapon ) {
				SetState ( "Lower", 4 );
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}
			
