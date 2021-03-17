#pragma once

//餌などに関する関数
int EatImage();		//食べる生物の表示と処理
void EatMove(void);			//魚(食べ物)の移動処理
void PlayerGrowth(void);	//プレイヤーが成長した(レベルが上がった)時の処理
void PlayerEat(int *e);		//プレイヤーが餌を食べたとき

//変数
int Leve = 1;			//レベル
float Scke = 1.5;			//大きくなる量
int LeveUp = 10;			//レベルが上がるのに必要な量
int EatAmount;		//食べたものの量(合計)
int em, am, im;

struct Eat {

	int flg;			//使用フラグ
	int	type;			//タイプ
	int image;			//表示する餌
	int e_x, e_y;		//餌の座標

};

struct Eat eat[10];				//餌の表示する数
struct Eat eat0 = { TRUE,0,0,1410,0};

int Hit(Player* p, Eat* e);				//敵とのあたり判定