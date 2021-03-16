#include "DxLib.h"
#include "player.h"
#include "image.h"
#include "Feed.h"
int	g_OldKey;				// 前回の入力キー
int	g_NowKey;				// 今回の入力キー
int	g_KeyFlg;				// 入力キー情報
char key[256];

int GameState = 0;          //初期化　

//画面領域の大きさ
const int SCREEN_WIDTH = 1410;
const int SCREEN_HEIGHT = 800;

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

	SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);

	//SetGraphMode(800, 600, 16);

	if (DxLib_Init() == -1) return -1;	// DXライブラリの初期化処理

	SetDrawScreen(DX_SCREEN_BACK);	// 描画先画面を裏にする

	if (LoadImages() == -1)return -1; //画像呼び出し

	//ゲームループ
	while (ProcessMessage() == 0 && GetHitKeyStateAll(key) == 0 && GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

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

	//餌の初期化
	for (int i = 0; i < 10; i++) {
		eat[i].flg = FALSE;
	}
	//ゲームメインへ
	GameState = 1;
}

/***********************************************
/*ゲームメインの処理*/
/***********************************************/
void GameMain() {
	BackScrool();
	PlayerMove();
	EatImage();
	Hit();
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
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//透過
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);*/
	DrawGraph(ScroolSpeed % 640, 0, StageImage, TRUE);
	DrawGraph(640 + (ScroolSpeed % 640), 0, StageImage, TRUE);
	//設定を元に戻す。
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//エリアを戻す
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//岩画像表示

	////描画可能エリアを設定
	//SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH, 40, Iwa, TRUE);
	//DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH), 40, Iwa, TRUE);

	////エリアを戻す
	//SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	SetFontSize(30);
	//DrawFormatString(0, 0, 0x000000, "Level = %d", Leve);
	/*DrawFormatString(100, 160, 0x000000, "Scke = %f", Scke);*/
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


	/*act_index++;
	act_index %= MAX_MOTION_INDEX;*/
	int motion_index = anime[act_index];
	/*DrawGraph(player.x, player.y, sakana[motion_index], TRUE);*/
	//DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
	DrawGraph(player.x, player.y, sakana[Leve - 1][motion_index], TRUE);
	/*ScreenFlip();*/

}

int LoadImages() {
	//プレイヤー画像
	//魚レベル1
	if ((LoadDivGraph("Image/sakana.png", 10, 10, 1, 30, 30, sakana[0])) == -1)return -1;
	//魚レベル2
	if ((LoadDivGraph("Image/sakana2.png", 9, 10, 1, 100, 100, sakana[1])) == -1)return-1;
	//ステージ背景
	if ((StageImage = LoadGraph("Image/Hikei.png")) == -1) return -1;
	//手前の背景
	if ((Iwa = LoadGraph("Image/手前の背景2.png")) == -1) return -1;
	//餌(食べれる生き物)画像
	//エビ
	if ((feedImage[0] = LoadGraph("Image/ebi.png")) == -1)return 0;
	//アジ
	if ((feedImage[1] = LoadGraph("Image/azi.png")) == -1)return 0;
	//イカ
	if ((feedImage[2] = LoadGraph("Image/ika.png")) == -1)return 0;

	return 0;
}

void EatImage() {

	/*for (int i = 0; i < 10; i++) {
		if (eat[i].flg == FALSE) {

		}
	}*/

	DrawGraph(e_x + ((ScroolSpeed * 2) % SCREEN_WIDTH), e_y, feedImage[0], FALSE);
	//スクロールしている間の座標を格納している
	e_xs = e_x + ((ScroolSpeed * 2) % SCREEN_WIDTH);

	/*DrawFormatString(100,100,0xffffff,"x %d y %d",e_xs,e_ys);*/
}

//成長
void PlayerGrowth() {

	//プレイヤーのサイズ変更
	player.w *= Scke;
	player.h *= Scke;

	//サイズの変更量の増加
	Scke++;
	//レベルを上げる
	Leve++;
}

//ゲージ
void PlayerEat() {

	//食べたものを量を増加させる
	EatAmount++;

	//食べたものの量が一定量に達したら処理を移す
	if (EatAmount <= LeveUp) {
		PlayerGrowth();
	}
}

void Hit() {

	//魚に当たったら場合の処理
	if (e_xs + 40 >= player.x && e_xs <= player.x + 30 &&
		e_y + 40 >= player.y && e_y <= player.y + 30) {
		feedImage[0] = 0;
		PlayerEat();
	}

}
