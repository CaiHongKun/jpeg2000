#ifndef J2K_TILE_CODER
#define J2K_TILE_CODER

#define S(i) data[numRes*(i)*2] //data�붼�����Ե������ǱߵĲ���
#define D(i) data[numRes*(1+(i)*2)]
#define S_(i) ((i)<0?S(0):((i)>=sn?S(sn-1):S(i)))
#define D_(i) ((i)<0?D(0):((i)>=dn?D(dn-1):D(i)))
/* new */
#define SS_(i) ((i)<0?S(0):((i)>=dn?S(dn-1):S(i)))
#define DD_(i) ((i)<0?D(0):((i)>=sn?D(sn-1):D(i)))

//��8λ���巽��
#define T1_SIG_NE 0x0001//North East
#define T1_SIG_SE 0x0002//South East
#define T1_SIG_SW 0x0004//South West
#define T1_SIG_NW 0x0008//North West
#define T1_SIG_N 0x0010//North
#define T1_SIG_E 0x0020//East
#define T1_SIG_S 0x0040//South
#define T1_SIG_W 0x0080//West

#define T1_SIG_OTH (T1_SIG_N|T1_SIG_NE|T1_SIG_E|T1_SIG_SE|T1_SIG_S|T1_SIG_SW|T1_SIG_W|T1_SIG_NW)//����ȫ����8������
#define T1_SIG_PRIM (T1_SIG_N|T1_SIG_E|T1_SIG_S|T1_SIG_W)//ֻ�����������������ҵ�

#define T1_SGN_N 0x0100
#define T1_SGN_E 0x0200
#define T1_SGN_S 0x0400
#define T1_SGN_W 0x0800
#define T1_SGN (T1_SGN_N|T1_SGN_E|T1_SGN_S|T1_SGN_W)

//����ÿ��ԭ�����ж��ٸ�������
#define T1_NUMCTXS_AGG 1
#define T1_NUMCTXS_ZC 9/* ZC */
#define T1_NUMCTXS_MAG 3 /* MR */
#define T1_NUMCTXS_SC 5 /* SC */
#define T1_NUMCTXS_UNI 1 /* RLC,UNIFORM������ */

#define T1_CTXNO_AGG 0 
#define T1_CTXNO_ZC (T1_CTXNO_AGG+T1_NUMCTXS_AGG)
#define T1_CTXNO_MAG (T1_CTXNO_ZC+T1_NUMCTXS_ZC)
#define T1_CTXNO_SC (T1_CTXNO_MAG+T1_NUMCTXS_MAG)//���������ı����ʼ=MAG��ʼ+����
#define T1_CTXNO_UNI (T1_CTXNO_SC+T1_NUMCTXS_SC)
#define T1_NUMCTXS (T1_CTXNO_UNI+T1_NUMCTXS_UNI)

#define T1_NMSEDEC_BITS 7
#define T1_NMSEDEC_FRACBITS (T1_NMSEDEC_BITS-1)

#define T1_SIG 0x1000
#define T1_REFINE 0x2000 //�����magnitude refinement ���Ƿ��Ѿ����
#define T1_VISIT 0x4000 //����Ƿ��Ѿ�����

#define T1_TYPE_MQ 0
#define T1_TYPE_RAW 1

#define T1_MAXCBLKW 1024
#define T1_MAXCBLKH 1024


#include <stdlib.h>
#include "j2kCustom.h"
#include "CodeParam.h"
#include "TileCodeParam.h"
#include "jp2Image.h"
#include "StepSize.h"
#include "j2kTag.h"
#include "jpegMath.h"
#include "j2kMQC.h"
#include "j2kDWT.h"
class j2kTileCoder_Pass
{
private:
	int rate;
	double distortiondec;
	int term,len;
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
	tcd_seg_t segs[100];		/* segments informations */
	unsigned char data[8192];	/* Data */
	int numpassesinlayers;	/* number of passes in the layer */
	tcd_layer_t layers[100];	/* layer information */
	int totalpasses;		/* ��¼������� */
	j2kTileCoder_Pass passes[100];;	/* information about the passes */
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
};

class j2kTileCoder_Tile
{
public:
	int x0,y0;
	int x1,y1;
	int numComponents;
	j2kTileCoder_Component *comps;
	double distotile;
	double distolayer[100];
};

class j2kTileCoder_Image
{
public:
	int tw,th;
	j2kTileCoder_Tile *tiles;
};

class j2kTileCoder
{
private:
	/*9/7�������˲�����������*/
	const int alpha=12993;//ԭ-1.58,12993/8192=1.58,��ͬ
	const int beta=434;
	const int gama=7233;
	const int sita=3633;
	const int K0=5038;
	const int K1=6659;//1/(6659/8192)=1.23,����K���ڳ���K�ĵ���

	int lastSizeOfB;
	int *widthData;


	/*---------TIER1--------*/
	//������
	int zcContextNo[1024];
	int scContextNo[256];
	int maContextNo[4096];
	int spbNo[256];

	int nmSeDecSig[1<<T1_NMSEDEC_BITS];
	int nmSeDecSig0[1<<T1_NMSEDEC_BITS];
	int nmSeDecRef[1<<T1_NMSEDEC_BITS];
	int nmSeDecRef0[1<<T1_NMSEDEC_BITS];

	int tierOneData[1024][1024];
	int tierOneFlags[1026][1026];

	/*----------MQC----------*/
	mqc_state_t *mqcCtxs[32];
	mqc_state_t **mqcCurCtx;/*��ǰ����������Ϣ*/
	unsigned int mqc_a;
	unsigned int mqc_c;
	unsigned int mqc_ct;
	unsigned int *mqc_bp;
	unsigned int *mqc_start;
private:

	j2kTileCoder_Image image;/* ��ǰtile����ͼ�� */
	
	jp2Image *img;/* ��ǰͼ������ */
	CodeParam *cp;

	j2kTileCoder_Tile *j2ktile;/*��ǰ��Ƭ����*/
	TileCodeParam *tcp;
	int currentTileNo;

	j2kTileCoder_Tile *tile;
	j2kTileCoder_Component *tilec;/*tile������Ϣ*/
	j2kTileCoder_Resolution *resolution;/*tile�ֱ���*/
	j2kTileCoder_Band *band;/*tile���Ӵ�*/
	j2kTileCoder_Precinct *precinct;/*tile����*/
	j2kTileCoder_CodeBlock *codeblock;/*tile�Ĵ����*/

public:
	j2kTileCoder(CodeParam *comp,jp2Image *image,int currentTile);

	void tcdMallocEncode();
	void tcdInitEncode();
	int tcdEncodeTilePxm(int tileno,unsigned char *dest,int len);/* ����������  */
private:
	
	void multiCompTransform(TileCodeParam *tcp,j2kTileCoder_Tile *tile);
	void multiEncodeReal(j2kTileCoder_Tile *tile);/* RGB ת YUV ,P292 */
	void multiEncode(j2kTileCoder_Tile *tile);/* RCT����,p293*/

	void discreteWaveletTransform(TileCodeParam *tcp,j2kTileCoder_Tile *tile);
	void dwtEncodeReal(j2kTileCoder_Tile *tilec);/* Forward 9/7 wavelet transform in 2-D */
	void dwtEncode(j2kTileCoder_Tile *tilec);
	void dwtEncodeLowReal(int *data,int resWidth,int numRes,int resLowWidth,int iv);/* 9/7��ɢС�����任 Forward Discrete Wavelet Transform*/
	void dwtEncodeLow(int *data,int resWidth,int numRes,int resLowWidth,int iv);/* 5/3 ��ɢС�����任 Forward  5/3 Wavelet Transform*/
	void dwtLazyTransform(int *data,int resWidth,int numRes,int resLowWidth,int iv);/* LAZY�ָ�*/
	void dwtClean();

	void tierOneInitContext();/* ��ʼ�������� */
	void tierEncodeCodeBlocks(j2kTileCoder_Tile *tile,TileCodeParam *tcp);
	void tierEncodeCodeBlock(j2kTileCoder_CodeBlock *codeBlock,j2kTileCoder_Tile *tile,int orient,int compno,int level,int isDWT,double stepSize,int codeBlockStyle,int numComps);
	int initCtxZC(int ctx,int bandno);
	int initCtxSC(int ctx);
	int initCtxMAG(int ctx);
	int initSPB(int ctx);
	int getCtxNoZC(int orient,int flag);
	int getCtxNoSC(int flag);
	int getSPB(int flag);

	void mqcResetStates();
	void mqcSetState(int ctxno,int msb,int prob);/* (�����ı��,�����ĵ�MSB��state,��ʶ����������state��symbols�Ŀ�����) */
	void mqcInitEncode(unsigned char *data);
	void mqcSegMarkEncode();/*ģʽ�仯 ,SEGMARK*/
	void mqcByPassInitEncode();/*ģʽ�仯 ,BYPASS*/
	void mqcByPassEncode(int d);/* BYPASS ģʽ�л�*/
	void mqcRestartInitEncode();/* ģʽ�仯 ,RESTART */
	void mqcErtermEncode();/*ģʽ�仯,ENTREM*/
	void mqcFlush();/* ����������� */
	void mqcSetBits();/* ���mqc_C*/
	void mqcSetCurCtx(int ctx);/*���õ�ǰ�����ı��*/
	int mqcNumBytes();/*��ȡ�Գ�ʼ����������/д�����ݳ��� */
	double getWmSeDec(int nmSeDec,int compno,int level,int orient,int bpsno,int isDWT,int stepSize,int numComps);
	void tierOneSignPass(int w,int h,int bpsno,int orient,int *nmSeDec ,char type,int cbstyle);/*��Ҫ�Ա������ */
	void tierOneSignPassStep(int *fp, int *dp, int orient, int bpno, int one,int *nmsedec, char type, int vsc);
	int tierOneGetNmSeDecSig(int x,int bitpos);
	void tierOneRefPass(int w,int h,int bpsno,int *nmSeDec,char type,int cbstyle);/*ϸ��������� */
	void tierOneRefPassStep(int *fp, int *dp, int orient, int bpno, int one,int *nmsedec, char type, int vsc);
	int tierOneGetNmSeDecRef(int x,int bitpos)
	void tierOneCleanPass(int w,int h,int bpsno,int orient,int *nmSeDec,int cbstyle);/*���������� */
	void tierOneClnPassStep(int *fp, int *dp, int orient, int bpsno, int one,int *nmsedec, int partial, int vsc);

	void tierOneUpdateFlags(int *fp,int s);

	void mqcEncode(int d);/*����һ��λsymbol*/
	void mqcCodeMPS();/*��most probales symbol���б���*/
	void mqcCodeLPS();/*��most least probales symbol���б��� */
	void mqcRenormalize();/* ��mqc_A������,ʹ��һ����0x8000~0x10000*/
	void mqcByteOut();/* ���һ���ֽ�,���Ҫ�����λ���,��0xff֮��,��һ���ֽ�һ����С��0x90*/
	int dwtGetGain(int orient);
};

#endif