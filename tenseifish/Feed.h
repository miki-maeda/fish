#pragma once

//�a�ȂǂɊւ���֐�
int EatImage();		//�H�ׂ鐶���̕\���Ə���
void EatMove(void);			//��(�H�ו�)�̈ړ�����
void PlayerGrowth(void);	//�v���C���[����������(���x�����オ����)���̏���
void PlayerEat(int *e);		//�v���C���[���a��H�ׂ��Ƃ�

//�ϐ�
int Leve = 1;			//���x��
float Scke = 1.5;			//�傫���Ȃ��
int LeveUp = 10;			//���x�����オ��̂ɕK�v�ȗ�
int EatAmount;		//�H�ׂ����̗̂�(���v)
int em, am, im;

struct Eat {

	int flg;			//�g�p�t���O
	int	type;			//�^�C�v
	int image;			//�\������a
	int e_x, e_y;		//�a�̍��W

};

struct Eat eat[10];				//�a�̕\�����鐔
struct Eat eat0 = { TRUE,0,0,1410,0};

int Hit(Player* p, Eat* e);				//�G�Ƃ̂����蔻��