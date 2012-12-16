#include "construct.h"


namespace Construct
{
	void GJK2d::distance(const Polygon& polygon1, const Polygon& polygon2, Vector2f& witnessPoint1, Vector2f& witnessNormal1, Vector2f& witnessPoint2, Vector2f& witnessNormal2)
	{
		const std::vector<Vector2f>& edges1 = polygon1.edges();
		const std::vector<Vector2f>& edges2 = polygon2.edges();

		Simplex simplex;
		simplex.vertices[0].index1 = 0;
		simplex.vertices[0].index2 = 0;
		simplex.vertices[0].p1 = edges1[0];
		simplex.vertices[0].p2 = edges2[0];
		simplex.vertices[0].p = edges2[0] - edges1[0];
		simplex.count = 1;

		int save1[3], save2[3];
		
		const int maxIterations = 20;
		int iter = 0;

		while (iter < maxIterations)
		{
			// Copy simplex so we can identify duplicates.
			int saveCount = simplex.count;

			for (int i = 0; i < saveCount; ++i)
			{
				save1[i] = simplex.vertices[i].index1;
				save2[i] = simplex.vertices[i].index2;
			}

			solveSimplex(Vector2f(0.0f, 0.0f), simplex);

			// the origin is in the triangle
			if (simplex.count == 3)
			{
				break;
			}

			Vector2f d = getSearchDirection(simplex);

			// ensure that the search direction non-zero
			if (d.dot(d) == 0.0f)
			{
				break;
			}
		
			Vertex& v = simplex.vertices[simplex.count];
			
			v.index1 = findSupportPoint(d * -1.0f, edges1);
			v.p1 = edges1[v.index1];
			v.index2 = findSupportPoint(d, edges2);
			v.p2 = edges2[v.index2];
			v.p = v.p2 - v.p1;

			// check for duplicate support points
			bool duplicate = false;
			for (int i = 0; i < saveCount; ++i)
			{
				if (v.index1 == save1[i] && v.index2 == save2[i])
				{
					duplicate = true;
					break;
				}
			}

			if (duplicate)
			{
				break;
			}

			iter++;
			simplex.count++;
		}

		getWitnessPoints(simplex, witnessPoint1, witnessNormal1, witnessPoint2, witnessNormal2);
	}


	void GJK2d::solveSimplex(const Vector2f& p, Simplex& simplex)
	{
		switch (simplex.count)
		{
		case 1:
			simplex.vertices[0].u = 1.0f;
			break;

		case 2:
			{
				Vector2f& a = simplex.vertices[0].p;
				Vector2f& b = simplex.vertices[1].p;

				Vector2f ba(b - a);
				float u = (p - a).dot(ba);
				float v = (b - p).dot(ba);

				if (u <= 0.0f)
				{
					// region A
					simplex.count = 1;
					simplex.vertices[0].u = 1.0f;
					simplex.divisor = 1.0f;

					break;
				}

				if (v <= 0.0f)
				{
					// region B
					simplex.count = 1;
					simplex.vertices[0] = simplex.vertices[1];
					simplex.vertices[0].u = 1.0f;
					simplex.divisor = 1.0f;

					break;
				}

				// region AB
				simplex.vertices[0].u = v;
				simplex.vertices[1].u = u;
				simplex.divisor = ba.dot(ba);

				break;
			}
		case 3:
			{
				Vector2f& a = simplex.vertices[0].p;
				Vector2f& b = simplex.vertices[1].p;
				Vector2f& c = simplex.vertices[2].p;

				Vector2f ab(a - b);
				float u_ab = (p - b).dot(ab);
				float v_ab = (a - p).dot(ab);

				Vector2f bc(b - c);
				float u_bc = (p - c).dot(bc);
				float v_bc = (b - p).dot(bc);

				Vector2f ca(c - a);
				float u_ca = (p - a).dot(ca);
				float v_ca = (c - p).dot(ca);

				if (v_ab <= 0.0f && u_ca <= 0.0f)
				{
					// region A
					simplex.count = 1;
					simplex.vertices[0].u = 1.0f;
					simplex.divisor = 1.0f;

					break;
				}

				if (u_ab <= 0.0f && v_bc <= 0.0f)
				{
					// region B
					simplex.count = 1;
					simplex.vertices[0] = simplex.vertices[1];
					simplex.vertices[0].u = 1.0f;
					simplex.divisor = 1.0f;

					break;
				}

				if (u_bc <= 0.0f && v_ca <= 0.0f)
				{
					// region C
					simplex.count = 1;
					simplex.vertices[0] = simplex.vertices[2];
					simplex.vertices[0].u = 1.0f;
					simplex.divisor = 1.0f;

					break;
				}

				// signed triangle area * 2
				float area = (b - a).cross(ca);

				if (u_ab > 0.0f && v_ab > 0.0f)
				{
					float w_abc = (a - p).cross(b - p);

					if (w_abc * area <= 0.0f)
					{
						// region AB
						simplex.count = 2;
						simplex.vertices[0].u = u_ab;
						simplex.vertices[1].u = v_ab;
						simplex.divisor = ab.dot(ab);

						break;
					}
				}

				if (u_bc > 0.0f && v_bc > 0.0f)
				{
					float u_abc = (b - p).cross(c - p);

					if (u_abc * area <= 0.0f)
					{
						// region BC
						simplex.count = 2;
						simplex.vertices[0] = simplex.vertices[1];
						simplex.vertices[1] = simplex.vertices[2];
						simplex.vertices[0].u = u_bc;
						simplex.vertices[1].u = v_bc;
						simplex.divisor = bc.dot(bc);
						
						break;
					}
				}

				if (u_ca > 0.0f && v_ca > 0.0f)
				{
					float v_abc = (c - p).cross(a - p);

					if (v_abc * area <= 0.0f)
					{
						// region CA
						simplex.count = 2;
						simplex.vertices[1] = simplex.vertices[0];
						simplex.vertices[0] = simplex.vertices[2];
						simplex.vertices[0].u = u_ca;
						simplex.vertices[1].u = v_ca;
						simplex.divisor = ca.dot(ca);

						break;
					}
				}

				// region ABC
				Vector2f bp = b - p;
				Vector2f cp = c - p;
				Vector2f ap = a - p;

				simplex.count = 3;
				simplex.vertices[0].u = bp.cross(cp);
				simplex.vertices[1].u = cp.cross(ap);
				simplex.vertices[2].u = ap.cross(bp);
				simplex.divisor = area;

				break;
			}
		}
	}


	size_t GJK2d::findSupportPoint(const Vector2f& d, const std::vector<Vector2f>& points)
	{
		size_t support = 0;
		float maxDistance = points[0].dot(d);

		for (size_t i = 1; i < points.size(); ++i)
		{
			float distance = points[i].dot(d);

			if (distance > maxDistance)
			{
				support = i;
				maxDistance = distance;
			}
		}

		return support;
	}


	Vector2f GJK2d::getSearchDirection(const Simplex& simplex)
	{
		switch (simplex.count)
		{
		case 1:
			return (simplex.vertices[0].p * -1.0f);

		case 2:
			{
				Vector2f ab = simplex.vertices[1].p - simplex.vertices[0].p;
			
				if (ab.cross(simplex.vertices[0].p * -1.0f) > 0.0f)
				{
					// origin is left of AB
					return Vector2f(-ab.y, ab.x);
				}
				else
				{
					// origin is right of AB
					return Vector2f(ab.y, -ab.x);
				}
			}

		default:
			assert(false);
			return Vector2f(0.0f, 0.0f);
		}
	}


	void GJK2d::getWitnessPoints(const Simplex& simplex, Vector2f& point1, Vector2f& normal1, Vector2f& point2, Vector2f& normal2)
	{
		switch (simplex.count)
		{
		case 1:
			point1 = simplex.vertices[0].p1;
			point2 = simplex.vertices[0].p2;
			break;

		case 2:
			point1 = (simplex.vertices[0].p1 * simplex.vertices[0].u + simplex.vertices[1].p1 * simplex.vertices[1].u) / simplex.divisor;
			
			if (simplex.vertices[0].index1 > simplex.vertices[1].index1)
			{
				normal1 = simplex.vertices[0].p1 - simplex.vertices[1].p1;
			}
			else
			{
				normal1 = simplex.vertices[1].p1 - simplex.vertices[0].p1;
			}
			normal1 = Vector2f(-normal1.y, normal1.x);

			point2 = (simplex.vertices[0].p2 * simplex.vertices[0].u + simplex.vertices[1].p2 * simplex.vertices[1].u) / simplex.divisor;
			if (simplex.vertices[0].index2 > simplex.vertices[1].index2)
			{
				normal2 = simplex.vertices[0].p2 - simplex.vertices[1].p2;
			}
			else
			{
				normal2 = simplex.vertices[1].p2 - simplex.vertices[0].p2;
			}
			normal2 = Vector2f(-normal2.y, normal2.x);
			break;

		case 3:
			point1 = (simplex.vertices[0].p1 * simplex.vertices[0].u + simplex.vertices[1].p1 * simplex.vertices[1].u + simplex.vertices[2].p1 * simplex.vertices[2].u) / simplex.divisor;
			point2 = point1;
			break;
		}
	}
}
