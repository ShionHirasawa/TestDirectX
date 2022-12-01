/*==========================================================================================================================================================

																色のマクロ定義[color.h]
																	Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

#define COLOR_MIN	(0)		//RGBACOLORの最小数
#define COLOR_MAX	(255)	//RGBACOLORの最大数

//D3DXCOLOR_RGBA のマクロ
#define RGBA_WHITE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX)	//白
#define RGBA_BLACK		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MIN, COLOR_MAX)	//黒

//D3DXCOLOR のマクロ
#define XCOL_WHITE		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	//白
#define XCOL_BLACK		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	//黒
#define XCOL_YELLOW		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)	//黄色

#define PARTICLE_COL	D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f)	//パーティクルの色

#endif