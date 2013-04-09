#include "jp2Writer.h"
#include "charInputOutput.h"

int jp2Writer::encode(jp2Struct *jp2_struct,CodeParam *cp,char *outbuf)
{
	charInputOutput::init((unsigned char*)outbuf,cp->XTsiz*cp->YTsiz*cp->tw*cp->th*10);//��ʼ��

	writeSignature();/*��д��signature��buffer */
	writeFileType(jp2_struct);/* д�� FILE_TYPE��buffer */
	writeHeader(jp2_struct);/* д��JP2 HEADER��buffer */
	int len = writeContent(jp2_struct->image, cp, outbuf);
	return charInputOutput::getPosition();
}

void jp2Writer::writeSignature()
{
	jp2Box box;
	box.init_pos = charInputOutput::getPosition();//��ȡ��ǰ�ֽ������
	charInputOutput::skipBytes(4);//����signature ���ȿ�
	charInputOutput::writeBytes(JP2_JP, 4);		// д��'jp'
	charInputOutput::writeBytes(0x0d0a870a, 4);//signature��C������

	box.length = charInputOutput::getPosition() - box.init_pos;//�����ȡ�ĳ���
	charInputOutput::setPosition(box.init_pos);//�ص���ʼ 
	charInputOutput::writeBytes(box.length, 4);	/*    д��򳤶�       */
	charInputOutput::setPosition(box.init_pos + box.length);

}

void jp2Writer::writeFileType(jp2Struct *jp2_struct)
{
	unsigned int i;
	jp2Box box;

	//ǰ���Ѿ�������file type ��ǰ
	box.init_pos = charInputOutput::getPosition();
	charInputOutput::skipBytes(4);//�������ȿ�
	charInputOutput::writeBytes(JP2_FTYP, 4);	/* FTYP       */

	charInputOutput::writeBytes(jp2_struct->brand, 4);	/* BR   ,���������õľ����ļ���ʽ      */
	charInputOutput::writeBytes(jp2_struct->minversion, 4);	/* MinV,�����̱���С�汾��       */

	for (i = 0; i < jp2_struct->numcl; i++)
		charInputOutput::writeBytes(jp2_struct->cl[i], 4);	/* CL ,�����б�          */

	box.length = charInputOutput::getPosition() - box.init_pos;//���㳤��
	charInputOutput::setPosition(box.init_pos);
	charInputOutput::writeBytes(box.length, 4);	/*    L       */
	charInputOutput::setPosition(box.init_pos + box.length);
}

void jp2Writer::writeHeader(jp2Struct *jp2_struct)
{
	jp2Box box;

	box.init_pos =  charInputOutput::getPosition();
	charInputOutput::skipBytes(4);
	charInputOutput::writeBytes(JP2_JP2H, 4);	/* JP2H */

	writeIHDR(jp2_struct);

	if (jp2_struct->bpc == 255)
		writeBPCC(jp2_struct);//д��bits per component��

	writeColor(jp2_struct);//д�� COLOR SPECIFICATION �� 

	box.length = charInputOutput::getPosition() - box.init_pos;
	charInputOutput::setPosition(box.init_pos);
	charInputOutput::writeBytes(box.length, 4);	/*    L       */
	charInputOutput::setPosition(box.init_pos + box.length);
}

void jp2Writer::writeIHDR(jp2Struct * jp2_struct)
{
	jp2Box box;

	box.init_pos = charInputOutput::getPosition();
	charInputOutput::skipBytes(4);
	charInputOutput::writeBytes(JP2_IHDR, 4);	// IHDR,Image Header

	charInputOutput::writeBytes(jp2_struct->height, 4);	// HEIGHT,ͼ�������еĸ߶�
	charInputOutput::writeBytes(jp2_struct->width, 4);	// WIDTH,ͼ�������еĿ��
	charInputOutput::writeBytes(jp2_struct->numComponents, 2);	// NC

	charInputOutput::writeBytes(jp2_struct->bpc, 1);	// BPC  

	charInputOutput::writeBytes(jp2_struct->C, 1);	// C : Always 7
	charInputOutput::writeBytes(jp2_struct->UnkC, 1);	// UnkC, colorspace unknow
	charInputOutput::writeBytes(jp2_struct->IPR, 1);	// IPR, no intellectual property

	box.length = charInputOutput::getPosition() - box.init_pos;
	charInputOutput::setPosition(box.init_pos);
	charInputOutput::writeBytes(box.length, 4);	/*    L       */
	charInputOutput::setPosition(box.init_pos + box.length);
}

void jp2Writer::writeBPCC(jp2Struct * jp2_struct)
{
	unsigned int i;
	jp2Box box;

	box.init_pos = charInputOutput::getPosition();
	charInputOutput::skipBytes(4);
	charInputOutput::writeBytes(JP2_BPCC, 4);	// BPCC

	for (i = 0; i < jp2_struct->numComponents; i++)
		charInputOutput::writeBytes(jp2_struct->comps[i].bpcc, 1);//���ÿһ��ͼ�������λ���

	box.length = charInputOutput::getPosition() - box.init_pos;
	charInputOutput::setPosition(box.init_pos);
	charInputOutput::writeBytes(box.length, 4);	/*    L       */
	charInputOutput::setPosition(box.init_pos + box.length);
}

void jp2Writer::writeColor(jp2Struct * jp2_struct)
{
	jp2Box box;

	box.init_pos = charInputOutput::getPosition();
	charInputOutput::skipBytes(4);
	charInputOutput::writeBytes(JP2_COLR, 4);	// COLR

	charInputOutput::writeBytes(jp2_struct->meth, 1);	// METH,M����
	charInputOutput::writeBytes(jp2_struct->precedence, 1);	// PRECEDENCE
	charInputOutput::writeBytes(jp2_struct->approx, 1);	// APPROX

	if (jp2_struct->meth == 1)
		charInputOutput::writeBytes(jp2_struct->ECS, 4);	// EnumCS,ECS
	else
		charInputOutput::writeBytes(0, 1);		// PROFILE (??)

	box.length = charInputOutput::getPosition() - box.init_pos;
	charInputOutput::setPosition(box.init_pos);
	charInputOutput::writeBytes(box.length, 4);	/*    L       */
	charInputOutput::setPosition(box.init_pos + box.length);
}

int jp2Writer::writeContent(jp2Image *img,CodeParam *cp,char *buffer)
{
	int len;
	jp2Box box;

	box.init_pos = charInputOutput::getPosition();
	charInputOutput::skipBytes(4);
	charInputOutput::writeBytes(JP2_JP2C, 4);	// JP2C

	j2kCoder *coder=new j2kCoder();
	len = coder->j2kEncode(
		img, 
		cp, 
		buffer,
		cp->XTsiz * cp->XTsiz * cp->th * cp->tw * 2 /* (tile��С*tile����*2) */
		);//���ݳ���,��λbyte

	box.length = charInputOutput::getPosition() - box.init_pos;
	charInputOutput::setPosition(box.init_pos);
	charInputOutput::writeBytes(box.length, 4);	/*    L       */
	charInputOutput::setPosition(box.init_pos + box.length);
	return box.length;
}