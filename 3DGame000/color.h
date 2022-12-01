/*==========================================================================================================================================================

																�F�̃}�N����`[color.h]
																	Author:���V����

============================================================================================================================================================*/
#ifndef _COLOR_H_
#define _COLOR_H_

#include "main.h"

#define COLOR_MIN	(0)		//RGBACOLOR�̍ŏ���
#define COLOR_MAX	(255)	//RGBACOLOR�̍ő吔

//D3DXCOLOR_RGBA �̃}�N��
#define RGBA_WHITE		D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, COLOR_MAX)	//��
#define RGBA_BLACK		D3DCOLOR_RGBA(COLOR_MIN, COLOR_MIN, COLOR_MIN, COLOR_MAX)	//��

//D3DXCOLOR �̃}�N��
#define XCOL_WHITE		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)	//��
#define XCOL_BLACK		D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)	//��
#define XCOL_YELLOW		D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f)	//���F

#define PARTICLE_COL	D3DXCOLOR(1.0f, 1.0f, 0.5f, 0.5f)	//�p�[�e�B�N���̐F

#endif