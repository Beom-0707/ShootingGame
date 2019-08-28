#include "game.h"

int checkCrush(int cx, int cy) {
	if (mainFrame[cy][cx] != EMPTY && (mainFrame[cy][cx] == WALL || mainFrame[cy][cx] == CELLING)) return false; // 막힘 false
	else return true; // 안막힘 true
}
int checkHit(int n) {
	for (int i = 0; i < MAX_MONSTER; ++i) {
		if (monster[i].flagSurv == false) continue;

		if (monster[i].mx == (int)bullet[n].bx && monster[i].my == (int)bullet[n].by) {
			monster[i].flagHit = true;
			monster[i].hp -= bullet[n].power - monster[i].armor;
			if (monster[i].hp <= 0) {
				mainFrame[monster[i].my][monster[i].mx] = EMPTY;
				--countSurvMonster;
				switch (monster[i].level) {
				case LEVEL_1:
					score += 50;
					break;
				case LEVEL_2:
					score += 100;
					break;
				case LEVEL_3:
					score += 150;
					break;
				case LEVEL_4:
					score += 200;
					break;
				}
				defMonster(i, 0, 0, 0, 0, 0, 0, false, false);
			}
			else
				mainFrame[monster[i].my][monster[i].mx] = MONSTER;
			return true;
		}
	}
	return false;
}
double getDegree(double x, double y) {
	return (double)((long long)((atan2(y, x) * 180 / PI) + 360) % 360);
}
// Function of Player
void defPlayer(int px, int py, int hp, int power, int armor, int ulti) {
	player.px = px;
	player.py = py;
	player.hp = hp;
	player.power = power;
	player.armor = armor;
	player.ulti = ulti;
}
void checkKbhit() {
	key = 0;
	if (kbhit()) {
		key = getch();
		if (key == DIR) {
			key = getch();
			switch (key) {
			case UP:
				if (checkCrush(player.px, player.py - 1) == true) {
					mainFrame[player.py][player.px] = EMPTY;
					--player.py;
					mainFrame[player.py][player.px] = PLAYER;
				}
				break;
			case DOWN:
				if (checkCrush(player.px, player.py + 1) == true) {
					mainFrame[player.py][player.px] = EMPTY;
					++player.py;
					mainFrame[player.py][player.px] = PLAYER;
				}
				break;
			case LEFT:
				if (checkCrush(player.px - 1, player.py) == true) {
					mainFrame[player.py][player.px] = EMPTY;
					--player.px;
					mainFrame[player.py][player.px] = PLAYER;
				}
				break;
			case RIGHT:
				if (checkCrush(player.px + 1, player.py) == true) {
					mainFrame[player.py][player.px] = EMPTY;
					++player.px;
					mainFrame[player.py][player.px] = PLAYER;
				}
				break;
			}
		}
		else {
			switch (key) {
			case SPACE:
				createBullet(player.px, player.py, levelBullet);
				break;
			case ESC:
				system("cls");
				exit(1);
				break;
			case P:
			case p:
				pause();
				break;
			}
		}
	}
}


// Function of Bullet
void defBullet(int n, double bx, double by, int power, int flagShot, int flagFirst) {
	bullet[n].power = power;
	bullet[n].bx = bx;
	bullet[n].by = by;
	bullet[n].flagShot = flagShot;
	bullet[n].flagFirst = flagFirst;
}
void createBullet(int px, int py, int levelBullet) {
	int flag = false;
	if (countShotBullet >= MAX_SHOT_AT_ONCE) return;

	for (int i = 0; i < MAX_BULLET; ++i) {
		if (bullet[i].flagShot == true) continue;

		switch (levelBullet) {
		case LEVEL_1:
			if (checkCrush(px, py - 1) == false) return;
			defBullet(i, px, py - 1, POWER_BULLET_1, true, true);
			++countShotBullet;
			break;
		case LEVEL_2:
			if (checkCrush(px - 1, py - 1) == true) { defBullet(i, px - 1, py - 1, POWER_BULLET_1, true, true); ++countShotBullet; }
			else flag = true;
			if (checkCrush(px + 1, py - 1) == true && flag == true) { defBullet(i, px + 1, py - 1, POWER_BULLET_1, true, true); ++countShotBullet; }
			if (checkCrush(px + 1, py - 1) == true && flag == false) { defBullet(i + 1, px + 1, py - 1, POWER_BULLET_1, true, true); ++countShotBullet; }
			break;
		case LEVEL_3:
			defBullet(i, px, py - 1, POWER_BULLET_1, true, true);
			++countShotBullet;
			break;
		case LEVEL_4:
			break;
		}
		return;
	}
}
void moveBullet() {
	for (int i = 0; i < MAX_BULLET; ++i) {
		if (bullet[i].flagShot == false) continue;

		if (bullet[i].flagFirst == true) {
			mainFrame[(int)bullet[i].by][(int)bullet[i].bx] = BULLET;
			bullet[i].flagFirst = false;
			if (checkHit(i) == true) {
				// if this code below works monster will disappear when the one doesn't move and hit bullet.
				//mainFrame[(int)bullet[i].by][(int)bullet[i].bx] = EMPTY;
				defBullet(i, 0, 0, 0, 0, false);
				--countShotBullet;
			}
			continue;
		}
		if (checkCrush((int)bullet[i].bx, (int)bullet[i].by - 1) == true) {
			mainFrame[(int)bullet[i].by][(int)bullet[i].bx] = EMPTY;
			bullet[i].by -= SPEED_BULLET * deltaTime / CLOCKS_PER_SEC; // 한번 실행될 때 (dt당) 만큼 이동
			mainFrame[(int)bullet[i].by][(int)bullet[i].bx] = BULLET;
		}
		else {
			mainFrame[(int)bullet[i].by][(int)bullet[i].bx] = EMPTY;
			defBullet(i, 0, 0, 0, 0, false);
			--countShotBullet;
		}
		if (checkHit(i) == true) {
			// if this code below works monster will disappear when the one doesn't move and hit bullet.
			//mainFrame[(int)bullet[i].by][(int)bullet[i].bx] = EMPTY;
			defBullet(i, 0, 0, 0, 0, false);
			--countShotBullet;
		}
	}
}

// Function of Monster
void defMonster(int n, int mx, int my, int level, int hp, int power, int armor, int flagHit, int flagSurv) {
	monster[n].mx = mx;
	monster[n].my = my;
	monster[n].level = level;
	monster[n].hp = hp;
	monster[n].power = power;
	monster[n].armor = armor;
	monster[n].flagHit = flagHit;
	monster[n].flagSurv = flagSurv;
}
void createMonster() {
	if (countSurvMonster >= MAX_MONSTER) return;
	

	srand((unsigned)time(0));
	switch (stage) {
	case STAGE_1: // 3 7 11
		defMonster(0, 2, 3, LEVEL_4, HP_MONSTER_3, POWER_MONSTER_3, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[0].my][monster[0].mx] = MONSTER;
		defMonster(1, 22, 3, LEVEL_4, HP_MONSTER_3, POWER_MONSTER_3, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[1].my][monster[1].mx] = MONSTER;
		defMonster(2, 12, 3, LEVEL_4, HP_MONSTER_3, POWER_MONSTER_3, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[2].my][monster[2].mx] = MONSTER;
		break;
	case STAGE_2: // (2) - 3 7 11, (4) - 4 10
		break;
	case STAGE_3:
		break;
	case STAGE_4:
		break;
	case STAGE_5:
		defMonster(0, 16, 1, LEVEL_4, HP_MONSTER_4, POWER_MONSTER_4, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[0].my][monster[0].mx] = MONSTER;

		defMonster(1, 11, 2, LEVEL_3, HP_MONSTER_3, POWER_MONSTER_3, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[1].my][monster[1].mx] = MONSTER;
		defMonster(2, 21, 2, LEVEL_3, HP_MONSTER_3, POWER_MONSTER_3, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[2].my][monster[2].mx] = MONSTER;

		defMonster(3, 8, 6, LEVEL_2, HP_MONSTER_2, POWER_MONSTER_2, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[3].my][monster[3].mx] = MONSTER;
		defMonster(4, 24, 6, LEVEL_2, HP_MONSTER_2, POWER_MONSTER_2, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[4].my][monster[4].mx] = MONSTER;
		defMonster(5, 15, 7, LEVEL_2, HP_MONSTER_2, POWER_MONSTER_2, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[5].my][monster[5].mx] = MONSTER;
		defMonster(6, 17, 7, LEVEL_2, HP_MONSTER_2, POWER_MONSTER_2, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[6].my][monster[6].mx] = MONSTER;

		defMonster(7, 6, 3, LEVEL_1, HP_MONSTER_1, POWER_MONSTER_2, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[7].my][monster[7].mx] = MONSTER;
		defMonster(8, 26, 3, LEVEL_1, HP_MONSTER_1, POWER_MONSTER_2, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[8].my][monster[8].mx] = MONSTER;
		defMonster(9, 11, 8, LEVEL_1, HP_MONSTER_1, POWER_MONSTER_2, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[9].my][monster[9].mx] = MONSTER;
		defMonster(10, 21, 8, LEVEL_1, HP_MONSTER_1, POWER_MONSTER_2, ARMOR_MONSTER_1, false, true);
		++countSurvMonster;
		mainFrame[monster[10].my][monster[10].mx] = MONSTER;

		break;
	}
}
void moveMonster() {
	for (int i = 0; i < MAX_MONSTER; ++i) {
		if (rand() % 100 + 1 > 1) continue;
		if (monster[i].flagSurv == false) continue; // 죽어있거나 생성 안됐으면 넘김
		if (monster[i].flagShot == true) continue; // 공격중이면 넘김
		int tempx = movePos[rand() % 3];
		int tempy = movePos[rand() % 3];

		if (checkCrush(monster[i].mx + tempx, monster[i].my + tempy) == true) {
			mainFrame[monster[i].my][monster[i].mx] = EMPTY;
			monster[i].mx += tempx;
			monster[i].my += tempy;
			mainFrame[monster[i].my][monster[i].mx] = MONSTER;
		}
		else --i;
	}
}

void drawBoss() {
	for (int i = 0; i < BOSS_Y; ++i) {
		for (int j = 0; j < BOSS_X - 1; ++j) {
			gotoxy(MAIN_X_ADJ + j + 4, MAIN_Y_ADJ + i + 1);
			switch (boss[i][j]) {
			case '1':
				printf("▦");
				break;
			}

		}
	}
}



// Function of Laser
void stackGaugeLaser() {
	for (int i = 0; i < MAX_LASER; ++i) {
		if (laser[i].flagShot == false) continue;

		laser[i].gaugeX += SPEED_LASER * deltaTime / CLOCKS_PER_SEC;
		laser[i].gaugeY += SPEED_LASER * deltaTime / CLOCKS_PER_SEC;
		if (laser[i].gaugeX >= 1) {
			--laser[i].gaugeX;
			++laser[i].stackX;
		}
		if (laser[i].gaugeY >= 1) {
			--laser[i].gaugeY;
			++laser[i].stackY;
		}
	}
}
void defLaser(int n, double lx, double ly, int level, int direct, int flagShot, int flagFirst) {
	laser[n].lx = lx;
	laser[n].ly = ly;
	laser[n].level = level;
	laser[n].direct = direct;
	laser[n].flagShot = flagShot;
	laser[n].flagFirst = flagFirst;
}
void createLaser() {
	int flagMid;
	int flagLeft;
	int flagRight;
	for (int i = 0; i < MAX_MONSTER; ++i) {
		if (monster[i].flagSurv == false) continue; // 사망, 비생성 상태일시 continue

		for (int j = 0; j < MAX_LASER; ++j) {
			if (laser[j].flagShot == true) continue;

			switch (monster[i].level) {
			case LEVEL_1:
				flagMid = false;
				if (checkCrush(monster[i].mx, monster[i].my + 1) == true) flagMid = true;
				if (flagMid == false) break;
				if (rand() % 100 + 1 > PROB_ATTACK_LEVEL_1) break;
				defLaser(j, monster[i].mx, monster[i].my + 1, monster[i].level, MID_LASER, true, true);
				
				break;
			case LEVEL_2:
				flagLeft = false;
				flagRight = false;
				if (checkCrush(monster[i].mx - 1, monster[i].my + 1) == true) flagLeft = true;// checkCrush Left
				if (checkCrush(monster[i].mx + 1, monster[i].my + 1) == true) flagRight = true;// checkCrush Right
				if (flagLeft == false && flagRight == false) break;
				if (rand() % 100 + 1 > PROB_ATTACK_LEVEL_2) break;

				if (flagLeft == true && flagRight == false) { // LEFT
					//if (checkCrush(monster[i].mx - 1, monster[i].my + 1) == false) break;
					defLaser(j, monster[i].mx - 1, monster[i].my + 1, monster[i].level, LEFT_LASER, true, true);
				}
				else if (flagLeft == false && flagRight == true) { // RIGHT
					//if (checkCrush(monster[i].mx + 1, monster[i].my + 1) == false) break;
					defLaser(j, monster[i].mx + 1, monster[i].my + 1, monster[i].level, RIGHT_LASER, true, true);
				}
				else { // LEFT OR RIGHT
					if (rand() % 100 + 1 > 50) { // LEFT
						//if (checkCrush(monster[i].mx - 1, monster[i].my + 1) == false) break;
						defLaser(j, monster[i].mx - 1, monster[i].my + 1, monster[i].level, LEFT_LASER, true, true);
					}
					else { // RIGHT
						//if (checkCrush(monster[i].mx + 1, monster[i].my + 1) == false) break;
						defLaser(j, monster[i].mx + 1, monster[i].my + 1, monster[i].level, RIGHT_LASER, true, true);
					}
				}
				break;
			case LEVEL_3:
				flagLeft = false;
				flagRight = false;
				if (checkCrush(monster[i].mx - 1, monster[i].my + 1) == true) flagLeft = true;// checkCrush Left
				if (checkCrush(monster[i].mx + 1, monster[i].my + 1) == true) flagRight = true;// checkCrush Right
				if (flagLeft == false && flagRight == false) break;
				if (rand() % 100 + 1 > PROB_ATTACK_LEVEL_3) break;

				if (flagLeft == true && flagRight == false) { // LEFT
					//if (checkCrush(monster[i].mx - 1, monster[i].my + 1) == false) break;
					defLaser(j, monster[i].mx - 1, monster[i].my + 1, monster[i].level, LEFT_LASER, true, true);
				}
				else if (flagLeft == false && flagRight == true) { // RIGHT
					//if (checkCrush(monster[i].mx + 1, monster[i].my + 1) == false) break;
					defLaser(j, monster[i].mx + 1, monster[i].my + 1, monster[i].level, RIGHT_LASER, true, true);
				}
				else{
					defLaser(j, monster[i].mx - 1, monster[i].my + 1, monster[i].level, LEFT_LASER, true, true);
					for (int k = 0; k < MAX_LASER; ++k) {
						if (laser[k].flagShot == true) continue;
						defLaser(k, monster[i].mx + 1, monster[i].my + 1, monster[i].level, RIGHT_LASER, true, true);
						break;
					}
				}
				break;
			case LEVEL_4:
				flagMid = false;
				flagLeft = false;
				flagRight = false;
				if (checkCrush(monster[i].mx, monster[i].my + 1) == true) flagMid = true;// checkCrush Mid
				if (checkCrush(monster[i].mx - 1, monster[i].my + 1) == true) flagLeft = true;// checkCrush Left
				if (checkCrush(monster[i].mx + 1, monster[i].my + 1) == true) flagRight = true;// checkCrush Right
				if (flagMid == false && flagLeft == false && flagRight == false) break;
				if (rand() % 100 + 1 > PROB_ATTACK_LEVEL_4) break;

				if (flagMid == true) {
					defLaser(j, monster[i].mx, monster[i].my + 1, monster[i].level, MID_LASER, true, true);
					
				}
				if (flagLeft == true && flagRight == false) { // LEFT
					//if (checkCrush(monster[i].mx - 1, monster[i].my + 1) == false) break;
					for (int k = 0; k < MAX_LASER; ++k) {
						if (laser[k].flagShot == true) continue;
						defLaser(k, monster[i].mx - 1, monster[i].my + 1, monster[i].level, LEFT_LASER, true, true);
						break;
					}
				}
				else if (flagLeft == false && flagRight == true) { // RIGHT
					//if (checkCrush(monster[i].mx + 1, monster[i].my + 1) == false) break;
					for (int k = 0; k < MAX_LASER; ++k) {
						if (laser[k].flagShot == true) continue;
						defLaser(k, monster[i].mx + 1, monster[i].my + 1, monster[i].level, RIGHT_LASER, true, true);
						break;
					}
				}
				else {
					for (int k = 0; k < MAX_LASER; ++k) {
						if (laser[k].flagShot == true) continue;
						defLaser(k, monster[i].mx - 1 , monster[i].my + 1, monster[i].level, LEFT_LASER, true, true);
						for (int l = 0; l < MAX_LASER; ++l) {
							if (laser[l].flagShot == true) continue;
							defLaser(l, monster[i].mx + 1, monster[i].my + 1, monster[i].level, RIGHT_LASER, true, true);
							break;
						}
						break;
					}
				}
				break;
			}
			break;
		}
	}
}
void moveLaser() {
	for (int i = 0; i < MAX_LASER; ++i) {
		if (laser[i].flagShot == false) continue;
		
		switch (laser[i].direct) {
		case MID_LASER:
			if (laser[i].stackY == 0) continue;
			--laser[i].stackY;

			if (laser[i].flagFirst == true) { // 첫발인 경우
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = LASER;
				laser[i].flagFirst = false;
				continue;
			}
			if (checkCrush((int)laser[i].lx, (int)laser[i].ly + 1) == true) {
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = EMPTY;
				++laser[i].ly;
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = LASER;
			}
			else {
				if (stage >= STAGE_1) {
					convBlock(i, PROB_MAKE_BLOCK);
					break;
				}
				resetLaser(i);
			}
			break;
		case LEFT_LASER:
			if (laser[i].stackX == 0 || laser[i].stackY == 0) continue;
			--laser[i].stackX;
			--laser[i].stackY;

			if (laser[i].flagFirst == true) { // 첫발인 경우
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = LASER;
				laser[i].flagFirst = false;

				if ((int)laser[i].lx == 1) laser[i].direct = RIGHT_LASER;
				continue;
			}

			if (checkCrush((int)laser[i].lx - 1, (int)laser[i].ly + 1) == true) {
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = EMPTY;
				--laser[i].lx;
				++laser[i].ly;
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = LASER;

				if ((int)laser[i].lx == 1) laser[i].direct = RIGHT_LASER;
			}
			else {
				if (stage >= STAGE_1) {
					convBlock(i, PROB_MAKE_BLOCK);
					break;
				}
				resetLaser(i);
			}
			break;
		case RIGHT_LASER:
			if (laser[i].stackX == 0 || laser[i].stackY == 0) continue;
			--laser[i].stackX;
			--laser[i].stackY;

			if (laser[i].flagFirst == true) { // 첫발인 경우
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = LASER;
				laser[i].flagFirst = false;
				if ((int)laser[i].lx == MAIN_X - 2) laser[i].direct = LEFT_LASER;
				continue;
			}
			if (checkCrush((int)laser[i].lx + 1, (int)laser[i].ly + 1) == true) {
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = EMPTY;
				++laser[i].lx;
				++laser[i].ly;
				mainFrame[(int)laser[i].ly][(int)laser[i].lx] = LASER;

			if ((int)laser[i].lx == MAIN_X - 2) laser[i].direct = LEFT_LASER;

			}
			else {
				if (stage >= STAGE_1) {
					convBlock(i, PROB_MAKE_BLOCK);
					break;
				}
				resetLaser(i);
			}
			break;
		}
	}
}
void resetLaser(int n) {
	mainFrame[(int)laser[n].ly][(int)laser[n].lx] = EMPTY;
	defLaser(n, 0, 0, 0, 0, false, true);
	laser[n].gaugeX = 0;
	laser[n].gaugeY = 0;
	laser[n].stackX = 0;
	laser[n].stackY = 0;
}
void convBlock(int n, int prob) {
	if (rand() % 100 + 1 > prob)
		resetLaser(n);
	else {
		mainFrame[(int)laser[n].ly][(int)laser[n].lx] = WALL;
		defLaser(n, 0, 0, 0, 0, false, true);
	}
}


void stackGaugeGLaser() {
	for (int i = 0; i < MAX_GLASER; ++i) {
		if (gLaser[i].flagSurv == false) continue;

		gLaser[i].gaugeX += SPEED_GLASER * deltaTime / CLOCKS_PER_SEC;
		gLaser[i].gaugeY += SPEED_GLASER * deltaTime / CLOCKS_PER_SEC;
		if (gLaser[i].gaugeX >= 1) {
			--gLaser[i].gaugeX;
			++gLaser[i].stackX;
		}
		if (gLaser[i].gaugeY >= 1) {
			--gLaser[i].gaugeY;
			++gLaser[i].stackY;
		}
	}
}
void defGLaser(int n, double gmx, double gmy, int flagSurv) {
	gLaser[n].gmx = gmx;
	gLaser[n].gmy = gmy;
	gLaser[n].flagSurv = flagSurv;
}
void createGLaser() {
	for (int i = 0; i < MAX_GLASER; ++i) {
		if (gLaser[i].flagSurv == true) continue;

		switch (stage) {
		case STAGE_1:
			defGLaser(i, 5, 5, true);
			mainFrame[(int)gLaser[i].gmy][(int)gLaser[i].gmx] = GLASER;
			break;
		case STAGE_2:
			break;
		case STAGE_3:
			break;
		case STAGE_4:
			break;
		case STAGE_5:
			break;
		}
	}
}
void moveGLaser() {
	double tempx = 0;
	double tempy = 0;
	for (int i = 0; i < MAX_GLASER; ++i) {
		if (gLaser[i].flagSurv == false) continue;
		if (gLaser[i].gmx == player.px && gLaser[i].gmy == player.py) continue;
		if (gLaser[i].stackX == 0 || gLaser[i].stackY == 0) continue;

		// x
		if (getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) >= 22.5 * 13 ||
			(getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) >= 22.5 * 0 && getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) <= 22.5 * 3))
			tempx = 1;
		else if (getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) >= 22.5 * 5 &&
			getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) <= 22.5 * 11)
			tempx = -1;
		else tempx = 0;

		// y
		if (getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) >= 22.5 * 1 &&
			getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) <= 22.5 * 7) tempy = 1;
		else if (getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) >= 22.5 * 9 &&
			getDegree((double)(player.px - gLaser[i].gmx), (double)(player.py - gLaser[i].gmy)) <= 22.5 * 15) tempy = -1;
		else tempy = 0;

		if (checkCrush(gLaser[i].gmx + tempx, gLaser[i].gmy + tempy) == true) {
			mainFrame[(int)gLaser[i].gmy][(int)gLaser[i].gmx] = EMPTY;
			gLaser[i].gmx += tempx;
			gLaser[i].gmy += tempy;
			mainFrame[(int)gLaser[i].gmy][(int)gLaser[i].gmx] = GLASER;
			--gLaser[i].stackX;
			--gLaser[i].stackY;
		}
	}
}

// Function of Frame
void reset() {

	//----Initialize Variables----
	// Game
	tempTime = -1;
	t = 0;
	dt = 1 / 60.0;
	// Player
	defPlayer(MAIN_X / 2, MAIN_Y - 4, 100, 100, 0, 0);
	key = 0;
	stage = STAGE_5;
	score = 0;

	// Bullet
	maxBullet = MAX_SHOT_AT_ONCE;
	levelBullet = LEVEL_1;
	countBullet = 0;
	countShotBullet = 0;
	
	for (int i = 0; i < MAX_BULLET; ++i)
		bullet[i].flagShot = false;
	// Monster
	strcpy(boss[0],  "0010001010100010101000100");
	strcpy(boss[1],  "0101001010100010101001010");
	strcpy(boss[2],  "1000101010100010101010001");
	strcpy(boss[3],  "1000101010100010101010001");
	strcpy(boss[4],  "1000100100011100010010001");
	strcpy(boss[5],  "0101010000000000000101010");
	strcpy(boss[6],  "0010001110000000111000100");
	strcpy(boss[7],  "0010010001000001000100100");
	strcpy(boss[8],  "0001100000100010000011000");
	strcpy(boss[9],  "0000010001111111000100000");
	strcpy(boss[10], "0000001110000000111000000");
	strcpy(boss[11], "0000000111111111110000000");



	movePos[0] = -1;
	movePos[1] = 0;
	movePos[2] = 1;
	countSurvMonster = 0;

	for (int i = 0; i < MAX_MONSTER; ++i)
		monster[i].flagShot = false;

	// Laser
	levelLaser = LEVEL_1;

	for (int i = 0; i < MAX_LASER; ++i)
		laser[i].flagShot = false;
}
void resetMF() {
	for (int i = 0; i < MAIN_Y; ++i)
		for (int j = 0; j < MAIN_X; ++j)
			mainFrame[i][j] = 0;

	for (int i = 0; i < MAIN_Y; ++i) { // Side Walls
		mainFrame[i][0] = WALL;
		mainFrame[i][MAIN_X - 1] = WALL;
	}

	for (int i = 0; i < MAIN_X; ++i) // Floor Wall
		mainFrame[MAIN_Y - 1][i] = WALL;

	for (int i = 1; i < MAIN_X - 1; ++i) { // Celling
		mainFrame[0][i] = CELLING;
	}
	mainFrame[player.py][player.px] = PLAYER; // Player
}
void resetCF() {
	for (int i = 0; i < MAIN_Y; ++i)
		for (int j = 0; j < MAIN_X; ++j)
			copyFrame[i][j] = 100;
}
void drawMF() {
	for (int i = 0; i < MAIN_Y; ++i) {
		for (int j = 0; j < MAIN_X; ++j) {
			if (copyFrame[i][j] != mainFrame[i][j]) {
				gotoxy(MAIN_X_ADJ + j, MAIN_Y_ADJ + i);
				switch (mainFrame[i][j]) {
				case GLASER:
					printf("◈");
					break;
				case LASER:
					printf("⊙");
					break;
				case BULLET:
					printf("º");
					break;
				case CELLING:
					printf("..");
					break;
				case WALL:
					printf("▩");
					break;
				case EMPTY:
					printf("  ");
					break;
				case MONSTER:
					printf("＠");
					break;
				case PLAYER:
					printf("△");
					break;
				}
			}
		}
	}

	for (int i = 0; i < MAIN_Y; ++i) {
		for (int j = 0; j < MAIN_X; ++j) {
			copyFrame[i][j] = mainFrame[i][j];
		}
	}
}
void drawMap() {
	int x = MAIN_X_ADJ + MAIN_X + MAIN_X_ADJ;
	int y = MAIN_Y_ADJ;
	gotoxy(x, y + 0);
	printf("┌───────────────────────────┐ ");
	gotoxy(x, y + 1);
	if (stage == STAGE_1)
		printf("│ STAGE : ☆--＊--＊--＊--★│ ");
	else if (stage == STAGE_2)
		printf("│ STAGE : ＊--☆--＊--＊--★│ ");
	else if (stage == STAGE_3)
		printf("│ STAGE : ＊--＊--☆--＊--★│ ");
	else if (stage == STAGE_4)
		printf("│ STAGE : ＊--＊--＊--☆--★│ ");
	else
		printf("│ STAGE : ＊--＊--＊--＊--★│ ");

	gotoxy(x, y + 2);
	printf("│ SCORE :              %5d│ ", score);
	gotoxy(x, y + 3);
	printf("│ BULLET LEVEL :           %d│ ", levelBullet);
	gotoxy(x, y + 4);
	printf("│ SURVIVE MONSTER :       %2d│ ", countSurvMonster);
	gotoxy(x, y + 5);
	printf("└───────────────────────────┘ ");
	gotoxy(x, y + 6);
	printf("┌─────  PLAYER ─────┐ ");
	gotoxy(x, y + 7);
	printf("│ HP           : %3d│ ", player.hp);
	gotoxy(x, y + 8);
	printf("│ POWER        : %3d│ ", player.power);
	gotoxy(x, y + 9);
	printf("│ ARMOR        : %3d│ ", player.armor);
	gotoxy(x, y + 10);
	printf("│ ULTIMATE     : %3d│ ", player.ulti);
	gotoxy(x, y + 11);
	printf("└───────────────────┘ ");

	gotoxy(x, y + 17);
	printf("  △   : UP             SPACE : SHOOTING");
	gotoxy(x, y + 18);
	printf("◁  ▷ : LEFT / RIGHT     P   : PAUSE");
	gotoxy(x, y + 19);
	printf("  ▽   : DOWN            ESC  : QUIT");

	x = MAIN_X_ADJ;
	y = MAIN_Y_ADJ + MAIN_Y;
	gotoxy(x, y + 0);
	printf("x : %2d, y : %2d		DEGREE : %.1lf", player.px, player.py, getDegree((double)(player.px - gLaser[0].gmx), (double)(player.py - gLaser[0].gmy)));
	gotoxy(x, y + 1);
	printf("TIME  : %2dm %2ds", ((int)t / CLOCKS_PER_SEC) / 60, ((int)t / CLOCKS_PER_SEC) % 60);
}
void result() { // 1. 점수, 2. 스테이지, 3. 플레이타임
	int x = MAIN_X_ADJ;
	int y = MAIN_Y_ADJ;
	gotoxy(x + 10, y + 8);
	printf("┌───────────────────┐ ");
	gotoxy(x + 10, y + 9);
	printf("│  SCORE :    %4d  │ ", score);
	gotoxy(x + 10, y + 10);
	printf("│  STAGE :    %4d  │ ", stage);
	gotoxy(x + 10, y + 11);
	printf("│  TIME  : %2dm %2ds  │ ", ((int)t / CLOCKS_PER_SEC) / 60, ((int)t / CLOCKS_PER_SEC) % 60);
	gotoxy(x + 10, y + 12);
	printf("└───────────────────┘ ");


	getch();
	exit(1);
}
void pause() {
	int x = MAIN_X_ADJ + MAIN_X + MAIN_X_ADJ;
	int y = MAIN_Y_ADJ;
	gotoxy(x, y + 12);
	printf("┌───────────  P A U S E ───────────┐ ");
	gotoxy(x, y + 13);
	printf("│                                  │ ");
	gotoxy(x, y + 14);
	printf("│     PRESS ANY KEY TO CONTINUE    │ ");
	gotoxy(x, y + 15);
	printf("│                                  │ ");
	gotoxy(x, y + 16);
	printf("└──────────────────────────────────┘ ");

	tempTime = newTime;

	getch();

	system("cls");
	resetCF();
	drawMF();
	drawMap();
}
