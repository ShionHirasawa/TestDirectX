/*==========================================================================================================================================================

														�p�[�e�B�N������[parthicle.h]
															Author:���V����

============================================================================================================================================================*/
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//�p�[�e�B�N�������^�C�v
typedef enum
{
	SET_PARTICLE_NORMAL = ZERO,		//���ʂ̃p�[�e�B�N��(�����ʁ@�P)
	SET_PARTICLE_EXPLOSION,			//�����̂悤�ȃp�[�e�B�N��(�����ʁ@30)
	SET_PARTICLE_MAX
}SET_PARTICLE;

//�v���g�^�C�v�錾
void InitParticle(void);
void UninitParticle(void);
void UpdateParticle(void);
void DrawParticle(void);

void SetParticleRadius(int nCntParticle);

void SetParticle(D3DXVECTOR3 pos,SET_PARTICLE nType);
void SetParticleNor(D3DXVECTOR3 pos);
void SetParticleExplosion(D3DXVECTOR3 pos);

void BoundParticle(int nCntParticle);

#endif