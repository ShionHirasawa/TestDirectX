/*==========================================================================================================================================================

													フィールド(地面)処理[field.h]
															Author:平澤詩苑

============================================================================================================================================================*/
#ifndef _FIELD_H_
#define _FIELD_H_

typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX	mtxWorld;				//ワールドマトリックス
}Field;

void InitField(void);
void UninitField(void);
void UpdateField(void);
void DrawField(void);

#endif 