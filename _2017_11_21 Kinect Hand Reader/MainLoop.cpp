#include "stdafx.h"
#include "MainLoop.h"

#include "KinectManager.h"
KinectManager* g_kinectManager = nullptr;

#include "HandInputCalculator.h"
HandInputCalculator* g_handInputCalculator = nullptr;

MainLoop::MainLoop()
{
}
MainLoop::~MainLoop()
{
}


HRESULT MainLoop::Initialize()
{
	if (AllocConsole())
	{
		freopen("CONIN$", "rb", stdin);
		freopen("CONOUT$", "wb", stdout);
		freopen("CONOUT$", "wb", stderr);
	}
	else
		return E_FAIL;

	g_handInputCalculator = new HandInputCalculator();

	g_kinectManager = new KinectManager();
	return g_kinectManager->Initialize();
}

void MainLoop::Update()
{
	g_kinectManager->CalculateHandState();
	g_handInputCalculator->SendHandData(g_kinectManager->GetHandData(g_kinectManager->GetMainBody(), Hand::Right));
}

void MainLoop::Render()
{
	g_handInputCalculator->CalculateHandMoving();
	if (g_handInputCalculator->IsSlidedAtUp())		cout << "Up" << endl;
	if (g_handInputCalculator->IsSlidedAtDown())	cout << "Down" << endl;
	if (g_handInputCalculator->IsSlidedAtLeft())	cout << "Left" << endl;
	if (g_handInputCalculator->IsSlidedAtRight())	cout << "Right" << endl;
	
	//auto& data = g_kinectManager->GetHandData(g_kinectManager->GetMainBody(), Hand::Right);
	//cout << "State : " << data.handState << "\tX : " << data.x << "\tY : " << data.y << endl;
}

void MainLoop::Release()
{
	SAFE_DELETE(g_kinectManager);
	FreeConsole();
}
