#pragma once


int ColorCheck(int x, int y); //色を使用した当たり判定用関数

int Cr1 = 0; //取得した色の保存用変数

int Damege = 0;//岩との衝突判定　//0：衝突無し　1：衝突　

int r = 0, g = 0, b = 0;//取得した色のRGBのそれぞれの値

int CollXadd;//ノックバック距離　Xの値
int CollYadd;//ノックバック距離　Yの値

int PointNom = 0;//衝突の左右判定 0：判定なし　1：左　2：右

//泡のアニメーション
int awaanime[] = { 0,1,0,2 };
int awa[3] = { 0 };
int awa_ANI_SPEED = 20;
int awa_act_wait = awa_ANI_SPEED;
int awa_act_index = 0;
int awa_MOTION_INDEX = 4;
int motion_index10;
//泡の初期座標設定
int AWA_Y;
int AWA_X;
int AWA_aX;
int AWA_aY;
int AWA_bX;
int AWA_bY;
int AWA_rX;
int AWA_rY;
int AWA_cX;
int AWA_cY;
//泡のスピード用
int Awaspeed;