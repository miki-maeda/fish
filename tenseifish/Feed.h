#pragma once

//餌などに関する関数
int EatImage();		//食べる生物の表示と処理
void EatMove(void);			//魚(食べ物)の移動処理
void PlayerGrowth(void);	//プレイヤーが成長した(レベルが上がった)時の処理
void PlayerEat(int* e);		//プレイヤーが餌を食べたとき

//変数
int Leve = 1;			//レベル
int Scke = 1;			//大きくなる量
int LeveUp = 10;			//レベルが上がるのに必要な量
int EatAmount;		//食べたものの量(合計)
int em, am, im;
int MoveSpeed = 50;		//アニメーションが変わるスピード
int MoveEat = MoveSpeed;	//アニメーションが変わるまでの時間
int CountEat = 0;		//カウントする
int Eatflg = 0;			//エネミーが岩の中にいる時のフラグ
int EatCont = 0;		//岩の中のチェック処理にかかった回数を格納する変数
int er = 0, eg = 0, eb = 0;	//色を格納する変数	
int Feedflg = FALSE;	//岩に当たった時にランダムでゲージを減らす処理フラグ
int LeveUpflg1 = FALSE;	//レベルが上がった時のフラグ1(レベルが1から2)
int LeveUpflg2 = FALSE;	//レベルが上がった時のフラグ2(レベルが2から3)
int LCount = 0;			//レベルが上がった時の点滅			
int LeveC;				//レベルの表記
int Deatflg = FALSE;	//フラグ
int DSpeed;				//吐き出した餌のスピード
int RandEat = 0;		//吐き出す餌の種類

//仮
int OnOff=FALSE;

struct Eat {

	int flg;			//使用フラグ
	int	type;			//タイプ
	int image;			//表示する餌
	int e_x, e_y;		//餌の座標
	int e_w, e_h;		//横幅、縦幅
	int typeD;			//出現させる種類(FALSE:餌,TRUE:敵)
	int DMove;			//魚の向き
};

struct Eat eat[10];				//餌の表示する数
struct Eat eat0 = { TRUE,0,0,1410,0,0,0,FALSE,FALSE };

int Hit(Player* p, Eat* e);				//敵とのあたり判定
int EatCheck(int x, int y);				//敵の出現場所の確認
void DEat();							//岩もしくは敵に当たった時ランダムでエサゲージを下げる処理