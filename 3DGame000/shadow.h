/*==========================================================================================================================================================

															âeÇÃèàóù[shadow.h]
															Author:ïΩ‡Véçâë

============================================================================================================================================================*/
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos);
void BreakShadow(int nIdxShadow);

#endif 