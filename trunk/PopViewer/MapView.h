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

#pragma once

#ifdef WIN32
#include <Windows.h>
#endif

#include <gl/GL.h>
#include <gl/GLU.h>
#include <vector>
#include <fstream>

using namespace std;

#define _Get(x, y) x Get##y() { return y; }
#define _Set(x, y) void Set##y(x value) { this->y = value; }
#define _GetSet(x, y) _Get(x, y); _Set(x,y)

// Vertex Structure
struct Vertex
{
	float r, g, b;
	float x, y, z;
	Vertex(float r, float g, float b, float x, float y, float z)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->x = x;
		this->y = y;
		this->z = z;
	}
};


// Populous Map Object
#pragma pack(1)
struct Object
{
	unsigned char Model;
	unsigned char Type;
	char Owner;
	unsigned short PosX;
	unsigned short PosY;
};
#pragma pack(8)

struct _Color
{
	float r, g, b;
	_Color()
	{
		r=0;
		g=0;
		b=0;
	}
	_Color(float r, float g, float b)
	{
		this->r = r;
		this->g = g;
		this->b = b;
	}
};

// Pulous map viewer class
class PopMap
{	
	vector<Vertex> Verts; //vertices to store Mapdata
	vector<GLushort> Index; //Map Index data
	vector<Vertex> Players; // set of vertices to store player startup position
	vector<Vertex> Trees; // set of vertices to store Trees data
	vector<Vertex> StoneHeads; // set of vertices to store StoneHeads data

	vector<Vertex> Vec;
	vector<Vertex> pls;
	vector<float> heights;

	bool ViewTrees; // Show Tress
	bool ViewStone; // Show StoneHeads
	bool ViewPlayers; // Show Player startup Positions

	_Color BgColor; // Background Color
	GLenum FillMode; // File mode for map (POINT, WAIREFRAME, SOLID) 

	float RotateX;
	float RotateY;

	float ZDistance;
	float YDistance;

	bool Loaded;
	float HeightModifier;

	bool DrawOnlyLines;
	bool DrawOnlyPoints;

	int PX;
	int PY;

	bool FlatMode;

public:	
	PopMap();
	_GetSet(bool, FlatMode);
	_GetSet(bool, DrawOnlyLines);
	_GetSet(bool, DrawOnlyPoints);
	_GetSet(bool, ViewTrees);
	_GetSet(bool, ViewStone);
	_GetSet(bool, ViewPlayers);
	_GetSet(_Color, BgColor);
	_GetSet(GLenum, FillMode);
	_GetSet(float, RotateX);
	_GetSet(float, RotateY);
	_GetSet(float, ZDistance);
	_GetSet(float, YDistance);
	_GetSet(float, HeightModifier);
	// Loading map from file
	bool LoadMap(char* path);
	// Draw map in current opengl window
	void Draw();
	// Resize opengl window and refresh viewport
	void ReSize(unsigned int w, unsigned int h);
	// Preset Opengl Settings
	void Init();

	void RotateSpin(float value);
	void RotateUpDown(float value);

	float GetHeight(float x, float z);

	void UpdatePos(int px, int py);

	void UnloadMap();
};