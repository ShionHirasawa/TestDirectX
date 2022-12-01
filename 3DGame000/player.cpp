/*==========================================================================================================================================================

														プレイヤーの処理[player.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "main.h"
#include "input.h"
#include "player.h"
#include "model.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"
#include "model.h"
#include "billboard.h"

#define PLAYER_TEX_MAX			(10)		//モデルのテクスチャの最大数
#define PLAYER_SPEED			(0.5f)		//プレイヤーの移動スピード
#define MAX_SPEED				(4.0f)		//プレイヤーの限界移動量
#define CROSS_SPEED_COR			(0.9f)		//斜め方向への移動量補正値

#define PLAYER_ROT_CORRECT_MOVE	(0.05f)		//プレイヤーの進行方向への向き修正の補正値

#define LIMIT_PLAYER_LEFT		(-600.0f)	//プレイヤーの移動限界地点　左
#define LIMIT_PLAYER_RIGHT		(600.0f)	//プレイヤーの移動限界地点　右
#define LIMIT_PLAYER_CEELING	(100.0f)	//プレイヤーの移動限界地点　上
#define LIMIT_PLAYER_FLOAR		(0.0f)		//プレイヤーの移動限界地点　下
#define LIMIT_PLAYER_FRONT		(600.0f)	//プレイヤーの移動限界地点　手前
#define LIMIT_PLAYER_BACK		(-600.0f)	//プレイヤーの移動限界地点　奥

//プレイヤーの弾の発射におけるマクロ
#define BULLET_COLOR					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0)	//弾の色
#define PLAYER_BULLET_SPEED				(-5.0f)								//弾の発射速度
#define PLAYER_BULLET_CORRECT_HEIGHT	(9.0f)								//弾の発射位置の補正値

//グローバル変数
LPDIRECT3DTEXTURE9			g_pTexturePlayer[PLAYER_TEX_MAX] = {};	//テクスチャへのポインタ
LPD3DXMESH					g_pMeshPlayer = NULL;					//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER				g_pBuffMatPlayer = NULL;				//マテリアルへのポインタ
DWORD						g_dwNumMatPlayer = ZERO;				//マテリアルの数
Player						g_player;								//プレイヤーの情報

//プレイヤーの初期化処理
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	g_pMeshPlayer = NULL;						//メッシュ初期化
	g_pBuffMatPlayer = NULL;					//マテリアルへのポインタ初期化
	g_dwNumMatPlayer = ZERO;					//マテリアルの数初期化
	ResetPlayer();								//プレイヤーの情報初期化

	//Xファイル読み込み
	D3DXLoadMeshFromX("data/MODEL/floating_car.x", D3DXMESH_SYSTEMMEM, pDevice, NULL, &g_pBuffMatPlayer, NULL, &g_dwNumMatPlayer, &g_pMeshPlayer);

	//マテリアルデータへのポインタを取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntPlayer = START_CNT; nCntPlayer < (int)g_dwNumMatPlayer; nCntPlayer++)
	{
		if (pMat[nCntPlayer].pTextureFilename != NULL)
		{//テクスチャファイルが存在する
		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice, pMat[nCntPlayer].pTextureFilename, &g_pTexturePlayer[nCntPlayer]);
		}
	}

	//影の設定
	g_player.nIdxShadow = SetShadow();
}

//プレイヤーの終了処理
void UninitPlayer(void)
{
	//テクスチャの破棄
	for (int nCntPlayer = START_CNT; nCntPlayer < PLAYER_TEX_MAX; nCntPlayer++)
	{
		if (g_pTexturePlayer[nCntPlayer] != NULL)
		{
			g_pTexturePlayer[nCntPlayer]->Release();
			g_pTexturePlayer[nCntPlayer] = NULL;
		}
	}

	//テクスチャの破棄
	if (g_pMeshPlayer != NULL)
	{
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatPlayer != NULL)
	{
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//プレイヤーの更新処理
void UpdatePlayer(void)
{
	//前回の位置更新
	g_player.posOld = g_player.pos;

	//プレイヤーの移動処理
	MovePlayer();

	//プレイヤーの回転処理
	RotPlayer();

	//位置更新
	g_player.pos += g_player.move;

	//プレイヤーの位置修正
	FixPlayerPos();

	//モデルとの当たり判定
	CollisionModel(&g_player.pos, &g_player.posOld);

	//プレイヤーのリセット処理
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		ResetPlayer();
	}

	//弾発射
	if (GetKeyboardPress(DIK_G) == true)
	{
		D3DXVECTOR3 BulletMove;

		BulletMove.x = sinf(g_player.rot.y) * PLAYER_BULLET_SPEED;
		BulletMove.z = cosf(g_player.rot.y) * PLAYER_BULLET_SPEED;
		BulletMove.y = F_ZERO;

		SetBullet(D3DXVECTOR3(g_player.pos.x, g_player.pos.y + PLAYER_BULLET_CORRECT_HEIGHT, g_player.pos.z), BulletMove, BULLET_COLOR, 0.8f);
	}
	
	//プレイヤーの影描画
	SetPositionShadow(g_player.nIdxShadow, g_player.pos);
}

//プレイヤーの描画処理
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;				//計算用マトリックス
	D3DMATERIAL9 matDef;						//現在のマテリアル保存用
	D3DXMATERIAL *pMat;							//マテリアルデータへのポインタ
	BillTest *pBill = GetAlphaTest();			//ビルボードのα・Zテスト情報を取得

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);

	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);

	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL *)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntPlayer = START_CNT; nCntPlayer < (int)g_dwNumMatPlayer; nCntPlayer++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntPlayer].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(ZERO, g_pTexturePlayer[nCntPlayer]);

		//モデル(パーツ)の描画
		g_pMeshPlayer->DrawSubset(nCntPlayer);
	}

	//マテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//プレイヤーの情報
Player *GetPlayer(void)
{
	return &g_player;
}

//プレイヤーの移動処理
void MovePlayer(void)
{
	D3DXVECTOR3 DirectionRot;			//プレイヤーとカメラの角度から進行方向を計算
	float fAngle;						//角度を決める
	float fMoveRot;						//移動方向
	Camera *pCamera = GetCamera();		//カメラの情報取得

	DirectionRot = g_player.rot - pCamera->rot;	//進行方向計算

	//左に移動・方向転換
	if (GetKeyboardPress(DIK_LEFT) == true)
	{
		//進行方向より角度が小さく、進行方向の真逆より大きい
		if (RIGHT + pCamera->rot.y >= DirectionRot.y && DirectionRot.y >= LEFT + pCamera->rot.y)
		{//右旋回
			g_player.rot.y += PLAYER_ROT_CORRECT_MOVE;	//プレイヤーを進行方向に向ける

			//角度が進行方向を過ぎてしまった
			if (DirectionRot.y >= RIGHT - pCamera->rot.y)
			{
				g_player.rot.y = RIGHT - pCamera->rot.y;					//進行方向に向ける
			}
		}
		
		else
		{
			g_player.rot.y -= PLAYER_ROT_CORRECT_MOVE;	//プレイヤーを進行方向に向ける

			//角度が進行方向を過ぎてしまった
			if (RIGHT - pCamera->rot.y >= DirectionRot.y && DirectionRot.y >= LEFT - pCamera->rot.y)
			{
				g_player.rot.y = RIGHT - pCamera->rot.y;					//進行方向に向ける
			}
		}

		g_player.move.x += sinf(LEFT - pCamera->rot.y) * PLAYER_SPEED;	//X方向に移動
		g_player.move.z += sinf(DOWN - pCamera->rot.y) * PLAYER_SPEED;	//Z方向に移動
	}

	//右に移動・方向転換
	if (GetKeyboardPress(DIK_RIGHT) == true)
	{
		fMoveRot = (LEFT - pCamera->rot.y);								//進行方向計算

		g_player.rot.y += (fMoveRot - g_player.rot.y) * PLAYER_ROT_CORRECT_MOVE;	//プレイヤーを進行方向に向ける

		g_player.move.x += sinf(RIGHT + pCamera->rot.y) * PLAYER_SPEED;
		g_player.move.z += sinf(UP - pCamera->rot.y) * PLAYER_SPEED;
	}

	//奥に移動・方向転換
	if (GetKeyboardPress(DIK_UP) == true)
	{
		//進行方向の間反対より、角度が小さいか	角度が小さければ左旋回で進行方向に向かせる　角度が大きければ右旋回で向かせる
		if (g_player.rot.y <  DOWN || GetKeyboardPress(DIK_RIGHT) == true)
		{//右旋回
			fMoveRot = (REVERSE_UP - pCamera->rot.y);								//進行方向計算
		}
		else
		{//左旋回
			fMoveRot = (UP - pCamera->rot.y);								//進行方向計算
		}

		g_player.rot.y += (fMoveRot - g_player.rot.y) * PLAYER_ROT_CORRECT_MOVE;	//プレイヤーを進行方向に向ける

		g_player.move.x += sinf(UP + pCamera->rot.y) * PLAYER_SPEED;
		g_player.move.z += sinf(RIGHT - pCamera->rot.y) * PLAYER_SPEED;

		//横移動している
		if (GetKeyboardPress(DIK_LEFT) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{
			//移動量を補正する
			g_player.move.x *= CROSS_SPEED_COR;
			g_player.move.z *= CROSS_SPEED_COR;
		}
	}

	//手前に移動・方向転換
	if (GetKeyboardPress(DIK_DOWN) == true)
	{
		fMoveRot = (DOWN - pCamera->rot.y);								//進行方向計算

		g_player.rot.y += (fMoveRot - g_player.rot.y) * PLAYER_ROT_CORRECT_MOVE;	//プレイヤーを進行方向に向ける

		g_player.move.x += sinf(DOWN + pCamera->rot.y) * PLAYER_SPEED;
		g_player.move.z += sinf(LEFT - pCamera->rot.y) * PLAYER_SPEED;

		//横移動している
		if (GetKeyboardPress(DIK_LEFT) == true || GetKeyboardPress(DIK_RIGHT) == true)
		{
			//移動量を補正する
			g_player.move.x *= CROSS_SPEED_COR;
			g_player.move.z *= CROSS_SPEED_COR;
		}
	}

	//上昇
	if (GetKeyboardPress(DIK_SPACE) == true)
	{
		g_player.move.y -= cosf(UP) * PLAYER_SPEED;
	}

	//下降
	if (GetKeyboardPress(DIK_TAB) == true)
	{
		g_player.move.y -= cosf(DOWN) * PLAYER_SPEED;
	}

	//プレイヤーの移動量修正
	FixPlayerMove();

	//プレイヤーの移動慣性処理
	InertiaPlayerMove();
}

//プレイヤーの移動量修正
void FixPlayerMove(void)
{
	//プレイヤーの移動量が2.0f以上になった
	if (g_player.move.x >= MAX_SPEED)
	{
		g_player.move.x = MAX_SPEED;
	}

	//プレイヤーの移動量が -2.0f以下になった
	if (g_player.move.x <= -MAX_SPEED)
	{
		g_player.move.x = -MAX_SPEED;
	}

	//プレイヤーの移動量が2.0f以上になった
	if (g_player.move.y >= MAX_SPEED)
	{
		g_player.move.y = MAX_SPEED;
	}

	//プレイヤーの移動量が -2.0f以下になった
	if (g_player.move.y <= -MAX_SPEED)
	{
		g_player.move.y = -MAX_SPEED;
	}

	//プレイヤーの移動量が2.0f以上になった
	if (g_player.move.z >= MAX_SPEED)
	{
		g_player.move.z = MAX_SPEED;
	}

	//プレイヤーの移動量が -2.0f以下になった
	if (g_player.move.z <= -MAX_SPEED)
	{
		g_player.move.z = -MAX_SPEED;
	}
}

//プレイヤーの移動慣性処理
void InertiaPlayerMove(void)
{
	g_player.move.x += (F_ZERO - g_player.move.x) * 0.1f;
	g_player.move.y += (F_ZERO - g_player.move.y) * 0.1f;
	g_player.move.z += (F_ZERO - g_player.move.z) * 0.1f;
}

//プレイヤーの位置修正
void FixPlayerPos(void)
{
	//「移動限界地点　左」を超えた
	if (g_player.pos.x < LIMIT_PLAYER_LEFT)
	{
		g_player.pos.x = LIMIT_PLAYER_LEFT;
	}

	//「移動限界地点　右」を超えた
	if (g_player.pos.x > LIMIT_PLAYER_RIGHT)
	{
		g_player.pos.x = LIMIT_PLAYER_RIGHT;
	}

	//「移動限界地点　上」を超えた
	if (g_player.pos.y > LIMIT_PLAYER_CEELING)
	{
		g_player.pos.y = LIMIT_PLAYER_CEELING;
	}

	//「移動限界地点　下」を超えた
	if (g_player.pos.y < LIMIT_PLAYER_FLOAR)
	{
		g_player.pos.y = LIMIT_PLAYER_FLOAR;
	}

	//「移動限界地点　手前」を超えた
	if (g_player.pos.z > LIMIT_PLAYER_FRONT)
	{
		g_player.pos.z = LIMIT_PLAYER_FRONT;
	}

	//「移動限界地点　奥」を超えた
	if (g_player.pos.z < LIMIT_PLAYER_BACK)
	{
		g_player.pos.z = LIMIT_PLAYER_BACK;
	}
}

//プレイヤーの回転処理
void RotPlayer(void)
{
	//左旋回
	if (GetKeyboardPress(DIK_LSHIFT) == true)
	{
		g_player.rot.y += sinf(LEFT) * 0.1f;
	}

	//右旋回
	if (GetKeyboardPress(DIK_RSHIFT) == true)
	{
		g_player.rot.y += sinf(RIGHT) * 0.1f;
	}

	/*==============================================
						角度修正
	================================================*/
	//------------------------------
	//	角度が-3.14を超えてしまった
	//------------------------------
	// X の角度判定
	if (g_player.rot.x >= D3DX_PI)
	{
		g_player.rot.x -= D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	// Y の角度判定
	if (g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	// Z の角度判定
	if (g_player.rot.z >= D3DX_PI)
	{
		g_player.rot.z -= D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	//------------------------------
	//	角度が3.14を超えてしまった
	//------------------------------
	// X の角度判定
	if (g_player.rot.x <= -D3DX_PI)
	{
		g_player.rot.x += D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	// Y の角度判定
	if (g_player.rot.y <= -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}

	// Z の角度判定
	if (g_player.rot.z <= -D3DX_PI)
	{
		g_player.rot.z += D3DX_PI * F_DOUBLE;		//角度を１周分減らす
	}
}

//プレイヤーのリセット処理
void ResetPlayer(void)
{
	g_player.pos	= ZERO_SET;		// 位置初期化
	g_player.posOld = ZERO_SET;		// 前回の位置初期化
	g_player.move	= ZERO_SET;		// 移動量初期化
	g_player.rot	= ZERO_SET;		// 向き初期化
}