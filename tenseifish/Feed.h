#pragma once
//餌などに関する関数
void EatImage(void);		//食べる生物の表示の表示
void PlayerGrowth(void);	//プレイヤーが成長した(レベルが上がった)時の処理
void PlayerEat(void);		//プレイヤーが餌を食べたとき
void Hit(void);				//敵とのあたり判定

//変数
int e_x = 250, e_y = 260;	//餌の仮座標
int Leve = 1;			//レベル
float Scke = 1.5;			//大きくなる量
int LeveUp = 1;			//レベルが上がるのに必要な量
int EatAmount = 0;		//食べたものの量(仮)