#pragma once

#include <iostream>

#include "Vec3.h"

void WriteColor(std::ostream& out, Color pixelColor, int samplesPerPixel)
{
	auto r = pixelColor[0];
	auto g = pixelColor[1];
	auto b = pixelColor[2];

	auto scale = 1.0 / samplesPerPixel;

	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	out << static_cast<int>(256 * Clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * Clamp(b, 0.0, 0.999)) << '\n';
}