#ifndef J2K_CODE_PARAM
#define J2K_CODE_PARAM
#include "PPM.h"
#include "TileCodeParam.h"

class CodeParam
	{
	public:
		int format;
		bool isIntermedFile;/* �Ƿ�ֿ������ļ��� */
		int imageType;
		int XTOsiz;
		int YTOsiz;
		int XTsiz;
		int YTsiz;

		int numTileWidth;
		int numTileHeight;

								char *comment;
		int *tileNo;/*tile�ı�ʶ*/
		int tileNoSize;/* tile������*/

int disto_alloc;		/* Allocation by rate/distortion     */
  int fixed_alloc;		/* Allocation by fixed layer         */
  int fixed_quality;		/* add fixed_quality,PSNRֵ */

  int tw;
		int th;
  PPM *ppm;
  TileCodeParam *tcps;


	};
#endif
