#pragma once
//�a�ȂǂɊւ���֐�
void EatImage(void);		//�H�ׂ鐶���̕\���Ə���
void PlayerGrowth(void);	//�v���C���[����������(���x�����オ����)���̏���
void PlayerEat(void);		//�v���C���[���a��H�ׂ��Ƃ�
void Hit(void);				//�G�Ƃ̂����蔻��

//�ϐ�
int e_x = 1450, e_y = 260;	//�a�̉����W
int e_xs;				//�a�̃X�N���[�����Ă��鎞�̍��W
int Leve = 1;			//���x��
float Scke = 1.5;			//�傫���Ȃ��
int LeveUp = 1;			//���x�����オ��̂ɕK�v�ȗ�
int EatAmount = 0;		//�H�ׂ����̗̂�(��)

struct Eat {

	int flg;			//�g�p�t���O
	int image;			//�\������a
	int e_x = 640, e_y;		//�a�̍��W
	int e_xs;			//�a�̃X�N���[�����Ă��鎞�̍��W

};

struct Eat eat[10];				//�a�̕\�����鐔
//struct Eat eat0={}