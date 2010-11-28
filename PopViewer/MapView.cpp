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

#include "MapView.h"

PopMap::PopMap()
{
	SetViewPlayers(1);
	SetViewStone(0);
	SetViewTrees(0);
	SetBgColor(_Color(0.0f,0.0f,0.0f));
	SetFillMode(GL_FILL);
	RotateX = 0;
	RotateY = 0;
	ZDistance = 40.0f;
	YDistance = 150.0f;
	Loaded=false;
	PX = 0;
	PY = 0;
	HeightModifier = 2.5f;
	DrawOnlyPoints = false;
	DrawOnlyLines = false;
	FlatMode = false;
}

void PopMap::UnloadMap()
{
	Verts.clear();
	Vec.clear();
	Trees.clear();
	StoneHeads.clear();
	Players.clear();
	pls.clear();
	heights.clear();
	Index.clear();
	Loaded = false;
}

void PopMap::UpdatePos(int px, int py)
{
	px = PX + px;
	py = PY + py;
	while(px>=128) px = px - 128;
	while(px<0) px = 128 + px;
	while(py>=128) py = py - 128;
	while(py<0) py = 128 + py;
	PX = px;
	PY = py;
}

void PopMap::RotateSpin(float value)
{
	RotateY += value;
	if(RotateY>360) RotateY-=360;
	if(RotateY<-360) RotateY+=360;
}

void PopMap::RotateUpDown(float value)
{
	RotateX += value;
	if(RotateX>360) RotateX-=360;
	if(RotateX<-360) RotateX+=360;
}

void PopMap::Init()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,30.0, 50.0, 0, 0, 0, 0, 1, 0);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POINT_SMOOTH);
	glCullFace(GL_BACK);
}

void PopMap::ReSize(unsigned int w, unsigned int h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, float(w)/float(h), 1.0, 1000.0);

	glMatrixMode(GL_MODELVIEW);
}

//Loading Map
bool PopMap::LoadMap(achar* path)
{
	UnloadMap();
	Loaded=false;
	//HANDLE h= CreateFile(L"D:\\Bullfrog\\levels\\levl2131.dat", GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	fstream file(path, fstream::in | fstream::binary);
	if(file.fail()) return 0;

	unsigned short data[16384];
	//DWORD read;
	//ReadFile(h, data, 16384*2, &read, 0);
	file.read((char*)data, 16384*2);
	if(file.fail())
	{
		file.close();
		return 0;
	}

	file.seekg(0x14043, ios::beg);
	if(file.fail())
	{
		file.close();
		return 0;
	}
	//SetFilePointer(h, 0x14043, 0, FILE_BEGIN);

	Object* obj;
	char buff[55];

	while(1)
	{
		//ReadFile(h, buff, 55, &read, 0);
		file.read(buff, 55);
		if(file.fail())
		{
			file.close();
			break;
		}

		obj = (Object*) buff;
		if(obj->Model == 7 && obj->Type == 1)
		{
			switch(obj->Owner)
			{
			case 0:
				Players.push_back(Vertex(0,0,1, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 0, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				Players.push_back(Vertex(0,0,1, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 4, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				break;
			case 1:
				Players.push_back(Vertex(1,0,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 0, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				Players.push_back(Vertex(1,0,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 4, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				break;
			case 2:
				Players.push_back(Vertex(1,1,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 0, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				Players.push_back(Vertex(1,1,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 4, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				break;
			case 3:
				Players.push_back(Vertex(0,1,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 0, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				Players.push_back(Vertex(0,1,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 4, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				break;
			default:
				Players.push_back(Vertex(1,1,1, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 0, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
				Players.push_back(Vertex(1,1,1, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 4, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
			}
			
		}
		else if(obj->Type == 5 && obj->Model < 9)
		{
			Trees.push_back(Vertex(0,0.5f,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 0, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
			Trees.push_back(Vertex(0,0.5f,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 2, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
		}
		else if(obj->Type == 5 && obj->Model == 9)
		{
			StoneHeads.push_back(Vertex(0.8f,0.5f,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 0, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
			StoneHeads.push_back(Vertex(0.8f,0.5f,0, (((obj->PosX/ float(MAXWORD)) * 128) - 64) / 2.0f + 0.5f, 2, (((1-(obj->PosY/ float(MAXWORD))) * 128) - 65) / 2.0f + 0.5f));
		}
	}

	file.close();
	//CloseHandle(h);

	
	for(int i=-64; i<64; i++)
	{
		for(int f=-64; f<64; f++)
		{
			//float curve = (cos(f/45.0f) + cos(i/45.0f)) * 48.0f;

			if(data[((-i+63) * 128) + (f+64)]==0)
			{				
				//((f%2)+(i%2))*0.8f,f/2.0f
				//verts.push_back(vertex(0,0,max(min(0.1f,abs(sin(float((f*i)/4096)))), 1.0f),f/2.0f + 0.5f, 0, i/2.0f + 0.5f));
				if(((i%2)*(f%2))==0)
				{
					Verts.push_back(Vertex(0,0,0.25f,f/2.0f + 0.5f, 0 , i/2.0f + 0.5f));
				}
				else
				{
					Verts.push_back(Vertex(0,0,0.3f,f/2.0f + 0.5f, 0, i/2.0f + 0.5f));
				}
			}
			else if(data[((-i+63) * 128) + (f+64)]<400)
			{
				Verts.push_back(Vertex(0,data[((-i+63) * 128) + (f+64)]/600.0f,0.1f,f/2.0f + 0.5f, data[((-i+63) * 128) + (f+64)]/280.0f , i/2.0f + 0.5f));
			}
			else
			{
				Verts.push_back(Vertex(data[((-i+63) * 128) + (f+64)]/1100.0f,0.7f,0.1f,f/2.0f + 0.5f, data[((-i+63) * 128) + (f+64)]/280.0f, i/2.0f + 0.5f));
			}

			heights.push_back(data[((-i+63) * 128) + (f+64)]/280.0f);
		}
	}

	for(GLushort y=0; y<31; y++)
	{
		for(GLushort x=0; x<127; x++)
		{
			Index.push_back((y*128) + x);
			Index.push_back((y*128) + x + 1);
			Index.push_back(((y+1)*128) + x);

			Index.push_back(((y+1)*128) + x);
			Index.push_back((y*128) + x + 1);
			Index.push_back(((y+1)*128) + x + 1);
		}
	}
	Loaded=true;
	return true;
}

float PopMap::GetHeight(float x, float z)
{
	 x = (x  - 0.5f) * 2.0f;
	 z = (z  - 0.5f) * 2.0f;

	 x += 64;
	 z += 64;

     int row = (int)floorf(x);
	 int col = (int)floorf(z);
	 	 
	 float A = heights[(128*col)+row]; 
     float B = heights[(128*col)+row+1];
     float C = heights[(128*(col+1))+row];
     float D = heights[(128*(col+1))+(row+1)];

	 float s = z - (float)col;
	 float t = x - (float)row;

	 if(t < 1.0f - s)
	 {
          float uy = B - A;
          float vy = C - A;
          return A + s*uy + t*vy;
     }

    float uy = C - D;
    float vy = B - D;
    return D + (1.0f-s)*uy + (1.0f-t)*vy;
}


void PopMap::Draw()
{
	glClearColor(BgColor.r, BgColor.g, BgColor.b, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(Loaded)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		if(FlatMode) gluLookAt(0,YDistance/4.0f, ZDistance, 0, 0, 0, 0, 1.0f, 0);
		else gluLookAt(0,YDistance, 0.01f, 0, 0, 0, 0, 1.0f, 0);
		
		glRotatef(RotateY, 0.0f, 1.0f, 0.0f);
		if(FlatMode) glRotatef(RotateX, 0.0f, 0.0f, 1.0f);

		Vec.clear();

		for(int y = 0; y<128; y++)
		{
			for(int x = 0; x<128; x++)
			{
				float curve = 0;
				if(!FlatMode) curve = (cos((x-64)/45.0f) + cos((y-64)/45.0f)) * 48.0f;

				int ty, tx;
				if(y+PY < 128) ty = y+PY;
				else ty = y+PY-128;
				if(x+PX < 128) tx = x+PX;
				else tx = x+PX-128;
				//int ta = (128*ty)+tx;
				int ta = (128*ty)+tx;
				int tb = (128*y)+x;
				
				if(!FlatMode)
				{
					Vertex v(Verts[ta].r, Verts[ta].g, Verts[ta].b, Verts[tb].x, Verts[ta].y * HeightModifier + curve, Verts[tb].z);
					Vec.push_back(v);
				}
				else
				{
					Vertex v(Verts[ta].r, Verts[ta].g, Verts[ta].b, Verts[tb].x, Verts[ta].y * HeightModifier/2.0f + curve, Verts[tb].z);
					Vec.push_back(v);
				}
			}
		}

		if(GetViewPlayers())
		{
			glPointSize(7);
			pls.clear();
			for(unsigned int i=0; i<Players.size(); i++)
			{
				Vertex v = Players[i];
				float height = GetHeight(v.x, v.z);
				v.x = (v.x  - 0.5f) * 2.0f;
				v.z = (v.z  - 0.5f) * 2.0f;

				v.x += 64;
				v.z += 64;

				if(v.z-PY >= 0) v.z = v.z-PY;
				else v.z = v.z-PY+128;

				if(v.x-PX >= 0) v.x = v.x-PX;
				else v.x = v.x-PX+128;

				if(DrawOnlyPoints && !DrawOnlyLines) v.y = 0;
				float curve =  0;
				if(!FlatMode) curve =  (cos((v.x-64)/45.0f) + cos((v.z-64)/45.0f)) * 48.0f;
				
				if(!FlatMode) v.y = (v.y+height+0.2f) * HeightModifier + curve;
				else v.y = (v.y+height+0.2f) * HeightModifier/2 + curve;
				
				v.x -= 64;
				v.z -= 64;

				v.x = v.x / 2.0f + 0.5f;
				v.z = v.z / 2.0f + 0.5f;

				
				pls.push_back(v);
			}

			glInterleavedArrays(GL_C3F_V3F, 0, pls.data());
			if(!DrawOnlyLines)glDrawArrays(GL_POINTS, 0, pls.size());
			if(!DrawOnlyPoints)glDrawArrays(GL_LINES, 0, pls.size());
			glPointSize(1);
		}

		if(GetViewTrees())
		{
			glPointSize(5);
			pls.clear();
			for(unsigned int i=0; i<Trees.size(); i++)
			{
				Vertex v = Trees[i];
				float height = GetHeight(v.x, v.z);
				v.x = (v.x  - 0.5f) * 2.0f;
				v.z = (v.z  - 0.5f) * 2.0f;

				v.x += 64;
				v.z += 64;

				if(v.z-PY >= 0) v.z = v.z-PY;
				else v.z = v.z-PY+128;

				if(v.x-PX >= 0) v.x = v.x-PX;
				else v.x = v.x-PX+128;

				if(DrawOnlyPoints && !DrawOnlyLines) v.y = 0;
				float curve =  0;
				if(!FlatMode) curve =  (cos((v.x-64)/45.0f) + cos((v.z-64)/45.0f)) * 48.0f;
				if(!FlatMode) v.y = (v.y+height+0.2f) * HeightModifier + curve;
				else v.y = (v.y+height+0.2f) * HeightModifier/2 + curve;

				v.x -= 64;
				v.z -= 64;

				v.x = v.x / 2.0f + 0.5f;
				v.z = v.z / 2.0f + 0.5f;

				
				pls.push_back(v);
			}

			glInterleavedArrays(GL_C3F_V3F, 0, pls.data());
			if(!DrawOnlyLines)glDrawArrays(GL_POINTS, 0, pls.size());
			if(!DrawOnlyPoints)glDrawArrays(GL_LINES, 0, pls.size());
			glPointSize(1);
		}

		if(GetViewStone())
		{
			glPointSize(5);
			pls.clear();
			for(unsigned int i=0; i<StoneHeads.size(); i++)
			{
				Vertex v = StoneHeads[i];
				float height = GetHeight(v.x, v.z);
				v.x = (v.x  - 0.5f) * 2.0f;
				v.z = (v.z  - 0.5f) * 2.0f;

				v.x += 64;
				v.z += 64;

				if(v.z-PY >= 0) v.z = v.z-PY;
				else v.z = v.z-PY+128;

				if(v.x-PX >= 0) v.x = v.x-PX;
				else v.x = v.x-PX+128;

				if(DrawOnlyPoints && !DrawOnlyLines) v.y = 0;
				float curve =  0;
				if(!FlatMode) curve =  (cos((v.x-64)/45.0f) + cos((v.z-64)/45.0f)) * 48.0f;
				if(!FlatMode) v.y = (v.y+height+0.2f) * HeightModifier + curve;
				else v.y = (v.y+height+0.2f) * HeightModifier/2 + curve;

				v.x -= 64;
				v.z -= 64;

				v.x = v.x / 2.0f + 0.5f;
				v.z = v.z / 2.0f + 0.5f;

				
				pls.push_back(v);
			}

			glInterleavedArrays(GL_C3F_V3F, 0, pls.data());
			if(!DrawOnlyLines)glDrawArrays(GL_POINTS, 0, pls.size());
			if(!DrawOnlyPoints)glDrawArrays(GL_LINES, 0, pls.size());
			glPointSize(1);
		}

		glPolygonMode(GL_FRONT, FillMode);		
				
		glInterleavedArrays(GL_C3F_V3F, 0, Vec.data());
		glDrawElements(GL_TRIANGLES, 23622, GL_UNSIGNED_SHORT, Index.data());

		glInterleavedArrays(GL_C3F_V3F, 0, &Vec[3968]);
		glDrawElements(GL_TRIANGLES, 23622, GL_UNSIGNED_SHORT, Index.data());

		glInterleavedArrays(GL_C3F_V3F, 0, &Vec[7936]);
		glDrawElements(GL_TRIANGLES, 23622, GL_UNSIGNED_SHORT, Index.data());

		glInterleavedArrays(GL_C3F_V3F, 0, &Vec[11904]);
		glDrawElements(GL_TRIANGLES, 23622, GL_UNSIGNED_SHORT, Index.data());	

		Vec.clear();
		pls.clear();
	}
}