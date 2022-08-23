#include <MD_MAX72xx.h>
#include <LiquidCrystal_I2C.h>

#define	MAX_DEVICES	    2

// Matric pins
#define	MATRIX_DIN	    11
#define	MATRIC_CS		    10
#define	MATRIC_CLK		  13

// Joystic pins
#define JOY_VERT        A0
#define JOY_HORI        A1
#define JOY_SEL         2

#define BUZZER          12

#define LED_RED         8
#define LED_GREEN       9

// Game defines
#define bricksCount     15

#define arenaXMin       0
#define arenaXMax       15
#define arenaYMax       0
#define arenaYMin       7

MD_MAX72XX mx = MD_MAX72XX(MD_MAX72XX::PAROLA_HW, MATRIC_CS, MAX_DEVICES);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int player_x = 6;
int player_y = 6;

int ball_y = 2;
int ball_x = 7;
int ball_dir = 0;

int bricks[bricksCount][2];

int score = 0;
int scorePrev = 0;

bool endGame = false;

void restartGame();

void setup() {

  randomSeed(analogRead(A2));

  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: ");
  lcd.print(score);
  lcd.print(" / ");
  lcd.print(bricksCount);
  scorePrev = score;

  mx.begin();
  mx.control(MD_MAX72XX::INTENSITY, MAX_INTENSITY / 2);
  mx.clear();

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(JOY_VERT, INPUT);
  pinMode(JOY_HORI, INPUT);
  pinMode(JOY_SEL, INPUT_PULLUP);

  Serial.begin(115200);

  restartGame();

}

void restartGame() {

  player_x = 6;
  player_y = 6;

  ball_y = 2;
  ball_x = 7;
  ball_dir = 0;

  score = 0;

  int brick_x, brick_y;

  // Генерация кирпичей + чек чтобы не было одинаковых кирпичей на координате
  for (int i = 0; i < bricksCount; i++) {
    generateBrickCords:
      brick_x = random(arenaXMin, arenaXMax);
      brick_y = random(arenaYMax, arenaYMin / 2);
    for (int j = 0; j < i; j++) {
      if (bricks[j][0] == brick_x && bricks[j][1] == brick_y) {
        goto generateBrickCords;
        break;
      }
    }
      bricks[i][0] = brick_x;
      bricks[i][1] = brick_y;
  }

  endGame = false;

}

void loop() {

  scorePrev = score;

  /* Направления
  0 - вниз
  1 - вверх
  2 - налево вверх
  3 - направо вверх
  4 - вниз налево
  5 - вниз направо
  */
  
  int horz = analogRead(JOY_HORI);
  int vert = analogRead(JOY_VERT);

  // Рестарт игры
  if (digitalRead(JOY_SEL) == LOW)
    restartGame();

  if (endGame) {
    digitalWrite(LED_RED, HIGH);
    return;
  } else {
    digitalWrite(LED_RED, LOW);
  }

  // Логика победы и поражения
  if (ball_y >= arenaYMin) {
    lcd.setCursor(0, 1);
    lcd.print("Lose! Press 'B'.");
    endGame = true;
  }

  if (score == bricksCount) {
    lcd.setCursor(0, 1);
    lcd.print("Win! Press 'B'.");
    endGame = true;
  }

  // Движение игрока
  if (horz > 700 && player_x < 13)
    player_x++;

  if (horz < 100 && player_x > 0) 
    player_x--;

  // Логика движения при направлении
  if (ball_dir == 0) {
    ball_y++;
  } else if (ball_dir == 1) {
    ball_y--;
  } else if (ball_dir == 2) {
    ball_x--;
    ball_y--;
  } else if (ball_dir == 3) {
    ball_x++;
    ball_y--;
  } else if (ball_dir == 4) {
    ball_x--;
    ball_y++;
  } else if (ball_dir == 5) {
    ball_x++;
    ball_y++;
  }

  // При ударе о противника ставим лайк и меняем направление
  for (int i = 0; i < bricksCount; i++) {
    if (bricks[i][0] == ball_x && bricks[i][1] == ball_y) {
      if (ball_dir == 1) {
        ball_dir = 0;
      } else if (ball_dir == 2) {
        ball_dir = 4;
      } else if (ball_dir == 3) {
        ball_dir = 5;
      }
      bricks[i][0] = -1;
      bricks[i][1] = -1;
      tone(BUZZER, 50, 50);
      digitalWrite(LED_GREEN, HIGH);
      delay(100);
      digitalWrite(LED_GREEN, LOW);
      score++;
      break;
    }
  }

  // При ударе о потолок меняем направление
  if (ball_y == arenaYMax) {
    if (ball_dir == 1) {
      ball_dir = 0;
    } else if (ball_dir == 3) {
      ball_dir = 5;
    } else if (ball_dir == 2) {
      ball_dir = 4;
    }
    tone(BUZZER, 750, 10);
  }

  // При ударе о край арены меняем направление 
  if (ball_x >= arenaXMax && ball_dir == 3) {
    tone(BUZZER, 750, 10);
    ball_dir = 2;
  } else if (ball_x <= arenaXMin && ball_dir == 2) {
    tone(BUZZER, 750, 10);
    ball_dir = 3;
  } else if (ball_x >= arenaXMax && ball_dir == 5) {
    tone(BUZZER, 750, 10);
    ball_dir = 4;
  } else if (ball_x <= arenaXMin && ball_dir == 4) {
    tone(BUZZER, 750, 10);
    ball_dir = 5;
  }
  
  // При ударе о игрока меняем направление
  if (ball_y == player_y) {
    if (ball_x == player_x + 1) {
      ball_dir = 1;
    }
    if (ball_x == player_x) {
      ball_dir = 2;
    }
    if (ball_x == player_x + 2) {
      ball_dir = 3;
    }
    tone(BUZZER, 750, 10);
  }

  // Арена постоянно обновляется с изменениями
  mx.clear();

  mx.setPoint(6, player_x, true);
  mx.setPoint(6, player_x + 1, true);
  mx.setPoint(7, player_x + 1, true);
  mx.setPoint(6, player_x + 2, true);

  mx.setPoint(ball_y, ball_x, true);

  for (int i = 0; i < bricksCount; i++)
    mx.setPoint(bricks[i][1], bricks[i][0], true);

  // Чтобы экран не мерцал
  if (scorePrev != score) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Score: ");
    lcd.print(score);
    lcd.print(" / ");
    lcd.print(bricksCount);
    scorePrev = score;
  }
  
  delay(100);

}
