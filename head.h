#pragma once//ȷ��ͷ�ļ�ֻ������һ��
#include<stdio.h>
#include<iostream>
#include <graphics.h>
#include <conio.h>
#include<string>
#include <Windows.h>
#include <mmsystem.h>

#define WIDTH 1138//������
#define HEIGHT 600//������
#define BULLET_NUM 30//�ӵ���Ŀ
#define MAX_PLAYER_HP 100//���Ѫ��
#define ENEMY_NUM 15//�л���Ŀ
#define MAX_ENEMY_HP 50 //�л�Ѫ��
#define SPEED 2//����ƶ��ٶ�

using namespace std;

typedef struct Member {
	int x;
	int y;
	int hp;
	bool life;
}member;

void dataInit();//���ݳ�ʼ��
void load_image();//ͼƬ����
void creat_bullet();//���һ���ӵ�����
void creat_bullet2();//��Ҷ����ӵ�����
void bullet_move();//�ӵ��ƶ�
void creat_enemy();//�л�����
void enemy_move();//�л��ƶ�
void player_action(int speed);//��ɫ����
void  check_collision();//������ײ
void bullet_enemy_collision();//�е���ײ
void draw_hp();//��ӡѪ���ͷ���
void game_draw();//�������
void game_begin();//�˵�����
void game_start();//��Ϸ��ʼ
void game_victory();//��Ϸʤ��
void game_failure();//��Ϸʧ��
void game_win1();//���һʤ��
void game_win2();//��Ҷ�ʤ��
