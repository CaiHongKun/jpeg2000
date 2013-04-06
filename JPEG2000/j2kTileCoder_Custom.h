#ifndef J2K_TILE_CODER_CUSTOM
#define J2K_TILE_CODER_CUSTOM
#include "j2kTag.h"
#include "j2kMQState.h"
class j2kTileCoder_Pass
{
public:
	int rate;
	double distortiondec;
	int term,len;
};

class j2kTileCoder_Layer
{
public:
	int numpasses;/*��ǰ�����Ĵ���;*/
	int len;/* ��Ϣ����*/
	int disto;
	unsigned char *data;
};
/************************************************************************/
/* �������Ϣ����                                                                      */
/************************************************************************/
class j2kTileCoder_CodeBlock
{
public:
	int x0,y0;
	int x1,y1;
	int numbps;

	int lastbp;			/* Add antonin : quantizbug1 */
	int numlenbits;
	int len;			/* length */
	int numpasses;		/* number of pass already done for the code-blocks */
	int numnewpasses;		/* number of pass added to the code-blocks */
	int numsegs;			/* number of segments */
	//tcd_seg_t segs[100];		/* segments informations */
	unsigned char data[8192];	/* Data */
	int numpassesinlayers;	/* number of passes in the layer */
	j2kTileCoder_Layer layers[100];	/* layer information */
	int totalpasses;		/* ��¼������� */
	j2kTileCoder_Pass passes[100];	/* information about the passes */
};


class j2kTileCoder_Precinct
{
public:
	int x0,y0;
	int x1,y1;
	int codeBlockNumInWidth;
	int codeBlockNumInHeight;
	int width,height;
	j2kTileCoder_CodeBlock *codeBlockInfo;
	j2kTagTree *inclusionTree;
	j2kTagTree *imsbTree;
};

/************************************************************************/
/* �Ӵ���Ϣ����                                                                     */
/************************************************************************/
class j2kTileCoder_Band
{
public:
	int x0,y0;
	int x1,y1;
	int bandno;/*�Ӵ����*/
	j2kTileCoder_Precinct *precincts;
	int numbps;
	int stepSize;
};

/************************************************************************/
/* �ֱ�����Ϣ����                                                                      */
/************************************************************************/
class j2kTileCoder_Resolution
{
public:
	int x0,y0;
	int x1,y1;
	int precinctWidth;/*�˷ֱ�����ˮƽ���ж��ٸ����� */
	int precinctHeight;/*�˷ֱ����´�ֱ���ж��ٸ����� */
	int numBands;/* ����ڴ˷ֱ������ж��ٸ��Ӵ� */
	j2kTileCoder_Band bands[3];
};

/************************************************************************/
/* tile������Ϣ����                                                                      */
/************************************************************************/
class j2kTileCoder_Component
{
public:
	int x0,y0;
	int x1,y1;
	int numResolutions;
	j2kTileCoder_Resolution *resolutions;
	int *data;
	int fixedQuality;
};

class j2kTileCoder_Tile
{
public:
	int x0,y0;
	int x1,y1;
	int numComponents;
	j2kTileCoder_Component *comps;
	int fixedQuality;
	double distotile;
	double distolayer[100];
};

class j2kTileCoder_Image
{
public:
	int tw,th;
	j2kTileCoder_Tile *tiles;
};

#endif