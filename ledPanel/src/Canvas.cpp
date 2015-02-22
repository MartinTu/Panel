/*
 * Canvas.cpp
 *
 *  Created on: 07.09.2014
 *      Author: keKs
 */

#include "Canvas.h"

using namespace std;

Canvas::Canvas(int _width, int _height) :
		width(_width), height(_width) {
	pixel.resize(width);
	for (int i = 0; i < width; i++) {
		pixel[i].resize(height);
	}
	setColor(color_black);
	//seed
	srand((unsigned int) Utils::getTime());
}

Canvas::~Canvas() {
}

void Canvas::setPixel(int x, int y, color_t color, bool errorMsg) {
	if (!Utils::isInRange(x, 0, width - 1)) {
		if (errorMsg)
			cerr << "setPixel() x out of bounds: " << x << endl;
		return;
	}
	if (!Utils::isInRange(y, 0, height - 1)) {
		if (errorMsg)
			cerr << "setPixel() y out of bounds: " << y << endl;
		return;
	}
	pixel[x][y] = color;
}

void Canvas::setPixel(int num, color_t color, bool errorMsg) {
	if ((num >= width * height) || (num < 0)) {
		if (errorMsg)
			cerr << "setPixel() num out of bounds: " << num << endl;
		return;
	}
	int x = num % width;
	int y = num / width;
	pixel[x][y] = color;
}

void Canvas::setColor(color_t color) {
	for (int x = 0; x < width; x++)
		for (int y = 0; y < height; y++)
			pixel[x][y] = color;
}

void Canvas::drawFrame(const vector<vector<color_t> >& vec) {
	if ((vec.size() == pixel.size()) && (vec[0].size() == pixel[0].size())) {
		//copy first
		vector<vector<color_t> > neu(vec);
		pixel.swap(neu);
	} else {
		cerr << "drawFrame() does not got correct vector size" << endl;
	}
}

void Canvas::drawFrame(int dataLength, const uint8_t * data) {
	if (dataLength == width * height * 3) {
		int i = 0;
		//0-0 = topleft
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				pixel[x][y].red = data[i++];
				pixel[x][y].green = data[i++];
				pixel[x][y].blue = data[i++];
			}
		}
	} else {
		cerr << "drawFrame() does not got correct data length" << endl;
	}
}

void Canvas::drawSector(int dataLength, const uint8_t* data, int width,
		int height, int xOffset, int yOffset) {
	if (((height + yOffset) * (width + xOffset) * 3 == dataLength)
			&& (height + yOffset <= this->height)
			&& (width + xOffset <= this->width)) {
		int i = 0;
		for (int x = xOffset; x < xOffset + width; x++) {
			for (int y = xOffset; y < yOffset + height; y++) {
				pixel[x][y].red = data[i++];
				pixel[x][y].green = data[i++];
				pixel[x][y].blue = data[i++];
			}
		}
	}

}

void Canvas::drawRandomPixel(int x, int y) {
	if ((x < 0) || (x >= width)) {
		cerr << "drawRandomPixel() x out of bounds" << endl;
		return;
	}
	if ((y < 0) || (x >= height)) {
		cerr << "drawRandomPixel() y out of bounds" << endl;
		return;
	}
	this->pixel[x][y].red = (uint8_t) rand() & 0xff;
	this->pixel[x][y].green = (uint8_t) rand() & 0xff;
	this->pixel[x][y].blue = (uint8_t) rand() & 0xff;
}

void Canvas::drawRandomPicture() {
	for (int y = 0; y < this->height; y++) {
		for (int x = 0; x < this->width; x++) {
			this->pixel[x][y].red = (uint8_t) rand() & 0xff;
			this->pixel[x][y].green = (uint8_t) rand() & 0xff;
			this->pixel[x][y].blue = (uint8_t) rand() & 0xff;
		}
	}
}

void Canvas::drawLine(int x0, int y0, int x1, int y1, color_t color,
		int width) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2; /* error value e_xy */

	for (;;) { /* loop */
		setPixel(x0, y0, color, 0);
		if (x0 == x1 && y0 == y1)
			break;
		e2 = 2 * err;
		if (e2 > dy) {
			err += dy;
			x0 += sx;
		} /* e_xy+e_x > 0 */
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		} /* e_xy+e_y < 0 */
	}
}

void Canvas::drawRect(int x0, int y0, int x1, int y1, color_t color) {
	int xa = Utils::min(x0, x1);
	int xb = Utils::max(x0, x1);

	int ya = Utils::min(y0, y1);
	int yb = Utils::max(y0, y1);
	for (int y = ya; y <= yb; y++) {
		for (int x = xa; x <= xb; x++) {
			setPixel(x, y, color, 0);
		}
	}

}

void Canvas::drawCircle(int x0, int y0, int radius, color_t color,
		uint8_t width) {
	int f = 1 - radius;
	int ddF_x = 0;
	int ddF_y = -2 * radius;
	int x = 0;
	int y = radius;

	setPixel(x0, y0 + radius, color, 0);
	setPixel(x0, y0 - radius, color, 0);
	setPixel(x0 + radius, y0, color, 0);
	setPixel(x0 - radius, y0, color, 0);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		setPixel(x0 + x, y0 + y, color, 0);
		setPixel(x0 - x, y0 + y, color, 0);
		setPixel(x0 + x, y0 - y, color, 0);
		setPixel(x0 - x, y0 - y, color, 0);
		setPixel(x0 + y, y0 + x, color, 0);
		setPixel(x0 - y, y0 + x, color, 0);
		setPixel(x0 + y, y0 - x, color, 0);
		setPixel(x0 - y, y0 - x, color, 0);
	}
}

void Canvas::dimPixel(int x, int y, float dimm, bool errorMsg) {
	if (!Utils::isInRange(x, 0, width - 1)) {
		if (errorMsg)
			cerr << "setPixel() x out of bounds: " << x << endl;
		return;
	}
	if (!Utils::isInRange(y, 0, height - 1)) {
		if (errorMsg)
			cerr << "setPixel() y out of bounds: " << y << endl;
		return;
	}
	pixel[x][y].setDimmer(dimm);
}

void Canvas::dimFrame(float dimm) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			pixel[x][y].setDimmer(dimm);
		}
	}
}

color_t Canvas::getPixel(int x, int y) {
	if ((x < 0) || (x >= width)) {
		cerr << "getPixel() x out of bounds: " << x << endl;
		return color_black;
	}
	if ((y < 0) || (x >= height)) {
		cerr << "getPixel() y out of bounds: " << y << endl;
		return color_black;
	}
	return pixel[x][y];
}

color_t Canvas::getPixel(int num) {
	if ((num < 0) || (num >= width * height)) {
		cerr << "getPixel() num out of bounds: " << num << endl;
		return color_black;
	}
	int x = num % width;
	int y = num / width;
	return pixel[x][y];
}

vector<vector<color_t> >& Canvas::getPicture() {
	vector<vector<color_t> >& pr = pixel;
	return pr;
}

int Canvas::getWidth() {
	return width;
}

int Canvas::getHeight() {
	return height;
}
