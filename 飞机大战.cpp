#include "head.h"

int score = 0;//单人计分
int model = 0;//模式选择
int winner = -1;//赢家

DWORD time_start, time_last;//用于子弹创建
DWORD time_start1, time_last1;//用于玩家二的子弹创建
DWORD time_start2, time_last2;//用于敌机创建

IMAGE img_menu_bk;//菜单背景图
IMAGE img_bk;//背景图
IMAGE img_player1;//玩家一图
IMAGE img_player1_bk;//玩家一掩码图
IMAGE img_player2;//玩家二图
IMAGE img_player2_bk;//玩家二掩码图
IMAGE img_bullet1;//子弹图
IMAGE img_bullet1_bk;//子弹掩码图
IMAGE img_enemy1;//敌机图
IMAGE img_enemy1_bk;//敌机掩码图
IMAGE img_over_1;//死了
IMAGE img_over0;//赢了
IMAGE img_over1;//玩家一赢了
IMAGE img_over2;//玩家二赢了

member player1,player2, bull1[BULLET_NUM], bull2[BULLET_NUM], enemy1[ENEMY_NUM];//定义玩家，子弹，敌机结构体数组

//--------------------------------------------------------------------------------- 数据初始化
void dataInit() {
	//玩家初始化
	player1.x = 0;
	player1.y = 250;
	player1.life = true;
	player1.hp = MAX_PLAYER_HP;
	//玩家二初始化
	player2.x = 1100;
	player2.y = 250;
	player2.life = true;
	player2.hp = MAX_PLAYER_HP;
	//子弹初始化
	for (int i = 0; i < BULLET_NUM; i++) {
		bull1[i].x = player1.x + 30;
		bull1[i].y = player1.y;
		bull1[i].life = false;
	}
	//子弹二初始化
	for (int i = 0; i < BULLET_NUM; i++) {
		bull2[i].x = player1.x - 30;
		bull2[i].y = player1.y;
		bull2[i].life = false;
	}
	//敌机初始化
	for (int i = 0; i < ENEMY_NUM; i++) {
		enemy1[i].life = false;
		enemy1[i].hp = MAX_ENEMY_HP;
	}
	//时间初始化
	time_start = time_last = GetTickCount();
	time_start1 = time_last1 = GetTickCount();
	time_start2 = time_last2 = GetTickCount();
}

//--------------------------------------------------------------------------------- 图片加载
void load_image() {
	loadimage(&img_bk, "./images/bk.jpg");
	loadimage(&img_player1, "./images/player1.png");
	loadimage(&img_player1_bk, "./images/player1_bk.png");
	loadimage(&img_player2, "./images/player2.png");
	loadimage(&img_player2_bk, "./images/player2_bk.png");
	loadimage(&img_bullet1, "./images/bullet1.png");
	loadimage(&img_bullet1_bk, "./images/bullet1_bk.png");
	loadimage(&img_enemy1, "./images/enemy1.png");
	loadimage(&img_enemy1_bk, "./images/enemy1_bk.png");
}

//--------------------------------------------------------------------------------  玩家一的子弹创建
void creat_bullet() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (!bull1[i].life) {
			bull1[i].x = player1.x + 30;
			bull1[i].y = player1.y;
			bull1[i].life = true;
			break;
		}
	}
}

//--------------------------------------------------------------------------------  玩家二的子弹创建
void creat_bullet2() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (!bull2[i].life) {
			bull2[i].x = player2.x - 30;
			bull2[i].y = player2.y;
			bull2[i].life = true;
			break;
		}
	}
}
//--------------------------------------------------------------------------------- 子弹移动
void bullet_move() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bull1[i].life) {
			bull1[i].x++;
			if (bull1[i].x > 1138) {
				bull1[i].life = false;
			}
			else {
				bullet_enemy_collision();
			}
		}
		if (bull2[i].life) {
			bull2[i].x--;
			if (bull2[i].x <=0) {
				bull2[i].life = false;
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
		if (!enemy1[i].life&& time_last2 - time_start2 > 400)
		{
			enemy1[i].life = true;
			enemy1[i].x = 1138;
			enemy1[i].y = rand()%600;
			enemy1[i].hp = MAX_ENEMY_HP;
			time_start2 = time_last2;
			break;//每次一个
		}
		time_last2 = GetTickCount();
	}
}

//--------------------------------------------------------------------------------- 敌机移动
void enemy_move() {
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy1[i].life)
		{
			enemy1[i].x -= 0.3;
			if (enemy1[i].x <= 0) {
				enemy1[i].life = false;
				enemy1[i].hp = MAX_ENEMY_HP;
			}
		}
	}
}

//--------------------------------------------------------------------------------- 角色操作
void player_action(int speed) {
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
	if (GetAsyncKeyState(VK_UP) && player1.y > -22) {
		player1.y -= speed;
	}
	if (GetAsyncKeyState(VK_DOWN) && player1.y < HEIGHT - 22) {
		player1.y += speed;
	}
	if (GetAsyncKeyState(VK_LEFT) && player1.x > 0) {
		player1.x -= speed;
	}
	if (GetAsyncKeyState(VK_RIGHT) && player1.x < WIDTH) {
		player1.x += speed;
	}
#endif
	//玩家二移动
	if (GetAsyncKeyState('W') && player2.y > -22) {
		player2.y -= speed;
	}
	if (GetAsyncKeyState('S') && player2.y < HEIGHT - 22) {
		player2.y += speed;
	}
	if (GetAsyncKeyState('A') && player2.x > 0) {
		player2.x -= speed;
	}
	if (GetAsyncKeyState('D') && player2.x < WIDTH) {
		player2.x += speed;
	}
	//空格发射子弹
	if (GetAsyncKeyState(VK_SPACE) & 0x8000 && time_last - time_start > 160) {
		creat_bullet();
		time_start = time_last;
	}
	time_last = GetTickCount();
	if (GetAsyncKeyState('J') && time_last1 - time_start1 > 160) {
		creat_bullet2();
		time_start1 = time_last1;
	}
	time_last1 = GetTickCount();
}

//--------------------------------------------------------------------------------- 敌我碰撞
void check_collision()
{
	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (enemy1[i].life && player1.life)
		{
			if (player1.x < enemy1[i].x + 78 && player1.x + 30 > enemy1[i].x &&
				player1.y < enemy1[i].y + 52 && player1.y +44  > enemy1[i].y)
			{
				player1.hp -= 10;
				if (player1.hp <= 0)
				{
					player1.life = false;
					player1.hp = 0; 
				}
				enemy1[i].life = false;
				enemy1[i].hp = MAX_ENEMY_HP; 
			}
		}
	}
}

//--------------------------------------------------------------------------------- 敌弹碰撞
void bullet_enemy_collision() {
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bull1[i].life) {
			for (int j = 0; j < ENEMY_NUM; j++) {
				if (enemy1[j].life && bull1[i].x < enemy1[j].x + 50 && bull1[i].x + 10 > enemy1[j].x &&
					bull1[i].y < enemy1[j].y + 50 && bull1[i].y + 10 > enemy1[j].y) {
					enemy1[j].hp -= 20;
					if (enemy1[j].hp <= 0) {
						score++;
						enemy1[j].life = false;
						enemy1[j].hp = 0; 
					}
					bull1[i].life = false;
					break; 
				}
				if (bull1[i].x < player2.x + 71 && bull1[i].x + 10 > player2.x &&
					bull1[i].y < player2.y + 44 && bull1[i].y + 10 > player2.y) {
					player2.hp -= 10;
					bull1[i].life = false;
					break;
				}
			}
		}
		if (bull2[i].life) {
			for (int j = 0; j < ENEMY_NUM; j++) {
				if (bull2[i].x < player1.x + 30 && bull2[i].x + 10 > player1.x &&
					bull2[i].y < player1.y + 44 && bull2[i].y + 10 > player1.y) {
					player1.hp -= 10;
					bull2[i].life = false;
					break;
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------- 打印血量和分数
void draw_hp() {
	settextstyle(20, 0, _T("微软雅黑"));
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	outtextxy(10, 10, ("HP: " + to_string(player1.hp)).c_str());
	if(model==1) 
		outtextxy(WIDTH - 100, 10, ("HP: " + to_string(player2.hp)).c_str());
	else
		outtextxy(WIDTH - 100, 10, ("Score: " + to_string(score)).c_str());
}

//--------------------------------------------------------------------------------- 画面呈现
void game_draw() {
	load_image();
	BeginBatchDraw();//双缓冲绘图，防闪烁
	putimage(0, 0, &img_bk);
	if (player1.life) { 
		putimage(player1.x, player1.y, &img_player1,SRCAND);
		putimage(player1.x, player1.y, &img_player1_bk,SRCPAINT);
	}
	for (int i = 0; i < BULLET_NUM; i++) {
		if (bull1[i].life) {
			putimage(bull1[i].x, bull1[i].y, &img_bullet1, SRCAND);
			putimage(bull1[i].x, bull1[i].y, &img_bullet1_bk, SRCPAINT);
		}
	}
	if (model == 1) {
		if (player2.life) {
			putimage(player2.x, player2.y, &img_player2, SRCAND);
			putimage(player2.x, player2.y, &img_player2_bk, SRCPAINT);
		}
		for (int i = 0; i < BULLET_NUM; i++) {
			if (bull2[i].life) {
				putimage(bull2[i].x, bull2[i].y, &img_bullet1, SRCAND);
				putimage(bull2[i].x, bull2[i].y, &img_bullet1_bk, SRCPAINT);
			}
		}
	}
	for (int i = 0; i < ENEMY_NUM; i++) {
		if (enemy1[i].life) {
			putimage(enemy1[i].x, enemy1[i].y, &img_enemy1, SRCAND);
			putimage(enemy1[i].x, enemy1[i].y, &img_enemy1_bk, SRCPAINT);
		}
	}
	draw_hp();
	EndBatchDraw();
}

//--------------------------------------------------------------------------------- 菜单
void game_begin() {
	loadimage(&img_menu_bk, "./images/menu_bk.jpg");
	putimage(0, 0, &img_menu_bk); 
	PlaySound(TEXT("./music/menu_bk.wav"), NULL, SND_FILENAME | SND_ASYNC);
	while (1) {
		MOUSEMSG m;
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN && m.x >= 530 && m.x <= 675 && m.y >= 165 && m.y <= 210) {
			break;
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 530 && m.x <= 675 && m.y >= 235 && m.y <= 280) {
			model = 1; break;
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 530 && m.x <= 675 && m.y >= 305 && m.y <= 350) {
			setbkmode(TRANSPARENT);
			outtextxy(150, 170, "玩家一：上下左右 空格射击"); 
			outtextxy(150, 200, "玩家二：WSAD J射击");
		}
		else if (m.uMsg == WM_LBUTTONDOWN && m.x >= 530 && m.x <= 675 && m.y >= 440 && m.y <= 490) {
			setbkmode(TRANSPARENT);
			outtextxy(150, 400, "制作人：@真心坑@"); 
		}
	}
}

//--------------------------------------------------------------------------------- 开始游戏
void game_start() {
	dataInit();
	PlaySound(TEXT("./music/sky.wav"), NULL, SND_FILENAME | SND_ASYNC);
	if (model == 0) {
		while (1) {
			creat_enemy();
			enemy_move();
			player_action(SPEED);
			bullet_move();
			game_draw();
			check_collision();
			if (score >= 20) {
				winner = 0;
				break;
			}
			else if (player1.hp <= 0) {
				PlaySound(TEXT("./music/over_1.wav"), NULL, SND_FILENAME | SND_ASYNC);
				break;
			}
		}
	}
	else {
		while (1) {
			player_action(SPEED);
			bullet_move();
			game_draw();
			check_collision();
			if (player1.hp==0) {
				winner = 2;
				break;
			}
			else if(player2.hp==0){
				winner = 1;
				break;
			}
		}
	}
}

//--------------------------------------------------------------------------------- 游戏胜利
void game_victory() {
	loadimage(&img_over0, "./images/over0.jpg");
	putimage(0, 0, &img_over0);
}
//--------------------------------------------------------------------------------- 游戏失败
void game_failure() {
	loadimage(&img_over_1, "./images/over_1.jpg");
	putimage(0, 0, &img_over_1);
}
//--------------------------------------------------------------------------------- 玩家一胜利
void game_win1() {
	loadimage(&img_over1, "./images/over1.jpg");
	putimage(0, 0, &img_over1);
}
//--------------------------------------------------------------------------------- 玩家二胜利
void game_win2() {
	loadimage(&img_over2, "./images/over2.jpg");
	putimage(0, 0, &img_over2);
}

int main()
{
	initgraph(WIDTH, HEIGHT);
	while (1) {
		game_begin();
		game_start();
		if (winner == -1) game_failure();
		else if (winner == 0)  game_victory();
		else if (winner == 1)  game_win1();
		else if (winner == 2) game_win2();
		MOUSEMSG m;
		while (1) {
			m = GetMouseMsg();
			if (m.uMsg == WM_LBUTTONDOWN && m.x >= 440 && m.x <= 740 && m.y >= 95 && m.y <= 165) {
				break;
			}
		}
		dataInit();
	}
}
