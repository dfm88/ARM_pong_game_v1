
#include "game.h"
#include "lpc17xx.h"
#include "../GLCD/GLCD.h"

/**
 * @brief  Function that initializes game
 */
void GAME_init(void)
{

    uint16_t t;
    t = 5;
}

int is_colliding(struct struct_ball ball, int8_t direction)
{

    uint8_t delta_h, delta_v;
    int v_limit, h_limit, current_pos;

    delta_h = ball.h_direc > 0 ? 4 : 0;
    delta_v = ball.v_direc > 0 ? 4 : 0;

    // 0x68 = h    0x76 = v
    current_pos = direction == 'h' ? ball.posX + delta_h : ball.posY + delta_v;

    v_limit = ball.v_direc > 0 ? 276 : 5; // (last or first valid pixel)
    h_limit = ball.h_direc > 0 ? 234 : 5; // (last or first valid pixel)

    switch (direction)
    {
    case 'h': /* moving horizontally */
        /**@todo handle wall noise**/
        return current_pos - h_limit == 0;

    case 'v': /* moving vertically */
        // if ball was moving up
        if (ball.v_direc < 0)
            /**@todo handle wall noise**/
            return current_pos - v_limit == 0;
        // if ball was moving down, check paddle position
        else
        {
            // is colliding with paddle high && paddle is under the ball
            if (current_pos - v_limit == 0)
            {
                if ((paddle.posX - 4) <= ball.posX)
                {
                    if (ball.posX <= (paddle.posX + 49))
                    {
                        /**@todo handle score increase**/
                        /**@todo handle paddle noise**/
                        return 1;
                    }
                }
            }
            return 0;
        }

    default:
        return 0;
    }
}

void handle_paddle_collsion()
{
    // Dichotomic search
    /**
     *            Speed change after paddle hit (v vertical, h horizontal, r right, l left))
     *  --------------------------------------------------------------------------------------------------------------
     * | hl=4,v=1 | hl=3,v=2 | hl=2,v=3 | hl=1,v=4 | hl=1,v=5 || hr=1,v=5 | hr=1,v=4 | hr=2,v=3 | hr=3,v=2 | hr=4,v=1 |
     *
     * |   1-5    |   6-10   |   7-15   |   16-20  |   21-25  ||   25-30  |   31-35  |   36-40  |   41-45  |   46-50  |
     *  --------------------------------------------------------------------------------------------------------------
     *                                      Paddle lenght
     *
     *  Speed change will be handle by the portion of the pad hit by at least sup(ball_lenght/2)=3 ball pixels
     */

    /** 1-23 **/
    if (ball.posX <= (paddle.posX + 22))
    {
        ball.h_direc = -1;
        // 1-13
        if (ball.posX <= (paddle.posX + 12))
        {
            // 1-8
            if (ball.posX <= (paddle.posX + 7))
            {
                // 1-3
                if (ball.posX <= paddle.posX + 2)
                {
                    ball.v_speed = 1;
                    ball.h_speed = 4;
                    return;
                }
                // 4-8
                else
                {
                    ball.v_speed = 2;
                    ball.h_speed = 3;
                    return;
                }
            }
            // 9-13
            else
            {
                ball.v_speed = 3;
                ball.h_speed = 2;
                return;
            }
        }
        // 14-23
        else
        {
            // 14-18
            if (ball.posX <= paddle.posX + 17)
            {
                ball.v_speed = 4;
                ball.h_speed = 1;
                return;
            }
            // 19-23
            else
            {
                ball.v_speed = 5;
                ball.h_speed = 1;
                return;
            }
        }
    }
    /** 24-50 **/
    else
    {
        ball.h_direc = 1;
        // 39-50
        if (ball.posX >= paddle.posX + 38)
        {
            // 44-50
            if (ball.posX >= paddle.posX + 43)
            {
                ball.v_speed = 1;
                ball.h_speed = 4;
                return;
            }
            // 39-43
            else
            {
                ball.v_speed = 2;
                ball.h_speed = 3;
                return;
            }
        }
        // 24-38
        else
        {
            // 29-38
            if (ball.posX >= paddle.posX + 28)
            {
                // 34-38
                if (ball.posX >= paddle.posX + 33)
                {
                    ball.v_speed = 3;
                    ball.h_speed = 2;
                    return;
                }
                // 29-33
                else
                {
                    ball.v_speed = 4;
                    ball.h_speed = 1;
                    return;
                }
            }
            // 24-28
            else
            {
                ball.v_speed = 5;
                ball.h_speed = 1;
                return;
            }
        }
    }
}

void move_paddle(unsigned short mov)
{
    uint8_t increment = 0;
    int8_t dir = 0;
    uint8_t i = 0;
    uint8_t delta_del;
    uint8_t delta_draw;
    // mov == 0-3 move left
    // mov == 4   hold
    // mov == 5-8 move right

    dir = mov > 4 ? 1 : -1;        // -1
    delta_del = dir > 0 ? 0 : 49;  // 49
    delta_draw = dir > 0 ? 49 : 0; // 0

    switch (mov)
    {
    case 0:
    case 8:
        increment = 14;
        break;
    case 1:
    case 7:
        increment = 12;
        break;
    case 2:
    case 6:
        increment = 10;
        break;
    case 3:
    case 5:
        increment = 8;
        break;
    case 4:
        increment = 0;
        dir = 0;
        return;
    }

    for (i = 0; i < increment; i++)
    {
        if ((dir > 0 && paddle.posX < 188) || (dir < 0 && paddle.posX > 0))
        {
            // delete line
            LCD_DrawLine(paddle.posX + (delta_del /* + i*dir*/), paddle.posY, paddle.posX + (delta_del /* + i*dir*/), paddle.posY + 9, Black);
            // change paddle position
            paddle.posX = paddle.posX + dir;
            // draw line
            LCD_DrawLine(paddle.posX + (delta_draw /* + i*dir*/), paddle.posY, paddle.posX + (delta_draw /* + i*dir*/), paddle.posY + 9, Green);
        }
        else
        {
            return;
        }
    }
    return;
}

void move_ball(struct struct_ball ball)
{

    uint8_t previous_h_speed, previous_v_speed, longer_speed, isColliding;
    uint8_t delta = 4;
    uint8_t is_game_over = 0;
    uint8_t i = 0;

    while (!is_game_over)
    {

        // calcolo movimento
        //  h_mov = ball.h_speed * ball.h_direc;
        //  v_mov = ball.v_speed * ball.v_direc;
        //  h_mov
        //  v_mov
        previous_h_speed = ball.h_speed;
        previous_v_speed = ball.v_speed;

        longer_speed = ball.v_speed >= ball.h_speed ? ball.v_speed : ball.h_speed;

        for (i = 0; i < longer_speed; i++)
        {

            // if ball is moving HORIZONTALLY and Horizontal speed is not lapsed
            if (ball.h_direc && ball.h_speed)
            {
                /**@todo HANDLE HORIZONTAL COLLISION WITH WALLS AND PADDLE**/
                isColliding = is_colliding(ball, 'h');

                if (isColliding)
                {
                    ball.h_speed = previous_h_speed;
                    ball.v_speed = previous_v_speed;
                    ball.h_direc = -ball.h_direc;
                    // move_ball(ball);
                    return;
                }
                if (ball.h_direc < 0)
                {
                    // MOVE horizontal left
                    // delete horizontal right
                    LCD_DrawLine(ball.posX + delta, ball.posY, ball.posX + delta, ball.posY + delta, Red);
                    // paint horizontal left
                    LCD_DrawLine(ball.posX + ball.h_direc, ball.posY, ball.posX + ball.h_direc, ball.posY + delta, Blue);
                }
                else
                {
                    // MOVE horizontal right
                    // delete horizontal left
                    LCD_DrawLine(ball.posX, ball.posY, ball.posX, ball.posY + delta, Red);
                    // paint horizontal right
                    LCD_DrawLine(ball.posX + ball.h_direc + delta, ball.posY, ball.posX + ball.h_direc + delta, ball.posY + delta, Blue);
                }
                // update x
                ball.posX = ball.posX + ball.h_direc;
                // decrement horizontal speed
                ball.h_speed--;
            }
            // if ball is moving VERTICALLY and Vertical speed is not Elapsed
            if (ball.v_direc && ball.v_speed)
            {
                isColliding = is_colliding(ball, 'v');
                if (isColliding)
                {
                    ball.h_speed = previous_h_speed;
                    ball.v_speed = previous_v_speed;
                    ball.v_direc = -ball.v_direc;
                    // move_ball(ball);
                    return;
                }
                if (ball.v_direc < 0)
                {
                    // MOVE vertical up
                    // delete vertical down
                    LCD_DrawLine(ball.posX, ball.posY + delta, ball.posX + delta, ball.posY + delta, Red);
                    // paint vertical up
                    LCD_DrawLine(ball.posX, ball.posY + ball.v_direc, ball.posX + delta, ball.posY + ball.v_direc, Blue);
                }
                else
                {
                    // MOVE vertical down
                    // delete vertical up
                    LCD_DrawLine(ball.posX, ball.posY, ball.posX + delta, ball.posY, Red);
                    // paint vertical down
                    LCD_DrawLine(ball.posX, ball.posY + ball.v_direc + delta, ball.posX + delta, ball.posY + ball.v_direc + delta, Blue);
                }
                // update y
                ball.posY = ball.posY + ball.v_direc;

                is_game_over = ball.posY > 278;
                // decrement vertical speed
                ball.v_speed--;
            }

            // richiamo la funzione se il ciclo � finito senza collisioni
            if (longer_speed - 1 == i)
            {
                ball.h_speed = previous_h_speed;
                ball.v_speed = previous_v_speed;
                // move_ball(ball);
                return;
            }
        }
    }
}
