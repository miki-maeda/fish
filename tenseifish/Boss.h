#pragma once
void BossInit();
void BossMove();
void BossStage();
void BossBackScrool();

//Bossの初期値
int BOSS_POS_X = 1000;
int BOSS_POS_Y = 300;
int BOSS_WIDTH = 350;
int BOSS_HEIGHT = 350;
int BOSS_SPEED = 5;

int Boss1[6] = { 0 };		//ボスの画像を格納する配列
int BOSSanime[] = { 0,1,0,2 };		//泳ぐときのアニメーションの番号 
int BOSSAttack[] = { 3,4,3,5 };		//突進するときのアニメーションの番号 

int BOSS_ACT_SPEED = 3;
int BOSS_act_wait = BOSS_ACT_SPEED;

int BOSS_act_index = 0;

int motion_index2;

int BOSS_MOTION_INDEX = 4;	// 0,1,2,1 の4種類 
int BOSSA_MOTION_INDEX = 4;	// 3,4,3,5 の4種類 

struct Boss//プレイヤー
{
	int flg = true;
	int bx = 0, by = 0, bw = 0, bh = 0;
	int speed = 0;
};
//自機
struct Boss boss;
int HitBoxPlayer(Player* p, Boss* b);