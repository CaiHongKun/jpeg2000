/************************************************************************/
/* ������ű���ͽ���ͨ�ò��������е�tiles                                                                     */
/************************************************************************/

#ifndef TCP
#define TCP
#include "TileCompCodeParam.h"
#include "PPT.h"
#include "POC.h"
class TileCodeParam
{
public:
	int codingStyle;
	int progressionOrder;
	int numLayers;
	int numPocs;
	bool isMCT;
	int rates[100];

	float distoratio[100];/* ÿһ���ʧ���� */

	TileCompCodeParam *tccps;
	PPT *ppt;
	POC *pocs;
};
#endif