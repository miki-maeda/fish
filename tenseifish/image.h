#pragma once
int StageImage;
int Iwa[3];//手前の背景
int Life;
int Gameclear;
int Meter[3][6];
int sakana[3][10] = { 0 };		//魚の画像を格納する配列
int anime[] = { 0,1,0,2 };		//泳ぐときのアニメーションの番号 尾びれが真ん中、上、真ん中、下
int	feedImage[4][3];	//餌の画像を格納する変数 1エビ,2アジ,3イカ