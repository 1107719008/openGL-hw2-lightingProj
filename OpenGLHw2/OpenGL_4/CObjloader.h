#ifndef COBJLOADER_H
#define COBJLOADER_H
#include "Header/Angel.h"
#include "Common/CShape.h"
#include <vector>
#include <iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<algorithm>

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define OBJLOADER_NUM 36  // 6 faces, 2 triangles/face , 3 vertices/triangle

class CObjloader : public CShape
{
private:
	vec4 _vertices[8];
	int  _iIndex;

	void Quad( int a, int b, int c, int d );
public:
	CObjloader(const char* );
	~CObjloader(){};

	void LoadObj(const char * );

	void update(float dt, point4 vLightPos, color4 vLightI);
	void update(float dt, const LightSource &Lights);
	void update(float dt, const LightSource& Lights, const LightSource& Lights2, const LightSource& Lights3, const LightSource& Lights4);
	void update(float dt); // ���p��������ө�

	void draw();
	void drawW();

	// �C�⪺�p������浹�����O CShape �B�z�A�����H�T���������i��p��
	//void renderWithFlatShading(point4 vLightPos, color4 vLightI);
	//void renderWithGouraudShading(point4 vLightPos, color4 vLightI);
	//void renderWithFlatShading(const LightSource &Lights);
	//void renderWithGouraudShading(const LightSource &Lights);
};

#endif