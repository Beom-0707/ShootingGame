#include "main.h"

//--------------------------------------
// Fucntion
//--------------------------------------

int checkCrush(int cx, int cy);
double getDegree(double x, double y);
// Function of Player
void defPlayer(int px, int py, int hp, int power, int armor, int ulti);
void checkKbhit();

// Function of Bullet
void defBullet(int n, double bx, double by, int power, int flagShot, int flagFirst);
void createBullet(int px, int py, int levelBullet);
void moveBullet();

// Function of Monster
void defMonster(int n, int mx, int my, int level, int hp, int power, int armor, int flagHit, int flagSurv);
void createMonster();
int checkHit(int n);

void drawBoss();

// Function of Laser
void stackGaugeLaser();
void defLaser(int n, double lx, double ly, int level, int direct, int flagShot, int flagFirst);
void createLaser();
void moveLaser();
void resetLaser(int n);
void convBlock(int n, int prob);

// Function of GLaser
void stackGaugeGLaser();
void defGLaser(int n, double gmx, double gmy, int flagSurv);
void createGLaser();
void moveGLaser();

// Function of Item
void stackGaugeItem();
void defItem(int n, int ix, int iy, int flagOn);
void spawnItem();
void moveItem();

// Function of Frame
void reset();
void resetMF(); // Set MainFrame
void resetCF();
void drawMF(); // Draw MainFrame
void drawMap();
void result();
void pause();
// Function of Score & Level
