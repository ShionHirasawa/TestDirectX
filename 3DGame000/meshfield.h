/*==========================================================================================================================================================

													メッシュフィールド処理[meshfield.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _MESHFIELD_H
#define _MESHFIELD_H

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX	mtxWorld;				//ワールドマトリックス
}MeshField;

//プロトタイプ宣言
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(void);

#endif