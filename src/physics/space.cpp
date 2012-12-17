#include "construct.h"


namespace Construct
{
	Space::~Space()
	{
		for (size_t i = 0; i < mBodies.size(); ++i)
		{
			delete mBodies[i];
		}
	}


	void Space::setGravity(const Vector2f& gravity)
	{
		mGravity = gravity;
	}


	Body* Space::addBody(Body::Type bodyType)
	{
		Body* body = new Body(bodyType);
		mBodies.push_back(body);
		return body;
	}


	void drawNormal(const Vector2f& t, const Vector2f& n)
	{
		Vector2f& endp = t + n * 20.0f;
		Vertex_Vector_XYZ_RGBA line1;
		line1.resize(2);
		line1[0].setPosition(t);
		line1[0].setColor(1.0f, 1.0f, 1.0f, 1.0f);
		line1[1].setPosition(endp);
		line1[1].setColor(1.0f, 1.0f, 1.0f, 1.0f);
		Index_Vector ind1;
		ind1.resize(2);
		ind1[0] = 0;
		ind1[1] = 1;
		GL_Render& gl = GL_Render::get();
		gl.setDrawMode(GL_LINES);
		gl.draw_XYZ_RGBA(line1, ind1);

		Quad q;
		q.transformation().setTranslation(endp.x, endp.y);
		q.setSize(5, 5);
		q.update();
		q.draw();
	}


	void Space::update(unsigned int deltaTimeMs)
	{
		for (size_t i = 0; i < mBodies.size(); ++i)
		{
			if (mBodies[i]->getType() != Body::StaticBody)
			{
				Vector2f v = mBodies[i]->getVelocity() + mGravity;
				mBodies[i]->setVelocity(v);
			}
		}

		for (size_t i = 0; i < mBodies.size(); ++i)
		{
			mBodies[i]->update(deltaTimeMs);
		}

		contacts.clear();

		for (size_t i = 0; i < mBodies.size(); ++i)
		{
			Body* body1 = mBodies[i];
			Shape* shape1 = body1->getShape();

			for (size_t j = i + 1; j < mBodies.size(); ++j)
			{
				Body* body2 = mBodies[j];
				
				if (body1->getType() == Body::StaticBody && body2->getType() == Body::StaticBody)
				{
					// static bodies do not collide
					continue;
				}
				
				Shape* shape2 = body2->getShape();

				if (shape1 && shape2)
				{
					contacts.push_back(Contact());
					Contact& contact1 = contacts.back();
					contacts.push_back(Contact());
					Contact& contact2 = contacts.back();

					contact1.body1 = body1;
					contact1.body2 = body2;

					contact2.body1 = body2;
					contact2.body2 = body1;

					// polygon - polygon collision
					if (shape1->getType() == Shape::PolygonShape && shape2->getType() == Shape::PolygonShape)
					{
						Polygon* p1 = static_cast<Polygon*>(shape1);
						Polygon* p2 = static_cast<Polygon*>(shape2);
						Vector2f minSeparation;
						Vector2f n1, n2;
						GJK2d::distance(*p1, *p2, contact1.witness1, n1, contact1.witness2, n2);
						
						contact1.distance = (contact1.witness2 - contact1.witness1).length() * 0.9;
						contact2.distance = contact1.distance;
						contact2.witness1 = contact1.witness2;
						contact2.witness2 = contact1.witness1;

						if (contact1.distance > 0.0f)//NumericTraits<float>::eps())
						{
							contact1.normal = (contact1.witness2 - contact1.witness1);
							contact1.normal /= contact1.distance;

							contact2.normal = (contact1.witness1 - contact1.witness2);
							contact2.normal /= contact2.distance;
						}
						/*else
						{
							contact1.normal = n1;
							contact1.normal.normalize();

							contact2.normal = n2;
							contact2.normal.normalize();
						}*/
						else if (Collision2d::intersectPolygons(p1->edges(), p1->transformation().translation(), p2->edges(), p2->transformation().translation(), minSeparation))
						{
							contact1.distance = -minSeparation.length();
							contact1.normal = minSeparation / contact1.distance;

							contact2.distance = -contact1.distance;
							contact2.normal = minSeparation / contact2.distance;
						}
					}
				}
			}
		}

		const int iterations = 3;
		for (int i = 0;  i < iterations; ++i)
		{
			for (std::list<Contact>::iterator it  = contacts.begin(); it != contacts.end(); ++it)
			{
				Body* body1 = it->body1;
				Body* body2 = it->body2;

				float relativeNormalVelocity = (body2->getVelocity() - body1->getVelocity()).dot(it->normal);
				float remove = relativeNormalVelocity + it->distance;/* / ((deltaTimeMs + 1) / 1000.0f);*/

				Vector2f tN(-it->normal.y, it->normal.x);
				float vtN = (body2->getVelocity() - body1->getVelocity()).dot(tN);


				float d = it->distance;
				if (remove < 0.0f)
				{
					// compute impulse
					float impulse = remove / (body1->getInvMass() + body2->getInvMass());

					float absMag = fabs(impulse) * 0.9f;
					// friction
					float mag = MathUtil::Numeric::clamp(vtN / (body1->getInvMass() + body2->getInvMass()), -absMag, absMag);
					Vector2f imp2 = tN * mag;

					// apply impulse
					Vector2f impulse1 = it->normal * (impulse * body1->getInvMass());
					Vector2f impulse2 = it->normal * (-impulse * body2->getInvMass());

					Vector2f f_impulse1 = imp2 * body1->getInvMass();
					Vector2f f_impulse2 = imp2 * -body2->getInvMass();

					body1->setVelocity(body1->getVelocity() + impulse1 + f_impulse1);
					body2->setVelocity(body2->getVelocity() + impulse2 + f_impulse2);

					if (body1->getType() == Body::DynamicBody)
					{
						body1->contact(body2, impulse1);
					}
					
					if (body2->getType() == Body::DynamicBody)
					{
						body2->contact(body1, impulse2);
					}
				}
			}
		}

		for (size_t i = 0; i < mBodies.size(); ++i)
		{
			mBodies[i]->update(deltaTimeMs);
		}
	}


	void Space::debug()
	{
		for (std::list<Contact>::iterator it  = contacts.begin(); it != contacts.end(); ++it)
		{
			drawNormal(it->witness1, it->normal);
		}
	}


	void Space::handleCollision(Body* body1, Body* body2, const Vector2f& minSeparation)
	{
		Vector2f n = minSeparation;
		n.normalize();

		// clamp sum friction and elasticity
		float friction = std::min(body1->getFriction() + body2->getFriction(), 1.0f);
		float elasticity = std::min(body1->getElasticity() + body2->getElasticity(), 1.0f);

		if (body1->getType() == Body::StaticBody)
		{
			Vector2f v = calculateVelocity(body2->getVelocity(), n, elasticity, friction);

			body2->setVelocity(v);
			body2->translate(minSeparation * -1.0f);
			body2->contact(body1, minSeparation * -1.0f);
		}
		else if (body2->getType() == Body::StaticBody)
		{
			Vector2f v = calculateVelocity(body1->getVelocity(), n, elasticity, friction);

			body1->setVelocity(v);
			body1->translate(minSeparation);
			body1->contact(body2, minSeparation);
		}
		else
		{
			Vector2f v = calculateVelocity(body1->getVelocity() - body2->getVelocity(), n, elasticity + 1.0f, friction + 1.0f);
		
			body1->setVelocity(v * 0.5f);
			body1->translate(minSeparation * 0.5f);
			body1->contact(body2, minSeparation * 0.5f);

			body2->setVelocity(v * -0.5f);
			body2->translate(minSeparation * -0.5f);
			body2->contact(body1, minSeparation * -0.5f);
		}
	}


	Vector2f Space::calculateVelocity(const Vector2f& v, const Vector2f& n, float elasticity, float friction) const
	{
		/*

		V: velocity of the body
		N: normalized separation vector

		// velocity projected to separation
		Vn = (V dot N) * N;

		// horizontal component of Vn
		Vt = V – Vn;

		// the new velocity, where friction is [0,1] elasticity is [0,1] 
		V' = Vt * (1 – friction) + Vn  * -(elasticity);

		*/

		Vector2f vn = n * v.dot(n);
		Vector2f vt = v - vn;

		return vt * (1 - friction) + vn * -elasticity;
	}
}
