#ifndef CWIRECUBE_H
#define CWIRECUBE_H
#include "../header/Angel.h"
#include "CShape.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

#define WIRECUBE_NUM 24  // 6 faces, 4 vertices/face

class CWireCube : public CShape
{
private:
	vec4 _vertices[8];
	int  _iIndex;

	void Quad( int a, int b, int c, int d );
public:
	CWireCube();
	~CWireCube(){};

	void update(float dt, point4 vLightPos, color4 vLightI);
	void update(float dt, const LightSource &Lights);
	void update(float dt); // ���p��������ө�
	void draw();
	void drawW();

	// �C�⪺�p������浹�����O CShape �B�z�A�����H�T���������i��p��
	//void renderWithFlatShading(point4 vLightPos, color4 vLightI);
	//void renderWithGouraudShading(point4 vLightPos, color4 vLightI);
};

#endif