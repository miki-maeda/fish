#pragma once
//�a�ȂǂɊւ���֐�
void EatImage(void);		//�H�ׂ鐶���̕\���̕\��
void PlayerGrowth(void);	//�v���C���[����������(���x�����オ����)���̏���
void PlayerEat(void);		//�v���C���[���a��H�ׂ��Ƃ�
void Hit(void);				//�G�Ƃ̂����蔻��

//�ϐ�
int e_x = 250, e_y = 260;	//�a�̉����W
int Leve = 1;			//���x��
float Scke = 1.5;			//�傫���Ȃ��
int LeveUp = 1;			//���x�����オ��̂ɕK�v�ȗ�
int EatAmount = 0;		//�H�ׂ����̗̂�(��)