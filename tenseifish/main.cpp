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
	//食べた量個別
	em = 0;
	am = 0;
	im = 0;
	EatAmount = 0;
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
	EatMove();
	LifeImage();
	MeterImage();
}
/*************************************
 *背景画像スクロール処理
 *引数：なし
 *戻り値：なし
 *************************************/
void BackScrool()
{

	Range += player.speed;

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

	//描画可能エリアを設定
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	static int count = 0;
	count = (count + 1) % 2541;
	

	if (count > 0 && count < 139) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH + 1410, 100, Iwa[0], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH) + 1410, 100, Iwa[0], TRUE);
	}
	if (count > 139 && count < 694 + 139) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH, 100, Iwa[0], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH), 100, Iwa[0], TRUE);
	}
	if (count > 847 && count < 986) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH + 1410, 100, Iwa[1], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH) + 1410, 100, Iwa[1], TRUE);
	}
	if (count > 986 && count < 1541 + 139) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH, 100, Iwa[1], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH), 100, Iwa[1], TRUE);
	}
	if (count > 1694 && count < 1833) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH + 1410, 100, Iwa[2], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH) + 1410, 100, Iwa[2], TRUE);
	}
	if (count > 1833 && count < 2388 + 139) {
		DrawGraph((ScroolSpeed * 2) % SCREEN_WIDTH, 100, Iwa[2], TRUE);
		DrawGraph(SCREEN_WIDTH + ((ScroolSpeed * 2) % SCREEN_WIDTH), 100, Iwa[2], TRUE);
	}

	//エリアを戻す
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//レベル表示
	SetFontSize(60);
	DrawFormatString(0, 0, 0x000000, "Lv.%d", Leve);
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

	//画面からはみ出さないようにする
	if (player.x < 0)player.x = 0;
	if (player.x > SCREEN_WIDTH - player.w)player.x = SCREEN_WIDTH - player.w;
	if (player.y < player.h/Scke)player.y = player.h/Scke;
	if (player.y > SCREEN_HEIGHT - player.h)player.y = SCREEN_HEIGHT - player.h;

	/*act_index++;
	act_index %= MAX_MOTION_INDEX;*/
	int motion_index = anime[act_index];
	/*DrawGraph(player.x, player.y, sakana[motion_index], TRUE);*/
	DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
	//DrawGraph(player.x, player.y, sakana[Leve - 1][motion_index], TRUE);
	/*ScreenFlip();*/

}

int LoadImages() {
	//プレイヤー画像
	//魚レベル1
	if ((LoadDivGraph("Image/sakana.png", 10, 10, 1, 30, 30, sakana[0])) == -1)return -1;
	//魚レベル2
	/*if ((LoadDivGraph("Image/sakana2.png", 9, 10, 1, 100, 100, sakana[1])) == -1)return-1;*/
	//魚レベル2
	if ((LoadDivGraph("Image/sakana2.png", 3, 3, 1, 150, 150, sakana[1])) == -1)return-1;
	//ステージ背景
	if ((StageImage = LoadGraph("Image/Hikei.png")) == -1) return -1;
	//手前の背景
	if ((Iwa[0] = LoadGraph("Image/temae.png")) == -1) return -1;
	if ((Iwa[1] = LoadGraph("Image/temae2.png")) == -1) return -1;
	if ((Iwa[2] = LoadGraph("Image/temae3.png")) == -1) return -1;
	//餌(食べれる生き物)画像
	//エビ
	if ((feedImage[0] = LoadGraph("Image/ebi.png")) == -1)return 0;
	//アジ
	if ((feedImage[1] = LoadGraph("Image/azi.png")) == -1)return 0;
	//イカ
	if ((feedImage[2] = LoadGraph("Image/ika.png")) == -1)return 0;

	//UI画像
	//ライフ
	if ((Life = LoadGraph("Image/Life.png")) == -1)return 0;
	//メーター
	for (int i = 0; i < 3; i++) {
		if ((Meter[i][0] = LoadGraph("Image/Meter.png")) == -1)return 0;
		//エビメーター
		if (i == 0) {
			if ((Meter[i][1] = LoadGraph("Image/MeterE1.png")) == -1)return 0;
			if ((Meter[i][2] = LoadGraph("Image/MeterE2.png")) == -1)return 0;
			if ((Meter[i][3] = LoadGraph("Image/MeterE3.png")) == -1)return 0;
			if ((Meter[i][4] = LoadGraph("Image/MeterE4.png")) == -1)return 0;
			if ((Meter[i][5] = LoadGraph("Image/MeterE5.png")) == -1)return 0;
		}
		//アジメーター
		if (i == 1) {
			if ((Meter[i][1] = LoadGraph("Image/MeterA1.png")) == -1)return 0;
			if ((Meter[i][2] = LoadGraph("Image/MeterA2.png")) == -1)return 0;
			if ((Meter[i][3] = LoadGraph("Image/MeterA3.png")) == -1)return 0;
			if ((Meter[i][4] = LoadGraph("Image/MeterA4.png")) == -1)return 0;
			if ((Meter[i][5] = LoadGraph("Image/MeterA5.png")) == -1)return 0;
		}
		//イカメーター
		if (i == 2) {
			if ((Meter[i][1] = LoadGraph("Image/MeterI1.png")) == -1)return 0;
			if ((Meter[i][2] = LoadGraph("Image/MeterI2.png")) == -1)return 0;
			if ((Meter[i][3] = LoadGraph("Image/MeterI3.png")) == -1)return 0;
			if ((Meter[i][4] = LoadGraph("Image/MeterI4.png")) == -1)return 0;
			if ((Meter[i][5] = LoadGraph("Image/MeterI5.png")) == -1)return 0;
		}
	}

	return 0;
}

void EatMove() {
	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == TRUE) {
			//餌の表示
			DrawGraph(eat[i].e_x, eat[i].e_y, eat[i].image, TRUE);

			//真っすぐ左に移動
			eat[i].e_x -= 10;

			if (eat[i].flg == FALSE)continue;

			//画面外に行ったら削除
			if (eat[i].e_x < 0 - 40)eat[i].flg = FALSE;

			//魚に当たったら場合の処理
			if (Hit(&player, &eat[i]) == TRUE) {
				PlayerEat(&eat[i].type);
				eat[i].flg = FALSE;
			}
		}
	}

	//餌の設定
	if (Range / 5 % 50 == 0) {
		EatImage();
	}
}

int EatImage() {

	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == FALSE) {
			eat[i] = eat0;
			eat[i].type = GetRand(2);
			eat[i].image = feedImage[eat[i].type];
			switch (eat[i].type) {
			case 0:
				eat[i].e_y = (GetRand(1) + 4) * 100 + 150;
				break;
			case 1:
				eat[i].e_y = GetRand(2) * 100 + 150;
				break;
			case 2:
				eat[i].e_y = GetRand(5) * 100 + 150;
				break;
			}
			return TRUE;
		}
	}

	return FALSE;
}

//ライフ
void LifeImage() {

	DrawGraph(LifeX, LIfeY, Life, TRUE);
	DrawGraph(LifeX + 60, LIfeY, Life, TRUE);
	DrawGraph(LifeX + 120, LIfeY, Life, TRUE);
}

//メーター表示
void MeterImage() {

	//エビメーター
	DrawGraph(m_x-10, m_y, Meter[0][em], TRUE);
	DrawGraph(0, 80, feedImage[0], TRUE);

	//アジメーター
	DrawGraph(m_x+240 , m_y, Meter[1][am], TRUE);
	DrawGraph(237, 75, feedImage[1], TRUE);

	//イカメーター
	DrawGraph(m_x+480, m_y, Meter[2][im], TRUE);
	DrawGraph(480, 65, feedImage[2], TRUE);

}

//成長
void PlayerGrowth() {

	//サイズの変更量の増加
	Scke++;

	//プレイヤーのサイズ変更
	player.w *= Scke;
	player.h *= Scke;
	//レベルを上げる
	Leve++;
}

//食べた時の処理
void PlayerEat(int* e) {

	switch (*e) {
	case 0:
		if (em < 5) em++;
		break;
	case 1:
		if (am < 5)am++;
		break;
	case 2:
		if (im < 5)im++;
		break;
	}

	//食べたものを量を増加させる
	EatAmount++;

	//食べたものの量が一定量に達したら処理を移す
	if (EatAmount == LeveUp) {
		PlayerGrowth();
	}
}

//餌とプレイヤーのあたり判定
int Hit(Player* p, Eat* e) {

	int px = p->x + 30 * Scke;
	int py = p->y + 45 * Scke;
	int ph = p->h - 40 * Scke;
	int pw = p->w - 80 * Scke;
	int ex = e->e_x;
	int ey = e->e_y;
	int et = e->type;

	//餌とのあたり判定判定
	if (et == 0) {
		if (ex + 40 >= px && ex <= px + ph &&
			ey + 40 >= py && ey <= py + pw) {
			e->flg = FALSE;
			return TRUE;
		}
	}
	else {
		if (ex + 50 >= px && ex <= px + ph &&
			ey + 50 >= py && ey <= py + pw) {
			e->flg = FALSE;
			return TRUE;
		}
	}

	return FALSE;
}
