/*==========================================================================================================================================================

															弾処理[bullet.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "bullet.h"
#include "billboard.h"
#include "color.h"
#include "player.h"
#include "input.h"
#include "particle.h"
#include "shadow.h"
#include "explosion.h"
#include "effect.h"
#include "particle.h"
#include "wall.h"

//マクロ定義
#define MAX_BULLET			(256)		//弾の最大数
#define BULLET_HP			(400)		//弾の体力
#define FANGLE_CORRECT		(6.28f)		//角度の修正に使用
#define DAMAGE				(1)			//弾の攻撃力
#define BULLET_SIZE			(4.0f)		//弾の幅
#define BULLET_LOMIT_POS_X	(600.0f)	//弾の限界座標　X方向
#define BULLET_LOMIT_POS_Z	(600.0f)	//弾の限界座標　Z方向

//グローバル変数宣言
LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		//頂点バッファへのポインタ
Bullet					g_aBullet[MAX_BULLET];			//弾の情報

//弾の初期化処理
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			//デバイスへのポインタ
	int nCntBullet;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/bullet.png", &g_pTextureBullet);

	//弾の情報の初期化
	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos				= ZERO_SET;		//位置初期化
		g_aBullet[nCntBullet].posOld			= ZERO_SET;		//前回の位置初期化
		g_aBullet[nCntBullet].move				= ZERO_SET;		//移動量初期化
		g_aBullet[nCntBullet].nLife				= BULLET_HP;	//寿命初期化
		g_aBullet[nCntBullet].EffectAlphaColor	= F_ZERO;		//弾の軌跡のα値を指定する変数
		g_aBullet[nCntBullet].bUse				= false;		//使用していない状態にする

		g_aBullet[nCntBullet].nIdxShadow = SetShadow();		//影の設定
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_BULLET, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffBullet, NULL);

	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
		{
			pVtx[0].pos = D3DXVECTOR3(-BULLET_SIZE,  BULLET_SIZE, F_ZERO);
			pVtx[1].pos = D3DXVECTOR3( BULLET_SIZE,  BULLET_SIZE, F_ZERO);
			pVtx[2].pos = D3DXVECTOR3(-BULLET_SIZE, -BULLET_SIZE, F_ZERO);
			pVtx[3].pos = D3DXVECTOR3( BULLET_SIZE, -BULLET_SIZE, F_ZERO);
		}

		//rhwの設定
		{
			pVtx[0].nor = SET_NOR;
			pVtx[1].nor = SET_NOR;
			pVtx[2].nor = SET_NOR;
			pVtx[3].nor = SET_NOR;
		}

		//頂点カラーの設定
		{
			pVtx[0].col = XCOL_WHITE;
			pVtx[1].col = XCOL_WHITE;
			pVtx[2].col = XCOL_WHITE;
			pVtx[3].col = XCOL_WHITE;
		}

		//テクスチャの座標
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += NUM_VERTEX;			//頂点データのポインタを4つ分進める
	}

	//頂点バッファをロックする
	g_pVtxBuffBullet->Unlock();
}

//弾の終了処理
void UninitBullet(void)
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//弾の更新処理
void UpdateBullet(void)
{
	//ポインタを設定
	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffBullet->Lock(ZERO, ZERO, (void* *)&pVtx, ZERO);

	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			//前回の位置更新
			g_aBullet[nCntBullet].posOld = g_aBullet[nCntBullet].pos;

			//弾の位置の更新
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			pVtx[ZERO].col = g_aBullet[nCntBullet].col;
			pVtx[ONE].col = g_aBullet[nCntBullet].col;
			pVtx[SEC].col = g_aBullet[nCntBullet].col;
			pVtx[SAN].col = g_aBullet[nCntBullet].col;

			//寿命を減らす
			g_aBullet[nCntBullet].nLife--;

			//弾が画面外に出た場合
			if (CollisionWall(&g_aBullet[nCntBullet].pos, &g_aBullet[nCntBullet].posOld) == true ||
				g_aBullet[nCntBullet].nLife <= ZERO)
			{
				g_aBullet[nCntBullet].bUse = false;		//使用していない状態にする
			}

			//影描画
			SetPositionShadow(g_aBullet[nCntBullet].nIdxShadow, g_aBullet[nCntBullet].pos);

			//エフェクト描画
			SetEffect(g_aBullet[nCntBullet].pos, XCOL_YELLOW, 10.0f, 60);

			//使われなくなった弾の初期化
			if (g_aBullet[nCntBullet].bUse == false)
			{
				//爆発設定
				SetExplosion(g_aBullet[nCntBullet].pos, XCOL_WHITE);

				//パーティクル設定
				if (GetKeyboardPress(DIK_B) == true)
				{
					//SetParticle(g_aBullet[nCntBullet].pos, SET_PARTICLE_NORMAL);
				}
				else
				{
					//SetParticle(g_aBullet[nCntBullet].pos, SET_PARTICLE_EXPLOSION);
				}

				BreakShadow(g_aBullet[nCntBullet].nIdxShadow);	//影の使用を破棄する
			}
		}

		//頂点データのポインタを4つ分進める
		pVtx += NUM_VERTEX;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBullet->Unlock();
}

//弾の描画処理
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxTrans;						//計算用マトリックス
	D3DXMATRIX mtxView;							//ビューマトリックス取得用
	BillTest *pBill = GetAlphaTest();			//ビルボードのα・Zテスト情報を取得

	if (pBill->bAlphaTest == true)
	{
		//αテストを有効にする
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

	//ビルボードの描画設定
	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCntBullet].mtxWorld);

			//ビューマトリックスをGet
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCntBullet].mtxWorld, NULL, &mtxView);	//逆行列を求める
			g_aBullet[nCntBullet].mtxWorld._41 = F_ZERO;		//→↓
			g_aBullet[nCntBullet].mtxWorld._42 = F_ZERO;		//	反転させて求めた逆行列の不要な部分をゼロにする
			g_aBullet[nCntBullet].mtxWorld._43 = F_ZERO;		//→↑

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCntBullet].pos.x, g_aBullet[nCntBullet].pos.y, g_aBullet[nCntBullet].pos.z);

			D3DXMatrixMultiply(&g_aBullet[nCntBullet].mtxWorld, &g_aBullet[nCntBullet].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCntBullet].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(ZERO, g_pVtxBuffBullet, ZERO, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(ZERO, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * NUM_VERTEX, SEC);
		}
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	//LESS EQUAL は 「<=」のこと
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		//Zバッファの有効無効設定

	//αテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, pBill->AlphaTest);
}

//弾の設定処理
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float Alpha)
{
	for (int nCntBullet = START_CNT; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{//弾が使用されていない

			//弾の情報を設定
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].move = move;
			g_aBullet[nCntBullet].rot.z = ZERO;
			g_aBullet[nCntBullet].col = col;
			g_aBullet[nCntBullet].EffectAlphaColor = Alpha;

			g_aBullet[nCntBullet].nLife = BULLET_HP;
			g_aBullet[nCntBullet].bUse = true;			//使用している状態にする
			break;
		}
	}
}