#pragma once

bool Initialize();
void Update(double deltaTime);
bool Render();
bool Release();
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);