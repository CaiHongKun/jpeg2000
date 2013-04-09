#include "jp2Struct.h"

int jp2Struct::jp2StructInit(jp2Image *img)
{
	int depth_0, sign, depth, i;

	height = img->Ysiz - img->YOsiz;	// HEIGHT
	width = img->Xsiz - img->XOsiz;	// WIDTH
	numComponents = img->numComponents;	// NC
	comps = (jp2Component *) malloc(numComponents * sizeof(jp2Component));

	depth_0 = img->comps[0].precision - 1;//ͼ������һ�������ķ�����
	sign = img->comps[0].sgnd;
	bpc = depth_0 + (sign << 7);

	for (i = 1; i < img->numComponents; i++) {
		depth = img->comps[i].precision - 1;//��ȡ���������ķ�����
		sign = img->comps[i].sgnd;
		if (depth_0 != depth)//�������λ�����һ��������λ���
			bpc = 255;// ����ÿ��ͼ�����λ����Ϊ255
	}

	C = 7;		// C : Always 7
	UnkC = 0;		// UnkC, colorspace specified in colr box
	IPR = 0;		// IPR, no intellectual property,֪ʶ��Ȩ

	for (i = 0; i < img->numComponents; i++)
		comps[i].bpcc =
		img->comps[i].precision - 1 + (img->comps[i].sgnd << 7);

	precedence = 0;	// PRECEDENCE,���ȼ� 
	approx = 0;	// APPROX

	if ((img->numComponents== 1 || img->numComponents == 3)//�Ҷ�ͼ���� RGBͼ
		&& (bpc != 255))//�Լ�λ�����255
		meth = 1;//ECS
	else
		meth = 2;//ICP

	if (meth == 1) {
		if (img->colorSpace == 1)
			//�����ͨ��ECS������ɫ��ΪsRGB
			ECS = 16;
		else if (img->colorSpace == 2)
			//��ɫ�ռ�
			ECS = 17;
		else if (img->colorSpace == 3)
			ECS = 18;	// YUV                          
	} else
		ECS = 0;	// PROFILE (??)

	brand = JP2_JP2;	/* BR         */
	minversion = 0;	/* MinV       */

	numcl = 1;//�����б�
	cl = (unsigned int *) malloc(numcl * sizeof(unsigned int));
	cl[0] = JP2_JP2;	/* CL0 : JP2  */
	return 0;
}