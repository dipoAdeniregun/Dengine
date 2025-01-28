#pragma once
#include <unordered_map>

namespace Dengine {
	enum class Colors
	{
		WHITE,
		BLACK,
		BROWN,
		RED,
		YELLOW,
		BLUE,
		ORANGE,
		GREY,
		PURPLE,
		GREEN,
		SILVER,
		GOLD,
		PINK
	};

	

	struct ColorRGBA8 {
		ColorRGBA8() :r(0), g(0), b(0), a(0) {}
		ColorRGBA8(GLubyte R, GLubyte G, GLubyte B, GLubyte A)
			: r(R), g(G), b(B), a(A) {}

		ColorRGBA8(int hexcode) {
			r = (hexcode >> 24) & 0xff;
			g = (hexcode >> 16) & 0xff;
			b = (hexcode >> 8) & 0xff;
			a = hexcode & 0xff;

		}

		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;

		
		ColorRGBA8(Colors color) {
			r = colorMap.at(color).r;
			g = colorMap.at(color).g;
			b = colorMap.at(color).b;
			a = 255;
		}

		static const std::unordered_map<Dengine::Colors, ColorRGBA8> colorMap;
		
	};

	const std::unordered_map<Dengine::Colors, ColorRGBA8> colorMap{
		{Colors::BLACK, ColorRGBA8(0,0,0,0)},
		{Colors::WHITE, ColorRGBA8(255,255,255,0)},
		{Colors::BROWN, ColorRGBA8(73,32,0,0)},
		{Colors::GREY, ColorRGBA8(128, 128, 128, 0)},
		{Colors::RED, ColorRGBA8(255,0,0,0)},
		{Colors::ORANGE, ColorRGBA8(255,165,0,0)},
		{Colors::YELLOW, ColorRGBA8(255,255,0,0)},
		{Colors::GREEN, ColorRGBA8(0,255,0,0)},
		{Colors::BLUE, ColorRGBA8(0,0,255,0)},
		{Colors::PURPLE, ColorRGBA8(128,0,128,0)},
		{Colors::PINK, ColorRGBA8(255,192,203,0)},
		{Colors::GOLD, ColorRGBA8(255,215,0,0)},
		{Colors::SILVER, ColorRGBA8(192,192,192,0)},
	};
}