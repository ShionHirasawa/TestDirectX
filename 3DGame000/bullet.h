/*==========================================================================================================================================================

															�e����[bullet.cpp]
															Author:���V����

============================================================================================================================================================*/
#ifndef _BULLET_H
#define _BULLET_H

#include "main.h"

//�G�t�F�N�g�̃��l�Ɋւ���}�N����`
#define BULLETEFFECT_PLAYER			(0.5f)
#define BULLETEFFECT_ENEMY			(0.6f)
#define BULLETEFFECT_FAST_ENEMY		(0.7f)

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 posOld;				//�O��̈ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 rot;				//����
	D3DXCOLOR	col;				//�F
	D3DXMATRIX	mtxWorld;			//���[���h�}�g���b�N�X
	int			nLife;				//����
	float		EffectAlphaColor;	//�e�̋O�Ղ̃��l���w�肷��ϐ�
	int			nIdxShadow;			//�Ώۂ̉e�̃C���f�b�N�X(�ԍ�)
	bool		bUse;				//�g�p���Ă��邩�ǂ���
}Bullet;

//�v���g�^�C�v�錾
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float Alpha);

#endif