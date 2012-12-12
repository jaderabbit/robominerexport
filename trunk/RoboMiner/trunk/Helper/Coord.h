#pragma once

struct Coord {
	int x;
	int y;

	Coord();
	~Coord(void);

	bool operator==(const Coord &other) const {
		if ( x == other.x && y == other.y) {
			return true;
		}
		return false;
	}

	bool operator<(const Coord &other) const {
		if ( x < other.x && y < other.y) {
			return true;
		}
		return false;
	}
};

