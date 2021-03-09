#include "DxLib.h"
#include "player.h"
#include "image.h"

int	g_OldKey;				// 前回の入力キー
int	g_NowKey;				// 今回の入力キー
int	g_KeyFlg;				// 入力キー情報
char key[256];

int GameState = 0;          //初期化　

//画面領域の大きさ
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int ScroolSpeed;

void GameInit();		//ゲーム初期化処理
void GameMain();		//ゲームメイン処理
void BackScrool();         //背景画像スクロール処理

int LoadImages();          //画像読み込み

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	SetMainWindowText("a");// タイトル変更

	ChangeWindowMode(TRUE);		// ウィンドウモードで起動

	if (DxLib_Init() == -1) return -1;	// DXライブラリの初期化処理

	SetDrawScreen(DX_SCREEN_BACK);	// 描画先画面を裏にする

	if (LoadImages() == -1)return -1; //画像呼び出し

	//ゲームループ
	while (ProcessMessage() == 0 && GetHitKeyStateAll(key) == 0 &&GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

		//入力キー取得
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		ClearDrawScreen();		// 画面の初期化

		switch (GameState) {
		case 0:
			GameInit();		//ゲーム初期処理
			break;
		case 1:
			GameMain();		//ゲームメイン処理
			break;
		}
		ScreenFlip();			// 裏画面の内容を表画面に反映
	}

	DxLib_End();	// DXライブラリ使用の終了処理
    return 0;	// ソフトの終了
}
/***********************************************
 * ゲーム初期処理
 ***********************************************/
void GameInit() {
	//スクロール速度を初期化
	ScroolSpeed = 0;
	//プレイヤーの初期化
	player.flg = TRUE;
	player.x = PLAYER_POS_X;
	player.y = PLAYER_POS_Y;
	player.w = PLAYER_WIDTH;
	player.h = PLAYER_HEIGHT;
	player.speed = PLAYER_SPEED;
	//ゲームメインへ
	GameState = 1;

}

/***********************************************
/*ゲームメインの処理*/
/***********************************************/
void GameMain() {
	BackScrool();
    PlayerMove();
}
/*************************************
 *背景画像スクロール処理
 *引数：なし
 *戻り値：なし
 *************************************/
void BackScrool()
{
	if (GameState == 1) {
		ScroolSpeed -= player.speed;

	}
	
	//ステージ画像表示

	//描画可能エリアを設定
	SetDrawArea(0, 0,640, 480);
	DrawGraph(ScroolSpeed % 640, 0, StageImage, FALSE);
	DrawGraph(640 + (ScroolSpeed % 640), 0, StageImage, FALSE);
	
	//エリアを戻す
	SetDrawArea(0,0,640,480);

}

void PlayerMove() {
	
	if (--act_wait <= 0)
	{
		act_index++;
		act_wait = ACT_SPEED;
		act_index %= MAX_MOTION_INDEX;
	}

	//上下左右移動
	if (player.flg == TRUE) {
		if (g_NowKey & PAD_INPUT_UP)player.y -= player.speed;
		if (g_NowKey & PAD_INPUT_DOWN)player.y += player.speed;
		if (g_NowKey & PAD_INPUT_LEFT)player.x -= player.speed;
		if (g_NowKey & PAD_INPUT_RIGHT)player.x += player.speed;
	}

	
	act_index++;
	act_index %= MAX_MOTION_INDEX;
	int motion_index = anime[act_index];
	DrawGraph(player.x, player.y,sakana[motion_index],TRUE);
	ScreenFlip();
	
}
int LoadImages() {
	//プレイヤー画像
	if((LoadDivGraph("Image/sakana.png", 10,10,1,30,30,sakana))==-1)return -1;
	//ステージ背景
	if ((StageImage = LoadGraph("Image/bg_natural_ocean.jpg")) == -1) return -1;
	return 0;
}