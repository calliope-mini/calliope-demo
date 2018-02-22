#pragma once

typedef enum MenuState {
    MenuStateOracle = 1,
    MenuStateRockPaperScissors,
    MenuStateLoveMeter,
    MenuStateSnake,
    MenuStateInterpreter,
    MenuStateMin = MenuStateOracle,
    MenuStateMax = MenuStateInterpreter
} menustate_t;

menustate_t menuWaitForChoice(menustate_t start);
