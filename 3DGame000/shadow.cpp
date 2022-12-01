/*==========================================================================================================================================================

															影の処理[shadow.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "input.h"
#include "color.h"
#include "shadow.h"
#include "player.h"

#define SHADOW_POS		D3DXVECTOR3(0.0f, 0.0f, 0.0f)
#define SHADOW_WIDTH	(10.0f)							//幅サイズ
#define SHADOW_DEPTH	(10.0f)							//奥行きサイズ
#define SHADOW_ALPTH	(200)							//影のα値
#define MAX_SHADOW		(512)							//影の最大数

//影の構造体
typedef struct
{
	D3DXVECTOR3		pos;		//位置
	D3DXVECTOR3		rot;		//向き
	D3DXMATRIX		mtxWorld;	//ワールドマトリックス
	int				aColor;		//影のα値
	float			fWidht;		//幅
	float			fDepth;		//奥行き
	bool			bSet;		//対象の影が決まっているかどうか
	bool			bUse;		//使われているかどうか
}Shadow;

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;		//頂点バッファ
LPDIRECT3DTEXTURE9		g_pTextureShadow = NULL;		//テクスチャ
Shadow g_aShadow[MAX_SHADOW];							//影の情報

//ポリゴンの初期化処理
void InitShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();							//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/shadow000.jpg", &g_pTextureShadow);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_SHADOW, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		g_aShadow[nCntShadow].pos = SHADOW_POS;			//位置初期化
		g_aShadow[nCntShadow].rot = ZERO_SET;			//向き初期化
		g_aShadow[nCntShadow].fWidht = SHADOW_WIDTH;	//幅初期化
		g_aShadow[nCntShadow].fDepth = SHADOW_DEPTH;	//奥行き初期化
		g_aShadow[nCntShadow].aColor = SHADOW_ALPTH;	//影のα値初期化
		g_aShadow[nCntShadow].bSet = false;				//使用されていない
		g_aShadow[nCntShadow].bUse = false;				//使用されていない

		//頂点座標の設定
		{
			pVtx[ZERO].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidht, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fDepth);
			pVtx[ONE].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidht, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z + g_aShadow[nCntShadow].fDepth);
			pVtx[SEC].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x - g_aShadow[nCntShadow].fWidht, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fDepth);
			pVtx[SAN].pos = D3DXVECTOR3(g_aShadow[nCntShadow].pos.x + g_aShadow[nCntShadow].fWidht, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z - g_aShadow[nCntShadow].fDepth);
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
			pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
			pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nCntShadow].aColor);
		}

		//テクスチャの座標
		{
			pVtx[ZERO].tex = D3DXVECTOR2(ORIGIN_POS, ORIGIN_POS);
			pVtx[ONE].tex = D3DXVECTOR2(F_TEN, ORIGIN_POS);
			pVtx[SEC].tex = D3DXVECTOR2(ORIGIN_POS, F_TEN);
			pVtx[SAN].tex = D3DXVECTOR2(F_TEN, F_TEN);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffShadow->Unlock();
}

//ポリゴンの終了処理
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_pTextureShadow != NULL)
	{
		g_pTextureShadow->Release();
		g_pTextureShadow = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffShadow != NULL)
	{
		g_pVtxBuffShadow->Release();
		g_pVtxBuffShadow = NULL;
	}
}

//ポリゴンの更新処理
void UpdateShadow(void)
{
	//ポインタを設定
	//VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	//頂点バッファをロックする
	//g_pVtxBuffShadow->Unlock();

}

//ポリゴンの描画処理
void DrawShadow(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス

	//αブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCntShadow = START_CNT; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_aShadow[nCntShadow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCntShadow].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCntShadow].rot.y, g_aShadow[nCntShadow].rot.x, g_aShadow[nCntShadow].rot.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCntShadow].pos.x, g_aShadow[nCntShadow].pos.y, g_aShadow[nCntShadow].pos.z);

			D3DXMatrixMultiply(&g_aShadow[nCntShadow].mtxWorld, &g_aShadow[nCntShadow].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCntShadow].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(ZERO, g_pVtxBuffShadow, ZERO, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureShadow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, ZERO, SEC);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//影の設定処理
int SetShadow(void)
{
	int nCntShadow = MINUS_ONE;

	for (int mCntShadow = START_CNT; mCntShadow < MAX_SHADOW; mCntShadow++)
	{
		nCntShadow++;

		if (g_aShadow[mCntShadow].bUse == false && g_aShadow[mCntShadow].bSet == false)
		{
			g_aShadow[mCntShadow].pos = SHADOW_POS;	//位置初期化
			g_aShadow[mCntShadow].rot = ZERO_SET;	//向き初期化
			g_aShadow[mCntShadow].bSet = true;		//対象が決まった
			break;
		}
	}

	return nCntShadow;
}

//影の位置設定処理
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nIdxShadow].pos = pos;
	g_aShadow[nIdxShadow].pos.y = F_ZERO;

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffShadow->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	pVtx += NUM_VERTEX * nIdxShadow;		//頂点座標をインデックス分進める

	g_aShadow[nIdxShadow].fWidht = pos.y / 20.0f + SHADOW_WIDTH;		//幅調整
	g_aShadow[nIdxShadow].fDepth = pos.y / 20.0f + SHADOW_DEPTH;		//奥行き調整
	g_aShadow[nIdxShadow].aColor = SHADOW_ALPTH - (int)(pos.y * 1.3f);	//α値調整	
	g_aShadow[nIdxShadow].bUse = true;									//使用する

	//頂点カラーの設定
	{
		pVtx[ZERO].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[ONE].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[SEC].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
		pVtx[SAN].col = D3DCOLOR_RGBA(COLOR_MAX, COLOR_MAX, COLOR_MAX, g_aShadow[nIdxShadow].aColor);
	}

	//頂点バッファをロックする
	g_pVtxBuffShadow->Unlock();
}

//影の使用を無くす
void BreakShadow(int nIdxShadow)
{
	g_aShadow[nIdxShadow].bUse = false;
}