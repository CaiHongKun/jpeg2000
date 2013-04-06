#ifndef J2K_TILE_CODER
#define J2K_TILE_CODER

#define S(i) data[numRes*(i)*2] //data�붼�����Ե������ǱߵĲ���
#define D(i) data[numRes*(1+(i)*2)]
#define S_(i) ((i)<0?S(0):((i)>=sn?S(sn-1):S(i)))
#define D_(i) ((i)<0?D(0):((i)>=dn?D(dn-1):D(i)))
/* new */
#define SS_(i) ((i)<0?S(0):((i)>=dn?S(dn-1):S(i)))
#define DD_(i) ((i)<0?D(0):((i)>=sn?D(sn-1):D(i)))

#include <malloc.h>
#include <iostream>
#include "j2kTileCoder_Custom.h"
#include "StepSize.h"
#include "j2kTier1.h"
#include "j2kTier2.h"

class j2kTileCoder
{
private:
	/*9/7�������˲�����������*/
	static const int alpha=12993;//ԭ-1.58,12993/8192=1.58,��ͬ
	static const int beta=434;
	static const int gama=7233;
	static const int sita=3633;
	static const int K0=5038;
	static const int K1=6659;//1/(6659/8192)=1.23,����K���ڳ���K�ĵ���

	static const int K=1;//Ӧ��ΪDOUBLE

	int lastSizeOfB;
	int *widthData;

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
	void dwtEncodeReal(j2kTileCoder_Component *tilec);/* Forward 9/7 wavelet transform in 2-D */
	void dwtEncode(j2kTileCoder_Component *tilec);
	void dwtEncodeLowReal(int *data,int resWidth,int numRes,int resLowWidth,int iv);/* 9/7��ɢС�����任 Forward Discrete Wavelet Transform*/
	void dwtEncodeLow(int *data,int resWidth,int numRes,int resLowWidth,int iv);/* 5/3 ��ɢС�����任 Forward  5/3 Wavelet Transform*/
	void dwtLazyTransform(int *data,int resWidth,int numRes,int resLowWidth,int iv);/* LAZY�ָ�*/
	void dwtClean();

	void makeLayerFixed(int layerno,int final);
	void makeLayer(int layerno,double thresh,int final);
	void rateAllocate(unsigned char *dest,int len,j2kTierTwo *tierTwo);
	void rateAllocateFixed();

	int dwtGetGain(int orient);
};

#endif