/*==========================================================================================================================================================

														�G�t�F�N�g����[effect.cpp]
															Author:���V����

============================================================================================================================================================*/
#ifndef _EFFECT_H
#define _EFFECT_H

#include "main.h"

//�v���g�^�C�v�錾
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife);

#endif