#pragma once
class GWaveManager
{
public:
	bool		Init() { return true; };
	bool		Frame(){
		
		g_iWave = 1;	//Wave Count, ǥ��
		g_iHP = 100;	//���ΰ� ü�� ǥ��
		g_iBoost = 100; //���ΰ� �ν�Ʈ ǥ��
		g_iShell = 10;  //���ΰ� ��ź ǥ��


		return true; 
	};
	bool		Render() { return true; };
	bool		Release() { return true; };

	GWaveManager();
	virtual ~GWaveManager();
};

