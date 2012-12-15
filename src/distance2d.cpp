#include "construct.h"


namespace Construct
{
	Vector2f Distance2d::closestPointOnLineSegment(const Vector2f& p, const Vector2f& a, const Vector2f& b)
	{
		Vector2f ba(b - a);
		float u = (p - a).dot(ba);
		float v = (b - p).dot(ba);

		if (u <= 0.0f)
		{
			// region A
			return a;
		}

		if (v <= 0.0f)
		{
			// region B
			return b;
		}

		// region AB
		return ((a * v) + (b * u)) / ba.dot(ba);
	}


	Vector2f Distance2d::closestPointOnTriangle(const Vector2f& p, const Vector2f& a, const Vector2f& b, const Vector2f& c, int* region, int& regionCount)
	{
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
			region[0] = 0;
			regionCount = 1;
			return a;
		}

		if (u_ab <= 0.0f && v_bc <= 0.0f)
		{
			// region B
			region[0] = 1;
			regionCount = 1;
			return b;
		}

		if (u_bc <= 0.0f && v_ca <= 0.0f)
		{
			// region C
			region[0] = 2;
			regionCount = 1;
			return c;
		}

		// signed triangle area * 2
		float area = (b - a).cross(ca);

		if (u_ab > 0.0f && v_ab > 0.0f)
		{
			float w_abc = (a - p).cross(b - p);

			if (w_abc * area <= 0.0f)
			{
				// region AB
				region[0] = 0;
				region[1] = 1;
				regionCount = 2;
				return ((a * u_ab) + (b * v_ab)) / (ab).dot(ab);
			}
		}

		if (u_bc > 0.0f && v_bc > 0.0f)
		{
			float u_abc = (b - p).cross(c - p);

			if (u_abc * area <= 0.0f)
			{
				// region BC
				region[0] = 1;
				region[1] = 2;
				regionCount = 2;
				return ((b * u_bc) + (c * v_bc)) / (bc).dot(bc);
			}
		}

		if (u_ca > 0.0f && v_ca > 0.0f)
		{
			float v_abc = (c - p).cross(a - p);

			if ( v_abc * area <= 0.0f)
			{
				// region CA
				region[0] = 2;
				region[1] = 0;
				regionCount = 2;
				return ((c * u_ca) + (a * v_ca)) / (ca).dot(ca);
			}
		}

		region[0] = 0;
		region[1] = 1;
		region[2] = 2;
		regionCount = 3;

		// region ABC
		return p;
	}


	size_t findSupportPoint(const Vector2f& d, const std::vector<Vector2f>& convexSet)
	{
		size_t index = 0;
		float maxDistance = convexSet[0].dot(d);

		for (size_t i = 1; i < convexSet.size(); ++i)
		{
			float distance = convexSet[i].dot(d);
			if (distance > maxDistance)
			{
				index = i;
				maxDistance = distance;
			}
		}

		return index;
	}


	Vector2f Distance2d::pointConvexSet(const Vector2f& p, const std::vector<Vector2f>& convexSet)
	{
		size_t simplex[3];
		int count = 1;
		simplex[0] = 0;

		Vector2f d;
		Vector2f q;

		int region[3];
		int regionCount;

		while (true)
		{
			switch (count)
			{
			case 1:
				q = convexSet[simplex[0]];
				break;
			case 2:
				q = closestPointOnLineSegment(p, convexSet[simplex[0]], convexSet[simplex[1]]);
				break;
			case 3:
				q = closestPointOnTriangle(p, convexSet[simplex[0]], convexSet[simplex[1]], convexSet[simplex[2]], region, regionCount);
				break;
			default:
				break;
			}

			d = p - q;
			size_t support = findSupportPoint(d, convexSet);

			if (count == 1)
			{
				count = 2;
				simplex[1] = support;
				continue;
			}

			if (count == 2)
			{
				if (support != simplex[0] && support != simplex[1])
				{
					count = 3;
					simplex[2] = support;
					continue;
				}
				else
				{
					return q;
				}
			}

			if (count == 3)
			{
				if (regionCount == 2)
				{
					if (support != simplex[region[0]] && support != simplex[region[1]])
					{
						int x = simplex[region[0]];
						int y = simplex[region[1]];
						simplex[0] = x;
						simplex[1] = y;
						simplex[2] = support;
						count = 3;
						continue;
					}
					else
					{
						return q;
					}
				}

				if (regionCount == 1 && simplex[region[0]] != support)
				{
					simplex[0] = simplex[region[0]];
					simplex[1] = support;
					count = 2;
					continue;
				}
				
				return q;
			}
		}
	}


}
