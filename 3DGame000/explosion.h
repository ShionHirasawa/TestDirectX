/*==========================================================================================================================================================

														��������[explosion.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _EXPLOSION_H
#define _EXPLOSION_H

#include "main.h"

//�v���g�^�C�v�錾
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col);

#endif