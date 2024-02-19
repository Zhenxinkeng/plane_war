#include<stdio.h>
#include<iostream>
#include <graphics.h>
#include <conio.h>
#include<string>
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

enum My
{
	WIDTH = 1138,//背景宽
	HEIGHT = 600,//背景高
	BULLET_NUM = 15,//子弹数目
	ENEMY_NUM=15,//敌机数目
	MAX_PLAYER_HP = 100, // 最大玩家血量
	MAX_ENEMY_HP = 50     // 最大敌机血量
};

DWORD time_start, time_last;//定义时间变量
int score = 0;

//定义图片变量
IMAGE img_bk;//背景图
IMAGE img_player;//玩家图
IMAGE img_bullet1;//子弹图
IMAGE img_enemy;//敌机图

struct Plane {
	int x;
	int y;
	int hp;
	bool life;//是不是还活着
}player, bull[BULLET_NUM], enemy[ENEMY_NUM];

void dataInit();//数据初始化
void load_image();//图片加载
void load_audio();//音频加载
void creat_bullet();//子弹创建
void bullet_move();//子弹移动
void creat_enemy();//敌机创建
void enemy_move();//敌机移动
void playermove(int speed);//角色行动
void  check_collision();//敌我碰撞
void draw_hp(int hp);//打印剩余血量
void game_draw();//画面呈现
void game_start();//开始游戏
void bullet_enemy_collision();//子弹与敌机碰撞
void game_victory();//游戏胜利
void game_over();//游戏失败

//--------------------------------------------------------------------------------- 数据初始化
void dataInit() {
	//玩家初始化
	player.x = 0;
	player.y = 250;
	player.life = true;
	player.hp = MAX_PLAYER_HP;
	//子弹初始化
	for (int i = 0; i < BULLET_NUM; i++) {
		bull[i].x = player.x + 30;
		bull[i].y = player.y;
		bull[i].life = false;
	}
	//敌机初始化
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy[i].life = false;
		enemy[i].hp = MAX_ENEMY_HP;
	}
	//时间初始化
	time_start = time_last = GetTickCount();
}

//--------------------------------------------------------------------------------- 图片加载
void load_image() {
	loadimage(&img_bk, "./images/bk.jpg");
	loadimage(&img_player, "./images/player.png");
	loadimage(&img_bullet1, "./images/bullet1.jpg");
	loadimage(&img_enemy, "./images/enemy.png");
}

//--------------------------------------------------------------------------------- 音频加载
void load_audio() {
	PlaySound(TEXT("./music/click.wav"), NULL, SND_FILENAME);
}

//--------------------------------------------------------------------------------- 子弹创建
void creat_bullet() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (!bull[i].life) {
			bull[i].x = player.x + 30;
			bull[i].y = player.y;
			bull[i].life = true;
			break;
		}
	}
}

//--------------------------------------------------------------------------------- 子弹移动
void bullet_move() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bull[i].life) {
			bull[i].x++;
			if (bull[i].x > 1138) {
				bull[i].life = false;
			}
			else {
				bullet_enemy_collision();
			}
		}
	}
}

//--------------------------------------------------------------------------------- 敌机创建
void creat_enemy() {
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (!enemy[i].life&& time_last - time_start > 400)
		{
			enemy[i].life = true;
			enemy[i].x = 1138;
			enemy[i].y = rand()%600;
			enemy[i].hp = MAX_ENEMY_HP;
			time_start = time_last;
			break;//每次一个
		}
		time_last = GetTickCount();
	}
}

//--------------------------------------------------------------------------------- 敌机移动
void enemy_move() {
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].life)
		{
			enemy[i].x -= 0.3;
			if (enemy[i].x < 10)
				enemy[i].life = false;
		}
	}
}

//--------------------------------------------------------------------------------- 角色行动
void playermove(int speed) {
	//玩家移动
#if 0 
	if (_kbhit()) {
		char key = _getch();
		switch (key) {
		case 'w':case 'W': player.y -= speed; break;
		case 's':case 'S': player.y += speed; break;
		case 'a':case 'A': player.x -= speed; break;
		case 'd':case 'D': player.x += speed; break;
		}
	}
#elif 1
	if (GetAsyncKeyState(VK_UP) && player.y > -22) {
		player.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) && player.y < HEIGHT - 22) {
		player.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) && player.x > 0) {
		player.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && player.x < WIDTH) {
		player.x += speed;
	}
#endif
	//空格发射子弹
	if (GetAsyncKeyState(VK_SPACE) && time_last - time_start > 160) {
		creat_bullet();
		time_start = time_last;
	}
	time_last = GetTickCount();
}

//--------------------------------------------------------------------------------- 敌我碰撞
void check_collision()
{
	// 玩家和敌机的碰撞检测
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy[i].life && player.life)
		{
			if (player.x < enemy[i].x + 50 && player.x + 50 > enemy[i].x &&
				player.y < enemy[i].y + 50 && player.y + 50 > enemy[i].y)
			{
				// 玩家和敌机发生碰撞，玩家掉血
				player.hp -= 10;
				if (player.hp <= 0)
				{
					player.life = false;
					player.hp = 0; // 确保血量不会为负数
				}
				enemy[i].life = false;
				enemy[i].hp = MAX_ENEMY_HP; // 重置敌机血量
			}
		}
	}
}

//--------------------------------------------------------------------------------- 子弹与敌机碰撞
void bullet_enemy_collision() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bull[i].life) {
			for (int j = 0; j < ENEMY_NUM; j++) {
				if (enemy[j].life && bull[i].x < enemy[j].x + 50 && bull[i].x + 10 > enemy[j].x &&
					bull[i].y < enemy[j].y + 50 && bull[i].y + 10 > enemy[j].y) {
					// 子弹击中敌机，减少敌机血量
					enemy[j].hp -= 20;
					if (enemy[j].hp <= 0) {
						score++;
						enemy[j].life = false;
						enemy[j].hp = 0; // 确保血量不为负数
					}
					bull[i].life = false;
					break; // 一个子弹只能击中一个敌机
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------- 打印剩余血量
void draw_hp(int hp) {
	settextstyle(20, 0, _T("微软雅黑"));
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	outtextxy(10, 10, "HP: ");//显示字符串
	outtextxy(50, 10, std::to_string(hp).c_str());//显示整数
	outtextxy(WIDTH - 100, 10, ("Score: " + std::to_string(score)).c_str());//显示分数
}

//--------------------------------------------------------------------------------- 画面呈现
void game_draw() {
	load_image();
	BeginBatchDraw();//双缓冲绘图，防闪烁
	//打印背景图
	putimage(0, 0, &img_bk);
	//打印玩家
	if (player.life) { 
		putimage(player.x, player.y, &img_player); 
	}
	//打印子弹
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bull[i].life) {
			putimage(bull[i].x, bull[i].y, &img_bullet1);
		}
	}
	bullet_move();
	//打印敌机
	creat_enemy();
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy[i].life) {
			putimage(enemy[i].x, enemy[i].y, &img_enemy);
		}
	}
	enemy_move();
	//打印剩余血量
	draw_hp(player.hp);
	EndBatchDraw();
	check_collision();
}

//--------------------------------------------------------------------------------- 开始游戏
void game_start() {
	dataInit();
	PlaySound(TEXT("./music/sky.wav"), NULL, SND_FILENAME | SND_ASYNC);
	while (1) {
		game_draw();
		playermove(2);
		if (score >= 20 || player.hp == 0) break;
	}
}

//--------------------------------------------------------------------------------- 游戏胜利
void game_victory() {
	PlaySound(TEXT("./music/victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
	bool flas= true;
	DWORD last_flash_tim = GetTickCount();
	DWORD flash_interva = 300;
	while (1) {
		if (GetTickCount() - last_flash_tim >= flash_interva) {
			last_flash_tim = GetTickCount();
			if (flas) {
				settextstyle(40, 0, _T("宋体")); // 设置文本样式
				settextcolor(GREEN); // 设置文本颜色
				outtextxy(395, 265, _T("你赢了")); // 点击开始游戏
				flas = false;
			}
			else {
				settextstyle(40, 0, _T("宋体")); // 设置文本样式
				settextcolor(BLACK); // 设置文本颜色
				outtextxy(395, 265, _T("你赢了")); // 点击开始游戏
				flas = true;
			}
		}
	}
}

//--------------------------------------------------------------------------------- 游戏失败
void game_over() {
	PlaySound(TEXT("./music/victory.wav"), NULL, SND_FILENAME | SND_ASYNC);
	bool flas = true;
	DWORD last_flash_tim = GetTickCount();
	DWORD flash_interva = 300;
	while (1) {
		if (GetTickCount() - last_flash_tim >= flash_interva) {
			last_flash_tim = GetTickCount();
			if (flas) {
				settextstyle(40, 0, _T("宋体")); // 设置文本样式
				settextcolor(GREEN); // 设置文本颜色
				outtextxy(395, 265, _T("你死了")); // 点击开始游戏
				flas = false;
			}
			else {
				settextstyle(40, 0, _T("宋体")); // 设置文本样式
				settextcolor(BLACK); // 设置文本颜色
				outtextxy(395, 265, _T("你死了")); // 点击开始游戏
				flas = true;
			}
		}
	}
}

int main()
{
	initgraph(WIDTH, HEIGHT,SHOWCONSOLE);//创建窗口
	MOUSEMSG m;
	bool flash = true;
	DWORD last_flash_time = GetTickCount(); 
	DWORD flash_interval = 300; 
	while (1) {
		if (GetTickCount() - last_flash_time >= flash_interval) { 
			last_flash_time = GetTickCount(); 
			if (flash) {
				settextstyle(40, 0, _T("宋体")); // 设置文本样式
				settextcolor(GREEN); // 设置文本颜色
				outtextxy(395, 265, _T("点击开始游戏")); // 点击开始游戏
				flash = false;
			}
			else {
				settextstyle(40, 0, _T("宋体")); // 设置文本样式
				settextcolor(BLACK); // 设置文本颜色
				outtextxy(395, 265, _T("点击开始游戏")); // 点击开始游戏
				flash = true;
			}
		}
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN&&m.x>=395&&m.x<=600&&m.y>=265&&m.y<=315) {
			break;
		}
	}
	game_start();
	if (score >= 20) {
		game_victory();
	}
	else game_over();
}
