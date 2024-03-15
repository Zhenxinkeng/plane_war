#pragma once//确保头文件只被包括一次
#include<stdio.h>
#include<iostream>
#include <graphics.h>
#include <conio.h>
#include<string>
#include <Windows.h>
#include <mmsystem.h>

#define WIDTH 1138//背景宽
#define HEIGHT 600//背景高
#define BULLET_NUM 30//子弹数目
#define MAX_PLAYER_HP 100//玩家血量
#define ENEMY_NUM 15//敌机数目
#define MAX_ENEMY_HP 50 //敌机血量
#define SPEED 2//玩家移动速度

using namespace std;

typedef struct Member {
	int x;
	int y;
	int hp;
	bool life;
}member;

void dataInit();//数据初始化
void load_image();//图片加载
void creat_bullet();//玩家一的子弹创建
void creat_bullet2();//玩家二的子弹创建
void bullet_move();//子弹移动
void creat_enemy();//敌机创建
void enemy_move();//敌机移动
void player_action(int speed);//角色操作
void  check_collision();//敌我碰撞
void bullet_enemy_collision();//敌弹碰撞
void draw_hp();//打印血量和分数
void game_draw();//画面呈现
void game_begin();//菜单界面
void game_start();//游戏开始
void game_victory();//游戏胜利
void game_failure();//游戏失败
void game_win1();//玩家一胜利
void game_win2();//玩家二胜利
