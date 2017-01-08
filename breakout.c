#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

#define HEIGHT 600
#define WIDTH 400

#define ROWS 5

#define COLS 10

#define RADIUS 10

#define LIVES 3

void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    srand48(time(NULL));

    GWindow window = newGWindow(WIDTH, HEIGHT);

    initBricks(window);

    GOval ball = initBall(window);

    GRect paddle = initPaddle(window);

    GLabel label = initScoreboard(window);

    int bricks = COLS * ROWS;

    int lives = LIVES;

    int points = 0;
      
    double velocity_x = 2.0;
    double velocity_y = 1.0;

    while (lives > 0 && bricks > 0)
    {
        updateScoreboard(window, label, points);
        
        move(ball, velocity_x, velocity_y);
        
        pause(10);
        
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {   
                setLocation(paddle, getX(event) - getWidth(paddle) / 2, 550);
            }
        }    
       
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocity_x = -velocity_x;
        }
            
        else if (getX(ball) <= 0)
        {
            velocity_x = -velocity_x;
        }
            
        if (getY(ball) <= 0)
        {
            velocity_y = -velocity_y;
        }

        else if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives --;
            waitForClick();
            velocity_y = -velocity_y;
        }
        
        GObject object = detectCollision(window, ball);
        
        if (object != NULL)
        {
            if (object == paddle)
            {
                velocity_y = -velocity_y;
            }
            
            else if (strcmp(getType(object), "GRect") == 0)
            {
                removeGWindow(window, object);
                velocity_y = -velocity_y;
                points ++;
                bricks --;
            }

        }
        
    }

    waitForClick();

    closeGWindow(window);
    return 0;
}

void initBricks(GWindow window)
{
    char* color [5] = {"RED", "ORANGE", "YELLOW", "GREEN", "BLUE"};
    
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect((4.5 * (j + 1)) + (35 * j), 50 + (10 + 6) * i, 35, 10);
            setColor(brick, color[i]);
            setFilled(brick, true);
            add(window, brick);
        }
}

GOval initBall(GWindow window)
{
    GOval ball = newGOval(WIDTH / 2 - RADIUS, HEIGHT / 2 - RADIUS, RADIUS * 2, RADIUS * 2);
    setColor(ball, "BLACK");
    setFilled(ball, true);
    add(window, ball);
    
    return ball;
}

GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH / 2 - 35, 550, 70, 10);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);
    
    return paddle;
}

GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-70");
    setColor(label, "BLACK");
    add(window, label);
    setLocation(label, WIDTH / 2, HEIGHT / 2);
    
    return label;
}

void updateScoreboard(GWindow window, GLabel label, int points)
{
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

GObject detectCollision(GWindow window, GOval ball)
{
    double x = getX(ball);
    double y = getY(ball);

    GObject object;

    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    return NULL;
}
