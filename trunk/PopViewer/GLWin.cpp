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

#include "GLWin.h"

GLWin::GLWin(HWND hWnd)
{
	init = false;
	gldc = 0;
	dc = 0;
	if(hWnd==INVALID_HANDLE_VALUE) return;
	this->hWnd = hWnd;
	dc = GetDC(hWnd);
	if(dc==0) return;
	PIXELFORMATDESCRIPTOR pcd;
	ZeroMemory(&pcd, sizeof(PIXELFORMATDESCRIPTOR));
	pcd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pcd.nVersion = 1;
	pcd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pcd.iPixelType = PFD_TYPE_RGBA;
	pcd.cColorBits = 32;
	pcd.cDepthBits = 16;
	pcd.iLayerType = PFD_MAIN_PLANE;
	int format = ChoosePixelFormat(dc, &pcd);
	SetPixelFormat(dc, format, &pcd);

	gldc = wglCreateContext(dc);
	if(gldc==0) return;
	if(wglMakeCurrent(dc, gldc))
	init = true;
}

void GLWin::Update()
{
	if(dc!=0)
	{
		SwapBuffers(dc);
	}
}

bool GLWin::GetInit()
{
	return init;
}

void GLWin::Release()
{
	wglMakeCurrent( NULL, NULL );
	if(dc!=0)
	{
		ReleaseDC(hWnd, dc);
		dc = 0;
	}
	if(gldc!=0)
	{
		wglDeleteContext(gldc);
		gldc = 0;
	}
}

GLWin::~GLWin()
{
	Release();
}