//Copyright (c) 2010 Omkar Kanase
//
//Permission is hereby granted, free of charge, to any person obtaining a copy
//of this software and associated documentation files (the "Software"), to deal
//in the Software without restriction, including without limitation the rights
//to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//copies of the Software, and to permit persons to whom the Software is
//furnished to do so, subject to the following conditions:
//
//The above copyright notice and this permission notice shall be included in
//all copies or substantial portions of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//THE SOFTWARE.

#include "main.h"

GLWin* glwin=0;
PopMap map;

POINT mouse;
bool mdown=false;
bool lock=true;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	POINT tmp;
	int ty = 0, tx=0;
	if(msg==WM_CLOSE)
	{
		if(glwin!=0) glwin->Release();
		PostQuitMessage(0);
	}
	switch(msg)
	{
	case WM_LBUTTONDOWN:
		mdown = true;
		SetCapture(hwnd);
		mouse.x = LOWORD(lParam);
		mouse.y = HIWORD(lParam);
		break;
	case WM_LBUTTONUP:
		if(mdown)
		{
			ReleaseCapture();
			mdown = false;
		}
		break;
	case WM_MOUSEMOVE:
		if(mdown)
		{
			tmp.x = LOWORD(lParam);
			tmp.y = HIWORD(lParam);
			if(map.GetFlatMode())
			{
				map.RotateSpin(float(tmp.x - mouse.x));
				if(!lock)map.RotateUpDown(float(tmp.y - mouse.y));
			}
			else
			{
				map.UpdatePos(-(tmp.x - mouse.x), -(tmp.y - mouse.y));
			}
			mouse = tmp;
			map.Draw();
			glwin->Update();
		}
		break;
	case WM_MOUSEWHEEL:
		if(map.GetFlatMode()) map.SetZDistance(map.GetZDistance() - short(HIWORD(wParam))/120.0f);
		else map.SetYDistance(map.GetYDistance() - short(HIWORD(wParam))/120.0f);
		map.Draw();
		glwin->Update();
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case 'F':
			if(map.GetFillMode()==GL_FILL) map.SetFillMode(GL_POINT);
			else if(map.GetFillMode()==GL_POINT) map.SetFillMode(GL_LINE);
			else map.SetFillMode(GL_FILL);
			map.Draw();
			glwin->Update();
			break;
		case 'T':
			map.SetViewTrees(!map.GetViewTrees());
			map.Draw();
			glwin->Update();
			break;
		case 'P':
			map.SetViewPlayers(!map.GetViewPlayers());
			map.Draw();
			glwin->Update();
			break;
		case 'S':
			map.SetViewStone(!map.GetViewStone());
			map.Draw();
			glwin->Update();
			break;
		case 'L':
			lock = !lock;
			break;
		case 'Q':
			map.SetDrawOnlyLines(!map.GetDrawOnlyLines());
			map.Draw();
			glwin->Update();
			break;
		case 'W':
			map.SetDrawOnlyPoints(!map.GetDrawOnlyPoints());
			map.Draw();
			glwin->Update();
			break;
		case 'E':
			map.SetFlatMode(!map.GetFlatMode());
			map.Draw();
			glwin->Update();
			break;
		case VK_UP:
			map.SetHeightModifier(map.GetHeightModifier()+0.2f);
			map.Draw();
			glwin->Update();
			break;
		case VK_DOWN:
			map.SetHeightModifier(map.GetHeightModifier()-0.2f);
			map.Draw();
			glwin->Update();
			break;
		case VK_F1:
			MessageBox(hwnd, L"Help : \nF:Change Fill Mode\nT : Toggle View Trees\nP : Toggle view player positions\nQ : Toggle Draw only lines\nW : Draw only points\nE : Toggle Shperical and Flat Mode\n(up,down)Arrow : Change Land Height\nS : Toggle view stone heads\nL : Lock rotation on x axis\n\n Colors : \nDark green : Trees\nBrown : StoneHeads\nBlue : Blue shaman startup position\nRed : Red shaman startup position\nGreen : Green shaman startup position\nYellow : Yellow shaman startup position", L"Help", MB_ICONINFORMATION | MB_OK);
			break;
		}
		break;		
	case WM_SIZE:
		map.ReSize(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_PAINT:
		map.Draw();
		glwin->Update();
		break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR CmdLines, int cmdshow)
{
	#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF |  _CRTDBG_LEAK_CHECK_DF);
#endif
	WNDCLASS wc;
	ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"clspopview";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);

	HWND wnd = CreateWindow(L"clspopview", L"POPViewer (Press F1 for help)", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, 480, 0, 0, hInstance, 0);
	ShowWindow(wnd, cmdshow);

	glwin = new GLWin(wnd);

	if( !glwin->GetInit() )
	{
		delete glwin;
		return 0;
	}

	RECT rc;
	GetClientRect(wnd, &rc);

	map.ReSize(rc.right - rc.left, rc.bottom - rc.top);
	map.Init();

	if(!map.LoadMap("levl2131.dat"))
	{
		PostQuitMessage(1);
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while(msg.message != WM_QUIT)
	{
		if(GetMessage(&msg, 0, 0, 0))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		//{
		//	TranslateMessage(&msg);
		//	DispatchMessage(&msg);
		//}
		//else
		//{
		//	//map.RotateSpin(-1);
		//	map.Draw();
		//	glwin->Update();
		//}
	}

	delete glwin;
	return msg.wParam;
}