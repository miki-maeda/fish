#pragma once
//餌などに関する関数
void EatImage(void);		//食べる生物の表示と処理
void PlayerGrowth(void);	//プレイヤーが成長した(レベルが上がった)時の処理
void PlayerEat(void);		//プレイヤーが餌を食べたとき
void Hit(void);				//敵とのあたり判定

//変数
int e_x = 1450, e_y = 260;	//餌の仮座標
int e_xs;				//餌のスクロールしている時の座標
int Leve = 1;			//レベル
float Scke = 1.5;			//大きくなる量
int LeveUp = 1;			//レベルが上がるのに必要な量
int EatAmount = 0;		//食べたものの量(仮)

struct Eat {

	int flg;			//使用フラグ
	int image;			//表示する餌
	int e_x = 640, e_y;		//餌の座標
	int e_xs;			//餌のスクロールしている時の座標

};

struct Eat eat[10];				//餌の表示する数
//struct Eat eat0={}