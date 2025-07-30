#pragma once

#define CLAMP(value, min, max) (value > max ? max : (value < min ? min : value)) 