#pragma once
#include "fmod.hpp"
#include "fmod_errors.h"

#pragma comment( lib, "fmodex_vc.lib")

using namespace FMOD;

const int g_iMaxSound = SND_RESOURCES_LAST; //13개 음원을 처리 하려고 함.
// c,  c++
class GSound
{
public:
	int				m_iSoundList;
	FMOD::System*   m_pSystem;
	FMOD::Sound*    m_pSound[g_iMaxSound];
	FMOD::Channel*    m_pChannel[g_iMaxSound];
	//FMOD::Channel* m_pChannela;
public:
	bool	Init();
	int     Load(char* pLoadName);
	int     Load(char* pLoadName, bool bLoop);
	void	Play(int iIndex, bool bPlay, bool bReUse = false);
	void    Stop(int iIndex)
	{
		m_pChannel[iIndex]->stop();
	}
	void    Paused(int iIndex)
	{
		bool paused;
		m_pChannel[iIndex]->getPaused(&paused);
		m_pChannel[iIndex]->setPaused(!paused);
	}
	void   Volume(int iIndex, float fVolume, bool bUp);
	void	Release();
	void	Frame();
	
public:
	GSound();
	virtual ~GSound();
};

