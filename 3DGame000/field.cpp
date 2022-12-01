/*==========================================================================================================================================================

													フィールド(地面)処理[field.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "color.h"
#include "field.h"

#define FIELD_POS		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define FIELD_WIDTH		(600.0f)	//幅サイズ
#define FIELD_DEPTH		(600.0f)	//奥行きサイズ

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;		//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureField = NULL;		//テクスチャ
Field					g_Field;					//フィールドの情報

//フィールドの初期化処理
void InitField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	g_Field.pos = FIELD_POS;	//位置初期化
	g_Field.rot = ZERO_SET;		//向き初期化

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/field001.jpg", &g_pTextureField);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffField, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点座標の設定
	{
		pVtx[ZERO].pos = D3DXVECTOR3(g_Field.pos.x - FIELD_WIDTH, g_Field.pos.y, g_Field.pos.z + FIELD_DEPTH);
		pVtx[ONE].pos =  D3DXVECTOR3(g_Field.pos.x + FIELD_WIDTH, g_Field.pos.y, g_Field.pos.z + FIELD_DEPTH);
		pVtx[SEC].pos =  D3DXVECTOR3(g_Field.pos.x - FIELD_WIDTH, g_Field.pos.y, g_Field.pos.z - FIELD_DEPTH);
		pVtx[SAN].pos =  D3DXVECTOR3(g_Field.pos.x + FIELD_WIDTH, g_Field.pos.y, g_Field.pos.z - FIELD_DEPTH);
	}

	//norの設定
	{
		pVtx[ZERO].nor = SET_NOR;
		pVtx[ONE].nor = SET_NOR;
		pVtx[SEC].nor = SET_NOR;
		pVtx[SAN].nor = SET_NOR;
	}

	//頂点カラーの設定
	{
		pVtx[ZERO].col = RGBA_WHITE;
		pVtx[ONE].col = RGBA_WHITE;
		pVtx[SEC].col = RGBA_WHITE;
		pVtx[SAN].col = RGBA_WHITE;
	}

	//テクスチャの座標
	{
		pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
		pVtx[ONE].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
		pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
		pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
	}

	//頂点バッファをロックする
	g_pVtxBuffField->Unlock();
}

//フィールドの終了処理
void UninitField(void)
{
	//テクスチャの破棄
	if (g_pTextureField != NULL)
	{
		g_pTextureField->Release();
		g_pTextureField = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffField != NULL)
	{
		g_pVtxBuffField->Release();
		g_pVtxBuffField = NULL;
	}
}

//フィールドの更新処理
void UpdateField(void)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffField->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点バッファをロックする
	g_pVtxBuffField->Unlock();

}

//フィールドの描画処理
void DrawField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Field.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Field.rot.y, g_Field.rot.x, g_Field.rot.z);

	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_Field.pos.x, g_Field.pos.y, g_Field.pos.z);

	D3DXMatrixMultiply(&g_Field.mtxWorld, &g_Field.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Field.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(ZERO, g_pVtxBuffField, ZERO, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(ZERO, g_pTextureField);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
}