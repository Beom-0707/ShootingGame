#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
//--------------------------------------
// Constant
//--------------------------------------
// MATH
#define PI 3.1415926535
// ASCII
#define DIR 224 // For checking if it's direction
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define SPACE 32
#define ESC 27
#define P 80
#define p 112

// FRAME & GAME
#define MAIN_X 33 // Size of X of Frame
#define MAIN_Y 27 // Size of Y of Frame
#define MAIN_X_ADJ 3
#define MAIN_Y_ADJ 1
#define SPEED_GAME 60

// STAGE
#define STAGE_1 1
#define STAGE_2 2
#define STAGE_3 3
#define STAGE_4 4
#define STAGE_5 5

// LEVEL
#define LEVEL_1 1
#define LEVEL_2 2
#define LEVEL_3 3
#define LEVEL_4 4
#define LEVEL_5 5

// DRAWING
#define GLASER 6
#define LASER 5
#define BULLET 4
#define CELLING 3
#define WALL_SIDE 2
#define WALL 1
#define EMPTY 0
#define MONSTER -1
#define PLAYER -2
#define ITEM_HP -3
#define ITEM_ARMOR -4
#define ITEM_BULLET -5
#define ITEM_ULTI -6



// BULLET
#define MAX_BULLET 10
#define MAX_SHOT_AT_ONCE 5

#define SPEED_BULLET 20

#define POWER_BULLET_1 100
#define POWER_BULLET_2 150
#define POWER_BULLET_3 200
#define POWER_BULLET_4 300

// MONSTER
#define MAX_MONSTER 15
#define SPEED_MONSTER 1

#define HP_MONSTER_1 100
#define HP_MONSTER_2 200
#define HP_MONSTER_3 300
#define HP_MONSTER_4 400
#define HP_MONSTER_5 500

#define POWER_MONSTER_1 100
#define POWER_MONSTER_2 200
#define POWER_MONSTER_3 300
#define POWER_MONSTER_4 400
#define POWER_MONSTER_5 500

#define ARMOR_MONSTER_1 0
#define ARMOR_MONSTER_2 50
#define ARMOR_MONSTER_3 100
#define ARMOR_MONSTER_4 150
#define ARMOR_MONSTER_5 200

#define BOSS_X 26
#define BOSS_Y 12

// LASER
#define MAX_LASER 200
#define SPEED_LASER 10
#define PROB_ATTACK_LEVEL_1 5
#define PROB_ATTACK_LEVEL_2 5
#define PROB_ATTACK_LEVEL_3 5
#define PROB_ATTACK_LEVEL_4 5
#define PROB_ATTACK_LEVEL_5 5
#define PROB_MAKE_BLOCK 5
#define LEFT_LASER 1
#define MID_LASER 0
#define RIGHT_LASER -1

// GLASER
#define MAX_GLASER 1
#define SPEED_GLASER 10

// ITEM
#define MAX_ITEM 5
#define SPEED_ITEM 1
#define PROB_SPAWN_ITEM 10

// PERCENTAGE : ULTI < ARMOR < HP < BULLET
// THESE CONSTANTS ARE CUMULATIVE FREQUENCYS
#define PROB_SPAWN_ITEM_BULLET 100 // 65
#define PROB_SPAWN_ITEM_HP 35 // 20
#define PROB_SPAWN_ITEM_ARMOR 15 // 10
#define PROB_SPAWN_ITEM_ULTI 5 // 5

// T/F
#define true 1
#define false 0


//--------------------------------------
// Variable & Array & Struct
//--------------------------------------

typedef int bool;

// Frame
int mainFrame[MAIN_Y][MAIN_X];
int copyFrame[MAIN_Y][MAIN_X];

// Game
int stage;
int score;
double t;
double dt;
double deltaTime;
double currentTime;
double frameTime;
double newTime;
double tempTime;

// Player
int key;

typedef struct _Player {
	int px;
	int py;
	int hp;
	int power;
	int armor;
	int ulti;
} Player;
Player player;

// Bullet
int levelBullet;
int numBullet;
int countBullet;
int countShotBullet;
int maxBullet;

typedef struct _Bullet{
	double bx;
	double by;

	int speed;
	int power;

	int flagShot;
	int flagFirst;
} Bullet;
Bullet bullet[MAX_BULLET];

// Monster
char boss[BOSS_Y][BOSS_X];

int movePos[3];
int countSurvMonster;

typedef struct _Monster {
	int mx;
	int my;

	int level;
	int hp;
	int power;
	int armor;
	int countLaser;

	int flagShot;
	int flagHit;
	int flagSurv;
	double flagMove;
} Monster;
Monster monster[MAX_MONSTER];

// Laser
int levelLaser;


typedef struct _Laser {
	double gauge;
	int stack;

	int lx;
	int ly;

	int level;
	int direct;

	bool flagShot;
	bool flagFirst;
} Laser;
Laser laser[MAX_LASER];	


typedef struct _GuidedMonster {
	double gauge;
	int stack;
	int gmx;
	int gmy;

	bool flagSurv;
} GuidedMonster;
GuidedMonster gLaser[MAX_GLASER];

// Item
typedef struct _Item{
	double gauge;
	int stack;
	int ix;
	int iy;

	bool flagSurv;

} Item;
Item item[MAX_ITEM];