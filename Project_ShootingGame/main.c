#include "main.h"

int main() {
	init();
	reset();


	system("cls");
	resetMF();
	resetCF();
	createMonster();
	//createGLaser();
	drawMF();
	drawMap();

	currentTime = clock();
	while (1) {
		if (newTime != tempTime) {
			newTime = clock();
			frameTime = newTime - currentTime;
			currentTime = newTime;
		}
		else {
			tempTime = -1;

			currentTime = clock();
			newTime = clock();
			frameTime = newTime - currentTime;
			currentTime = newTime;
		}

		

		checkKbhit();
		//createLaser();

		//moveMonster();
		while (frameTime > 0.0) {
			deltaTime = min(frameTime, dt);

			moveBullet();
			//moveGLaser();
			//moveLaser();
			moveItem();


			if (countSurvMonster == 0) {
				for (int i = 0; i < MAX_MONSTER; ++i)
					defMonster(i, 0, 0, 0, 0, 0, 0, false, false);
				for (int i = 0; i < MAX_GLASER; ++i)
					defGLaser(i, 0, 0, false);
				countSurvMonster = 0;

				if (stage == 5)
					result();
				else {
					++stage;

					system("cls");
					resetMF();
					resetCF();
					createMonster();
					createGLaser();
					drawMF();
					drawMap();

					pause();
				}
			}
			frameTime -= deltaTime;
			t += deltaTime;
			stackGaugeLaser();
			stackGaugeGLaser();
			stackGaugeItem();
			drawMF();
		}

		//drawBoss();
		drawMF();
		drawMap();
   	}
}
