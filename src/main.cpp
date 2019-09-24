#include <Arduino.h>
#include <LedControlMS.h>


LedControl lc = LedControl(12, 11, 10, 1);
enum dir
{
  NONE,
  UP,
  LEFT,
  DOWN,
  RIGHT
};
int snake[64][2], fx, fy, dir = NONE, score = 0;
bool gameover = false;
unsigned long timestamp = 0;



void isr() {
  score = 0;
  fx = random(8);
  fy = random(8);
  gameover = false;
}

void setup()
{
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), isr, RISING);
  randomSeed(analogRead(2));
  fx = random(8);
  fy = random(8);
  for (int i = 0; i < 64; i++)
  {
    snake[i][0] = -1;
    snake[i][1] = -1;
  }
  snake[0][0] = 4;
  snake[0][1] = 4;

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  lc.shutdown(0, false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0, 8);
  /* and clear the display */
  lc.clearDisplay(0);
  
  
  
}

void loop()
{
  if (gameover) {
    lc.clearDisplay(0);
    char b[2];
    String str;
    str = String(score);
    str.toCharArray(b, 3);
    lc.displayChar(0, lc.getCharArrayPosition(b[0]));
    delay(300);
    if (score > 9) {
      lc.displayChar(0, lc.getCharArrayPosition(b[1]));
      delay(300);
    }
    lc.clearDisplay(0);
    delay(300);

    
    return;
  }

  int analogX = analogRead(A0);
  int analogY = analogRead(A1);

  if (analogX < 24)
  {
    dir = RIGHT;
  }
  else if (analogX > 1000)
  {
    dir = LEFT;
  }

  if (analogY < 24)
  {
    dir = DOWN;
  }
  else if (analogY > 1000)
  {
    dir = UP;
  }

  if (millis() - timestamp > 300)
  {
    if (snake[score][0] == fx && snake[score][1] == fy)
    {
      score++;
      snake[score][0] = fx;
      snake[score][1] = fy;
      fx = random(8);
      fy = random(8);
    }
    else
    {
      for (int i = 0; i < score; i++)
      {
        snake[i][0] = snake[i + 1][0];
        snake[i][1] = snake[i + 1][1];
      }
    }

    switch (dir)
    {
    case RIGHT:
      snake[score][0]++;
      break;
    case LEFT:
      snake[score][0]--;
      break;
    case DOWN:
      snake[score][1]++;
      break;
    case UP:
      snake[score][1]--;
      break;

    default:
      break;
    }
    snake[score][0] = (snake[score][0] + 8) % 8;
    snake[score][1] = (snake[score][1] + 8) % 8;

    for(int i = 0; i < score; i++) {
      if(snake[score][0] == snake[i][0] && snake[score][1] == snake[i][1]) {
        gameover = true;
      } 
    }
    timestamp = millis();
  }
  //draw
  lc.clearDisplay(0);
  for (int i = 0; i < score+1; i++)
  {
    if (snake[i][0] == -1 || snake[i][1] == -1)
      continue;
    lc.setLed(0, snake[i][0], snake[i][1], true);
  }
  if ((millis() / 500) % 2)
    lc.setLed(0, fx, fy, true);
}