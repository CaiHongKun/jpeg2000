#ifndef COMPONENT
#define COMPONENT
#include "TileCodeParam.h"

class Component
{

public:
	int XRsiz;
	int YRsiz;

	//data ��С 
	int width;
	int height;
	//���������ͼ���ƫ��
	int x0,y0;

	int precision;

	int bpp;/*ͼ�����*/

	int sgnd;

};

#endif