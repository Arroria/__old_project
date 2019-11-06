#include "stdafx.h"
#include "StageRestart.h"


StageRestart::StageRestart()
{
}
StageRestart::~StageRestart()
{
}



bool StageRestart::Activated()
{
	return false;
}

bool StageRestart::Update()
{
	GetSingleton(SceneManager)->ChangeScene(L"Stage");
	return false;
}

bool StageRestart::Render()
{
	return false;
}

void StageRestart::Disabled()
{
}
