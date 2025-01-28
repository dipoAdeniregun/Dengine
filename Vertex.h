#pragma once

#include <GL/glew.h>

#include "ColorRGBA8.h"

namespace Dengine {

	

	struct Position {
		float x;
		float y;
	};

	

	struct UV {
		float u;
		float v;
	};

	//pick smallest multiple of 4bytes you can for vertex info
	struct Vertex {
		Position position;	//x, y

		ColorRGBA8 color;	//red, green, blue, alpha

		UV uv;

		void setColor(GLubyte _r, GLubyte _g, GLubyte _b, GLubyte _a) {
			color.r = _r;
			color.g = _g;
			color.b = _b;
			color.a = _a;
		}

		void setPosition(float _x, float _y) {
			position.x = _x;
			position.y = _y;
		}

		void setUV(float _u, float _v) {
			uv.u = _u;
			uv.v = _v;
		}
	};
}