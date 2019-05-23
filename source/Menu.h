/*!
 * @file Menu.h
 *
 * Selestion menu for applications on Calliope-mini.
 *
 * @copyright (c) Calliope gGmbH.
 *
 * Licensed under the Apache Software License 2.0 (ASL 2.0)
 * Portions (c) Copyright British Broadcasting Corporation under MIT License.
 *
 * @author Torsten Curdt		<https://github.com/tcurdt>
 * @author Waldemar Gruenwald 	<https://github.com/gruenwaldi>
 */

#ifndef MENU_H
#define MENU_H

typedef enum MenuState {
	MenuStateOracle = 1,
	MenuStateRockPaperScissors,
	MenuStateMultiplication,
	MenuStateVolumeMeter,
	MenuStateInterpreter,
	MenuStateMin = MenuStateOracle,
	MenuStateMax = MenuStateInterpreter
} menustate_t;

menustate_t menuWaitForChoice(menustate_t start);

#endif // MENU_H