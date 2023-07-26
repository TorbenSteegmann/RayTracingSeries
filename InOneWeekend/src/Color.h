#pragma once

#include <iostream>

#include "Vec3.h"

inline void WriteColor(std::vector<uint8_t>& colors, Color& pixelColor, uint16_t samplesPerPixel, uint32_t pixelNumber)
{
	auto scale = 1.0 / samplesPerPixel;

	pixelColor[0] = sqrt(scale * pixelColor[0]);
	pixelColor[1] = sqrt(scale * pixelColor[1]);
	pixelColor[2] = sqrt(scale * pixelColor[2]);

	auto correctedPixel = pixelNumber * 3;
	colors[correctedPixel] = static_cast<int>(256 * Clamp(pixelColor[0], 0.0, 0.999));
	colors[correctedPixel +1] = static_cast<int>(256 * Clamp(pixelColor[1], 0.0, 0.999));
	colors[correctedPixel +2] = static_cast<int>(256 * Clamp(pixelColor[2], 0.0, 0.999));
}