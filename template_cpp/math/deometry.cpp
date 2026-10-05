

template<typename Tp>
struct Point {
	using point = Point<Tp>;
    Tp x;
    Tp y;

    Point() : x(0), y(0)
    {}

	Point(Tp _x, Tp _y): x(_x), y(_y)
	{}


	Tp cross(const point& b) const {
		return x * b.y - y * b.x;
	}


	Tp operator*(const point& b) const {
		return x * b.x + y * b.y;
	}

	// B is self, this is AB
	inline point lookAt(const point& a) const {
		return point(x - a.x, y - a.y);
	}

	// a - b = ab
	point operator-(const point& a) const {
		return lookAt(a);
	}

	point& operator-=(const point& a) {
		x -= a.x;
		y -= a.y;
		return *this;
	}
};



template<typename Tp>
struct triangle {
	using point = Point<Tp>;
	point a;
	point b;
	point c;

	triangle(){}

	triangle(const point& _a, const point& _b, const point& _c): a(_a), b(_b), c(_c) {}

	// include on edge and in triangle
	bool inTriangle(const point& d) {
		auto da =  a - d;
		auto db = b - d;
		auto dc = c - d;

		Tp ab = da.cross(db), bc = db.cross(dc) >= 0, ca = dc.cross(da);
		if ((ab >= 0 and bc >= 0 and ca >= 0) or (ab <= 0 and bc <= 0 and ca <= 0)) {
			return true;
		} else {
			return false;
		}
	}
};