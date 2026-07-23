#include "raylib.h"
#include <stdlib.h>
#include <time.h>

typedef struct {
  int x, y;
} Point;

const int WIDTH = 1280, HEIGHT = 960, BOX_SIZE = 40, FONT_SIZE = 20;
const int MAX_SNAKE_SIZE = (WIDTH / BOX_SIZE) * (HEIGHT / BOX_SIZE);
const int INITIAL_SNAKE_SIZE = 4;

int getRandomNumber(int min, int max) { return min + rand() % (max - min + 1); }

void DrawBackground(int score) {
  ClearBackground(WHITE);

  for (int x = BOX_SIZE; x < WIDTH; x += BOX_SIZE) {
    DrawLine(x, 0, x, HEIGHT, BLACK);
  }

  for (int y = BOX_SIZE; y < HEIGHT; y += BOX_SIZE) {
    DrawLine(0, y, WIDTH, y, BLACK);
  }

  const char *scoreText = TextFormat("Score: %d", score * 10);
  int textWidth = MeasureText(scoreText, FONT_SIZE);
  DrawText(scoreText, WIDTH / 2 - textWidth / 2, 10, FONT_SIZE, BLUE);
}

void DrawPoint(int x, int y, Color color) {
  DrawRectangle(x * BOX_SIZE, y * BOX_SIZE, BOX_SIZE, BOX_SIZE, color);
}

int main(void) {
  srand(time(NULL));
  InitWindow(WIDTH, HEIGHT, "Snake");

  int score = 0;
  int snakeSize = INITIAL_SNAKE_SIZE + score;

  Point apple = {getRandomNumber(0, WIDTH / BOX_SIZE - 1),
                 getRandomNumber(0, HEIGHT / BOX_SIZE - 1)};

  int startX = WIDTH / BOX_SIZE / 2;
  int startY = HEIGHT / BOX_SIZE / 2;
  Point snake[MAX_SNAKE_SIZE];
  for (int i = 0; i < INITIAL_SNAKE_SIZE; i++) {
    snake[i].x = startX - i;
    snake[i].y = startY;
  }

  Point direction = {1, 0};
  float moveTimer = 0.0f;
  float moveInterval = 0.15f;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_UP) && direction.y != 1) {
      direction = (Point){0, -1};
    }
    if (IsKeyPressed(KEY_DOWN) && direction.y != -1) {
      direction = (Point){0, 1};
    }
    if (IsKeyPressed(KEY_LEFT) && direction.x != 1) {
      direction = (Point){-1, 0};
    }
    if (IsKeyPressed(KEY_RIGHT) && direction.x != -1) {
      direction = (Point){1, 0};
    }

    float dt = GetFrameTime();
    moveTimer += dt;

    if (moveTimer >= moveInterval) {
      moveTimer = 0;

      for (int i = snakeSize - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
      }

      snake[0].x += direction.x;
      snake[0].y += direction.y;
    }

    if (snake[0].x == apple.x && snake[0].y == apple.y) {
      snake[snakeSize] = snake[snakeSize - 1];

      score++;
      snakeSize++;

      apple.x = getRandomNumber(0, WIDTH / BOX_SIZE - 1);
      apple.y = getRandomNumber(0, HEIGHT / BOX_SIZE - 1);
    }

    BeginDrawing();

    DrawBackground(score);
    DrawPoint(apple.x, apple.y, RED);
    for (int i = 0; i < snakeSize; i++) {
      DrawPoint(snake[i].x, snake[i].y, GREEN);
    }

    EndDrawing();
  }

  CloseWindow();

  return 0;
}
