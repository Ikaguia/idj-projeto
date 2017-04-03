#ifndef COLLISIONHPP
#define COLLISIONHPP

#include <cmath>
#include <algorithm>

#include <geometry.hpp>

class Collision {
	public:
		static inline bool IsColliding(Rect& a, Rect& b, float angleOfA, float angleOfB) {
			Vec2 A[] = { Vec2( a.x, a.y + a.h ),
						  Vec2( a.x + a.w, a.y + a.h ),
						  Vec2( a.x + a.w, a.y ),
						  Vec2( a.x, a.y )
						};
			Vec2 B[] = { Vec2( b.x, b.y + b.h ),
						  Vec2( b.x + b.w, b.y + b.h ),
						  Vec2( b.x + b.w, b.y ),
						  Vec2( b.x, b.y )
						};

			for (auto& v : A) {
				v = (v - a.center()).rotate(angleOfA) + a.center();
			}

			for (auto& v : B) {
				v = (v - b.center()).rotate(angleOfB) + b.center();
			}

			Vec2 axes[] = { (A[0] - A[1]).unit(), (A[1] - A[2]).unit(), (B[0] - B[1]).unit(), (B[1] - B[2]).unit() };

			for (auto& axis : axes) {
				float P[4];

				for (int i = 0; i < 4; ++i) P[i] = Dot(A[i], axis);

				float minA = *std::min_element(P, P + 4);
				float maxA = *std::max_element(P, P + 4);

				for (int i = 0; i < 4; ++i) P[i] = Dot(B[i], axis);

				float minB = *std::min_element(P, P + 4);
				float maxB = *std::max_element(P, P + 4);

				if (maxA < minB || minA > maxB)return false;
			}

			return true;
		}

	private:

		static inline float Dot(const Vec2& a, const Vec2& b) {
			return a.x * b.x + a.y * b.y;
		}
};

#endif//COLLISIONHPP
