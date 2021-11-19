#pragma once

#include <cassert>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include "GameData/GlobalEnums.h"
#include "GameData/GlobalStructs.h"

#define assertm(exp, msg) assert(((void)msg, exp))