/*==========================================================================================================================================================

															壁の処理[wall.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _WALL_H_
#define _WALL_H_

//壁の情報構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	float			fWidth;		//幅
	float			fHeight;	//高さ
	float			fDepth;		//奥行き
}Wall;

void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);
bool CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld);
void RotWall(void);
void FixRotWall(void);
void ResetRotWall(void);
Wall *GetWall(void);

#endif 