/*==========================================================================================================================================================

														パーティクル処理[parthicle.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"

//パーティクル発生タイプ
typedef enum
{
	SET_PARTICLE_NORMAL = ZERO,		//普通のパーティクル(発生量　１)
	SET_PARTICLE_EXPLOSION,			//爆発のようなパーティクル(発生量　30)
	SET_PARTICLE_MAX
}SET_PARTICLE;

//プロトタイプ宣言
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