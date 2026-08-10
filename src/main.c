#include "raylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 1280
#define HEIGHT 960
#define BOX_SIZE 40
#define FONT_SIZE 20
#define BOX_NUM_WIDTH (WIDTH / BOX_SIZE)
#define BOX_NUM_HEIGHT (HEIGHT / BOX_SIZE)
#define MAX_SNAKE_SIZE (BOX_NUM_WIDTH * BOX_NUM_HEIGHT)
#define INITIAL_SNAKE_SIZE 4

typedef struct {
  int x, y;
} Point;

typedef struct {
  Point snake[MAX_SNAKE_SIZE];
  int snakeSize;
  Point direction;
  Point nextDirection;
  Point apple;
  int score;
  bool gameRunning;
  float moveTimer;
  float moveInterval;
} GameState;

int getRandomNumber(int min, int max) { return min + rand() % (max - min + 1); }

bool appleInSnake(GameState *state) {
  for (int i = 0; i < state->snakeSize; i++) {
    if (state->snake[i].x == state->apple.x &&
        state->snake[i].y == state->apple.y)
      return true;
  }

  return false;
}

void setApplePosition(GameState *state) {
  do {
    state->apple.x = getRandomNumber(0, BOX_NUM_WIDTH - 1);
    state->apple.y = getRandomNumber(0, BOX_NUM_HEIGHT - 1);
  } while (appleInSnake(state));
}

void initState(GameState *state) {
  state->score = 0;
  state->snakeSize = INITIAL_SNAKE_SIZE;
  state->gameRunning = true;
  state->moveTimer = 0.0f;
  state->moveInterval = 0.1f;

  int startX = BOX_NUM_WIDTH / 2;
  int startY = BOX_NUM_HEIGHT / 2;

  for (int i = 0; i < INITIAL_SNAKE_SIZE; i++) {
    state->snake[i].x = startX - i;
    state->snake[i].y = startY;
  }

  state->direction = (Point){1, 0};
  state->nextDirection = state->direction;

  setApplePosition(state);
}

void updateNextDirection(GameState *state) {
  const bool UP = IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W);
  const bool DOWN = IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S);
  const bool LEFT = IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A);
  const bool RIGHT = IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D);

  if (UP && state->direction.y != 1)
    state->nextDirection = (Point){0, -1};
  if (DOWN && state->direction.y != -1)
    state->nextDirection = (Point){0, 1};
  if (LEFT && state->direction.x != 1)
    state->nextDirection = (Point){-1, 0};
  if (RIGHT && state->direction.x != -1)
    state->nextDirection = (Point){1, 0};
}

void moveSnake(GameState *state) {
  for (int i = state->snakeSize - 1; i > 0; i--) {
    state->snake[i] = state->snake[i - 1];
  }

  int newX = state->snake[0].x + state->direction.x;
  int newY = state->snake[0].y + state->direction.y;

  state->snake[0].x = (newX + BOX_NUM_WIDTH) % BOX_NUM_WIDTH;
  state->snake[0].y = (newY + BOX_NUM_HEIGHT) % BOX_NUM_HEIGHT;
}

void gameStopCheck(GameState *state) {
  for (int i = 1; i < state->snakeSize; i++) {
    if (state->snake[i].x == state->snake[0].x &&
        state->snake[i].y == state->snake[0].y) {
      state->gameRunning = false;
      break;
    }
  }
}

void appleEatCheck(GameState *state) {
  if (state->snake[0].x == state->apple.x &&
      state->snake[0].y == state->apple.y) {
    state->snake[state->snakeSize] = state->snake[state->snakeSize - 1];

    state->score++;
    state->snakeSize++;

    setApplePosition(state);
  }
}

void drawScore(int score) {
  const char *scoreText = TextFormat("Score: %d", score * 10);
  int textWidth = MeasureText(scoreText, FONT_SIZE);
  DrawText(scoreText, WIDTH / 2 - textWidth / 2, 10, FONT_SIZE,
           GetColor(0x011618FF));
}

void drawPoint(int x, int y, Color color) {
  DrawRectangle(x * BOX_SIZE, y * BOX_SIZE, BOX_SIZE, BOX_SIZE, color);
}

void drawSnakeAndApple(GameState *state) {
  drawPoint(state->apple.x, state->apple.y, GetColor(0xDF2C20FF));
  for (int i = 0; i < state->snakeSize; i++) {
    drawPoint(state->snake[i].x, state->snake[i].y, GetColor(0x8B20DFFF));
  }
}

void drawGameOverScreen(GameState *state) {
  const char *finalScore = TextFormat("Your Score: %d", state->score * 10);
  const char *texts[] = {
      "GAME OVER",
      finalScore,
      "Press ENTER to play again.",
  };
  for (int i = 0; i < sizeof(texts) / sizeof(texts[0]); i++) {
    int textWidth = MeasureText(texts[i], FONT_SIZE);
    DrawText(texts[i], WIDTH / 2 - textWidth / 2,
             HEIGHT / 2 + FONT_SIZE * (i - 1) * 3 / 2, FONT_SIZE,
             GetColor(0x011618FF));
  }
}

int main(void) {
  srand(time(NULL));
  InitWindow(WIDTH, HEIGHT, "Snake");

  GameState state;
  initState(&state);

  Color backgroundColor = GetColor(0x74DF20FF);

  while (!WindowShouldClose()) {
    if (state.gameRunning) {
      updateNextDirection(&state);

      float dt = GetFrameTime();
      state.moveTimer += dt;

      if (state.moveTimer >= state.moveInterval) {
        state.moveTimer -= state.moveInterval;
        state.direction = state.nextDirection;

        moveSnake(&state);
        gameStopCheck(&state);
        appleEatCheck(&state);
      }

      BeginDrawing();
      ClearBackground(backgroundColor);

      drawSnakeAndApple(&state);
      drawScore(state.score);

      EndDrawing();
    } else {
      BeginDrawing();
      ClearBackground(backgroundColor);

      drawGameOverScreen(&state);

      EndDrawing();

      if (IsKeyPressed(KEY_ENTER)) {
        initState(&state);
      }
    }
  }

  CloseWindow();

  return 0;
}
