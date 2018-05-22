#include "RunSnake.h"
#include "MicroBit.h"
#include "Images.h"
#include "Utils.h"

extern MicroBit uBit;

#define SNAKE_EMPTY 0
#define SNAKE_UP 1
#define SNAKE_LEFT 2
#define SNAKE_RIGHT 3
#define SNAKE_DOWN 4

#define SNAKE_FRAME_DELAY 85
#define GROWTH_SPEED 3

struct Point {
    int x;
    int y;
};

static Point head;
static Point tail;
static Point food;
static MicroBitImage map(5, 5);

static void placeFood()
{
    int r = uBit.random(24);
    int x = 0;
    int y = 0;

    while (r > 0) {
        x = (x + 1) % 5;
        if (x == 0)
            y = (y + 1) % 5;

        if (map.getPixelValue(x, y) == SNAKE_EMPTY)
            r--;
    }

    food.x = x;
    food.y = y;
}

static void eatFood()
{
    uBit.rgb.setColour(0xff, 0x00, 0x00, 0x00);
    uBit.soundmotor.soundOn(392);
    fiber_sleep(500);
    uBit.soundmotor.soundOn(440);
    fiber_sleep(125);
    uBit.soundmotor.soundOff();
    uBit.rgb.off();
}

static bool accel_disabled = false;
static bool a_pressed = false;
static bool b_pressed = false;

static void buttonAPressed(MicroBitEvent)
{
    accel_disabled = true;
    a_pressed = true;
}

static void buttonBPressed(MicroBitEvent)
{
    accel_disabled = true;
    b_pressed = true;
}

void snake_run()
{
    uBit.display.clear();

    Point newHead; // Calculated placement of new head position based on user input.
    int hdirection = SNAKE_UP; // Head's direction of travel
    int tdirection; // Tail's direction of travel
    int snakeLength; // number of segments in the snake.
    int growing; // boolean state indicating if we've just eaten some food.
    int score;
    int steps = 0;

    // Start in the middle of the screen.
    tail.x = tail.y = 2;
    head.x = head.y = 2;
    snakeLength = 1;
    growing = 0;
    score = 0;
    map.clear();

    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_A,
        MICROBIT_BUTTON_EVT_CLICK,
        buttonAPressed);
    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_B,
        MICROBIT_BUTTON_EVT_CLICK,
        buttonBPressed);
    uBit.messageBus.listen(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);

    uBit.display.image.setPixelValue(head.x, head.y, 255);

    // Add some random food.
    placeFood();

    leave = false;
    while (!leave) {
        // Flash the food is necessary;
        uBit.display.image.setPixelValue(food.x, food.y, uBit.systemTime() % 500 < 250 ? 0 : 255);

        if (steps++ % 4 == 0) {
            newHead.x = head.x;
            newHead.y = head.y;

            // if buttons are pressed, give them precedence
            if (accel_disabled) {
                // switch direction according to buttons
                if (a_pressed || b_pressed) {
                    switch (hdirection) {
                    case SNAKE_LEFT:
                        hdirection = (a_pressed ? SNAKE_DOWN : (b_pressed ? SNAKE_UP : SNAKE_LEFT));
                        break;
                    case SNAKE_RIGHT:
                        hdirection = (a_pressed ? SNAKE_UP : (b_pressed ? SNAKE_DOWN : SNAKE_RIGHT));
                        break;
                    case SNAKE_UP:
                        hdirection = (a_pressed ? SNAKE_LEFT : (b_pressed ? SNAKE_RIGHT : SNAKE_UP));
                        break;
                    case SNAKE_DOWN:
                        hdirection = (a_pressed ? SNAKE_RIGHT : (b_pressed ? SNAKE_LEFT : SNAKE_DOWN));
                        break;
                    default:
                        break;
                    }
                }

                // do the actual move
                switch (hdirection) {
                case SNAKE_LEFT:
                    newHead.x = newHead.x == 0 ? 4 : newHead.x - 1;
                    break;
                case SNAKE_RIGHT:
                    newHead.x = newHead.x == 4 ? 0 : newHead.x + 1;
                    break;
                case SNAKE_UP:
                    newHead.y = newHead.y == 0 ? 4 : newHead.y - 1;
                    break;
                case SNAKE_DOWN:
                    newHead.y = newHead.y == 4 ? 0 : newHead.y + 1;
                    break;
                default:
                    break;
                }

            } else {
                int dx = uBit.accelerometer.getX();
                int dy = uBit.accelerometer.getY();

                // use accelerometer
                if (abs(dx) > abs(dy)) {
                    if (dx < 0) {
                        hdirection = SNAKE_LEFT;
                        newHead.x = newHead.x == 0 ? 4 : newHead.x - 1;
                    } else {
                        hdirection = SNAKE_RIGHT;
                        newHead.x = newHead.x == 4 ? 0 : newHead.x + 1;
                    }
                } else {
                    if (dy < 0) {
                        hdirection = SNAKE_UP;
                        newHead.y = newHead.y == 0 ? 4 : newHead.y - 1;
                    } else {
                        hdirection = SNAKE_DOWN;
                        newHead.y = newHead.y == 4 ? 0 : newHead.y + 1;
                    }
                }
            }
            a_pressed = false;
            b_pressed = false;

            int status = map.getPixelValue(newHead.x, newHead.y);
            if (status == SNAKE_UP || status == SNAKE_DOWN || status == SNAKE_LEFT || status == SNAKE_RIGHT) {
                uBit.display.scroll("The End");
                uBit.display.scroll(score);
                return;
            }

            // move the head.
            map.setPixelValue(head.x, head.y, hdirection);
            uBit.display.image.setPixelValue(newHead.x, newHead.y, 255);

            if (growing == GROWTH_SPEED) {
                growing = 0;
                snakeLength++;
            } else {
                // move the tail.
                tdirection = map.getPixelValue(tail.x, tail.y);
                map.setPixelValue(tail.x, tail.y, SNAKE_EMPTY);
                uBit.display.image.setPixelValue(tail.x, tail.y, 0);

                // Move our record of the tail's location.
                if (snakeLength == 1) {
                    tail.x = newHead.x;
                    tail.y = newHead.y;
                } else {
                    if (tdirection == SNAKE_UP)
                        tail.y = tail.y == 0 ? 4 : tail.y - 1;

                    if (tdirection == SNAKE_DOWN)
                        tail.y = tail.y == 4 ? 0 : tail.y + 1;

                    if (tdirection == SNAKE_LEFT)
                        tail.x = tail.x == 0 ? 4 : tail.x - 1;

                    if (tdirection == SNAKE_RIGHT)
                        tail.x = tail.x == 4 ? 0 : tail.x + 1;
                }
            }

            // Update our record of the head location and away we go!
            head.x = newHead.x;
            head.y = newHead.y;

            // if we've eaten some food, replace the food and grow ourselves!
            if (head.x == food.x && head.y == food.y) {
                invoke(eatFood);
                growing++;
                score++;
                placeFood();
            }
        }

        uBit.sleep(SNAKE_FRAME_DELAY);
    }

    uBit.display.clear();

    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_A,
        MICROBIT_BUTTON_EVT_CLICK,
        buttonAPressed);
    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_B,
        MICROBIT_BUTTON_EVT_CLICK,
        buttonBPressed);
    uBit.messageBus.ignore(
        MICROBIT_ID_BUTTON_AB,
        MICROBIT_BUTTON_EVT_CLICK,
        leaveHandler);
}