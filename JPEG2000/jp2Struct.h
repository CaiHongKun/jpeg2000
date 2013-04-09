#ifndef JP2_STRUCT
#define JP2_STRUCT
#include <malloc.h>
#include "jp2Component.h"
#include "jp2Image.h"

//��׼���ʶ
#define JPIP_JPIP 0x6a706970
#define JP2_JP   0x6a502020
#define JP2_FTYP 0x66747970
#define JP2_JP2H 0x6a703268
#define JP2_IHDR 0x69686472
#define JP2_COLR 0x636f6c72
#define JP2_JP2C 0x6a703263
#define JP2_URL  0x75726c20
#define JP2_DBTL 0x6474626c
#define JP2_BPCC 0x62706363
#define JP2_JP2  0x6a703220

/************************************************************************/
/* JP2 ��ṹ                                                                     */
/************************************************************************/
class jp2Struct
{
public:  
  unsigned int width;
  unsigned int height;
  unsigned int numComponents;//ͼ���������
  unsigned int bpc;//bits per component�� ,����,��ʾλ���
  unsigned int C;//image header ���е�CT��ʾѹ������,Ωһ�Ϸ�ֵ��7,
  unsigned int UnkC;
  unsigned int IPR;//��ѡ,֪ʶ��Ȩ��
  unsigned int meth;//header>image header���е�COLOR specification���е�M����,M=1:��ʾ��ɫ�ռ�ͨ�� ECS(ö�ٲ�ɫ�ռ�)����,M=2:��ʾ��ɫ�ռ�ͨ��ICP���� (ECS��ICPֻ����һ)
  unsigned int approx;
  unsigned int ECS;//ECS,ö�ٲ�ɫ�ռ�
  unsigned int precedence;//���ȼ�
  unsigned int brand;//file type���е�BR��,���������õľ����ļ���ʽ
  unsigned int minversion;//file type���е�MV��,�����̱���С�汾��
  unsigned int numcl;//FILE TYPE �� ��ָ���ļ������ϵı�׼�����б�,�˴��Ǽ��ݵ���Ŀ
  unsigned int *cl;//������ݵĲ���

  jp2Component *comps;
  jp2Image *image;

public:
	int jp2StructInit(jp2Image *img);
};
#endif