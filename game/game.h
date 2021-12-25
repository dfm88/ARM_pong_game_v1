#include "LPC17xx.h"

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

void move_ball(struct struct_ball ball);

int is_colliding(struct struct_ball ball, int8_t direction);
