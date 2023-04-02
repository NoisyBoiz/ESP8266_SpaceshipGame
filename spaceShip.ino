#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
using namespace std;

int button1 = 16;
int button3 = 12;
int button4 = 14;
int button5 = 13;
int button6 = 15;

Adafruit_SSD1306 display(128, 64, &Wire, -1);

static const unsigned char PROGMEM iconSpaceShip[] = { 0x10, 0x10, 0xBA, 0xFE, 0xFE, 0xFE, 0xBA, 0x92 };
static const unsigned char PROGMEM iconEnemy[] = { 0x42, 0x3C, 0x7E, 0xDB, 0xFF, 0x7E, 0xDB, 0xA5 };
static const unsigned char PROGMEM iconDropHeart[] = { 0xFE, 0x82, 0x92, 0xBA, 0x92, 0x82, 0xFE };
static const unsigned char PROGMEM iconDropBull[] = { 0xFE, 0x82, 0xAA, 0xAA, 0xAA, 0x82, 0xFE };
static const unsigned char PROGMEM iconHeart[] = { 0x6C, 0xFE, 0xFE, 0xFE, 0x7C, 0x38, 0x10 };
static const unsigned char PROGMEM iconBull[] = { 0x70, 0xF8, 0xF8, 0xF8, 0xF8, 0x70, 0xF8 };
static const unsigned char PROGMEM iconScores[] = { 0x38, 0x7C, 0xEE, 0xEE, 0xEE, 0x7C, 0x38 };
static const unsigned char PROGMEM iconBoss[] = { 0x04, 0x39, 0x9C, 0x20, 0x02, 0xFF, 0xFF, 0x40, 0x4F, 0xD7, 0xEB, 0xF2, 0x2F, 0xC7, 0xE3, 0xF4, 0x3F, 0xFE, 0x7F, 0xFC, 0xFF, 0xFC, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x66, 0x66, 0x66, 0x66 };

int gameStart = 0;
int widthBox = 100;
int Xship = widthBox / 2 - 3;
int Yship = 55;
int speechShip = 2;
int heart = 3;
int limitHeart = 3;
int checkDead = 0;
int timeUndead = 0;
int limitBull = 50;
int timeCreBull = 0;
int numberOfBull = 50;
int limitEnemy = 8;
int scores = 0;
int timeDelay = 20;
int timeCreEnemy = random(5, 10);
int limitDropHeart = 2;
int limitDropBull = 3;
int numberOfDropHeart = 0;
int numberOfDropBull = 0;
int checkCatchDropBull = 0;
int checkCatchDropHeart = 0;
int timeDropHeart = 30;
int timeDropBull = 30;
int creBoss = 0;
int winGame = 0;
class Bullet {
public:
  int x;
  int y;
  int checkClear = 0;
  int speech = 2;
  void bullRun() {
    this->y -= this->speech;
    if (this->y < 1) this->checkClear = 1;
  };
};
vector<Bullet> vecBull;

class Enemy {
public:
  int boss = 0;
  int widthEnemy = 8;
  int heightEnemy = 8;
  int hp = 1;
  int x;
  int y;
  int setX;
  int setY;
  int checkClear = 0;
  int speech = 1;
  int move = 0;
  int lengMove;
  int bullNow = 0;
  int limitBullEnemy = 10;
  int timeCreBullEnemy;
  void run() {
    if (move == 0) {
      if (x > setX) x -= speech;
      else if (x < setX) x += speech;
      if (y < setY) y += speech;
      if (x == setX && y == setY) {
        if (x > widthBox - 5) move = -1;
        else move = 1;
      }
    }
    if (move == 1) {
      x += speech;
      if (x - setX > lengMove) move = -1;
    }
    if (move == -1) {
      x -= speech;
      if (setX - x > lengMove) move = 1;
    }
  }
  void checkShoot() {
    for (int i = 0; i < vecBull.size(); i++) {
      if (x <= vecBull[i].x && x + widthEnemy - 1 >= vecBull[i].x && y <= vecBull[i].y && y + heightEnemy - 1 >= vecBull[i].y) {
        hp--;
        if (hp <= 0) checkClear = 1;
        vecBull.erase(vecBull.begin() + i, vecBull.begin() + i + 1);
      }
    }
  }
};
vector<Enemy> vecEnemy;
class BullEnemy {
public:
  float x;
  float y;
  float speechX = 0;
  float speechY = 2;
  int checkClear = 0;
  int type = 0;
  void bullEnemyRun() {
    if (type != 0) {
      float minusX = Xship - x;
      float minusY = Yship - y;
      float rad = atan(minusX / minusY);
      if (minusX > 7 || minusY > 7) {
        speechX = sin(rad) * 2;
        speechY = cos(rad) * 2;
      }
    }
    x += speechX;
    y += speechY;
    if (y > 64 || x < 1 || x >= widthBox) checkClear = 1;
    if (y + 1 >= Yship && y <= Yship + 7 && x >= Xship && x <= Xship + 6 && timeUndead == 0) {
      checkDead = 1;
      checkClear = 1;
    }
  }
};
vector<BullEnemy> vecBullEnemy;
class DropHeart {
public:
  int x;
  int y;
  int checkClear = 0;
  int speech = 1;
  void run() {
    y += speech;
    if (y > 64) checkClear = 1;
    if (((y <= Yship && Yship <= y + 6) || (Yship <= this->y && this->y <= Yship + 6)) && ((this->x <= Xship && Xship <= this->x + 6) || (Xship <= this->x && this->x <= Xship + 6))) {
      if (heart < limitHeart) heart++;
      checkCatchDropHeart = 0;
      this->checkClear = 1;
    }
  }
};
class DropBull {
public:
  int x;
  int y;
  int checkClear = 0;
  int speech = 1;

  void run() {
    this->y += this->speech;
    if (this->y > 64) this->checkClear = 1;
    if (((this->y <= Yship && Yship <= this->y + 6) || (Yship <= this->y && this->y <= Yship + 6)) && ((this->x <= Xship && Xship <= this->x + 6) || (Xship <= this->x && this->x <= Xship + 6))) {
      numberOfBull += 25;
      if (numberOfBull > limitBull) numberOfBull = 50;
      this->checkClear = 1;
    }
  }
};
vector<DropHeart> vecDropHeart;
vector<DropBull> vecDropBull;
void setup() {
  Serial.begin(9600);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.display();
  display.setTextSize(1);
  display.setCursor(0, 10);
  display.setTextColor(WHITE);
  display.setRotation(0);
}
void loop() {
  display.clearDisplay();
  uint8_t valB1 = digitalRead(button1);
  if (valB1 == HIGH) gameStart = 1;
  if (gameStart == 1) {
    if (heart > 0) {
      if (winGame == 1) {
        uint8_t valB1 = digitalRead(button1);
        if (valB1 == HIGH) playAgain();
        printEndGame(1);
        delay(200);
      } else {
        printStatus();
        if (checkDead == 1) resetShip();
        uint8_t valB1 = digitalRead(button1);
        uint8_t valB4 = digitalRead(button4);
        uint8_t valB6 = digitalRead(button6);
        if (timeCreBull > 0) timeCreBull--;
        if (valB4 == HIGH && Xship + 7 < widthBox) Xship += speechShip;
        if (valB6 == HIGH && Xship > 1) Xship -= speechShip;
        if (timeUndead != 0) {
          if (timeUndead % 10 < 5) display.drawBitmap(Xship, Yship, iconSpaceShip, 7, 8, 1);
          timeUndead--;
        } else display.drawBitmap(Xship, Yship, iconSpaceShip, 7, 8, 1);
        if (scores == 5 && creBoss == 0) creBoss = 1;
        if (creBoss == 0) creNewEnemy(0);
        else {
          if (vecEnemy.size() == 0 && creBoss == 1) {
            creBoss = 2;
            creNewEnemy(1);
          }
        }
        printEnemy();
        printBullEnemy();
        if (valB1 == HIGH) creNewBullet();
        if (vecBull.size() > 0) printBullet();
        if (numberOfBull < 5 && checkCatchDropBull == 0) {
          if (timeDropBull == 0) {
            checkCatchDropBull = 1;
            timeDropBull = 30;
            creDropBull(random(5, widthBox - 5), 0);
          } else timeDropBull--;
        }
        if (heart == 1 && checkCatchDropHeart == 0) {
          if (timeDropHeart == 0) {
            checkCatchDropHeart = 1;
            timeDropHeart = 30;
            creDropHeart(random(5, widthBox - 5), 0);
          } else timeDropHeart--;
        }
        if (vecDropHeart.size() > 0) printDropHeart();
        if (vecDropBull.size() > 0) printDropBull();
        display.display();
        delay(timeDelay);
      }
    } else {
      uint8_t valB1 = digitalRead(button1);
      if (valB1 == HIGH) playAgain();
      printEndGame(0);
      delay(200);
    }
  } else {
    display.setCursor(0, 25);
    display.println("Press OK");
    display.print("to start game!");
    display.display();
    delay(200);
  }
}
void printStatus() {
  display.drawLine(widthBox, 0, widthBox, 64, 1);
  display.drawBitmap(widthBox + 3, 25, iconScores, 7, 7, 1);
  display.setCursor(widthBox + 12, 25);
  display.print(scores, 1);
  display.drawBitmap(widthBox + 3, 38, iconHeart, 7, 7, 1);
  display.setCursor(widthBox + 12, 38);
  display.print(heart);
  display.drawBitmap(widthBox + 3, 51, iconBull, 7, 7, 1);
  display.setCursor(widthBox + 12, 51);
  display.print(numberOfBull);
}
void printEndGame(int x) {
  display.setCursor(0, 0);
  display.setTextSize(2);
  if (x == 0) display.println("Game Over!");
  else display.println("You Win!");
  display.setTextSize(1);
  display.println("");
  display.println("");
  display.print("Your Scores: ");
  display.print(scores);
  display.display();
}
void creNewBullet() {
  if (timeCreBull == 0 && numberOfBull != 0) {
    Bullet bul;
    bul.x = Xship + 3;
    bul.y = Yship - 1;
    vecBull.push_back(bul);
    timeCreBull = 4;
    numberOfBull--;
  }
}
void printBullet() {
  for (int i = 0; i < vecBull.size(); i++) {
    if (vecBull[i].checkClear == 0) {
      display.drawRect(vecBull[i].x, vecBull[i].y, 1, 2, 1);
      vecBull[i].bullRun();
    } else vecBull.erase(vecBull.begin() + i, vecBull.begin() + i + 1);
  }
}
void creNewEnemy(int x) {
  if (x == 0) {
    if (timeCreEnemy == 0) {
      timeCreEnemy = 60;
      if (vecEnemy.size() < limitEnemy) {
        Enemy enemy;
        enemy.setX = random(1, widthBox / 8 - 1) * 8 + random(-3, 4);
        enemy.setY = random(2, 5) * 3 + random(0, 2);
        enemy.x = enemy.setX + random(-5, 5);
        enemy.y = 1;
        enemy.lengMove = random(8, 16);
        enemy.timeCreBullEnemy = random(20, 50);
        vecEnemy.push_back(enemy);
      }
    } else timeCreEnemy--;
  } else {
    Enemy enemy;
    enemy.boss = 1;
    enemy.widthEnemy = 32;
    enemy.heightEnemy = 8;
    enemy.hp = 50;
    enemy.setX = widthBox / 2 - 16;
    enemy.setY = 12;
    enemy.x = widthBox / 2 - 16;
    enemy.y = 1;
    enemy.lengMove = 30;
    enemy.timeCreBullEnemy = random(20, 50);
    vecEnemy.push_back(enemy);
  }
}
void printEnemy() {
  for (int i = 0; i < vecEnemy.size(); i++) {
    vecEnemy[i].checkShoot();
    if (vecEnemy[i].checkClear == 0) {
      if (vecEnemy[i].boss == 0) display.drawBitmap(vecEnemy[i].x, vecEnemy[i].y, iconEnemy, vecEnemy[i].widthEnemy, vecEnemy[i].heightEnemy, 1);
      else {
        display.drawRect(1, 1, 52, 5, 1);
        display.fillRect(1, 1, vecEnemy[i].hp, 3, 1);
        display.drawBitmap(vecEnemy[i].x, vecEnemy[i].y,iconBoss, vecEnemy[i].widthEnemy, vecEnemy[i].heightEnemy, 1);
      }
      vecEnemy[i].run();
      if (vecEnemy[i].timeCreBullEnemy == 0) {
        vecEnemy[i].timeCreBullEnemy = random(50, 100);
        if (vecEnemy[i].boss == 0) {
          if (vecEnemy[i].bullNow < vecEnemy[i].limitBullEnemy) {
            BullEnemy bull;
            vecEnemy[i].bullNow++;
            bull.x = vecEnemy[i].x + 4;
            bull.y = vecEnemy[i].y + 8;
            vecBullEnemy.push_back(bull);
          } else vecEnemy[i].bullNow -= random(3, vecEnemy[i].limitBullEnemy);
        } else {
          int randC = random(0, 3);
          if (randC == 0) {
            int xRand = random(0, widthBox / 14);
            for (int i = 0; i < 6; i++) {
              BullEnemy bull;
              bull.x = i * (widthBox / 6) + 1 + xRand;
              bull.y = 15;
              vecBullEnemy.push_back(bull);
            }
          }
          if (randC == 1) {
            for (int i = 1; i < 8; i++) {
              BullEnemy bull;
              float rad = M_PI + i * M_PI / 8;
              bull.speechX = cos(rad) * 2;
              bull.speechY = -sin(rad) * 2;
              bull.x = vecEnemy[0].x + 16;
              bull.y = 15;
              vecBullEnemy.push_back(bull);
            }
          }
          if (randC == 2) {
            BullEnemy bull;
            bull.type = 1;
            bull.x = vecEnemy[0].x + 16;
            bull.y = 15;
            vecBullEnemy.push_back(bull);
          }
        }
      } else vecEnemy[i].timeCreBullEnemy--;
    } else {
      scores++;
      if (creBoss == 2) winGame = 1;
      int rand = random(0, 15);
      if (rand == 5) creDropHeart(vecEnemy[i].x, vecEnemy[i].y);
      if (rand < 3) creDropBull(vecEnemy[i].x, vecEnemy[i].y);
      vecEnemy.erase(vecEnemy.begin() + i, vecEnemy.begin() + i + 1);
    }
  }
}

void printBullEnemy() {
  for (int i = 0; i < vecBullEnemy.size(); i++) {
    if (!vecBullEnemy[i].checkClear) {
      display.drawRect(vecBullEnemy[i].x, vecBullEnemy[i].y, 1, 2, 1);
      vecBullEnemy[i].bullEnemyRun();
    } else {
      vecBullEnemy.erase(vecBullEnemy.begin() + i, vecBullEnemy.begin() + i + 1);
    }
  }
}

void resetShip() {
  Xship = widthBox / 2 - 3;
  Yship = 55;
  heart--;
  timeUndead = 100;
  checkDead = 0;
}
void creDropHeart(int x, int y) {
  if (numberOfDropHeart < limitDropHeart) {
    numberOfDropHeart++;
    DropHeart dHeart;
    dHeart.x = x;
    dHeart.y = y;
    vecDropHeart.push_back(dHeart);
  }
}
void creDropBull(int x, int y) {
  if (numberOfDropBull < limitDropBull) {
    numberOfDropBull++;
    DropBull dBull;
    dBull.x = x;
    dBull.y = y;
    vecDropBull.push_back(dBull);
  }
}
void printDropHeart() {
  for (int i = 0; i < vecDropHeart.size(); i++) {
    if (!vecDropHeart[i].checkClear) {
      display.drawBitmap(vecDropHeart[i].x, vecDropHeart[i].y, iconDropHeart, 7, 7, 1);
      vecDropHeart[i].run();
    } else {
      numberOfDropHeart--;
      vecDropHeart.erase(vecDropHeart.begin() + i, vecDropHeart.begin() + i + 1);
    }
  }
}
void printDropBull() {
  for (int i = 0; i < vecDropBull.size(); i++) {
    if (!vecDropBull[i].checkClear) {
      display.drawBitmap(vecDropBull[i].x, vecDropBull[i].y, iconDropBull, 7, 7, 1);
      vecDropBull[i].run();
    } else {
      checkCatchDropBull = 0;
      numberOfDropBull--;
      vecDropBull.erase(vecDropBull.begin() + i, vecDropBull.begin() + i + 1);
    }
  }
}
void playAgain() {
  Xship = widthBox / 2 - 3;
  Yship = 55;
  heart = 3;
  checkDead = 0;
  timeUndead = 0;
  timeCreBull = 0;
  numberOfBull = 50;
  scores = 0;
  timeCreEnemy = random(5, 10);
  numberOfDropHeart = 0;
  numberOfDropBull = 0;
  checkCatchDropBull = 0;
  checkCatchDropHeart = 0;
  timeDropHeart = random(15, 30);
  timeDropBull = random(15, 30);
  creBoss = 0;
  winGame = 0;
  while (vecEnemy.size()) vecEnemy.pop_back();
  while (vecBull.size()) vecBull.pop_back();
  while (vecBullEnemy.size()) vecBullEnemy.pop_back();
  while (vecDropHeart.size()) vecDropHeart.pop_back();
  while (vecDropBull.size()) vecDropBull.pop_back();
}