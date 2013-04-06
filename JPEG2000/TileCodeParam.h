/************************************************************************/
/* ������ű���ͽ���ͨ�ò��������е�tiles                                                                     */
/************************************************************************/

#ifndef J2K_TILE_CODE_PARAM
#define J2K_TILE_CODE_PARAM
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
	int distoratioAlloc;
	int fixedQuality;

	TileCompCodeParam *tccps;
	j2kPPT *ppt;
	j2kPOC pocs[32];
	int pocUse;/*���POC MARKER �Ƿ��Ѿ�ʹ��*/
};
#endif