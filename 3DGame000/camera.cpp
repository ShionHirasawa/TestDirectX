/*==========================================================================================================================================================

														カメラ処理[camera.cpp]
															Author:平澤詩苑

============================================================================================================================================================*/
#include "camera.h"
#include "input.h"
#include "player.h"

#define POS_POSV			D3DXVECTOR3(0.0f, 200.0f, -300.0f)		//視点の初期位置
#define POS_POSR			D3DXVECTOR3(0.0f, 0.0f, 0.0f)			//注視点の初期位置
#define POS_VECU			D3DXVECTOR3(0.0f, 1.0f, 0.0f)			//上方向ベクトルの初期値
#define VIEW_ANGLE			(45.0f)									//視野角
#define MOVE_CAMERA_SPEED	(2.0f)									//カメラ・注視点座標の移動量
#define MIN_DRAW_Z			(10.0f)									//描画処理の最低奥行き
#define MAX_DRAW_Z			(2000.0f)								//描画処理の最高奥行き
#define MIN_LENGTH			(100.0f)								//視点と注視点の最低距離
#define MAX_LENGTH			(300.0f)								//視点と注視点の最大距離
#define CHANGE_LENGHT		(0.5f)									//長さの移動量
#define SPIN_ROT			(0.05f)									//回転量
#define COR_ROT_POSV		(3.14f)									//角度計算による視点操作の調整値
#define COR_ROT_POSR		(0.0f)									//角度計算による注視点操作の調整値

//グローバル変数
Camera g_Camera;													//カメラの情報

//カメラの初期化処理
void InitCamera(void)
{
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得
	g_Camera.posR = pPlayer->pos;	//注視点の位置をプレイヤーの位置に設定

	g_Camera.posV	= POS_POSV;		//視点初期化
	g_Camera.rot	= ZERO_SET;		//向き初期化
	g_Camera.vecU	= POS_VECU;		//上方向ベクトル初期化
	LengthCamera();					//カメラの距離初期化
	g_Camera.bHorming = true;		//カメラは追尾する
}

//カメラの終了処理
void UninitCamera(void)
{

}

//カメラの更新処理
void UpdateCamera(void)
{	
	Player *pPlayer = GetPlayer();	//プレイヤーの情報取得

	//カメラの追従がON
	if (g_Camera.bHorming == true)
	{
		g_Camera.posV += pPlayer->pos - pPlayer->posOld;	//プレイヤーが移動した分を視点にも同じように加算
		g_Camera.posR = pPlayer->pos;						//注視点の位置をプレイヤーの位置に設定
	}

	else
	{
		MovePosV();		//視点移動

		MovePosR();		//注視点移動
		SpinPosR();		//注視点回転
	}

	SpinPosV();		//視点回転

	//追従 ON / OFF
	//if (GetKeyboardTrigger(DIK_F7) == true)
	//{
	//	g_Camera.bHorming = g_Camera.bHorming ? false : true;

	//	//追従が ON になったときに注視点をプレイヤーに設定
	//	if (g_Camera.bHorming == true)
	//	{
	//		g_Camera.posR = pPlayer->pos;
	//	}
	//}

	//カメラの角度修正
	FixCameraRot();

	//視点間の長さ処理
	LengthCamera();

	//カメラの位置初期化
	ResetCameraPos();
}

//視点移動
void MovePosV(void)
{
	//視点、注視点ともに左に移動
	if (GetKeyboardPress(DIK_A) == true)
	{
		g_Camera.posV.x += sinf(LEFT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(LEFT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
									 
		g_Camera.posV.z += sinf(DOWN - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(DOWN - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//視点、注視点ともに右に移動
	if (GetKeyboardPress(DIK_D) == true)
	{
		g_Camera.posV.x += sinf(RIGHT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(RIGHT + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(UP - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(UP - g_Camera.rot.y) * MOVE_CAMERA_SPEED;		
	}

	//視点、注視点ともに奥に移動
	if (GetKeyboardPress(DIK_W) == true)
	{
		g_Camera.posV.x += sinf(UP + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(UP + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(RIGHT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(RIGHT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//視点、注視点ともに手前に移動
	if (GetKeyboardPress(DIK_S) == true)
	{
		g_Camera.posV.x += sinf(DOWN + g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.x += sinf(DOWN + g_Camera.rot.y) * MOVE_CAMERA_SPEED;

		g_Camera.posV.z += sinf(LEFT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
		g_Camera.posR.z += sinf(LEFT - g_Camera.rot.y) * MOVE_CAMERA_SPEED;
	}

	//視点上昇
	if (GetKeyboardPress(DIK_SPACE) == true || GetKeyboardPress(DIK_I) == true)
	{
		g_Camera.posV.y -= cosf(UP) * MOVE_CAMERA_SPEED;
	}

	//視点下降
	if (GetKeyboardPress(DIK_TAB) == true || GetKeyboardPress(DIK_K) == true)
	{
		g_Camera.posV.y -= cosf(DOWN) * MOVE_CAMERA_SPEED;
	}

	//視点離れる
	if (GetKeyboardPress(DIK_O) == true)
	{
		g_Camera.fLength += CHANGE_LENGHT;

		MoveCamera(&g_Camera.posR, &g_Camera.posV, COR_ROT_POSV);
	}

	//視点近づく
	if (GetKeyboardPress(DIK_L) == true)
	{
		//g_Camera.fLength -= CHANGE_LENGHT;

		MoveCamera(&g_Camera.posR, &g_Camera.posV, COR_ROT_POSV);
	}
}

//視点回転
void SpinPosV(void)
{
	//左旋回
	if (GetKeyboardPress(DIK_Z) == true)
	{
		g_Camera.rot.y -= SPIN_ROT;

		MoveCamera(&g_Camera.posR, &g_Camera.posV, COR_ROT_POSV);
	}

	//右旋回
	if (GetKeyboardPress(DIK_C) == true)
	{
		g_Camera.rot.y += SPIN_ROT;

		MoveCamera(&g_Camera.posR, &g_Camera.posV, COR_ROT_POSV);
	}
}

//注視点移動
void MovePosR(void)
{
	//注視点上昇
	if (GetKeyboardPress(DIK_SPACE) == true || GetKeyboardPress(DIK_U) == true)
	{
		g_Camera.posR.y -= cosf(UP) * MOVE_CAMERA_SPEED;
	}

	//注視点下降
	if (GetKeyboardPress(DIK_TAB) == true || GetKeyboardPress(DIK_J) == true)
	{
		g_Camera.posR.y -= cosf(DOWN) * MOVE_CAMERA_SPEED;

		//注視点が地面より下にもぐってしまった
		if (g_Camera.posR.y < ORIGIN_POS)
		{
			g_Camera.posR.y = ORIGIN_POS;	//注視点のY座標を原点に戻す
		}
	}
}

//注視点回転
void SpinPosR(void)
{
	if (GetKeyboardPress(DIK_Q) == true)
	{
		g_Camera.rot.y += SPIN_ROT;

		MoveCamera(&g_Camera.posV, &g_Camera.posR, COR_ROT_POSR);
	}

	if (GetKeyboardPress(DIK_E) == true)
	{
		g_Camera.rot.y -= SPIN_ROT;

		MoveCamera(&g_Camera.posV, &g_Camera.posR, COR_ROT_POSR);
	}
}

//角度修正
void FixCameraRot(void)
{
	//X方向の右回り修正(3.14以上)
	if (g_Camera.rot.x >= D3DX_PI)
	{
		g_Camera.rot.x = g_Camera.rot.x - FIX_ROT;
	}

	//X方向の左回り修正(-3.14以下)
	if (g_Camera.rot.x <= -D3DX_PI)
	{
		g_Camera.rot.x = g_Camera.rot.x + FIX_ROT;
	}

	//Y方向の右回り修正(3.14以上)
	if (g_Camera.rot.y >= D3DX_PI)
	{
		g_Camera.rot.y = g_Camera.rot.y - FIX_ROT;
	}

	//Y方向の左回り修正(-3.14以下)
	if (g_Camera.rot.y <= -D3DX_PI)
	{
		g_Camera.rot.y = g_Camera.rot.y + FIX_ROT;
	}

	//Z方向の右回り修正(3.14以上)
	if (g_Camera.rot.z >= D3DX_PI)
	{
		g_Camera.rot.z = g_Camera.rot.z - FIX_ROT;
	}

	//Z方向の左回り修正(-3.14以下)
	if (g_Camera.rot.z <= -D3DX_PI)
	{
		g_Camera.rot.z = g_Camera.rot.z + FIX_ROT;
	}
}

//カメラの移動処理
void MoveCamera(D3DXVECTOR3 *pParentPos, D3DXVECTOR3 *pChildPos, float CorRot)
{
	pChildPos->x = pParentPos->x + sinf((CorRot - g_Camera.rot.y)) * g_Camera.fLength;
	pChildPos->z = pParentPos->z + cosf((CorRot - g_Camera.rot.y)) * g_Camera.fLength;
}

//カメラの位置リセット
void ResetCameraPos(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_Camera.posV = POS_POSV;			//視点初期化
		g_Camera.posR = POS_POSR;			//注視点初期化
		g_Camera.rot = ZERO_SET;			//向き初期化
		g_Camera.vecU = POS_VECU;			//上方向ベクトル初期化
		LengthCamera();						//カメラの距離初期化
	}
}

//視点間の長さ処理
void LengthCamera(void)
{
	float PosDiffX, PosDiffY, PosDiffZ;

	PosDiffX = pow(g_Camera.posR.x - g_Camera.posV.x, 2.0f);	//2乗
	PosDiffY = pow(g_Camera.posR.y - g_Camera.posV.y, 2.0f);	//2乗
	PosDiffZ = pow(g_Camera.posR.z - g_Camera.posV.z, 2.0f);	//2乗

	//長さを算出
	g_Camera.fLength = sqrtf(PosDiffX + PosDiffY + PosDiffZ);

	//距離が離れすぎた
	if (g_Camera.fLength > MAX_LENGTH)
	{
		g_Camera.fLength = MAX_LENGTH;
	}

	//距離が近すぎた
	if (g_Camera.fLength < MIN_LENGTH)
	{
		g_Camera.fLength = MIN_LENGTH;
	}
}

//設定処理
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスへのポインタ

	//プロジェクションマトリックス初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_Camera.mtxProjection, D3DXToRadian(VIEW_ANGLE), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, MIN_DRAW_Z, MAX_DRAW_Z);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	//ビューマトリックス
	D3DXMatrixIdentity(&g_Camera.mtxview);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_Camera.mtxview,
						&g_Camera.posV,		//視点
						&g_Camera.posR,		//注視点
						&g_Camera.vecU);	//上方向ベクトル

	//ビューマトリックス
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxview);
}

//カメラの情報取得
Camera *GetCamera(void)
{
	return &g_Camera;
}