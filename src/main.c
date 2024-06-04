#include "raylib.h"
#include "raymath.h"

#define FPS 60
#define DELTA_TIME_SEC (1.0f/(FPS))
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 450
#define BALL_SIZE 10
#define MOVE_SPEED 500
#define BAR_WIDTH 100
#define BAR_HEIGHT 20
#define BAR_PAD 10
#define MT ((MOVE_SPEED)*(DELTA_TIME_SEC))

#define BALL_COLOR CLITERAL(Color){139, 56, 62, 255}
#define BACKGROUND_COLOR CLITERAL(Color){18, 18, 18, 255}

typedef enum GameScreen {
       	GAMESTART = 0,
       	GAMEPLAY,
       	GAMEOVER 
} GameScreen;

typedef struct Projectile {
	Vector2 pos;
	Vector2 size;
	Vector2 vel;
	Color c;
} Projectile;

typedef struct Paddle {
	Vector2 pos;
	Vector2 size;
	Color c;
	int dx;
} Paddle;

void update(Projectile* ball, Paddle* bar);
void render(Projectile* ball, Paddle* bar);
void draw_x_centered_text(const char* text, int y, int fs, Color color);

int main(void) {
	const char* TITLE = "BREAK--OUT";
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE);
	SetTargetFPS(FPS);
	SetExitKey(KEY_DELETE);

	GameScreen screen = GAMESTART;
	Projectile ball = {0};
	Paddle bar = {0};

	bar.pos.x = (SCREEN_WIDTH + BAR_WIDTH)*0.5 - BAR_WIDTH;
	bar.pos.y = SCREEN_HEIGHT - BAR_HEIGHT - BAR_PAD - BAR_HEIGHT*0.5;
	ball.pos.x = (SCREEN_WIDTH)*0.5;
	ball.pos.y = bar.pos.y - BALL_SIZE;
	bar.size  = (Vector2) {BAR_WIDTH, BAR_HEIGHT};
	ball.vel  = (Vector2) {1, 1};
	ball.size = (Vector2) {BALL_SIZE, BALL_SIZE};
	bar.c = GREEN;
	ball.c = BALL_COLOR;


	while (!WindowShouldClose())
        {
		// UPDATE
		switch(screen) {
			case GAMESTART: {
				if (IsKeyPressed(KEY_D) ||
			            IsKeyPressed(KEY_A) || 
				    IsKeyPressed(KEY_LEFT) || 
				    IsKeyPressed(KEY_RIGHT) || 
				    IsKeyPressed(KEY_SPACE))
				{
				    screen = GAMEPLAY;
				}
		        } break;
			case GAMEPLAY:{
				update(&ball, &bar);
			} break;
			case GAMEOVER: break;
			default: break;
		}

		// RENDER
		BeginDrawing();
		ClearBackground(BACKGROUND_COLOR);

		switch(screen) {
			case GAMESTART:{
				render(&ball, &bar);
				draw_x_centered_text(TITLE, SCREEN_HEIGHT*0.5 - 40, 40, DARKGREEN);
				draw_x_centered_text("PRESS A D or LEFT RIGHT to PLAY", SCREEN_HEIGHT*0.5 + 20, 20, DARKGREEN);
		        } break;
			case GAMEPLAY: {
				render(&ball, &bar);
		        } break;
			case GAMEOVER:{
				render(&ball, &bar);
				DrawText(TITLE, 0, 0, 30, DARKGREEN);
				draw_x_centered_text("GAME OVER", SCREEN_HEIGHT*0.5 - 40, 40, DARKGREEN);
				// DrawText(TextFormat("{%d, %d}", tx, ty), 10, 0, 20, RAYWHITE);

				// fs = 20;
				// tx = (SCREEN_WIDTH - MeasureText("GAME OVER", fs))*0.5;
				// ty = SCREEN_HEIGHT - MeasureText("GAME OVER", fs) - 40;
				// DrawText(TextFormat("{%d, %d}", tx, ty), 150, 0, 20, RAYWHITE);
				// DrawText("GAME OVER", tx, ty, fs, DARKGREEN);
			} break;
			default: break;
		}

		EndDrawing();
	}

	CloseWindow();
	return 0;
}

void render(Projectile* ball, Paddle* bar) {
	DrawCircleV(ball->pos, BALL_SIZE, ball->c);
	DrawRectangleV(bar->pos, bar->size, bar->c);
}

void update(Projectile* ball, Paddle* bar) {
	bar->dx = 0;
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) {
		bar->dx += 1;
	}
	if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) {
		bar->dx += -1;
	}

////////////////////////

	const float EPS = 0.006;
	bar->pos.x = Clamp(bar->pos.x + bar->dx * EPS * MOVE_SPEED * MT,
			  0, SCREEN_WIDTH - BAR_WIDTH);

	int nx = ball->pos.x + ball->vel.x*MT; 
	int ny = ball->pos.y + ball->vel.y*MT;

	if (0 > nx || nx + ball->size.x > SCREEN_WIDTH) {
		ball->vel.x *= -1;
		nx = ball->pos.x + ball->vel.x*MT;
	}
	
	if (0 > ny || ny + ball->size.y > SCREEN_HEIGHT) {  
		ball->vel.y *= -1;
		ny = ball->pos.y + ball->vel.y*MT;
	}

	if (CheckCollisionRecs((Rectangle){ball->pos.x, ball->pos.y, ball->size.x, ball->size.y}, (Rectangle){bar->pos.x, bar->pos.y, bar->size.x, bar->size.y})) {
		// TODO: Check collision
		//
	}

	ball->pos.x = nx;
	ball->pos.y = ny;
}

void draw_x_centered_text(const char* text, int y, int fs, Color color) {
	int tx = (SCREEN_WIDTH - MeasureText(text, fs))*0.5;
	DrawText(text, tx, y, fs, color);
}

// TODO: Brick Columns: N = (SCREEN_WIDTH - BRICK_PAD)/(BRICK_WIDTH + BRICK_PAD)
//     - Brick Rows: N = (BRICK_SECTION_HEIGHT - BRICK_PAD)/(BRICK_HEIGHT + BRICK_PAD)
//     - Brick Section [M * N] of Brick {Vector2 pos, Color c, bool broken}
// TODO: Score Board
// TODO: Particle Effect Upon Breaking a Brick
