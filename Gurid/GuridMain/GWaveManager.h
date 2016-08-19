#pragma once
class GWaveManager
{
public:
	bool		Init() { return true; };
	bool		Frame(){
		
		g_iWave = 1;	//Wave Count, 표시
		g_iHP = 100;	//주인공 체력 표시
		g_iBoost = 100; //주인공 부스트 표시
		g_iShell = 10;  //주인공 잔탄 표시


		return true; 
	};
	bool		Render() { return true; };
	bool		Release() { return true; };

	GWaveManager();
	virtual ~GWaveManager();
};

