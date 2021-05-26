#pragma once

void PlayerMove();//プレイヤーの動作処理
void LifeImage();
void MeterImage();

//プレイヤーのの初期値
int PLAYER_POS_X = 100;
int PLAYER_POS_Y = 300;
int PLAYER_WIDTH = 100;
int PLAYER_HEIGHT = 100;
int PLAYER_SPEED = 6;

//アニメーションの速さ
int ACT_SPEED = 5;
int act_wait = ACT_SPEED;

//モーションの初期値
int act_index = 0;
//モーションの数
int MAX_MOTION_INDEX = 4;	// 0,1,2,1 の4種類 

int Range;
int key1;
int key2;

//ライフ位置
int LifeX = 150;
int LIfeY = 10;
int LifeMax = 3;

//Lvメーター位置
int m_x = 50;
int m_y = 50;

struct Player//プレイヤーのクラス
{
	int flg = true;
	int x = 0, y = 0, w = 0, h = 0;
	int speed = 0;
	int life = 0;
	int muteki = 0;
};
struct Player player;

void GameClearHit();
void BossST(Player* P);//ボスステージ移行


//キー取得用配列
extern char key[256];

int deianime[] = { 0,1,2,3,4,5,6 };
int dei1[7] = { 0 };
int dei2[7] = { 0 };
int dei3[7] = { 0 };
int DEI_ANI_SPEED = 5;
int DEI_act_wait = DEI_ANI_SPEED;
int DEI_MOTION_INDEX = 7;
int DEI_act_index = 0;
int motion_index7;


