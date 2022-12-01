/*==========================================================================================================================================================

															弾処理[bullet.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _BULLET_H
#define _BULLET_H

#include "main.h"

//エフェクトのα値に関するマクロ定義
#define BULLETEFFECT_PLAYER			(0.5f)
#define BULLETEFFECT_ENEMY			(0.6f)
#define BULLETEFFECT_FAST_ENEMY		(0.7f)

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 posOld;				//前回の位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 rot;				//向き
	D3DXCOLOR	col;				//色
	D3DXMATRIX	mtxWorld;			//ワールドマトリックス
	int			nLife;				//寿命
	float		EffectAlphaColor;	//弾の軌跡のα値を指定する変数
	int			nIdxShadow;			//対象の影のインデックス(番号)
	bool		bUse;				//使用しているかどうか
}Bullet;

//プロトタイプ宣言
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float Alpha);

#endif