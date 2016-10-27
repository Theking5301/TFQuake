#include "../../idlib/precompiled.h"
#pragma hdrstop

#include "../Game_local.h"
#include "../Weapon.h"

const idEventDef EV_Railgun_RestoreHum( "<railgunRestoreHum>", "" );

class TFSniperRifle : public rvWeapon {
public:

	CLASS_PROTOTYPE( TFSniperRifle );

	TFSniperRifle ( void );

	virtual void			Spawn				( void );
	virtual void			Think				( void );
	void					Save				( idSaveGame *savefile ) const;
	void					Restore				( idRestoreGame *savefile );
	void					PreSave				( void );
	void					PostSave			( void );
	void					ClientUnstale		( void );

protected:
	jointHandle_t			jointBatteryView;

private:

	stateResult_t		State_Idle		( const stateParms_t& parms );
	stateResult_t		State_Fire		( const stateParms_t& parms );
	stateResult_t		State_Reload	( const stateParms_t& parms );

	void				Event_RestoreHum	( void );

	CLASS_STATES_PROTOTYPE ( TFSniperRifle );
};

CLASS_DECLARATION( rvWeapon, TFSniperRifle )
	EVENT( EV_Railgun_RestoreHum,			TFSniperRifle::Event_RestoreHum )
END_CLASS

/*
================
TFSniperRifle::TFSniperRifle
================
*/
TFSniperRifle::TFSniperRifle ( void ) {
}

/*
================
TFSniperRifle::Spawn
================
*/
void TFSniperRifle::Spawn ( void ) {
	SetState ( "Raise", 0 );	
}

/*
================
TFSniperRifle::Save
================
*/
void TFSniperRifle::Save ( idSaveGame *savefile ) const {
	savefile->WriteJoint( jointBatteryView );
}

/*
================
TFSniperRifle::Restore
================
*/
void TFSniperRifle::Restore ( idRestoreGame *savefile ) {
	savefile->ReadJoint( jointBatteryView );
}

/*
================
TFSniperRifle::PreSave
================
*/
void TFSniperRifle::PreSave ( void ) {

	//this should shoosh the humming but not the shooting sound.
	StopSound( SND_CHANNEL_BODY2, 0);
}

/*
================
TFSniperRifle::PostSave
================
*/
void TFSniperRifle::PostSave ( void ) {

	//restore the humming
	PostEventMS( &EV_Railgun_RestoreHum, 10);
}

/*
================
TFSniperRifle::Think
================
*/
void TFSniperRifle::Think ( void ) {

	// Let the real weapon think first
	rvWeapon::Think ( );

	if ( zoomGui && wsfl.zoom && !gameLocal.isMultiplayer ) {
		int ammo = AmmoInClip();
		if ( ammo >= 0 ) {
			zoomGui->SetStateInt( "player_ammo", ammo );
		}			
	}
}

/*
===============================================================================

	States 

===============================================================================
*/

CLASS_STATES_DECLARATION ( TFSniperRifle )
	STATE ( "Idle",				TFSniperRifle::State_Idle)
	STATE ( "Fire",				TFSniperRifle::State_Fire )
	STATE ( "Reload",			TFSniperRifle::State_Reload )
END_CLASS_STATES

/*
================
TFSniperRifle::State_Idle
================
*/
stateResult_t TFSniperRifle::State_Idle( const stateParms_t& parms ) {
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			if ( !AmmoAvailable ( ) ) {
				SetStatus ( WP_OUTOFAMMO );
			} else {
				StopSound( SND_CHANNEL_BODY2, false );
				StartSound( "snd_idle_hum", SND_CHANNEL_BODY2, 0, false, NULL );
				SetStatus ( WP_READY );
			}
			PlayCycle( ANIMCHANNEL_ALL, "idle", parms.blendFrames );
			return SRESULT_STAGE ( STAGE_WAIT );
		
		case STAGE_WAIT:			
			if ( wsfl.lowerWeapon ) {
				StopSound( SND_CHANNEL_BODY2, false );
				SetState ( "Lower", 4 );
				return SRESULT_DONE;
			}		
			if ( gameLocal.time > nextAttackTime && wsfl.attack && AmmoInClip ( ) ) {
				SetState ( "Fire", 0 );
				return SRESULT_DONE;
			}  
			// Auto reload?
			if ( AutoReload() && !AmmoInClip ( ) && AmmoAvailable () ) {
				SetState ( "reload", 2 );
				return SRESULT_DONE;
			}
			if ( wsfl.netReload || (wsfl.reload && AmmoInClip() < ClipSize() && AmmoAvailable()>AmmoInClip()) ) {
				SetState ( "Reload", 4 );
				return SRESULT_DONE;			
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}

/*
================
TFSniperRifle::State_Fire
================
*/
stateResult_t TFSniperRifle::State_Fire ( const stateParms_t& parms ) {
	enum {
		STAGE_INIT,
		STAGE_WAIT,
	};	
	switch ( parms.stage ) {
		case STAGE_INIT:
			nextAttackTime = gameLocal.time + (fireRate * owner->PowerUpModifier ( PMOD_FIRERATE ));
			Attack ( false, 1, spread, 0, 1.0f );
			PlayAnim ( ANIMCHANNEL_ALL, "fire", 0 );	
			return SRESULT_STAGE ( STAGE_WAIT );
	
		case STAGE_WAIT:		
			if ( ( gameLocal.isMultiplayer && gameLocal.time >= nextAttackTime ) || 
				 ( !gameLocal.isMultiplayer && ( AnimDone ( ANIMCHANNEL_ALL, 2 ) ) ) ) {
				SetState ( "Idle", 0 );
				return SRESULT_DONE;
			}		
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}


/*
================
TFSniperRifle::State_Reload
================
*/
stateResult_t TFSniperRifle::State_Reload ( const stateParms_t& parms ) {
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
				AddToClip ( ClipSize() );
				SetState ( "Idle", 4 );
				return SRESULT_DONE;
			}
			if ( wsfl.lowerWeapon ) {
				StopSound( SND_CHANNEL_BODY2, false );
				SetState ( "Lower", 4 );
				return SRESULT_DONE;
			}
			return SRESULT_WAIT;
	}
	return SRESULT_ERROR;
}

/*
===============================================================================

	Event 

===============================================================================
*/

/*
================
TFSniperRifle::State_Reload
================
*/
void TFSniperRifle::Event_RestoreHum ( void ) {
	StopSound( SND_CHANNEL_BODY2, false );
	StartSound( "snd_idle_hum", SND_CHANNEL_BODY2, 0, false, NULL );
}

/*
================
TFSniperRifle::ClientUnStale
================
*/
void TFSniperRifle::ClientUnstale( void ) {
	Event_RestoreHum();
}

