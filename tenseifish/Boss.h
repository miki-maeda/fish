#pragma once
void BossInit();
void BossMove();
void BossMove1();
void BossMove2();
void BossMove3();
void BossStage();
void BossBackScrool();

void Ami();
void Ship();
void BHA();


//Bossの初期値
int BOSS_POS_X = 1000;
int BOSS_POS_Y = 200;
int BOSS_WIDTH = 450;
int BOSS_HEIGHT = 450;
int BOSS_SPEED = 10;

int SONIC_POS_X = 1400;
int SONIC_POS_Y = 130;
int SONIC_WIDTH = 350;
int SONIC_HEIGHT = 350;
int SONIC_SPEED = 8;

int Boss1[13] = { 0 };		//ボスの画像を格納する配列
int Boss2[13] = { 0 };		//ボスの画像を格納する配列
int Sonic[3] = { 0 };      //ボスの必殺技を格納する配列
int Hokaku[5] = { 0 };    //ボスの捕獲を格納する配列


int BOSSBoom[] = { 5,5,4,4,3,3,2,1,0,2,1,0 };		//必殺技するときのアニメーションの番号 

int BOSSAnime[] = { 0,1,0,2 };		//泳ぐときのアニメーションの番号 
int BOSSAttack[] = { 3,4,3,5 };		//突進するときのアニメーションの番号 
int BOSSDown[] = { 6,7,6,8 };         //下降するときのアニメーションの番号 
int BOSSUp[] = { 9,10,9,11 };         //上昇するときのアニメーションの番号 
int BossSonic[] = { 0,1 };         //必殺技のアニメーション番号
int BossHokaku[] = { 0,1,2,3 };         //捕獲のアニメーション番号

int HOKAKU_act_index = 0;
int HOKAKU_SPEED = 20;
int HOKAKU_act_wait = HOKAKU_SPEED;

int BOSS_ACT_SPEED = 3;
int BOSS_act_wait = BOSS_ACT_SPEED;
int BOSS_ACT_SPEED2 = 10;
int BOSS_act_wait2 = BOSS_ACT_SPEED2;

int BOSS_act_index = 0;
int BOSS_act_index2 = 0;
int BOSS_act_index3 = 0;

int motion_index2;
int motion_index5;
int motion_index6;
int motion_index8;

int BOSS_PATTREN;
int count;
int MS = 0;
int sc;
int cr;
int cr2;

int BOSS_MOTION_INDEX = 4;// 0,1,2,1 の4種類 
int BOSSB_MOTION_INDEX = 12;	// 5,4,3,2,1,0 の6種類 
int SONIC_MOTION_INDEX = 2;	// 2コマ
int HOKAKU_MOTION_INDEX = 4;	// 4コマ


int shipanime[] = { 0,1,0,2 };      //船のアニメーション
int ship1[9]{ 0 };
int motion_index3;
int SHIP_X = 1400;
int SHIP_Y = -40;
int SHIP_W = 350;
int SHIP_H = 350;
int SHIP_SPEED = 5;
int SHIP_COUNT = 0;
int SHIP_COUNT2 = 0;
int SHIP_ANI_SPEED = 15;
int SHIP_act_wait = SHIP_ANI_SPEED;
int SHIP_act_index = 0;
int SHIP_MOTION_INDEX = 4;

//網
int netanime[] = { 0,1,2,3,4,5,6 };
int net1[7] = { 0 };
int motion_index4;
int NET_MOTION_INDEX = 7;
int NET_ANI_SPEED = 20;
int NET_act_wait = NET_ANI_SPEED;
int NET_act_index = 0;
int NET = 0;

struct Net //網
{

	int flg = true;
	int nx = 0, ny = 0, nw = 0, nh = 0;
	int speed = 0;
};
static Net net;

struct Boss//プレイヤー
{
	int flg = true;
	int bx = 0, by = 0, bw = 0, bh = 0;
	int speed = 0;
};
struct Boss boss;

struct Soni//必殺技
{
	int flg = true;
	int sx = 0, sy = 0, sw = 0, sh = 0;
	int speed = 0;
};
struct Soni soni;
int HitBoxPlayer(Player* p, Boss* b);
int HitBoxPlayer2(Player* p, Soni* s);