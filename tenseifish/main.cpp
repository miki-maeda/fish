#include "DxLib.h"
#include "player.h"
#include "image.h"
#include "Feed.h"
#include "Boss.h"
#include "Sound.h"
#include "CollisionDetiction.h"
int	g_OldKey;				// 前回の入力キー
int	g_NowKey;				// 今回の入力キー
int	g_KeyFlg;				// 入力キー情報
char key[256];

int GameState = 0;          //初期化　

//画面領域の大きさ
const int SCREEN_WIDTH = 1410;
const int SCREEN_HEIGHT = 800;

int ScroolSpeed;
int MenuNo2 = 0;


void GameTitle();
void GameInit();		//ゲーム初期化処理
void GameMain();		//ゲームメイン処理
void GameHelp();		//ゲームヘルプ処理
void BackScrool();         //背景画像スクロール処理
void GameClear();		//ゲームクリア処理
void GameOver();
void Pouse();
void GameGiyo();
void GameGiyo2();
void GameGiyo3();
void GameRule();
void GameRule2();
void dieam();
void AwaAnime();


int LoadImages();          //画像読み込み
int LoadSound();		// 音楽読込み

int Time = 2400;


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

	if (LoadSound() == -1)return -1;	// 音楽の読み込み

	//ゲームループ
	while (ProcessMessage() == 0 && GetHitKeyStateAll(key) == 0 && GameState != 99 && !(g_KeyFlg & PAD_INPUT_START)) {

		if (key1 < 1) {
			Time--;
		}

		//入力キー取得
		g_OldKey = g_NowKey;
		g_NowKey = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		g_KeyFlg = g_NowKey & ~g_OldKey;

		ClearDrawScreen();		// 画面の初期化

		switch (GameState) {
		case 0:
			GameTitle();
			break;
		case 1:
			GameInit();		//ゲーム初期処理
			break;
		case 2:
			GameMain();		//ゲームメイン処理
			break;
		case 3:
			GameClear();
			break;
		case 4:
			BossInit();
			break;
		case 5:
			BossStage();
			break;
		case 6:
			GameHelp();
			break;
		case 7:
			GameOver();
			break;
		case 8:
			GameGiyo();
			break;
		case 9:
			GameGiyo2();
			break;
		case 10:
			GameGiyo3();
			break;
		case 11:
			GameRule();
			break;
		case 12:
			GameRule2();
			break;
		case 13:
			Musicmixer();
			break;
		case 14:
			PictureBook1();
			break;
		case 15:
			PictureBook2();
			break;
		}
		ScreenFlip();			// 裏画面の内容を表画面に反映
	}

	DxLib_End();	// DXライブラリ使用の終了処理
	return 0;	// ソフトの終了
}

void GameTitle() {
	static bool push = 0;	// 押されたかどうか確認する関数

	SHIPFlg = FALSE;
	if (CheckSoundMem(TitleSound) == 0) {
		if (BGMFlg == FALSE) {
			PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
		}
	}
	StopSoundMem(HelpSound);
	
	DrawGraph(0, 0, Gametitle, TRUE);
	// メニューカーソルの描画
	DrawRotaGraph(465 + MenuNo2 * 340, 415 + MenuNo * 108, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo2 > 1)MenuNo2 = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo2 < 0)MenuNo2 = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0 && MenuNo2 == 0) {
			push = 0;
			GameState = 1;
		}
		else if (MenuNo == 0&& MenuNo2 == 1) {
			push = 0;
			GameState = 6;
			MenuNo = 0;
			MenuNo2 = 0;
			if (BGMFlg == FALSE) {
				PlaySoundMem(HelpSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
		else if (MenuNo == 1&& MenuNo2 == 0) {
			push = 0;
			GameState = 14;
			MenuNo = 0;
			MenuNo2 = 0;
			if (BGMFlg == FALSE) {
				PlaySoundMem(HelpSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
		else {
			push = 0;
			DxLib_End();
		}
	}
}

void GameHelp()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamehelp, TRUE);

	StopSoundMem(TitleSound);

	// メニューカーソルの描画
	DrawRotaGraph(530, 275 + MenuNo * 140, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 3)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 3;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 8;
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 11;
			MenuNo = 0;
		}
		else if (MenuNo == 2) {
			push = 0;
			GameState = 13;
			MenuNo = 0;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			if (BGMFlg == FALSE) {
				PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
	}
}

void GameGiyo()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamegiyo, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(455 + MenuNo * 325, 760, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 9;
		}
		else {
			push = 0;
			GameState = 6;
			MenuNo = 0;
		}
	}
}

void GameGiyo2()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamegiyo2, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(460 + MenuNo * 360, 755, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 10;
		}
		else {
			push = 0;
			GameState = 8;
			MenuNo = 0;
		}
	}
}

void GameGiyo3()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamegiyo3, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(400 + MenuNo * 320, 775, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 6;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			StopSoundMem(HelpSound);
		}
	}
}

void GameRule()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamerule, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(500 + MenuNo * 260, 770, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 12;
		}
		else {
			push = 0;
			GameState = 6;
			MenuNo = 0;
		}
	}
}

void GameRule2()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, Gamerule2, TRUE);

	// メニューカーソルの描画
	DrawRotaGraph(345 + MenuNo * 460, 755, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}



	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 6;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
		}
	}
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
	Leve = 1;
	LeveC = Leve;
	Scke = 1;
	LeveUp = 10;
	RandEat = 3;
	Deatflg = FALSE;
	Dx = 0, Dy = 0, Dw = 0, Dh = 0;
	DSpeed = 0;

	//当たり判定用変数の初期化
	CollXadd = 0;
	CollYadd = 0;
	PointNom = 0;

	BRMove = 0;
	BRflg = FALSE;
	if (BRflg == FALSE) {
		boss.bx = -500;
		boss.by = 200;
	}

	//プレイヤーの初期化
	player.flg = TRUE;
	player.x = PLAYER_POS_X;
	player.y = PLAYER_POS_Y;
	player.w = PLAYER_WIDTH;
	player.h = PLAYER_HEIGHT;
	player.speed = PLAYER_SPEED;
	LifeMax = 3;
	player.life = LifeMax;
	player.muteki = 0;
	Umispeed = 0;
	Time = 2400;
	/*Time = 60;*/
	Iwaspeed = 0;
	motion_index7 = 0;
	LeveUpflg1 = FALSE;
	LeveUpflg2 = FALSE;
	LCount = 0;
	countS = 0;
	bm = 0.143;
	boss_Y = 923;
	MovieCount = 0;
	//餌の初期化
	for (int i = 0; i < 10; i++) {
		eat[i].flg = FALSE;
	}

	motion_index9 = 0;
	sibuki_act_index = 0;
	//ゲームメインへ
	GameState = 2;

	StopSoundMem(TitleSound);
	if (BGMFlg == FALSE) {
		PlaySoundMem(MainSound, DX_PLAYTYPE_BACK, TRUE);
	}
	AWA_Y = 700;
	AWA_X = 700;
	AWA_aX = 850;
	AWA_aY = 600;
	AWA_bX = 1040;
	AWA_bY = 750;
	AWA_rX = 1300;
	AWA_rY = 650;
	AWA_cX = 1410;
	AWA_cY = 750;
}

/***********************************************
/*ゲームメインの処理*/
/***********************************************/
void GameMain() {
	BackScrool();
	PlayerMove();
	EatMove();
	if (Time > 60) {
		LifeImage();
		MeterImage();
		BossMeter();
	}
	BossST(&player);
	Pouse();
	AwaAnime();
}
/*************************************
 *背景画像スクロール処理
 *引数：なし
 *戻り値：なし
 *************************************/
void BackScrool()
{

	Range += player.speed;

	if (GameState == 2) {
		if (Time > 0) {
			ScroolSpeed -= player.speed;
		}
	}

	//ステージ画像表示

	//描画可能エリアを設定
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//透過
	/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 122);*/
	DrawGraph(Umispeed % SCREEN_WIDTH, 0, StageImage, TRUE);
	DrawGraph(SCREEN_WIDTH + (Umispeed % SCREEN_WIDTH), 0, StageImage, TRUE);
	//設定を元に戻す。
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//エリアを戻す
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	//気泡表示
	AwaAnime();
	if (AWA_Y > 148 && AWA_X > 0) {
		DrawExtendGraph(AWA_X, AWA_Y, AWA_X + 27, AWA_Y + 32, awa[motion_index10], TRUE);
	}
	else {
		AWA_X = 700;
		AWA_Y = 700;
	}
	if (AWA_bY > 148 && AWA_bX > 0) {
		DrawExtendGraph(AWA_bX, AWA_bY, AWA_bX + 27, AWA_bY + 32, awa[motion_index10], TRUE);
	}
	else {
		AWA_bX = 1040;
		AWA_bY = 750;
	}

	if (AWA_aY > 148 && AWA_aX > 0) {
		DrawExtendGraph(AWA_aX, AWA_aY, AWA_aX + 27, AWA_aY + 32, awa[motion_index10], TRUE);
	}
	else {
		AWA_aX = 850;
		AWA_aY = 600;
	}
	if (AWA_rY > 148 && AWA_rX > 0) {
		DrawExtendGraph(AWA_rX, AWA_rY, AWA_rX + 27, AWA_rY + 32, awa[motion_index10], TRUE);
	}
	else {
		AWA_rX = 1300;
		AWA_rY = 650;
	}
	if (AWA_cY > 148 && AWA_cX > 0) {
		DrawExtendGraph(AWA_cX, AWA_cY, AWA_cX + 27, AWA_cY + 32, awa[motion_index10], TRUE);
	}
	else {
		AWA_cX = 1410;
		AWA_cY = 750;
	}

	//岩画像表示

	//描画可能エリアを設定
	SetDrawArea(0, 0, IwaHaba, SCREEN_HEIGHT);

	if (Time > 60) {
		DrawGraph(Iwaspeed % IwaHaba, 50, Iwa[0], TRUE);
		DrawGraph(IwaHaba + (Iwaspeed % IwaHaba), 50, Iwa[0], TRUE);
	}
	if (Time < 59) {
		BossMovie();
	}

	//エリアを戻す
	SetDrawArea(0, 0, IwaHaba, SCREEN_HEIGHT);


	//レベル表示
	SetFontSize(60);
	DrawFormatString(0, 0, 0x000000, "Lv.%d", LeveC);
}
void BossMovie() {
	DrawBox(0, 0, 1410, MY, GetColor(0, 0, 0), TRUE);
	DrawBox(0, 800, 1410, MY2, GetColor(0, 0, 0), TRUE);
	DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);
	if (key1 < 1) {
		
		//DrawFormatString(100, 160, 0x000000, "%d", MovieCount);
		player.flg = FALSE;
		MovieCount = (MovieCount + 1) % 200;
		if (MovieCount == 1) {
			player.x = 500;
			player.y = 400;
			MY = 0;
			MY2 = 800;
		}
		if (MovieCount < 50) {
			if (MY < 130) {
				MY += 5;
			}
			if (MY2 > 670) {
				MY2 -= 5;
			}
		}
		if (MovieCount > 50) {
			player.x += player.speed;
		}

		if (MovieCount > 50) {
			if (BRflg == FALSE) {

				if (--BOSS_act_wait <= 0)
				{

					BOSS_act_index++;
					BOSS_act_wait = BOSS_ACT_SPEED;
					BOSS_act_index %= BOSS_MOTION_INDEX;

				}

				motion_index2 = BOSSAnime[BOSS_act_index];
				//BRflg = TRUE;
				boss.flg = TRUE;
				boss.dir = 0;

				boss.bx += 5;
				boss.by = BOSS_POS_Y;
				boss.bw = BOSS_WIDTH;
				boss.bh = BOSS_HEIGHT;

				//DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);
			}
		}
		if (BRMove == 0) {
			Iwaspeed = 0;
			BRMove = 1;
		}
		//DrawGraph(800, 50, BOSSROAD, TRUE);

	}
}
void AwaAnime() {

	if (key1 < 1) {
		if (AWA_Y > 148 || AWA_aY > 148 || AWA_bY > 148 || AWA_rY > 148 || AWA_cY > 148) {
			if (Leve == 1) {
				AWA_Y -= 1;
				AWA_aY -= 1;
				AWA_bY -= 1;
				AWA_rY -= 1;
				AWA_cY -= 1;

			}
			else if (Leve == 2) {
				AWA_Y -= 1;
				AWA_aY -= 1;
				AWA_bY -= 1;
				AWA_rY -= 1;
				AWA_cY -= 1;
			}
			else {
				AWA_Y -= 1;
				AWA_aY -= 1;
				AWA_bY -= 1;
				AWA_rY -= 1;
				AWA_cY -= 1;
			}
			if (player.flg == TRUE && LeveUpflg1 != TRUE) {
				if (Leve == 1) {
					AWA_X -= 1;
					AWA_aX -= 1;
					AWA_bX -= 1;
					AWA_rX -= 1;
					AWA_cX -= 1;
				}
				else if (Leve == 2) {
					AWA_X -= 2;
					AWA_aX -= 2;
					AWA_bX -= 2;
					AWA_rX -= 2;
					AWA_cX -= 2;
				}
				else {
					AWA_X -= 3;
					AWA_aX -= 3;
					AWA_bX -= 3;
					AWA_rX -= 3;
					AWA_cX -= 3;
				}
			}
		}
	}


	if (--awa_act_wait <= 0)
	{
		if (key1 < 1) {
			awa_act_index++;
			awa_act_wait = awa_ANI_SPEED;
			awa_act_index %= awa_MOTION_INDEX;
		}
		motion_index10 = awaanime[awa_act_index];
	}

}


void PlayerMove() {
	
	if (--act_wait <= 0)
	{
		if (key1 < 1) {
			act_index++;
			act_wait = ACT_SPEED;
			act_index %= MAX_MOTION_INDEX;
		}
	}
	if (key1 < 1) {
		//上下左右移動
		if (player.flg == TRUE) {
			if (g_NowKey & PAD_INPUT_UP)player.y -= player.speed;
			if (g_NowKey & PAD_INPUT_DOWN)player.y += player.speed;
			if (g_NowKey & PAD_INPUT_LEFT)player.x -= player.speed;
			if (g_NowKey & PAD_INPUT_RIGHT)player.x += player.speed;
		}
	}

	//画面からはみ出さないようにする
	if (player.x < 0)player.x = 0;
	if (player.x > SCREEN_WIDTH - player.w)player.x = SCREEN_WIDTH - player.w;
	if (Leve == 1) {
		if (player.y < player.h / Scke + 50)player.y = player.h / Scke + 50;
	}
	if (Leve == 2) {
		if (player.y < player.h / Scke)player.y = player.h / Scke;
	}
	if (Leve == 3) {
		if (player.y < player.h / Scke-100)player.y = player.h / Scke-100;
	}
	if (player.y > SCREEN_HEIGHT - player.h)player.y = SCREEN_HEIGHT - player.h;

	if (Damege != 1 && LeveUpflg1 != TRUE ) {
		if (Leve == 1 && key1 != 1  && key2 != 1) {
			Umispeed -= 2;
			Iwaspeed -= 5;
		}
		if (Leve == 2 && key1 != 1 && key2 != 1) {
			Umispeed -= 4;
			Iwaspeed -= 7;
			player.speed = 8;
		}
		if (Leve == 3 && key1 != 1 && key2 != 1) {
			Umispeed -= 6;
			Iwaspeed -= 9;
			player.speed = 9;
		}
	}

	if (Leve != 2) {
		CollXadd = 10;
		if (Leve < 2) CollYadd = 7;
		if (Leve > 2) CollYadd = 10;
	}
	else if (Leve == 2) {
		CollXadd = 40;
		CollYadd = 10;
	}

	int ColorCheck(int x, int y);

	if (Leve == 1) {
		ColorCheck(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd);		//左上
		ColorCheck(player.x + player.w / 2 + CollXadd * 3, player.y + player.h / 2 - CollYadd);	//右上
		ColorCheck(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 + CollYadd);		//左下
		ColorCheck(player.x + player.w / 2 + CollXadd * 3, player.y + player.h / 2 + CollYadd);	//右下
	}
	else if (Leve == 2) {
		//if (Leve == 2) DrawBox(player.x + player.w / 2, player.y + player.h / 2 - CollYadd / 4, player.x + player.w / 2 + CollXadd, player.y + player.h / 2 + CollYadd + 2, GetColor(0, 255, 0), TRUE);

		ColorCheck(player.x + player.w / 2, player.y + player.h / 2 - CollYadd / 4);		//左上
		ColorCheck(player.x + player.w / 2 + CollXadd, player.y + player.h / 2 + CollYadd + 2);		//右上
		ColorCheck(player.x + player.w / 2, player.y + player.h / 2 - CollYadd / 4);		//左下
		ColorCheck(player.x + player.w / 2 + CollXadd, player.y + player.h / 2 + CollYadd + 2);		//右下
	}
	else if (Leve == 3) {
		ColorCheck(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd);		//左上
		ColorCheck(player.x + player.w / 2 + CollXadd * 5 + 5, player.y + player.h / 2 + CollYadd * 2);		//右上
		ColorCheck(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd);		//左下
		ColorCheck(player.x + player.w / 2 + CollXadd * 5 + 5, player.y + player.h / 2 + CollYadd * 2);		//右下
	}

	if (player.muteki != 0 && Damege != 0 && player.life > 0) {
		if (key1 < 1) {
			player.flg = FALSE;
			if (PointNom == 1) player.x += 3;
			if (PointNom == 2) player.x -= 1;
			Feedflg = TRUE;
			player.y -= 5;
		}
	}
	else {
		PointNom = 0;
		Damege = 0;
		player.flg = TRUE;
	}
	if (Feedflg == TRUE) {
		if (key1 < 1) {
			DEat();
		}
	}

	/*act_index++;
	act_index %= MAX_MOTION_INDEX;*/
	//DrawGraph(player.x, player.y, sakana[Leve - 1][motion_index], TRUE);
	int motion_index = anime[act_index];
	if (player.muteki == 0&&key2 < 1) {
		if (key2 < 1) {
			DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
		}
	}

	if (player.muteki == 1) {
		if (key1 < 1 && key2 < 1) {

			// ダメージが入ると５回のうち２回表示する。
			static int count = 0;
			count = (count + 1) % 20;
			if (count % 2 == 0) {
				//表示
				DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, sakana[Leve - 1][motion_index], TRUE);
			}
			if (count == 19 || player.life == 0) {
				player.muteki = 0;

			}
		}
		
	}

	if (LeveUpflg1 == TRUE) {
		if (key1 < 1 || key2 < 1) {
			player.flg = FALSE;
			// ダメージが入ると５回のうち２回表示する。
			LCount = (LCount + 1) % 21;
			if (LCount % 5 == 0) {
				if (LeveUpflg2 != TRUE) {
					if (Leve == 1) {
						player.w *= Scke;
						player.h *= Scke;
						player.x -= PLAYER_WIDTH / 2;
						player.y -= PLAYER_HEIGHT / 2;
						Leve = 2;
					}
					else if (Leve == 2) {
						player.w = PLAYER_WIDTH;
						player.h = PLAYER_HEIGHT;
						player.x += player.w / 2;
						player.y += player.h / 2;
						Leve = 1;
					}
				}
				if (LeveUpflg2 == TRUE) {
					if (Leve == 2) {
						Leve = 3;
						Scke = 1.5;
					}
					else if (Leve == 3) {
						Leve = 2;
						Scke = 3;
					}
				}
			}

			if (LCount >= 20 || player.life == 0) {
				LeveUpflg1 = FALSE;
				LeveUpflg2 = FALSE;
				player.flg = TRUE;
				LCount = 0;
				LeveC = Leve;
			}
		}
	}
	
	//当たり判定の範囲確認用BOX
	//if (Leve == 1) DrawBox(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd, player.x + player.w / 2 + CollXadd * 3, player.y + player.h / 2 + CollYadd, GetColor(255, 0, 0), TRUE);		//右下
	//if (Leve == 2) DrawBox(player.x + player.w / 2, player.y + player.h / 2 - CollYadd / 4, player.x + player.w / 2 + CollXadd, player.y + player.h / 2 + CollYadd + 2, GetColor(0, 255, 0), TRUE);		//右下
	//if (Leve == 3) DrawBox(player.x + player.w / 2 - CollXadd, player.y + player.h / 2 - CollYadd, player.x + player.w / 2 + CollXadd * 5 + 5, player.y + player.h / 2 + CollYadd * 2, GetColor(0, 0, 255), TRUE);		//右下
}

int LoadImages() {
	//タイトル画像
	if ((Gametitle = LoadGraph("Image/umi.png")) == -1)return -1;
	//ゲームヘルプ画像
	if ((Gamehelp = LoadGraph("Image/GameHelp2.png")) == -1)return -1;
	//ゲーム概要画像
	if ((Gamegiyo = LoadGraph("Image/ゲーム概要.png")) == -1)return -1;
	//ゲーム概要画像2
	if ((Gamegiyo2 = LoadGraph("Image/ゲーム概要2.png")) == -1)return -1;
	//ゲーム概要画像3
	if ((Gamegiyo3 = LoadGraph("Image/ゲーム概要3.png")) == -1)return -1;
	// ゲームルール画像
	if ((Gamerule = LoadGraph("Image/ゲームルール1.png")) == -1)return -1;
	// ゲームルール2画像
	if ((Gamerule2 = LoadGraph("Image/ゲームルール2.png")) == -1)return -1;
	//図鑑の背景画像
	if ((Bookhaikei[0] = LoadGraph("Image/zukanhaikei.png")) == -1)return -1;
	if ((Bookhaikei[1] = LoadGraph("Image/zukanhaikei2.png")) == -1)return -1;
	// ゲーム音量調整画像
	if ((GameMS = LoadGraph("Image/GameMusicSelect.png")) == -1)return -1;
	//ポーズ画像
	if ((pauseImage = LoadGraph("Image/pause.png")) == -1)return-1;
	//気泡（背景）
	if ((LoadDivGraph("Image/awaanime.png", 3, 3, 1, 27, 32, awa)) == -1)return -1;
	// カーソル画像
	if ((Corsol = LoadGraph("Image/coursol.png")) == -1)return -1;
	//ポーズ画面のカーソル画像
	if ((Corsol2 = LoadGraph("Image/coursol2.png")) == -1)return -1;
	//ボスの捕獲メッセージ画像
	if ((Messege= LoadGraph("Image/magurohokaku.png")) == -1)return -1;
	//プレイヤー画像
	//魚レベル1
	if ((LoadDivGraph("Image/sakana.png", 10, 10, 1, 30, 30, sakana[0])) == -1)return -1;
	//魚レベル2
	if ((LoadDivGraph("Image/sakana2.png", 3, 3, 1, 150, 150, sakana[1])) == -1)return-1;
	//魚レベル3
	if ((LoadDivGraph("Image/sakana3.png", 3, 3, 1, 225, 225, sakana[2])) == -1)return-1;
	//ステージ背景
	if ((StageImage = LoadGraph("Image/Hikei.png")) == -1) return -1;
	//手前の背景
	if ((Iwa[0] = LoadGraph("Image/temae4.png")) == -1) return -1;
	if ((Iwa[1] = LoadGraph("Image/仮boss前1.png")) == -1) return -1;
	if ((Iwa[2] = LoadGraph("Image/仮boss前2.png")) == -1) return -1;
	//餌(食べれる生き物)画像
	//エビ
	if ((LoadDivGraph("Image/ebi.png", 3, 3, 1, 50, 50, feedImage[0])) == -1)return-1;
	if ((LoadDivGraph("Image/ebiLG.png", 3, 3, 1, 50, 50, feedImageur[0])) == -1)return-1;
	//アジ
	if ((LoadDivGraph("Image/azi.png", 3, 3, 1, 60, 60, feedImage[1])) == -1)return-1;
	if ((LoadDivGraph("Image/aziLG.png", 3, 3, 1, 60, 60, feedImageur[1])) == -1)return-1;
	//イカ
	if ((LoadDivGraph("Image/ika.png", 3, 3, 1, 50, 50, feedImage[2])) == -1)return-1;
	if ((LoadDivGraph("Image/ikaLG.png", 3, 3, 1, 50, 50, feedImageur[2])) == -1)return-1;

	//敵
	//ミノカサゴ
	if ((LoadDivGraph("Image/minokasago.png", 3, 3, 1, 120,120, EnemyImage[0])) == -1)return-1;
	if ((LoadDivGraph("Image/minokasagoLG.png", 3, 3, 1, 120, 120, feedImageur[3])) == -1)return-1;

	//クラゲ
	if ((LoadDivGraph("Image/kurage(仮).png", 3, 3, 1, 80, 80, EnemyImage[1])) == -1)return-1;

	//船
	if ((LoadDivGraph("Image/船.png", 3, 3, 1, 400, 350, ship1)) == -1)return -1;

	//船2
	if ((LoadDivGraph("Image/船2.png", 3, 3, 1, 400, 350, ship2)) == -1)return -1;

	//網
	if ((LoadDivGraph("Image/網.png", 7, 7, 1, 400, 400, net1)) == -1)return -1;

	//網2
	if ((LoadDivGraph("Image/網2.png", 7, 7, 1, 400, 400, net2)) == -1)return -1;

	//Boss
	if ((LoadDivGraph("Image/maguro.png", 12, 12, 1, 350, 350, Boss1)) == -1)return -1;
	if ((LoadDivGraph("Image/maguro2.png", 6, 6, 1, 350, 350, Boss2)) == -1)return -1;
	if ((LoadDivGraph("Image/rasubosu.png", 6, 6, 1, 350, 350, Boss3)) == -1)return -1;
	//Boss必殺技
	if ((LoadDivGraph("Image/ソニックブーム.png", 2, 2, 1, 450, 450, Sonic)) == -1)return -1;
	//Boss警告
	if ((Keikoku = LoadGraph("Image/keikoku.png")) == -1)return -1;
	//Boss捕獲
	if ((LoadDivGraph("Image/Hokaku.png", 4, 4, 1, 350, 500, Hokaku)) == -1)return -1;
	//Boss捕獲2
	if ((LoadDivGraph("Image/マグロ捕獲2.png", 4, 4, 1, 350, 500, Hokaku2)) == -1)return -1;
	//ゲームクリア画像
	if ((Gameclear[0] = LoadGraph("Image/GameClear(puran).png")) == -1)return -1;
	if ((Gameclear[1] = LoadGraph("Image/GameClear(nin).png")) == -1)return -1;
	if ((Gameclear[2] = LoadGraph("Image/GameClear(ika).png")) == -1)return -1;
	if ((Gameclear[3] = LoadGraph("Image/GameClear(azi).png")) == -1)return -1;
	if ((Gameclear[4] = LoadGraph("Image/GameClear(ebi).png")) == -1)return -1;
	if ((Gameclear[5] = LoadGraph("Image/GameClear(utubo).png")) == -1)return -1;
	if ((Gameclear[6] = LoadGraph("Image/GameClear(burobu).png")) == -1)return -1;
	if ((Gameclear[7] = LoadGraph("Image/GameClear(isogi).png")) == -1)return -1;
	if ((Gameclear[8] = LoadGraph("Image/GameClear(maguro).png")) == -1)return -1;
	if ((Gameclear[9] = LoadGraph("Image/GameClear(same).png")) == -1)return -1;
	if ((Gameclear[10] = LoadGraph("Image/GameClear(tai).png")) == -1)return -1;
	if ((Gameclear[11] = LoadGraph("Image/mikai.png")) == -1)return -1;
	//図鑑の時の画像 
	if ((ClearZukan[0] = LoadGraph("Image/zukan(puran).png")) == -1)return -1;
	if ((ClearZukan[1] = LoadGraph("Image/zukan(nin).png")) == -1)return -1;
	if ((ClearZukan[2] = LoadGraph("Image/zukan(ika).png")) == -1)return -1;
	if ((ClearZukan[3] = LoadGraph("Image/zukan(azi).png")) == -1)return -1;
	if ((ClearZukan[4] = LoadGraph("Image/zukan(ebi).png")) == -1)return -1;
	if ((ClearZukan[5] = LoadGraph("Image/zukan(utubo).png")) == -1)return -1;
	if ((ClearZukan[6] = LoadGraph("Image/zukan(burobu).png")) == -1)return -1;
	if ((ClearZukan[7] = LoadGraph("Image/zukan(isogi).png")) == -1)return -1;
	if ((ClearZukan[8] = LoadGraph("Image/zukan(maguro).png")) == -1)return -1;
	if ((ClearZukan[9] = LoadGraph("Image/zukan(same).png")) == -1)return -1;
	if ((ClearZukan[10] = LoadGraph("Image/zukan(tai).png")) == -1)return -1;
	//ゲームオーバー時のアニメーション
	if ((LoadDivGraph("Image/FishDie.png", 7, 7, 1, 30, 30, fishdie[0])) == -1)return -1;
	if ((LoadDivGraph("Image/FishDie Level2.png", 7, 7, 1, 150, 150, fishdie[1])) == -1)return -1;
	if ((LoadDivGraph("Image/FishDie Level3.png", 7, 7, 1, 225, 225, fishdie[2])) == -1)return -1;
	//ゲームオーバー画像
	if ((Gameover = LoadGraph("Image/GameOver.png")) == -1)return -1;
	if ((Gameover2 = LoadGraph("Image/GameOver2.png")) == -1)return -1;
	if ((Gameover3 = LoadGraph("Image/GameOver3.png")) == -1)return -1;

	//ボスへの道
	if ((BOSSROAD = LoadGraph("Image/ボスへの道.png")) == -1)return -1;
	if ((BOSSMETER = LoadGraph("Image/ボス戦への目印2.png")) == -1)return -1;
	if ((BOSSY = LoadGraph("Image/BOSS_Y.png")) == -1)return -1;
	//水しぶき
	if ((LoadDivGraph("Image/hasira.png", 8, 8, 1, 167, 500, sibuki)) == -1)return -1;

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

/******************************************************************
* BGMの読み込み
*******************************************************************/
int LoadSound() {
	// BGM
	if ((MainSound = LoadSoundMem("Sound/神秘の泉_-_8bit.mp3")) == -1)return -1;
	if ((TitleSound = LoadSoundMem("Sound/おさかなシャトル_-_8bit.mp3")) == -1)return -1;
	if ((BossSound = LoadSoundMem("Sound/よぞらのなみだ.mp3")) == -1)return -1;
	if ((HelpSound = LoadSoundMem("Sound/かえるのピアノ.mp3")) == -1)return -1;
	if ((GameOverSound = LoadSoundMem("Sound/不穏_-_8bit.mp3")) == -1)return -1;

	// SE
	if ((ClearSE = LoadSoundMem("Sound/bgm_fanfare_1.mp3")) == -1)return -1;
	if ((CarsolSE = LoadSoundMem("Sound/select09.mp3")) == -1)return -1;
	if ((DesitionSE = LoadSoundMem("Sound/button67.mp3")) == -1)return -1;
	if ((EatSE = LoadSoundMem("Sound/nc78521.mp3")) == -1)return -1;
	if ((DamegeSE = LoadSoundMem("Sound/se_dosun_2.mp3")) == -1)return -1;
	if ((WallSE = LoadSoundMem("Sound/se_wall_1.mp3")) == -1)return -1;
	if ((EvoSE = LoadSoundMem("Sound/powerup10.mp3")) == -1)return -1;
	if ((DieSE = LoadSoundMem("Sound/NES-General01-10(Pitch).mp3")) == -1)return -1;
	if ((KeikokuSE = LoadSoundMem("Sound/meka_ge_keihou03.mp3")) == -1)return -1;
	if ((SibukiSE = LoadSoundMem("Sound/水・ざぶーん05.mp3")) == -1)return -1;
	if((PouseSE=LoadSoundMem("Sound/button80.mp3"))==-1)return -1;

	//音量調整
		// BGM
		ChangeVolumeSoundMem(125, MainSound);
		ChangeVolumeSoundMem(125, TitleSound);
		ChangeVolumeSoundMem(125, GameOverSound);
		ChangeVolumeSoundMem(150, HelpSound);
		ChangeVolumeSoundMem(200, BossSound);
		// SE
		ChangeVolumeSoundMem(125, KeikokuSE);
		ChangeVolumeSoundMem(200, EatSE);
		ChangeVolumeSoundMem(200, CarsolSE);
		ChangeVolumeSoundMem(200, DesitionSE);
		ChangeVolumeSoundMem(200, SibukiSE);
		ChangeVolumeSoundMem(200, ClearSE);
		ChangeVolumeSoundMem(225, EvoSE);
	
	return 0;
}

/*************************************************
* 音ON:OFF
**************************************************/
void Musicmixer()
{
	static bool push = 0;	// 押されたかどうか確認する関数
	DrawGraph(0, 0, GameMS, TRUE);
	if (BGMFlg == FALSE) {
		SetFontSize(60);
		DrawString(710, 270, "ON", 0xff0000);
		DrawString(800, 270, "/", 0x000000);
		DrawString(850, 270, "OFF", 0x000000);
	}
	else if (BGMFlg == TRUE) {
		SetFontSize(60);
		DrawString(710, 270, "ON", 0x000000);
		DrawString(800, 270, "/", 0x000000);
		DrawString(850, 270, "OFF", 0xff0000);
	}

	if (SEFlg == FALSE) {
		SetFontSize(60);
		DrawString(710, 420, "ON", 0xff0000);
		DrawString(800, 420, "/", 0x000000);
		DrawString(850, 420, "OFF", 0x000000);
	}
	else if (SEFlg == TRUE) {
		SetFontSize(60);
		DrawString(710, 420, "ON", 0x000000);
		DrawString(800, 420, "/", 0x000000);
		DrawString(850, 420, "OFF", 0xff0000);
	}

	DrawRotaGraph(210, 300 + MenuNo * 147, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_DOWN) {
		if (++MenuNo > 2)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_UP) {
		if (--MenuNo < 0)MenuNo = 2;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}

		if ((MenuNo == 0) && (BGMFlg == FALSE)){
			BGMFlg = TRUE;
			StopSoundMem(HelpSound);
			push = 0;
		}
		else if ((MenuNo == 0) && (BGMFlg == TRUE)) {
			BGMFlg = FALSE;
			PlaySoundMem(HelpSound, DX_PLAYTYPE_LOOP, TRUE);
			push = 0;
		}
		else if ((MenuNo == 1) && (SEFlg == FALSE)){
			SEFlg = TRUE;
			StopSoundMem(DesitionSE);
			push = 0;
		}
		else if ((MenuNo == 1) && (SEFlg == TRUE)) {
			SEFlg = FALSE;
			PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			push = 0;
		}
		else {
			GameState = 6;
			MenuNo = 0;
		}
	}
}


void EatMove() {
	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == TRUE && Time > 60) {
			//餌の表示
			if (Time > 0) {
				if (eat[i].DMove == FALSE) {
					DrawExtendGraph(eat[i].e_x, eat[i].e_y, eat[i].e_x + e_w, eat[i].e_y + e_h, eat[i].image, TRUE);
				}
				else if(eat[i].DMove == TRUE&&eat[i].type<=3){
					DrawExtendGraph(eat[i].e_x, eat[i].e_y, eat[i].e_x + e_w, eat[i].e_y + e_h, eat[i].imageur, TRUE);
				}
			}

			if (Damege != 1 && LeveUpflg1 != TRUE && LeveUpflg2 != TRUE) {
				if (eat[i].e_y > 250) {
					if (eat[i].DMove == FALSE) {
						EatCheck(eat[i].e_x - 1 - (Leve * 2), eat[i].e_y);
						if (EatCont >= 1) eat[i].DMove = TRUE;
					}
					else {
						EatCheck(eat[i].e_x + 4 + (Leve * 2), eat[i].e_y);
						if (EatCont >= 1) eat[i].DMove = FALSE;
					}
					EatCont = 0;
				}

				if (eat[i].DMove == TRUE) {
					//真っすぐ右に移動
					if (Leve == 1 && key1 != 1 && key2 != 1) {
						eat[i].e_x += 6;
					}
					if (Leve == 2 && key1 != 1 && key2 != 1) {
						eat[i].e_x += 8;
					}
					if (Leve == 3 && key1 != 1 && key2 != 1) {
						eat[i].e_x += 10;
					}
				}
				else {
					//真っすぐ左に移動
					if (Leve == 1 && key1 != 1 && key2 != 1) {
						eat[i].e_x -= 6;
					}
					if (Leve == 2 && key1 != 1 && key2 != 1) {
						eat[i].e_x -= 8;
					}
					if (Leve == 3 && key1 != 1 && key2 != 1) {
						eat[i].e_x -= 10;
					}
				}
			}

			if (Leve == 1) {
				if (eat[i].e_x == SCREEN_WIDTH - 6)EatCheck1(eat[i].e_x - 1, eat[i].e_y + e_h);
				if (eat[i].e_x == SCREEN_WIDTH - (e_w / 2) + 5) EatCheck1(eat[i].e_x + (e_w / 2) - 1, eat[i].e_y + e_h);
				if (eat[i].e_x == SCREEN_WIDTH - e_w + 3) EatCheck1(eat[i].e_x + e_w - 1, eat[i].e_y + e_h);
			}
			if (Leve == 2) {
				if (eat[i].e_x == SCREEN_WIDTH - 8) EatCheck1(eat[i].e_x - 1, eat[i].e_y + e_h);
				if (eat[i].e_x == SCREEN_WIDTH - (e_w / 2) + 3) EatCheck1(eat[i].e_x + (e_w / 2) - 1, eat[i].e_y + e_h);
				if (eat[i].e_x == SCREEN_WIDTH - e_w + 5) EatCheck1(eat[i].e_x + e_w - 1, eat[i].e_y + e_h);
			}
			if (Leve == 3) {
				if (eat[i].e_x == SCREEN_WIDTH - 10) EatCheck1(eat[i].e_x - 1, eat[i].e_y + e_h);
				if (eat[i].e_x == SCREEN_WIDTH - (e_w / 2) + 3) EatCheck1(eat[i].e_x + (e_w / 2) - 1, eat[i].e_y + e_h);
				if (eat[i].e_x == SCREEN_WIDTH - e_w + 5) EatCheck1(eat[i].e_x + e_w - 1, eat[i].e_y + e_h);
			}

			if (EatCont >= 1)
			{
				EatCont = 0;
				eat[i].flg = FALSE;
			}

			if (eat[i].flg == FALSE)continue;

			//画面外に行ったら削除
			if (eat[i].e_x < 0 - 40)eat[i].flg = FALSE;
			if (eat[i].e_x > 1410 + e_w)eat[i].flg = FALSE;

			//魚に当たったら場合の処理
			if (Hit(&player, &eat[i]) == TRUE) {
				PlayerEat(&eat[i].type);
			}
			//アニメーションを動かす
			if (key1 < 1) {
				if (--MoveEat <= 0)
				{
					CountEat++;
					MoveEat = MoveSpeed;
					CountEat %= MAX_MOTION_INDEX;
				}

				int motion_index = anime[CountEat];
				if (eat[i].type <= 2) {
					eat[i].image = feedImage[eat[i].type][motion_index];
				}
				else {
					eat[i].image = EnemyImage[eat[i].type - 3][motion_index];
				}
				if (eat[i].type <= 3&&eat[i].DMove == TRUE) {
					eat[i].imageur = feedImageur[eat[i].type][motion_index];
				}

			}
		}
	}

	//餌の設定
	if (Range / 5 % 50 == 0 && key1 < 1 && Damege != 1 && LeveUpflg1 != TRUE && LeveUpflg2 != TRUE && Time > 60) {
		EatImage();
	}
}

int EatImage() {

	for (int i = 0; i < 10; i++) {
		if (eat[i].flg == FALSE) {
			eat[i] = eat0;
			eat[i].type = GetRand(7)%5;
			if (eat[i].type <= 2) {
				eat[i].image = feedImage[eat[i].type][0];
			}
			else if (eat[i].type <= 3) {
				eat[i].imageur = feedImageur[eat[i].type][0];
			}
			else {
				eat[i].image = EnemyImage[eat[i].type - 3][0];
			}
			switch (eat[i].type) {
			case 0:
				eat[i].e_y = (GetRand(3) + 2) * 100 + 150;
				break;
			case 1:
				eat[i].e_y = GetRand(2) * 100 + 150;
				break;
			case 2:
				eat[i].e_y = GetRand(5) * 100 + 150;
				break;
			case 3:
				eat[i].e_y = GetRand(3) * 100 + 150;
				eat[i].typeD = TRUE;
				break;
			case 4:
				eat[i].e_y = GetRand(1) * 100 + 150;
				eat[i].typeD = TRUE;
				break;
			}

			e_w = 50 * 1.5;
			e_h = 50 * 1.5;

			return TRUE;
		}
	}

	return FALSE;
}

//ライフ
void LifeImage() {

	for (int i = 0; i < player.life; i++)
	{
		DrawGraph(LifeX + (60 * i), LIfeY, Life, TRUE);

	}
	if (player.life < 1) {
		key2 = 1;
		player.flg = FALSE;
		boss.flg = FALSE;
		boss.speed = FALSE;
		dieam();

	}
}
void dieam() {

	if (--DEI_act_wait <= 0)
	{
		if (key1 < 1) {
			DEI_act_index++;
			DEI_act_wait = DEI_ANI_SPEED;
			DEI_act_index %= DEI_MOTION_INDEX;
		}

		motion_index7 = deianime[DEI_act_index];

	}

	DrawExtendGraph(player.x, player.y, player.x + player.w, player.y + player.h, fishdie[Leve - 1][motion_index7], TRUE);


	StopSoundMem(MainSound);
	StopSoundMem(BossSound);
	StopSoundMem(WallSE);
	StopSoundMem(DamegeSE);
	if (CheckSoundMem(DieSE) == 0) {
		if (SEFlg == FALSE) {
			PlaySoundMem(DieSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	if (motion_index7 > 5) {
		key2 = 0;
		GameState = 7;
		StopSoundMem(KeikokuSE);
	}
}

//メーター表示
void MeterImage() {

	//エビメーター
	DrawGraph(m_x - 10, m_y, Meter[0][em], TRUE);
	DrawGraph(0, 80, feedImage[0][0], TRUE);

	//アジメーター
	DrawGraph(m_x + 240, m_y, Meter[1][am], TRUE);
	DrawGraph(237, 75, feedImage[1][0], TRUE);

	//イカメーター
	DrawGraph(m_x + 480, m_y, Meter[2][im], TRUE);
	DrawGraph(480, 65, feedImage[2][0], TRUE);
}

//成長
void PlayerGrowth() {

	//サイズの変更量の増加
	if (Leve == 1) {
		Scke = 2;
		player.w *= Scke;
		player.h *= Scke;
		player.x -= PLAYER_WIDTH / 2;
		player.y -= PLAYER_HEIGHT / 2;
	}
	else if (Leve == 2) {
		Scke = 1.5;
		LeveUpflg2 = TRUE;
		player.w *= Scke;
		player.h *= Scke;
	}
	LeveUpflg1 = TRUE;
	LeveUp *= 2;
	Leve++;
	LifeMax++;
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

	if (*e <= 2) {
		//食べたものを量を増加させる
		EatAmount++;
		if (player.life < LifeMax && EatAmount % 3 == 0) player.life++;
	}

	//食べたものの量が一定量に達したら処理を移す
	if ((EatAmount >= LeveUp)&&(Leve <3)) {
		PlayerGrowth();
		if (SEFlg == FALSE) {
			PlaySoundMem(EvoSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
}

//エサを吐き出す処理
void DEat() {

	static int DEflg = FALSE;

	if (DEflg == FALSE) {
		RandEat = GetRand(2);
		switch (RandEat) {
		case 0:
			if (em < 6 && em>0) em--;
			else RandEat = 3;

			break;
		case 1:
			if (am < 6 && am>0)am--;
			else RandEat = 3;
			break;
		case 2:
			if (im < 6 && im>0)im--;
			else RandEat = 3;
			break;
		}
		DEflg = TRUE;
	}

	if (Deatflg == FALSE) {
		Dx = player.x;
		Dy = player.y;
		if (Leve == 1) {
			Dw = player.w;
			Dh = player.h;
		}
		else {
			Dw = player.w / 1.5;
			Dh = player.h / 1.5;
		}
		Deatflg = TRUE;
	}

	if (RandEat < 3) {
		DrawExtendGraph(Dx + Dw / 2 + 10 + DSpeed, Dy + Dh / 2 + 10, Dx + Dw + DSpeed - 10, Dy + Dh - 10, feedImage[RandEat][0], TRUE);
		DSpeed += 10;
		if (Dx + DSpeed >= SCREEN_WIDTH) {
			Deatflg = FALSE;
			Feedflg = FALSE;
			DEflg = FALSE;
			DSpeed = 0;
		}
	}
	else {
		DEflg = FALSE;
	}

	if (RandEat > 2) {
		Deatflg = FALSE;
		Feedflg = FALSE;
	}
}

//餌とプレイヤーのあたり判定
int Hit(Player* p, Eat* e) {

	if (Time > 0) {
		int px = player.x;
		int py = player.y;
		int ph = player.h;
		int pw = player.w;
		int phx = player.x;
		int phy = player.y;
		int phh = player.h;
		int phw = player.w;
		int ex = e->e_x;
		int ey = e->e_y;
		int ew = e_w;
		int eh = e_h;
		int etypeD = e->typeD;

		if (Leve == 1) {
			px += 10;
			py += 25;
			pw -= 10;
			ph -= 50;
			phx += 45;
			phy += 45;
			phw -= 90;
			phh -= 90;
		}
		else if (Leve == 2) {
			px += 50;
			py += 70;
			pw -= 80;
			ph -= 140;
			phx += 95;
			phy += 95;
			phw -= 190;
			phh -= 190;
		}
		else if (Leve == 3) {
			px += 40;
			py += 60;
			pw -= 50;
			ph -= 120;
			phx += 95;
			phy += 95;
			phw -= 150;
			phh -= 180;
		}

		//餌とのあたり判定
		if (etypeD == FALSE) {
			if (ex + ew >= px && ex <= px + pw &&
				ey + eh >= py && ey <= py + ph) {
				e->flg = FALSE;
				if (SEFlg == FALSE) {
					PlaySoundMem(EatSE, DX_PLAYTYPE_BACK, TRUE);
				}
				return TRUE;
			}
		}
		else {
			if (ex + ew >= phx && ex <= phx + phw &&
				ey + eh >= phy && ey <= phy + phh && player.muteki == 0) {
				player.life--;
				player.muteki = 1;
				if (SEFlg == FALSE) {
					PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
				}
				Feedflg = TRUE;
				return TRUE;
			}
		}
	}
	return FALSE;
}


//ゲームクリア（当たったら）
void GameClearHit() {

	GameState = 3;
	StopSoundMem(BossSound);
	StopSoundMem(KeikokuSE);
	if (SEFlg == FALSE) {
		PlaySoundMem(ClearSE, DX_PLAYTYPE_BACK, TRUE);
	}
	SHIPFlg = FALSE;
}

void GameClear() {
	static bool push = 0; // 押されたかどうか確認する関数

	//エンディング分岐変数
	int EndBranch = 0;

	//エンディング判定

	//レベルが1の時
	if (Leve == 1) {
		//イカが一番多い時
		if (im > am && im > em) {
			EndBranch = 2;
		}
		//アジが一番多い時
		else if (am >= em && am > im) {
			EndBranch = 3;
		}
		//エビが一番多い場合
		else if (em > am && em > im ) {
			EndBranch = 4;
		}
	}
	else if (Leve == 2) {
		//イカが一番多い時
		if (im >= am && im >= em ) {
			EndBranch = 5;
		}
		//アジが一番多い時
		else if (am >= em && am > im ) {
			EndBranch = 6;
		}
		//エビが一番多い場合
		else if (em > am && em > im ) {
			EndBranch = 7;
		}
	}
	else if (Leve == 3) {
		//イカが一番多い時
		if (im >= am && im >= em ) {
			EndBranch = 8;
		}
		//アジが一番多い時
		else if (am >= em && am > im ) {
			EndBranch = 9;
		}
		//エビが一番多い場合
		else if (em > am && em > im ) {
			EndBranch = 10;
		}
	}

	//何も食べなかった場合
	if (EatAmount == 0) {
		EndBranch = 0;
	}

	//全てが平均的な場合
	if (im + am + em > 0 && em == im && em == am && am == im) {
		EndBranch = 1;
	}
	

	//エンディング表示
	DrawGraph(0, 0, Gameclear[EndBranch], TRUE);
	clear[EndBranch] = TRUE;

	// メニューカーソルの描画
	DrawRotaGraph(420 + MenuNo * 300, 750, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 1;
			StopSoundMem(ClearSE);
			if (BGMFlg == FALSE) {
				PlaySoundMem(MainSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			StopSoundMem(ClearSE);
			if (BGMFlg == FALSE) {
				PlaySoundMem(TitleSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
	}
}
void  BossMeter() {

	DrawGraph(900, 0, BOSSMETER, TRUE);
	if (key1 < 1) {
		if (Time > 0) {
			if (boss_Y > 1273) {
				bm = 0;
			}

			boss_Y += bm;

		}
	}
	DrawGraph(boss_Y, 40, BOSSY, TRUE);

}
//ボスステージ移行（当たったら）
void BossST(Player* p) {

	int px = p->x;
	int py = p->y;
	int ph = p->h;
	int pw = p->w;

	if (Time <= 0) {
		
		if (1410 >= px && 1410 <= px + ph &&
			800 >= py && 0 <= py + pw) {
			GameState = 4;
			StopSoundMem(MainSound);
			if (BGMFlg == FALSE) {
				PlaySoundMem(BossSound, DX_PLAYTYPE_LOOP, TRUE);
			}
		}
	}
}

void BossInit() {

	//Bossの初期化
	boss.flg = TRUE;
	boss.dir = 0;
	boss.bx = BOSS_POS_X;
	boss.by = BOSS_POS_Y;
	boss.bw = BOSS_WIDTH;
	boss.bh = BOSS_HEIGHT;
	boss.speed = BOSS_SPEED;
	count = 0;
	counth = 0;
	BOSS_PATTREN = 1;


	soni.sx = SONIC_POS_X;
	soni.sy = SONIC_POS_Y;
	soni.sw = SONIC_WIDTH;
	soni.sh = SONIC_HEIGHT;


	SHIP_X = 1500;
	SHIP_lX = -400;
	SHIP_Y = -100;
	SHIP_W = 350;
	SHIP_H = 350;
	SHIP_SPEED = 5;
	SHIP_COUNT = 0;
	NET = 0;
	motion_index4 = 0;
	motion_index8 = 0;
	sc = 2;
	cr = 0;
	cr2 = 0;

	net.nx = 1350;
	net.lnx = -250;
	net.ny = 50;
	net.nw = 350;
	net.nh = 350;
	net.speed = 15;
	netflg = 0;

	//playerの位置初期化
	player.x = 500;
	player.y = PLAYER_POS_Y;

	
	motion_index4 = 0;
	NET_act_index = 0;
	motion_index9 = 0;

	//ゲームメインへ
	GameState = 5;
}

void BossBackScrool() {
	if (GameState == 5 && key1 < 1 && GameState == 5 && key2 < 1) {
		ScroolSpeed -= player.speed;

	}

	//ステージ画像表示
	//描画可能エリアを設定
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	DrawGraph(Umispeed % SCREEN_WIDTH, 0, StageImage, TRUE);
	DrawGraph(SCREEN_WIDTH + (Umispeed % SCREEN_WIDTH), 0, StageImage, TRUE);
	//エリアを戻す
	SetDrawArea(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	//レベル表示
	SetFontSize(60);
	DrawFormatString(0, 0, 0x000000, "Lv.%d", Leve);
}
void BossMove() {

	if (--BOSS_act_wait <= 0)
	{
		if (key1 < 1) {
			BOSS_act_index++;
			BOSS_act_wait = BOSS_ACT_SPEED;
			BOSS_act_index %= BOSS_MOTION_INDEX;
			BOSS_act_index3++;
			BOSS_act_index3 %= SONIC_MOTION_INDEX;
		}
	}

	if (BOSS_PATTREN == 1) {
	
		BossMove1();
	}
	if (BOSS_PATTREN == 2) {
	
		BossMove2();
	}
	if (BOSS_PATTREN == 3) {
	
		BossMove3();
	}
	if (player.muteki == 0) {
		if (key1 < 1) {
			if (player.muteki == 0) {
				//当たり判定
				if (HitBoxPlayer(&player, &boss) == TRUE) {

					player.life -= 1;
					player.muteki = 1;
					if (SEFlg == FALSE) {
						PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
					}
				}
			}
			//当たり判定
			if (HitBoxPlayer2(&player, &soni) == TRUE) {

				player.life -= 1;
				player.muteki = 1;
				if (SEFlg == FALSE) {
					PlaySoundMem(DamegeSE, DX_PLAYTYPE_BACK, TRUE);
				}
			}
		}
	}
}
void BossMove1() {

	if (key1 < 1) {

		count = (count + 1) % 500;
		//DrawFormatString(100, 160, 0x000000, "%d", count);
		/*DrawFormatString(100, 180, 0x000000, "%d", boss.bx);
		DrawFormatString(100, 240, 0x000000, "%d", boss.by);*/

		if (count == 1) {
			BOSS_SPEED = 20;
		}
		if (count > 0 && count < 100) {
			if (boss.bx == 0) {
				BOSS_SPEED = 0;
			}
			boss.bx += BOSS_SPEED;

			motion_index2 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}

		if (count > 101 && count < 150) {
			motion_index2 = BOSSAttack[BOSS_act_index];
			boss.by = player.y - 180;
			DrawGraph(boss.bx + 200, boss.by - 50, Keikoku, TRUE);
			if (Leve == 1) {
				BOSS_SPEED = 20;
			}
			else if (Leve == 2) {
				BOSS_SPEED = 25;
			}
			else if (Leve == 3) {
				BOSS_SPEED = 30;
			}
		}
		if (count > 151 && count < 300) {
			motion_index2 = BOSSAttack[BOSS_act_index];
			boss.bx += BOSS_SPEED;
		}
		if (count == 301) {
			boss.bx = 1400;
		}

		if (count > 302 && count < 499) {
			motion_index2 = BossKoutai[BOSS_act_index];
			boss.dir = 1;
			if (boss.bx < -450) {
				boss.dir = 0;
				BOSS_SPEED = 0;
			}
			boss.bx -= BOSS_SPEED;
			boss.by = 600;
			boss.bw = BOSS_WIDTH;
			boss.bh = BOSS_HEIGHT;
		}

		if (count == 499) {
			count = 0;
			boss.by = 200;
			BOSS_SPEED = 20;
			BOSS_PATTREN = 2;
			countS += 1;
		}
	}
	if (boss.dir == 0) {
		DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);
	}
	if (boss.dir == 1) {
		DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss3[motion_index2], TRUE);
	}
}

void BossMove2() {
	if (key1 < 1) {
		count = (count + 1) % 500;
		//DrawFormatString(100, 160, 0x000000, "%d", count);
		/*DrawFormatString(100, 180, 0x000000, "%d", boss.bx);
		DrawFormatString(100, 240, 0x000000, "%d", boss.by);*/
		if (count > 0 && count < 100) {
			if (boss.bx > 0) {

				BOSS_SPEED = 0;
			}
			boss.bx += BOSS_SPEED;
			motion_index2 = BOSSAnime[BOSS_act_index];
		}

		if (count > 100 && count < 200) {
			motion_index2 = BOSSDown[BOSS_act_index];
			BOSS_SPEED = 20;
			boss.bx += BOSS_SPEED;
			boss.by += 10;
		}
		if (count > 170 && count < 200) {
			DrawGraph(boss.bx + 100, 600, Keikoku, TRUE);
			boss.bx = player.x - 130;
		}
		if (count > 160 && count < 170) {
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		if (count > 201 && count < 399) {
			motion_index2 = BOSSUp[BOSS_act_index];
			BOSS_SPEED = 0;
			boss.by -= 20;
			/*if (count == 160) {
				sibuki_act_index = 0;
			}*/
			if (count > 270 && count < 351) {

				//sibuki_act_index = 0;
				Sibuki();
			}

		}

		if (count == 399) {
			boss.bx = BOSS_POS_X;
			BOSS_SPEED = 20;
		}

		if (count > 399 && count < 498) {
			motion_index2 = BOSSAnime[BOSS_act_index];
			if (boss.bx == 0) {

				BOSS_SPEED = 0;
			}
			boss.bx += BOSS_SPEED;
			boss.by = player.y - 180;
			boss.bw = BOSS_WIDTH;
			boss.bh = BOSS_HEIGHT;
		}
		if (count == 499) {
			count = 0;
			BOSS_SPEED = 20;
			BOSS_PATTREN = 3;
			countS += 1;
		}
	}

	DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index2], TRUE);


}

void BossMove3() {
	if (--BOSS_act_wait2 <= 0)
	{

		if (key1 < 1) {
			if (MS == 1) {
				BOSS_act_index2++;
				BOSS_act_wait2 = BOSS_ACT_SPEED2;
				BOSS_act_index2 %= BOSSB_MOTION_INDEX;
			}
		}
	}
	if (key1 < 1) {
		count = (count + 1) % 900;
		//DrawFormatString(100, 160, 0x000000, "%d", count);
		if (count > 0 && count < 25) {
			if (boss.bx == 0) {

				BOSS_SPEED = 0;
			}
			boss.bx += BOSS_SPEED;
			motion_index2 = BOSSAnime[BOSS_act_index];
		}
		if (count > 0 && count < 50) {
			BOSS_act_index2 = 0;
			MS = 0;
			motion_index5 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		if (count > 50 && count < 100) {
			MS = 1;
			boss.by = player.y - 180;
			DrawGraph(boss.bx + 130, boss.by - 50, Keikoku, TRUE);
			motion_index5 = BOSSBoom[BOSS_act_index2];
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);
		}
		if (count == 99) {
			soni.sx = 190;
			//soni.sy = 120;
			soni.sy = player.y - 180;
		}
		if (count > 100 && count < 300) {
			BOSS_act_index2 = 0;
			MS = 0;
			//BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			/*if (boss.by > 300) {
				BOSS_SPEED = 0;
			}
			boss.by += BOSS_SPEED;*/
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		if (count > 300 && count < 350) {
			MS = 1;
			boss.by = player.y - 180;
			DrawGraph(boss.bx + 130, boss.by - 50, Keikoku, TRUE);
			motion_index5 = BOSSBoom[BOSS_act_index2];
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);

		}
		if (count == 349) {
			soni.sx = 190;
			//soni.sy = 340;
			soni.sy = player.y - 100;
		}
		if (count > 350 && count < 500) {
			BOSS_act_index2 = 0;
			MS = 0;
			//BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			/*if (boss.by > 450) {
				BOSS_SPEED = 0;
			}
			boss.by += BOSS_SPEED;*/
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		if (count > 500 && count < 550) {
			MS = 1;
			boss.by = player.y - 180;
			DrawGraph(boss.bx + 130, boss.by - 50, Keikoku, TRUE);
			motion_index5 = BOSSBoom[BOSS_act_index2];
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);

		}
		if (count == 549) {
			soni.sx = 190;
			//soni.sy = 470;
			soni.sy = player.y - 100;
		}
		if (count > 550 && count < 800) {
			BOSS_act_index2 = 0;
			MS = 0;
			motion_index5 = BOSSAnime[BOSS_act_index];
			boss.by = player.y - 180;
			/*BOSS_SPEED = 10;
			motion_index5 = BOSSAnime[BOSS_act_index];
			if (boss.by < 300) {
				BOSS_SPEED = 0;
			}
			boss.by -= BOSS_SPEED;*/
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);
			if (SEFlg == FALSE) {
				PlaySoundMem(KeikokuSE, DX_PLAYTYPE_BACK, TRUE);
			}

		}

		if (count > 100 && count < 300 || count > 350 && count < 550 ||
			count > 550 && count < 800) {
			motion_index6 = BossSonic[BOSS_act_index3];
			if (Leve == 1) {
				soni.sx += SONIC_SPEED_Lev1;
			}
			else if (Leve == 2) {
				soni.sx += SONIC_SPEED_Lev2;
			}
			else if (Leve == 3) {
				soni.sx += SONIC_SPEED_Lev3;
			}
			DrawExtendGraph(soni.sx, soni.sy, soni.sx + soni.sw, soni.sy + soni.sh, Sonic[motion_index6], TRUE);
		}
		if (count == 799) {
			count = 0;
			BOSS_act_index2 = 0;
			BOSS_SPEED = 20;
			BOSS_PATTREN = 1;
			countS += 1;
			DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss1[motion_index5], TRUE);

		}
	}
	//DrawExtendGraph(boss.bx, boss.by, boss.bx + boss.bw, boss.by + boss.bh, Boss2[motion_index5], TRUE);
}

void Sibuki() {//水しぶきのアニメーション


	int count_s = 10;

	if (count_s != 0) {
		if (Leve == 1) {
			boss.bx -= 1.5;
		}
		else if (Leve == 2) {
			boss.bx -= 4;
		}
		else {
			boss.bx -= 6;
		}
	}

	if (--sibuki_act_wait <= 0)
	{
		if (key1 < 1) {
			sibuki_act_index++;
			sibuki_act_wait = sibuki_ANI_SPEED;
			sibuki_act_index %= sibuki_MOTION_INDEX;
		}

		motion_index9 = sibukianime[sibuki_act_index];

	}
	DrawExtendGraph(boss.bx, SIBUKI_Y, boss.bx + boss.bw, SIBUKI_Y + 162, sibuki[motion_index9], TRUE);
	//DrawExtendGraph(500, SIBUKI_Y, 500 + 300, SIBUKI_Y + 300, sibuki[motion_index9], TRUE);
	if (CheckSoundMem(SibukiSE) == 0) {
		if (SEFlg == FALSE) {
			PlaySoundMem(SibukiSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
}


//船アニメーション
void Ship() {
	if (SHIPFlg == FALSE) {
		if (key1 < 1) {
			SHIP_COUNT += sc;
			SHIP_COUNT2 = 400 - SHIP_COUNT;
			if (SHIP_COUNT2 == 0) {
				sc = 0;
			}
			if (SHIP_COUNT2 != 0) {
				//SetFontSize(50);
				//DrawFormatString(700, 30, 0x000000, "船が来るまで%d秒", SHIP_COUNT2 / 100);
			}
			if (countS >= 3)
			{
				SHIP_X -= SHIP_SPEED;
				net.nx -= SHIP_SPEED;
				DrawGraph(700, 30, Messege, TRUE);
				/*SetFontSize(50);
				DrawFormatString(700, 30, 0x000000, "マグロを捕獲しよう");*/
			}
			if (SHIP_X == 1300)
			{
				counth = (counth + 1) % 150;
				SHIP_SPEED = 0;

			}
			if (SHIP_X == 800)
			{
				counth = (counth + 1) % 150;
				SHIP_SPEED = 0;

			}
			if (SHIP_X == 300)
			{
				counth = (counth + 1) % 150;
				SHIP_SPEED = 0;
			}
			if (SHIP_X == -300)
			{
				SHIPFlg = TRUE;
				SHIP_X = 1500;
				net.nx = 1350;
			}

			if (counth > 1 && counth < 148) {
				NET = 1;
			}
			if (motion_index4 == 6) {
				netflg = 1;
			}

			//if (SHIP_X != 300)
			//{
			if (SHIPFlg != TRUE) {
				if (netflg == 1) {
					NET = 0;
					SHIP_SPEED = 10;
					netflg = 0;
					motion_index4 = 0;
					NET_act_index = 0;
				}
			}
			//}
			if (countS >= 3) {
				if (--SHIP_act_wait <= 0)
				{
					if (key1 < 1) {
						SHIP_act_index++;
						SHIP_act_wait = SHIP_ANI_SPEED;
						SHIP_act_index %= SHIP_MOTION_INDEX;
					}
				}

				motion_index3 = shipanime[SHIP_act_index];
			}
			DrawExtendGraph(SHIP_X, SHIP_Y, SHIP_X + SHIP_W, SHIP_Y + SHIP_H, ship1[motion_index3], TRUE);
		}
	}
}
// 船（反転）アニメーション
void ShipLs()
{
	if (SHIPFlg == TRUE)
	{
		SHIP_lX += SHIP_SPEED;
		net.lnx += SHIP_SPEED;
		/*SetFontSize(50);
		DrawFormatString(700, 30, 0x000000, "マグロを捕獲しよう");*/
		DrawGraph(700, 30, Messege, TRUE);
		if (SHIP_lX == 100)
		{
			counth = (counth + 1) % 150;
			SHIP_SPEED = 0;

		}
		if (SHIP_lX == 500)
		{
			counth = (counth + 1) % 150;
			SHIP_SPEED = 0;

		}
		if (SHIP_lX == 900)
		{
			counth = (counth + 1) % 150;
			SHIP_SPEED = 0;
		}
		if (SHIP_lX == 1500)
		{
			SHIPFlg = FALSE;
			SHIP_lX = -400;
			net.lnx = -250;
		}
		if (counth > 1 && counth < 148) {
			NET = 1;
		}
		if (motion_index4 == 6) {
			netflg = 1;
		}

		if (SHIPFlg != FALSE)
		{
			if (netflg == 1) {
				NET = 0;
				SHIP_SPEED = 10;
				netflg = 0;
				motion_index4 = 0;
				NET_act_index = 0;
			}
			//}

			if (--SHIP_act_wait <= 0)
			{
				if (key1 < 1) {
					SHIP_act_index++;
					SHIP_act_wait = SHIP_ANI_SPEED;
					SHIP_act_index %= SHIP_MOTION_INDEX;
				}

				motion_index3 = shipanime[SHIP_act_index];
			}
			DrawExtendGraph(SHIP_lX, SHIP_Y, SHIP_lX + SHIP_W, SHIP_Y + SHIP_H, ship2[motion_index3], TRUE);
		}
	}
}

//網アニメーション
void Ami() {
	if (SHIPFlg == FALSE) {
		if (key1 < 1) {
			if (cr == 0) {
				if (countS >= 3) {
					if (SHIP_SPEED == 0) {
						if (--NET_act_wait <= 0)
						{
							if (key1 < 1) {
								NET_act_index++;
								NET_act_wait = NET_ANI_SPEED;
								NET_act_index %= NET_MOTION_INDEX;
							}
						}
					}
					motion_index4 = netanime[NET_act_index];
				}
				int bbx = (boss.bx - (boss.bw - boss.bw - 180));
				int bby = (boss.by - (boss.bh - boss.bh - 150));
				int bbw = (boss.bx - (boss.bw - boss.bw - 180)) + boss.bh - 270;
				int bbh = (boss.by - (boss.bh - boss.bh - 150)) + boss.bw - 280;
				if (SHIP_X == 1300) {
					if (motion_index4 == 4) {
						//1200< bbh&& bbx< 1400&& 250< bbw&& bby< 270
						//DrawBox(1200, 250, 1400, 270, GetColor(255, 255, 255), FALSE);
						if (1200 < bbh && bbx < 1400 && 250 < bbw && bby < 270) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(1200, 310, 1400, 330, GetColor(255, 255, 255), FALSE);
						if (1200 < bbh && bbx < 1400 && 310 < bbw && bby < 330) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(1200, 360, 1400, 380, GetColor(255, 255, 255), FALSE);
						if (1200 < bbh && bbx < 1400 && 360 < bbw && bby < 380) {
							cr = 1;
						}
					}
				}
				else if (SHIP_X == 800) {
					if (motion_index4 == 4) {
						//700< bbh&& bbx< 900&& 250< bbw&& bby< 270
						//DrawBox(700, 250, 900, 270, GetColor(255, 255, 255), FALSE);
						if (700 < bbh && bbx < 900 && 250 < bbw && bby < 270) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(700, 310, 900, 330, GetColor(255, 255, 255), FALSE);
						if (700 < bbh && bbx < 900 && 310 < bbw && bby < 330) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(700, 360, 900, 380, GetColor(255, 255, 255), FALSE);
						if (700 < bbh && bbx < 900 && 360 < bbw && bby < 380) {
							cr = 1;
						}
					}
				}
				else if (SHIP_X == 300) {
					if (motion_index4 == 4) {
						//200 < bbh && bbx < 400 && 250 < bbw && bby < 270
						//DrawBox(200, 250, 400, 270, GetColor(255, 255, 255), FALSE);
						if (200 < bbh && bbx < 400 && 250 < bbw && bby < 270) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(200, 310, 400, 330, GetColor(255, 255, 255), FALSE);
						if (200 < bbh && bbx < 400 && 310 < bbw && bby < 330) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(200, 360, 400, 380, GetColor(255, 255, 255), FALSE);
						if (200 < bbh && bbx < 400 && 360 < bbw && bby < 380) {
							cr = 1;
						}
					}
				}
				DrawExtendGraph(net.nx, net.ny, net.nx + net.nw, net.ny + net.nh, net1[motion_index4], TRUE);
			}
		}
	}
}

void AmiLs() {
	if (SHIPFlg == TRUE) {
		if (key1 < 1) {
			if (cr == 0) {
				if (SHIP_SPEED == 0) {
					if (--NET_act_wait <= 0)
					{
						if (key1 < 1) {
							NET_act_index++;
							NET_act_wait = NET_ANI_SPEED;
							NET_act_index %= NET_MOTION_INDEX;
						}
					}
					motion_index4 = netanime[NET_act_index];
				}
				int bbx = (boss.bx - (boss.bw - boss.bw - 180));
				int bby = (boss.by - (boss.bh - boss.bh - 150));
				int bbw = (boss.bx - (boss.bw - boss.bw - 180)) + boss.bh - 270;
				int bbh = (boss.by - (boss.bh - boss.bh - 150)) + boss.bw - 280;
				if (SHIP_lX == 100) {
					if (motion_index4 == 4) {
						//350 < bbh && bbx < 550 && 250 < bbw && bby <270
						//DrawBox(350, 250, 550, 270, GetColor(255, 255, 255), FALSE);
						if (350 < bbh && bbx < 550 && 250 < bbw && bby < 270) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(350, 310, 550, 330, GetColor(255, 255, 255), FALSE);
						if (350 < bbh && bbx < 550 && 310 < bbw && bby < 330) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(350, 360, 550, 380, GetColor(255, 255, 255), FALSE);
						if (350 < bbh && bbx < 550 && 360 < bbw && bby < 380) {
							cr = 1;
						}
					}
				}
				else if (SHIP_lX == 500) {
					if (motion_index4 == 4) {
						//750 < bbh && bbx < 950 && 250 < bbw && bby < 270
						//DrawBox(750, 250, 950, 270, GetColor(255, 255, 255), FALSE);
						if (750 < bbh && bbx < 950 && 250 < bbw && bby < 270) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(750, 310, 950, 330, GetColor(255, 255, 255), FALSE);
						if (750 < bbh && bbx < 950 && 310 < bbw && bby < 330) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(750, 360, 950, 380, GetColor(255, 255, 255), FALSE);
						if (750 < bbh && bbx < 950 && 360 < bbw && bby < 380) {
							cr = 1;
						}
					}
				}
				else if (SHIP_lX == 900) {
					if (motion_index4 == 4) {
						//1150 < bbh && bbx <1350 && 250 < bbw && bby <270
						//DrawBox(1150, 250, 1350, 270, GetColor(255, 255, 255), FALSE);
						if (1150 < bbh && bbx < 1350 && 250 < bbw && bby < 270) {
							cr = 1;
						}
					}
					if (motion_index4 == 5) {
						//DrawBox(1150, 310, 1350, 330, GetColor(255, 255, 255), FALSE);
						if (1150 < bbh && bbx < 1350 && 310 < bbw && bby < 330) {
							cr = 1;

						}
					}
					if (motion_index4 == 6) {
						//DrawBox(1150, 360, 1350, 380, GetColor(255, 255, 255), FALSE);
						if (1150 < bbh && bbx < 1350 && 360 < bbw && bby < 380) {
							cr = 1;
						}
					}
				}
				DrawExtendGraph(net.lnx, net.ny, net.lnx + net.nw, net.ny + net.nh, net2[motion_index4], TRUE);
			}
		}
	}
}

void BHA() {
	if (SHIPFlg == FALSE) {
		if (key1 < 1) {
			if (--HOKAKU_act_wait <= 0)
			{
				if (key1 < 1) {
					HOKAKU_act_index++;
					HOKAKU_act_wait = HOKAKU_SPEED;
					HOKAKU_act_index %= HOKAKU_MOTION_INDEX;
				}
				motion_index8 = BossHokaku[HOKAKU_act_index];
			}
			if (motion_index8 == 3) {
				cr2 = 1;
			}
			DrawGraph(net.nx, net.ny, Hokaku[motion_index8], TRUE);
		}
	}
}

void BHALs() {
	if (SHIPFlg == TRUE) {
		if (key1 < 1) {
			if (--HOKAKU_act_wait <= 0)
			{
				if (key1 < 1) {
					HOKAKU_act_index++;
					HOKAKU_act_wait = HOKAKU_SPEED;
					HOKAKU_act_index %= HOKAKU_MOTION_INDEX;
				}
				motion_index8 = BossHokaku[HOKAKU_act_index];
			}
			if (motion_index8 == 3) {
				cr2 = 1;
			}
			DrawGraph(net.lnx, net.ny, Hokaku2[motion_index8], TRUE);
		}
	}
}

void BossStage() {

	BossBackScrool();
	PlayerMove();
	if (cr == 0) {
		BossMove();
		HOKAKU_act_index = 0;
	}
	LifeImage();
	MeterImage();
	if (cr == 1) {
		if (SHIPFlg == FALSE) {
			BHA();
		}
		else if (SHIPFlg == TRUE) {
			BHALs();
		}
	}
	if (cr2 == 1) {
		GameClearHit();
	}
	//Goal();
	if (NET == 1) {
		Ami();
		AmiLs();
	}
	Ship();
	ShipLs();
	Pouse();
}


/*************************************
*自機と敵機の当たり判定（四角）
* 引数：PLAYER　ポインタ
* 戻り値：TRUE：あたり、FALSE：はずれ
*************************************/
int HitBoxPlayer(Player* p, Boss* b)
{
	if (Leve == 1) {
		//x,yは中心座標とする
		int px = p->x - (p->w - player.w);
		int py = p->y - (p->h - player.h - 20);
		int ph = px + 20 + p->h - 30;
		int pw = py + p->w - 50;

		int bx1 = b->bx - (b->bw - boss.bw - 180);
		int by2 = b->by - (b->bh - boss.bh - 150);
		int bh1 = bx1 + b->bh - 270;
		int bw2 = by2 + b->bw - 280;

		//判定確認用
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(bx1, by2, bh1, bw2, 0xFFFFFF, FALSE);*/

		//短径が重なっていたら当たり
		if (px < bh1 && bx1 < ph && py < bw2 && by2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 2) {
		//x,yは中心座標とする
		int px = p->x - 20 - (p->w - player.w - 90);
		int py = p->y - 10 - (p->h - player.h - 90);
		int ph = px - 20 + p->h - 90;
		int pw = py - 5 + p->w - 150;

		int bx1 = b->bx - (b->bw - boss.bw - 180);
		int by2 = b->by - (b->bh - boss.bh - 150);
		int bh1 = bx1 + b->bh - 270;
		int bw2 = by2 + b->bw - 280;

		//判定確認用
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(bx1, by2, bh1, bw2, 0xFFFFFF, FALSE);*/

		//短径が重なっていたら当たり
		if (px < bh1 && bx1 < ph && py < bw2 && by2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 3) {
		//x,yは中心座標とする
		int px = p->x - (p->w - player.w - 70);
		int py = p->y - (p->h - player.h - 70);
		int ph = px + p->h - 90;
		int pw = py + p->w - 130;

		int bx1 = b->bx - (b->bw - boss.bw - 180);
		int by2 = b->by - (b->bh - boss.bh - 150);
		int bh1 = bx1 + b->bh - 270;
		int bw2 = by2 + b->bw - 280;

		//判定確認用
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(bx1, by2, bh1, bw2, 0xFFFFFF, FALSE);*/

		//短径が重なっていたら当たり
		if (px < bh1 && bx1 < ph && py < bw2 && by2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
}

int HitBoxPlayer2(Player* p, Soni* s)
{
	if (Leve == 1) {
		//x,yは中心座標とする
		int px = p->x - (p->w - player.w);
		int py = p->y - (p->h - player.h - 20);
		int ph = px + 20 + p->h - 30;
		int pw = py + p->w - 50;

		int sx1 = s->sx - (s->sw - soni.sw - 180);
		int sy2 = s->sy - (s->sh - soni.sh - 100);
		int sh1 = sx1 + s->sh - 390;
		int sw2 = sy2 + s->sw - 200;

		////判定確認用
		//DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		//DrawBox(sx1, sy2, sh1, sw2, 0xFFFFFF, FALSE);

		//短径が重なっていたら当たり
		if (px < sh1 && sx1 < ph && py < sw2 && sy2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 2) {
		//x,yは中心座標とする
		int px = p->x - 20 - (p->w - player.w - 90);
		int py = p->y - 10 - (p->h - player.h - 90);
		int ph = px - 20 + p->h - 90;
		int pw = py - 5 + p->w - 150;

		int sx1 = s->sx - (s->sw - soni.sw - 180);
		int sy2 = s->sy - (s->sh - soni.sh - 100);
		int sh1 = sx1 + s->sh - 390;
		int sw2 = sy2 + s->sw - 200;

		////判定確認用
		//DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		//DrawBox(sx1, sy2, sh1, sw2, 0xFFFFFF, FALSE);

		//短径が重なっていたら当たり
		if (px < sh1 && sx1 < ph && py < sw2 && sy2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
	if (Leve == 3) {
		//x,yは中心座標とする
		int px = p->x - (p->w - player.w - 70);
		int py = p->y - (p->h - player.h - 70);
		int ph = px + p->h - 90;
		int pw = py + p->w - 130;

		int sx1 = s->sx - (s->sw - soni.sw - 180);
		int sy2 = s->sy - (s->sh - soni.sh - 100);
		int sh1 = sx1 + s->sh - 390;
		int sw2 = sy2 + s->sw - 200;

		////判定確認用
		/*DrawBox(px, py, ph, pw, 0xFFFFFF, FALSE);
		DrawBox(sx1, sy2, sh1, sw2, 0xFFFFFF, FALSE);*/

		//短径が重なっていたら当たり
		if (px < sh1 && sx1 < ph && py < sw2 && sy2 < pw) {
			return TRUE;
		}

		return FALSE;
	}
}


int ColorCheck(int x, int y) {
	
	Cr1 = GetPixel(x, y);
	GetColor2(Cr1, &r, &g, &b);
	if (r == 98 && g == 96 && b == 94) {
		if (player.muteki == 0) {
			if (PointNom == 0) {

				//Level 1 左の判定
				if ((y == player.y + player.h / 2 - CollYadd) || (y == player.y + player.h / 2 + CollYadd)) {
					if (x == player.x + player.w / 2 - CollXadd) PointNom = 1;
					if (x == player.x + player.w / 2 + CollXadd * 3) PointNom = 2;
				}

				//Level 2 左の判定
				if ((player.x + player.w / 2) && ((y == player.y + player.h / 2 - CollYadd / 4) || (y == player.y + player.h / 2 + CollYadd / 4))) {
					PointNom = 1;
				}
				//Level 2 右の判定
				if ((player.x + player.w / 2 + CollXadd) && ((y == player.y + player.h / 2 - CollYadd + 2) || (y == player.y + player.h / 2 + CollYadd + 2))) {
					PointNom = 2;
				}

				//Level 3 左の判定
				if ((x == player.x + player.w / 2 - CollXadd) && ((y == player.y + player.h / 2 - CollYadd) || (y == player.y + player.h / 2 + CollYadd))) {
					PointNom = 1;
				}
				//Level 3 右の判定
				if ((x == player.x + player.w / 2 + CollXadd * 5 + 5) && ((y == player.y + player.h / 2 - CollYadd * 2) || (y == player.y + player.h / 2 + CollYadd * 2))) {
					PointNom = 2;
				}
			}
			Damege = 1;
			player.life -= 1;
			player.muteki = 1;
			if (SEFlg == FALSE) {
				PlaySoundMem(WallSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}
	
	return colorsum;
}

//岩との当たり判定(エサ)
void EatCheck(int x, int y) {

	int mx2;

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 155; j++) {
			mx = j * 50 + Iwaspeed % IwaHaba;
			my = i * 50 + 400;
			mx2 = mx + IwaHaba;
			mw = 50;
			mh = 50;
			if (EatI[i][j] == 1) {
				if (mx + mw > x && mx<x + e_w &&
					my + mh>y && my < y + e_h) {
					EatCont++;
				}
				if (mx2 + mw > x && mx2<x + e_w &&
					my + mh>y && my < y + e_h) {
					EatCont++;
				}
			}
		}
	}

	return;
}

//出現場所の判定
void EatCheck1(int x, int y) {

	Cr1 = GetPixel(x, y);
	GetColor2(Cr1, &er, &eg, &eb);

	if ((er == 98) && (eg == 96) && (eb == 94) && (Eatflg < TRUE)) {
		Eatflg = FALSE;
		EatCont++;
	}

	if (Eatflg == FALSE) Eatflg = TRUE;

	return;
}

void Pouse() {
	static bool push = 0;

	if (player.life > 0) {
		if (g_KeyFlg & PAD_INPUT_8 && key1 < 1) {
			key1 = 1;
			player.flg = FALSE;
			boss.flg = FALSE;
			boss.speed = FALSE;
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		else if (key1 > 0 && g_KeyFlg & PAD_INPUT_8) {
			player.flg = TRUE;
			boss.flg = TRUE;
			boss.speed = TRUE;
			key1 = 0;
			if (SEFlg == FALSE) {
				PlaySoundMem(PouseSE, DX_PLAYTYPE_BACK, TRUE);
			}
		}

		if (key1 > 0) {
			DrawGraph(355, 100, pauseImage, TRUE);
			//player.flg = FALSE;
			// メニューカーソルの描画
			DrawRotaGraph(590, 445 + MenuNo * 80, 0.3f, 0, Corsol2, TRUE);

			// メニューカーソル移動処理
			if (g_KeyFlg & PAD_INPUT_DOWN) {
				if (++MenuNo > 1)MenuNo = 0;
				if (SEFlg == FALSE) {
					PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			if (g_KeyFlg & PAD_INPUT_UP) {
				if (--MenuNo < 0)MenuNo = 1;
				if (SEFlg == FALSE) {
					PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
				}
			}
			if (g_KeyFlg & PAD_INPUT_2) {
				if (push == 0) {
					push = 1;
				}
				if (MenuNo == 0) {
					push = 0;
					//MenuNo = 0;
					StopSoundMem(MainSound);
					StopSoundMem(BossSound);
					key1 = 0;
					GameState = 1;
				}
				else if (MenuNo == 1) {
					push = 0;
					MenuNo = 0;
					key1 = 0;
					StopSoundMem(MainSound);
					StopSoundMem(BossSound);
					GameState = 0;
				}
			}
		}
	}
}
void GameOver() {
	static bool push = 0; // 押されたかどうか確認する関数
	SHIPFlg = FALSE;
	if (Leve == 2) {
		DrawGraph(0, 0, Gameover2, TRUE);
	}
	else if (Leve == 3) {
		DrawGraph(0, 0, Gameover3, TRUE);
	}
	else {
		DrawGraph(0, 0, Gameover, TRUE);
	}

	if (CheckSoundMem(GameOverSound) == 0) {
		if (BGMFlg == FALSE) {
			PlaySoundMem(GameOverSound, DX_PLAYTYPE_BACK, TRUE);
		}
	}


	// メニューカーソルの描画
	if (MenuNo != 1) {
		DrawRotaGraph(100 + MenuNo * 450, 715, 0.3f, 0, Corsol2, TRUE);
	}
	else {
		DrawRotaGraph(740, 715, 0.3f, 0, Corsol2, TRUE);
	}

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 1;
			StopSoundMem(GameOverSound);
			if (BGMFlg == FALSE) {
				PlaySoundMem(MainSound, DX_PLAYTYPE_BACK, TRUE);
			}
		}
		else if (MenuNo == 1) {
			push = 0;
			GameState = 0;
			MenuNo = 0;
			StopSoundMem(GameOverSound);
			if (BGMFlg == FALSE) {
				PlaySoundMem(TitleSound, DX_PLAYTYPE_BACK, TRUE);
			}
		}
	}
}
void PictureBook1() {

	StopSoundMem(TitleSound);

	static bool push = 0;	// 押されたかどうか確認する関数
	//背景
	DrawGraph(0, 0, Bookhaikei[0], TRUE);
	int x = 50, y = 50, w = 400, h = 325, U = 11, a = 0;

	for (int i = 1; i < 3; i++) {
		for (int j = 1; j < 4; j++) {
			if (clear[a] == TRUE) {
				DrawGraph(x, y, ClearZukan[a], TRUE);
			}
			else {
				DrawExtendGraph(x, y, x + w, y + h, Gameclear[U], TRUE);
			}
			x = x + w + 50;
			a++;
		}
		y = y + h + 50;
		x = 50;
	}

	// メニューカーソルの描画
	DrawRotaGraph(510 + MenuNo * 230, 773, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 15;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
		}
	}
}

void PictureBook2() {

	static bool push = 0;	// 押されたかどうか確認する関数
	//背景
	DrawGraph(0, 0, Bookhaikei[1], TRUE);
	int x = 50, y = 50, w = 400, h = 325, U = 11, a = 6;

	for (int i = 1; i < 3; i++) {
		for (int j = 1; j < 4; j++) {
			if (i + j < 5 && clear[a] == TRUE) {
				DrawGraph(x, y, ClearZukan[a], TRUE);
			}
			else if (i + j < 5 && clear[a] == FALSE) {
				DrawExtendGraph(x, y, x + w, y + h, Gameclear[U], TRUE);
			}
			x = x + w + 50;
			a++;
		}
		y = y + h + 50;
		x = 50;
	}

	// メニューカーソルの描画
	DrawRotaGraph(510 + MenuNo * 230, 773, 0.3f, 0, Corsol, TRUE);

	// メニューカーソル移動処理
	if (g_KeyFlg & PAD_INPUT_RIGHT) {
		if (++MenuNo > 1)MenuNo = 0;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}
	if (g_KeyFlg & PAD_INPUT_LEFT) {
		if (--MenuNo < 0)MenuNo = 1;
		if (SEFlg == FALSE) {
			PlaySoundMem(CarsolSE, DX_PLAYTYPE_BACK, TRUE);
		}
	}

	// Zキーでメニュー選択
	if (g_KeyFlg & PAD_INPUT_2) {
		if (push == 0) {
			if (SEFlg == FALSE) {
				PlaySoundMem(DesitionSE, DX_PLAYTYPE_BACK, TRUE);
			}
			push = 1;
		}
		// sceneStageに行く処理
		if (MenuNo == 0) {
			push = 0;
			GameState = 14;
		}
		else {
			push = 0;
			GameState = 0;
			MenuNo = 0;
		}
	}
}