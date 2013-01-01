#include "construct.h"


namespace Construct
{
	bool Collision2d::intersectLineSegments(const Vector2f& a1, const Vector2f& a2, const Vector2f& b1, const Vector2f& b2, Vector2f& intersection)
	{
		float s1_x = a2.x - a1.x;
		float s1_y = a2.y - a1.y;

		float s2_x = b2.x - b1.x;
		float s2_y = b2.y - b1.y;

		float s = (-s1_y * (a1.x - b1.x) + s1_x * (a1.y - b1.y)) / (-s2_x * s1_y + s1_x * s2_y);
		float t = ( s2_x * (a1.y - b1.y) - s2_y * (a1.x - b1.x)) / (-s2_x * s1_y + s1_x * s2_y);

		if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
		{
			intersection.x = a1.x + (t * s1_x);
			intersection.y = a1.y + (t * s1_y);
			return true;
		}

		return false;
	}


	void calculateProjectedInterval(const std::vector<Vector2f>& edges, const Vector2f& axis, float& a, float& b)
	{
		a = b = axis.dot(edges[0]);

		for (size_t i = 1; i < edges.size(); ++i)
		{
			float x = axis.dot(edges[i]);

			if (x < a)
			{
				a = x;
			}
			else if (x > b)
			{
				b = x;
			}
		}
	}


	bool axisSeparatesPolygons(Vector2f& axis, const std::vector<Vector2f>& edgesP1, const std::vector<Vector2f>& edgesP2)
	{
		float a0, a1;
		calculateProjectedInterval(edgesP1, axis, a0, a1);

		float b0, b1;
		calculateProjectedInterval(edgesP2, axis, b0, b1);

		if ((a1 < b0) || (b1 < a0))
		{
			return true;
		}

		// find the minimum overlap
		float d1 = a1 - b0;
		float d2 = b1 - a0;
		float d = d1 < d2 ? d1 : d2;

		// normalize axis to the lenght of the overlapping part
		axis *= d / axis.dot(axis);

		return false;
	}


	bool Collision2d::intersectPolygons(const std::vector<Vector2f>& edgesA, const Vector2f& positionA, const std::vector<Vector2f>& edgesB, const Vector2f& positionB, Vector2f& pushVector)
	{
		float minDistance = NumericTraits<float>::max();
		Vector2f minTranslation;

		for (size_t j = edgesA.size() - 1, i = 0; i < edgesA.size(); j = i, ++i)
		{
			const Vector2f& v0 = edgesA[j];
			const Vector2f& v1 = edgesA[i];

			// normal of ab
			Vector2f n(-(v1.y - v0.y), (v1.x - v0.x));

			if (axisSeparatesPolygons(n, edgesA, edgesB))
			{
				return false;
			}

			float distance = n.dot(n);
			if (distance < minDistance)
			{
				minDistance = distance;
				minTranslation = n;
			}
		}

		for (size_t j = edgesB.size() - 1, i = 0; i < edgesB.size(); j = i, ++i)
		{
			const Vector2f& v0 = edgesB[j];
			const Vector2f& v1 = edgesB[i];

			// normal of ab
			Vector2f n(-(v1.y - v0.y), (v1.x - v0.x));

			if (axisSeparatesPolygons(n, edgesA, edgesB))
			{
				return false;
			}

			float distance = n.dot(n);
			if (distance < minDistance)
			{
				minDistance = distance;
				minTranslation = n;
			}
		}

		Vector2f positionDelta = positionA - positionB;
		if (positionDelta.dot(minTranslation) < 0.0f)
		{
			minTranslation *= -1.0f;
		}

		pushVector = minTranslation;
		return true;
	}


	void getSupportVertices(const std::vector<Vector2f>& edges, const Vector2f& direction, size_t* supportPoints, int& numSupportPoints)
	{
		int index1 = 0;
		int index2 = -1;
		float d1 = edges[0].dot(direction);

		for (size_t i = 1; i < edges.size(); ++i)
		{
			float d = edges[i].dot(direction);
			if (d > d1)
			{
				index1 = i;
				d1 = d;
			}
			else if (d == d1)
			{
				index2 = i;
			}
		}

		supportPoints[0] = index1;
		numSupportPoints = 1;

		if (index2 != -1)
		{
			supportPoints[1] = index2;
			numSupportPoints++;
		}
	}


	Vector2f projectPointOntoEdge(const Vector2f& p, const Vector2f& e0, const Vector2f& e1)
	{
		Vector2f e = e1 - e0;
		float t = e.dot(p - e0) / e.length();

		// clamp to edge bounds
		t = MathUtil::Numeric::clamp(t, 0.0f, 1.0f);

		return e0 + (e * t);
	}


	void Collision::generateContacts(const Polygon& p1, const Polygon& p2)
	{
		const std::vector<Vector2f>& edgesA = p1.edges();
		size_t numEdgesA = edgesA.size();

		const std::vector<Vector2f>& edgesB = p2.edges();
		size_t numEdgesB = edgesB.size();

		const std::vector<Vector2f>& normalsA = p1.normals();
		const std::vector<Vector2f>& normalsB = p2.normals();

		for (size_t i = 0; i < numEdgesA; ++i)
		{
			const Vector2f& n = normalsA[i];

			const Vector2f& e1 = edgesA[i];
			const Vector2f& e2 = edgesA[(i + 1) % numEdgesA];

			size_t supportPoints[2];
			int numSupportPoints = 0;
			getSupportVertices(edgesB, n.neg(), supportPoints, numSupportPoints);

			for (int j = 0; j < numSupportPoints; ++j)
			{
				Vector2f minkowskiE1 = edgesB[supportPoints[j]] - e1;
				Vector2f minkowskiE2 = edgesB[supportPoints[j]] - e2;

				// Minkowski face
				float faceDistance = minkowskiE1.dot(n);

				Vector2f p = projectPointOntoEdge(Vector2f(0.0f, 0.0f), minkowskiE1, minkowskiE2);
				float distance = p.length() * (faceDistance < 0.0f ? -1.0f : 1.0f);
			}
		}
	}
}
