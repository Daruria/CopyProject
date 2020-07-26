#ifndef Struct_h__
#define Struct_h__

#define SC_PSTATE(eState) static_cast<int>(EPlayer::eState)
#define SC_WSTATE(eState) static_cast<int>(EWar::eState)
#define SC_SSTATE(eState) static_cast<int>(EStrife::eState)

union UState
{
public:
	UState(EPlayer eState) { ePlayer = eState; }
	UState(EWar eState) { eWar = eState; }
	UState(EStrife eState) { eStrife = eState; }

	EPlayer ePlayer;
	EWar	eWar;
	EStrife eStrife;

	bool operator != (EPlayer eState) { return ePlayer != eState; }
	bool operator != (EWar eState) { return eWar != eState; }
	bool operator != (EStrife eState) { return eStrife != eState; }

	bool operator == (EPlayer eState) { return ePlayer == eState; }
	bool operator == (EWar eState) { return eWar == eState; }
	bool operator == (EStrife eState) { return eStrife == eState; }
	bool operator == (UState eState) { return SC_INT(this->eWar) == SC_INT(eState.eWar); }



	int operator = (EPlayer eState) { return SC_INT(ePlayer = eState); }
	int operator = (EWar eState) { return SC_INT(eWar = eState); }
	int operator = (EStrife eState) { return SC_INT(eStrife = eState); }
};

typedef struct tagBoneLinker
{
	const _matrix*	pOriginBone;
	_matrix			matLink;
}BONE_LINKER;

//bool operator != (EPlayer eState, UState uState) { return eState != uState.ePlayer; }
//bool operator != (EWar eState, UState uState) { return eState != uState.eWar; }
//bool operator != (EStrife eState, UState uState) { return eState != uState.eStrife; }


#endif // Struct_h__
