#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/Graphics.hpp>

const int WATER = 0;
const int LAND = 1;
const int CITY = 2;
const int PORT = 3;
const int CAPITAL = 4;

const int TILE_WIDTH = 50;
const int TILE_HEIGHT = 40;

const int TOP_LEFT_CAPITAL = 12;
const int BOTTOM_LEFT_CAPITAL = 20;
const int TOP_RIGHT_CAPITAL = 199;
const int BOTTOM_RIGHT_CAPITAL = 207;

const int HEX_TO_TOP = 1;
const int HEX_TO_TOP_RIGHT = 2;
const int HEX_TO_BOTTOM_RIGHT = 3;
const int HEX_TO_BOTTOM = 4;
const int HEX_TO_BOTTOM_LEFT = 5;
const int HEX_TO_TOP_LEFT = 6;
const int NUMBER_OF_ADJACENT_TILES = 6;

const int NUMBER_OF_PLAYERS = 4;

const int MAX_ARMY_SIZE = 9;
const int MAX_EXPERIENCE = 9;

#endif