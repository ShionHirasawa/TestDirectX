/*==========================================================================================================================================================

														プレイヤーの処理[player.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _PLAYER_H_
#define _PLAYER_H_

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3		pos;					//位置
	D3DXVECTOR3		posOld;					//前回の位置
	D3DXVECTOR3		move;					//移動量
	D3DXVECTOR3		rot;					//向き
	D3DXMATRIX		mtxWorld;				//ワールドマトリックス
	int				nIdxShadow = MINUS_ONE;	//対象の影のインデックス(番号)
}Player;

void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);

void MovePlayer(void);
void FixPlayerMove(void);
void InertiaPlayerMove(void);	// Inertia は「慣性」という意味
void FixPlayerPos(void);
void RotPlayer(void);
void ResetPlayer(void);

#endif 