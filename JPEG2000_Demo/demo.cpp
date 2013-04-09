// JPEG2000_Demo.cpp : main project file.
#include "stdafx.h"
#include "preProcessor.h"
#include <time.h>

using namespace std;

int main(int argc, char **argv)
{
	CodeParam cp;
	TileCodeParam tcp_init;
	TileCompCodeParam tccp_init;
	j2kPOC *tcp_poc;
	jp2Image img;

	PreProcessor *preProcessor=new PreProcessor();

	int subSamplingX=1;
	int subSamplingY=1;//�������ݴ�С 

	char *inFile="3.bmp";
	char *outFile="1.jp2";

	//��ʼ��
	cp.imageType=2;//bmp
	cp.format=1;//jp2
	cp.comment="Created by demonk";
	
	cp.fixed_alloc = 0;
	cp.fixed_quality = 0;		//add fixed_quality,use rate,not distoratio
	cp.isIntermedFile=0;
	cp.XTsiz=200;
	cp.YTsiz=400;//�����ϵ��ͼ��ԭ�ֱ���,Ҫƥ��,�������
	cp.XTOsiz=0;
	cp.YTOsiz=0;//�Ȳ�ƫ��

	tcp_init.numLayers=1;
	tcp_init.rates[0]=1;//��һ��һ������,ȡֵ����1
// 	tcp_init.rates[1]=1;
// 	tcp_init.rates[2]=77;//��Ӧ����
	cp.disto_alloc = 1;//����������

	tcp_init.progressionOrder=1;/* -1~4 */
	tcp_init.codingStyle=0;
	tcp_init.numPocs=0;

	tccp_init.numResolutions=6;
	tccp_init.codeBlockWidth=64;
	tccp_init.codeBlockHeight=64;
	tccp_init.isROI=0;/*���˾͸�����*/
	tccp_init.codingStyle=0;
	tccp_init.isReversibleDWT=1;
	tccp_init.quantisationStyle=0;
	tccp_init.codeBlockStyle=0;
	tccp_init.numGuardBits=2;

	bmpReader::bmpToImage(inFile,&img,subSamplingX,subSamplingY);

	cp.tw=int_ceildiv(img.Xsiz-cp.XTOsiz,cp.XTsiz);
	cp.th=int_ceildiv(img.Ysiz-cp.YTOsiz,cp.YTsiz);

	cp.tcps=(TileCodeParam*)malloc(cp.tw*cp.th*sizeof(TileCodeParam));
	cp.tcps->tccps=(TileCompCodeParam*)malloc(img.numComponents*sizeof(TileCompCodeParam));

	int CodingStyle=0;

	for(int tileno=0;tileno<cp.th*cp.tw;tileno++)
	{
		TileCodeParam *tcp =&cp.tcps[tileno];
		tcp->numLayers=tcp_init.numLayers;

		for(int j=0;j<tcp_init.numLayers;j++)
		{
			if(cp.fixed_quality)
			{
				tcp->distoratio[j]=tcp_init.distoratio[j];
			}else{
				tcp->rates[j]=tcp_init.rates[j];
			}
		}

		tcp->codingStyle=CodingStyle;
		tcp->progressionOrder=preProcessor->calProgression("LRCP");//Ĭ�ϵĽ���
		tcp->isMCT=img.numComponents==3?1:0;
		tcp->numLayers=1 ;


		//POC��ѡ
		int numPocs=0;//�Ȳ���POC
		tcp->pocUse=0;//��Ǵ�POC����

		for(int i=0;i<numPocs;i++)
		{
			// T1=0,0,1,5,3,CPRL
			j2kPOC *poc=&tcp->pocs[i];

			poc->resolutionStart=0;
			poc->componentStart=0;
			poc->layerEnd=1;
			poc->resolutionEnd=5;//��ʾֻ��0~3�ֱ��ʲ���е���
			poc->componentEnd=3;
			poc->progressionOrder=preProcessor->calProgression("LRCP");
			poc->tile=1;
		}

		tcp->numPocs=numPocs;

		tcp->tccps=(TileCompCodeParam*)malloc(img.numComponents*sizeof(TileCompCodeParam));
		for(int i=0;i<img.numComponents;i++)
		{
			TileCompCodeParam *tccp=&tcp->tccps[i];

			tccp->codingStyle=CodingStyle&0x01;
			tccp->numResolutions=tccp_init.numResolutions;
			tccp->codeBlockWidth=int_floorlog2(tccp_init.codeBlockWidth);
			tccp->codeBlockHeight=int_floorlog2(tccp_init.codeBlockHeight);

			tccp->codeBlockStyle=tccp_init.codeBlockStyle;//ģʽ��Ͽ���
			tccp->isReversibleDWT=tccp_init.isReversibleDWT;//����
			tccp->quantisationStyle=tccp_init.quantisationStyle;//����ģʽ
			tccp->numGuardBits=tccp_init.numGuardBits;

			if(i%2==0)
			tccp->isROI=tccp_init.isROI;//�Ǹ���Ȥ
			else
				tccp->isROI=0;
			for(int j=0;j<tccp->numResolutions;j++)
			{
				tccp->precinctWidth[j]=15;
				tccp->precinctHeight[j]=15;
			}

		preProcessor->calStepSizes(tccp,img.comps[i].precision);//�����Ӵ���������
		}
	}

	jp2Struct *jp2struct=new jp2Struct();
	jp2struct->image=&img;
	jp2struct->jp2StructInit(&img);//��ʼ����ṹ

// 	if(cp.isIntermedFile==1)
// 		cp.isIntermedFile=0;

	char *outbuf=(char*)malloc(cp.XTsiz*cp.YTsiz*cp.tw*cp.th*10*sizeof(char));
	
	jp2Writer *write=new jp2Writer();

	clock_t startTime=clock();
	int len=write->encode(jp2struct,&cp,outbuf);
	clock_t endTime=clock();

	int second=(endTime-startTime)/1000;
	int microsecond=(endTime-startTime)%1000;
	cout<<"Time Consuming:"<<second<<"."<<microsecond<<"s"<<endl;

	if(len>0)
	{
		FILE* file=fopen(outFile,"wb");
		if(file)
		{
			fwrite(outbuf,1,len,file);
			free(outbuf);
			fclose(file);
		}
	}

	free(img.comps);
	free(cp.tcps);
	delete jp2struct;
	delete preProcessor;

	system("pause");
}


