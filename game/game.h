#include "LPC17xx.h"

static int record = 100;
static int score = 0;
static uint8_t is_game_over;
struct struct_ball
{
	uint8_t h_speed;
	uint8_t v_speed;
	int8_t h_direc;
	int8_t v_direc;
	uint16_t posX;
	uint16_t posY;
};

struct struct_paddle
{
	int8_t h_direc;
	uint16_t posX;
	uint16_t posY;
};

extern struct struct_ball ball;
extern struct struct_paddle paddle;

void GAME_init(void);
void handle_paddle_collsion(void);
void move_paddle(unsigned short);
void game_over(void);
void increase_score(void);
void pause_game(void);
void resume_game(void);
void restart_game(void);
void initialize_ball(void);
void draw_ball(uint16_t, uint16_t);
void delete_ball(void);

void move_ball(void);

int is_colliding(struct struct_ball ball, int8_t direction);
