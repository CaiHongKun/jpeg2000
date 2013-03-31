#ifndef JP2_STRUCT
#define JP2_STRUCT
#include "jp2Component.h"
#include "jp2Image.h"
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
};
#endif