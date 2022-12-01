/*==========================================================================================================================================================

														パーティクル処理[parthicle.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "particle.h"
#include "billboard.h"
#include "color.h"

//マクロ定義
#define MAX_PARTICLE				(5000)		//パーティクルの最大数
#define PARTICLE_LIFE				(60)		//パーティクルの寿命

#define MAX_RADIUS					(10.0f)		//半径の初期値
#define MINUS_RADIUS				(0.1f)		//半径の縮小値

//パーティクルの発生に使う移動方向のマクロ
#define ANGLE						(629)		//パーティクルが移動する向き
#define FIX_ANGLE					(314)		//ANGLEの角度を修正

#define TIP_UP						(3)		//跳ね上げる移動量
#define MIN_TIP						(2)		//最低限の移動量
#define FIX_TIP_UP					(3)			//TIP_UPの倍率修正
#define FIX_FLOAT					(100.0f)	//角度をfloat型に修正
#define MOVE_PARTICLE				(2.0f)		//パーティクルの基本移動量
#define PARTICLE_NUM_EXPLOSION		(30)		//爆発的なパーティクルの発生量

//パーティクルの重力マクロ
#define GRAVITY_PARTICLE			(-4.0f)		//パーティクルの重力加速度
#define GRAVITY_PART_COR			(0.2f)		//パーティクルの重力係数
#define BOUND_PARTICLE_COR			(-0.9f)		//パーティクルのバウンド補正値

#define REFLECT_PARTICEL			(-1.0f)		//跳ね返し係数

#define PARTICLE_LIMIT_WALL			(600.0f)	//壁の位置
#define PARTICLE_LIMIT_FLOOR		(000.0f)	//床の座標

//パーティクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置(発生位置)
	D3DXVECTOR3 move;		//移動量
	D3DXCOLOR	col;		//色
	D3DXMATRIX	mtxWorld;	//ワールドマトリックス
	int			nLife;		//寿命(発生時間)
	float		fRadius;	//パーティクルの半径
	bool		bUse;		//使用しているかどうか
}Particle;

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureParticle = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffParticle = NULL;	//頂点バッファへのポインタ
Particle				g_aParticle[MAX_PARTICLE];	//パーティクルの情報

//パーティクルの初期化
void InitParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();									//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/particle000.jpg", &g_pTextureParticle);	//薄いパーティクル
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_PARTICLE, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffParticle, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		//パーティクルの情報の初期化
		g_aParticle[nCntParticle].pos		= ZERO_SET;			//位置の初期化
		g_aParticle[nCntParticle].move		= ZERO_SET;			//移動量の初期化
		g_aParticle[nCntParticle].nLife		= PARTICLE_LIFE;	//寿命の初期化
		g_aParticle[nCntParticle].fRadius	= MAX_RADIUS;		//半径の初期化
		g_aParticle[nCntParticle].bUse		= false;			//使用していない状態にする

		//頂点座標 (半径を使って) の設定
		SetParticleRadius(nCntParticle);

		//rhwの設定
		{
			pVtx[0].nor = SET_NOR;
			pVtx[1].nor = SET_NOR;
			pVtx[2].nor = SET_NOR;
			pVtx[3].nor = SET_NOR;
		}

		//頂点カラーの設定
		{
			pVtx[0].col = PARTICLE_COL;
			pVtx[1].col = PARTICLE_COL;
			pVtx[2].col = PARTICLE_COL;
			pVtx[3].col = PARTICLE_COL;
		}

		//テクスチャの座標
		{
			pVtx[0].tex = ORIGIN_TEX;
			pVtx[1].tex = MA_ZE_TEX;
			pVtx[2].tex = ZE_MA_TEX;
			pVtx[3].tex = LIMIT_TEX;
		}

		pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffParticle->Unlock();
}

//パーティクルの終了処理
void UninitParticle(void)
{
	//テクスチャの破棄
	if (g_pTextureParticle != NULL)
	{
		g_pTextureParticle->Release();
		g_pTextureParticle = NULL;
	}
	
	//頂点バッファの破棄
	if (g_pVtxBuffParticle != NULL)
	{
		g_pVtxBuffParticle->Release();
		g_pVtxBuffParticle = NULL;
	}
}

//パーティクルの更新処理
void UpdateParticle(void)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++, pVtx += NUM_VERTEX)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//パーティクルを移動
			g_aParticle[nCntParticle].pos += g_aParticle[nCntParticle].move;

			//パーティクルに重力を加える
			g_aParticle[nCntParticle].move.y += (GRAVITY_PARTICLE - g_aParticle[nCntParticle].move.y) * GRAVITY_PART_COR;

			//パーティクルが 壁・床に当たったか判定
			BoundParticle(nCntParticle);

			//半径を縮小
			g_aParticle[nCntParticle].fRadius -= MINUS_RADIUS;

			//半径が0になった
			if (g_aParticle[nCntParticle].fRadius <= F_ZERO)
			{
				g_aParticle[nCntParticle].bUse = false;			//使用していない状態にする
			}

			//半径を使って頂点座標を設定
			SetParticleRadius(nCntParticle);
		}
	}

	//頂点バッファをロックする
	g_pVtxBuffParticle->Unlock();
}

//パーティクルの描画処理
void DrawParticle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用
	BillTest *pBill = GetAlphaTest();			//ビルボードのα・Zテスト情報を取得

	//αテストを有効にする
	if (pBill->bAlphaTest == true)
	{
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
	}

	//Zテストを無効にする
	if (pBill->bZTest == true)
	{
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		//Zバッファの有効無効設定
	}
	
	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//ビルボードの描画設定
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aParticle[nCntParticle].mtxWorld);

			//ビューマトリックスをGet
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aParticle[nCntParticle].mtxWorld, NULL, &mtxView);	//逆行列を求める
			g_aParticle[nCntParticle].mtxWorld._41 = F_ZERO;		//→↓
			g_aParticle[nCntParticle].mtxWorld._42 = F_ZERO;		//	反転させて求めた逆行列の不要な部分をゼロにする
			g_aParticle[nCntParticle].mtxWorld._43 = F_ZERO;		//→↑

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aParticle[nCntParticle].pos.x, g_aParticle[nCntParticle].pos.y, g_aParticle[nCntParticle].pos.z);

			D3DXMatrixMultiply(&g_aParticle[nCntParticle].mtxWorld, &g_aParticle[nCntParticle].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aParticle[nCntParticle].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(ZERO, g_pVtxBuffParticle, ZERO, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureParticle);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntParticle * NUM_VERTEX, SEC);
		}
	}

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL は 「<=」のこと
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zバッファの有効無効設定

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
}

//パーティクルの設定処理
void SetParticle(D3DXVECTOR3 pos, SET_PARTICLE nType)
{
	//発生パターン分岐
	switch (nType)
	{
	case SET_PARTICLE_NORMAL:		//パーティクル通常発生
		SetParticleNor(pos);
		break;

	case SET_PARTICLE_EXPLOSION:
		SetParticleExplosion(pos);	//パーティクル爆発発生
		break;
	}
}

//通常発生のパーティクル設定
void SetParticleNor(D3DXVECTOR3 pos)
{
	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			D3DXVECTOR3 fAngle;		//パーティクルが移動する角度を設定

			fAngle.x = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;	//それぞれの方向の角度を決定
			fAngle.y = (float)(rand() % TIP_UP + MIN_TIP);				//それぞれの方向の角度を決定
			fAngle.z = (float)(rand() % ANGLE - FIX_ANGLE) / FIX_FLOAT;	//それぞれの方向の角度を決定

			float fMove = (float)(rand() % ANGLE) / FIX_FLOAT + MOVE_PARTICLE;	//パーティクルの移動量

																			//パーティクル情報の設定
			g_aParticle[nCntParticle].pos = pos;							//位置
			g_aParticle[nCntParticle].move.x = (fAngle.x) * fMove;			//移動量計算
			g_aParticle[nCntParticle].move.y = (fAngle.y) * fMove;			//移動量計算
			g_aParticle[nCntParticle].move.z = (fAngle.z) * fMove;			//移動量計算
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;				//寿命
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;					//半径
			g_aParticle[nCntParticle].bUse = true;							//使用している状態にする

			break;
		}
	}
}

//爆発発生のパーティクル設定
void SetParticleExplosion(D3DXVECTOR3 pos)
{
	//発生量カウント
	int nCntBirth = ZERO;

	for (int nCntParticle = START_CNT; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{
			D3DXVECTOR3 fAngle;		//パーティクルが移動する角度を設定
				
			fAngle.x = (float)(rand() %  ANGLE  - FIX_ANGLE ) / FIX_FLOAT;	//それぞれの方向の角度を決定
			fAngle.y = (float)(rand() %  TIP_UP +  MIN_TIP  );				//それぞれの方向の角度を決定
			fAngle.z = (float)(rand() %  ANGLE  - FIX_ANGLE ) / FIX_FLOAT;	//それぞれの方向の角度を決定

			float fMove = (float)(rand() % ANGLE) / FIX_FLOAT + MOVE_PARTICLE;	//パーティクルの移動量

			//パーティクル情報の設定
			g_aParticle[nCntParticle].pos = pos;								//位置
			g_aParticle[nCntParticle].move.x = (fAngle.x) * fMove;			//移動量計算
			g_aParticle[nCntParticle].move.y = (fAngle.y) * fMove;			//移動量計算
			g_aParticle[nCntParticle].move.z = (fAngle.z) * fMove;			//移動量計算
			g_aParticle[nCntParticle].nLife = PARTICLE_LIFE;					//寿命
			g_aParticle[nCntParticle].fRadius = MAX_RADIUS;						//半径
			g_aParticle[nCntParticle].bUse = true;								//使用している状態にする

			//発生量増加
			nCntBirth++;

			//発生量が規定量を超えたら終了
			if (nCntBirth >= PARTICLE_NUM_EXPLOSION)
			{
				break;
			}
		}
	}
}

//パーティクルが壁に当たった時に跳ね返す
void BoundParticle(int nCntParticle)
{
	//X座標の判定
	if (g_aParticle[nCntParticle].pos.x >= PARTICLE_LIMIT_WALL || g_aParticle[nCntParticle].pos.x <= -PARTICLE_LIMIT_WALL)
	{
		g_aParticle[nCntParticle].move.x *= REFLECT_PARTICEL;		//移動量を逆転させる
	}

	//Y座標の判定
	if (g_aParticle[nCntParticle].pos.y <= PARTICLE_LIMIT_FLOOR)
	{//バウンドさせる
		g_aParticle[nCntParticle].move.y = (GRAVITY_PARTICLE + g_aParticle[nCntParticle].move.y) * BOUND_PARTICLE_COR;
	}

	//Z座標の判定
	if (g_aParticle[nCntParticle].pos.z >= PARTICLE_LIMIT_WALL || g_aParticle[nCntParticle].pos.z <= -PARTICLE_LIMIT_WALL)
	{
		g_aParticle[nCntParticle].move.z *= REFLECT_PARTICEL;		//移動量を逆転させる
	}
}

//パーティクルの半径処理
void SetParticleRadius(int nCntParticle)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffParticle->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	pVtx += nCntParticle * NUM_VERTEX;	//頂点バッファをパーティクルの番号の数増やす

	//頂点座標の設定
	{
		pVtx[ZERO].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius,  g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		 pVtx[ONE].pos = D3DXVECTOR3( g_aParticle[nCntParticle].fRadius,  g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		 pVtx[SEC].pos = D3DXVECTOR3(-g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
		 pVtx[SAN].pos = D3DXVECTOR3( g_aParticle[nCntParticle].fRadius, -g_aParticle[nCntParticle].fRadius, ORIGIN_POS);
	}

	//頂点バッファをロックする
	g_pVtxBuffParticle->Unlock();
}