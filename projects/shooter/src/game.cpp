#include <sstream>

#include "construct.h"
#include "game.h"

using namespace Construct;

float r = 0.0f;
Triangle t;
Quad q;
Circle c;
Construct::BitmapFont font;


void Sandbox::init()
{
	mSpace.setGravity(Vector2f(0.0f, 0.1f));
	mGround.setSize(500, 50);
	
	Body* b = mSpace.addBody(Body::StaticBody);
	b->setShape(&mGround);
	b->setPosition(512, 500);
	b->setElasticity(0.3f);
	b->setFriction(0.2f);
	mBodies.push_back(b);

	mBox.setSize(50, 50);
	b = mSpace.addBody(Body::DynamicBody);
	b->setMass(10.0f);
	b->setShape(&mBox);
	b->setPosition(512, 80);
	b->setContactCallback(createFunctor(this, &Sandbox::playerContact));
	mBodies.push_back(b);


	mBox2.setSize(50, 50);
	b = mSpace.addBody(Body::DynamicBody);
	b->setShape(&mBox2);
	b->setMass(10.0f);
	b->setPosition(512, 200);
	mBodies.push_back(b);


	mWalls[0].setSize(50, 100);
	b = mSpace.addBody(Body::StaticBody);
	b->setShape(&mWalls[0]);
	b->setPosition(700, 400);

	mWalls[1].setSize(100, 30);
	b = mSpace.addBody(Body::StaticBody);
	b->setShape(&mWalls[1]);
	b->setFriction(0.3f);
	b->setPosition(300, 400);

	mCircle.setRadius(100);
	b = mSpace.addBody(Body::StaticBody);
	b->setShape(&mCircle);
	b->setFriction(0.3f);
	b->setPosition(300, 400);

	mOnTheGround = false;
}


void Sandbox::update(unsigned int deltaTimeMs)
{
	mOnTheGround = false;
	mSpace.update(deltaTimeMs);
}


void Sandbox::jump(float x, float y)
{
	Vector2f velocity = mBodies[1]->getVelocity();

	if (mOnTheGround)
	{
		velocity.y += y;
	}

	velocity.x += x/10.0f;

	if (velocity.x < -10)
	{
		velocity.x = -10;
	}

	if (velocity.x > 10)
	{
		velocity.x = 10;
	}

	mBodies[1]->setVelocity(velocity);
}


void Sandbox::playerContact(Body* body, const Vector2f& pushVector)
{
	if (pushVector.dot(Vector2f(0, -1)) > 0.0f)
	{
		mOnTheGround = true;
	}
}

bool ret = false;
void Sandbox::draw()
{
	Context::push();
	Context::setFillColor(Color4(0, 170, 0, 255));
	Context::setOutlineColor(Color4(0, 255, 0, 255));
	Context::setFilled(true);
	Context::setOutlined(true);

	mGround.draw();
	mBox.draw();
	mBox2.draw();
	mWalls[0].draw();
	mWalls[1].draw();
	mCircle.draw();
	update(1);
	if (Input::isKeyDown(Input::K_RETURN))
	{
		ret = true;
		
	}
	if (ret && !Input::isKeyDown(Input::K_RETURN))
	{
		ret = false;
		//update(1);
	}
	mSpace.debug();

	std::stringstream strStream;
	
	strStream << "on the ground: " <<  mOnTheGround;
	font.drawString(strStream.str(), 0, 80, BitmapFont::ALIGN_TL);

	Context::pop();
}


Game::Game()
: mAtlas(0)
{

}


void Game::init(unsigned w, unsigned h)
{
	mW = w;
	mH = h;

	GL_Render::get().init();
	font.load("vector_skin_font.bff");

	mPlayer.init();
	mPlayer.setPosition(150, 150);

	Context::push();
	Context::setFillColor(Color4(0, 100, 0, 255));
	Context::setOutlineColor(Color4(0, 255, 0, 255));
	Context::setFilled(true);
	Context::setOutlined(true);

	mSandbox.init();
}


void Game::step(unsigned deltaTimeMs)
{
	GL_Render& gl = GL_Render::get();
	int drawCalls = gl.getNumDrawCalls();

	gl.beginFrame();
	gl.setOrthoProjection(0.0f, static_cast<float>(mW), static_cast<float>(mH), 0.0f, 1.0f, -1.0f);


	//mWorld.draw();

	Affine2df tr;
	int mouseX, mouseY;
	Input::getPointer(mouseX, mouseY);

	//pushVector.x = pushVector.y = 0.0f;
	t.transformation().create(MathUtil::Numeric::deg2Rad(r), (float)mouseX, (float)mouseY, 1.0f, 1.0f);
	t.setSize(100);
	t.update();

	c.transformation().create(MathUtil::Numeric::deg2Rad(r), 150.0f, 150.0f, 1.0f, 1.0f);
	c.setRadius(100);
	c.update();

	Vector2f pushVector;

	/*if (t.intersects(c, pushVector))
	{
		tr.create(MathUtil::Numeric::deg2Rad(r), mouseX + pushVector.x, mouseY + pushVector.y, 1.0f, 1.0f);
		t.setTransformation(tr);
		t.update();

		Context::push();
		Context::setFillColor(Color4(100, 0, 0, 128));
		Context::setFilled(true);
		c.draw();
		Context::pop();
	}
	else*/
	{
		c.draw();
	}

	t.draw();

	q.setSize(100, 100);
	q.transformation().create(MathUtil::Numeric::deg2Rad(r), 300.0f, 500.0f, 1.0f, 1.0f);
	q.update();
	q.draw();

	//{
	//	Vector2f p0, p1;
	//	GJK2d::distance(t, q, p0, p1);

	//	Vertex_Vector_XYZ_RGBA line1;
	//	line1.resize(2);
	//	line1[0].setPosition(p0);
	//	line1[0].setColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	line1[1].setPosition(p1);
	//	line1[1].setColor(1.0f, 1.0f, 1.0f, 1.0f);
	//	Index_Vector ind1;
	//	ind1.resize(2);
	//	ind1[0] = 0;
	//	ind1[1] = 1;
	//	gl.setDrawMode(GL_LINES);
	//	gl.draw_XYZ_RGBA(line1, ind1);
	//}
	

	//for (int i = 0; i < 100; ++i)
	//{
	//	sprite.setPosition(rand() % 1024, rand() % 1024);
	//	sprite.update(deltaMS);
	//	sprite.draw();
	//}

	float jx = 0.0f;
	float jy = 0.0f;
	if (Input::isKeyDown(Input::K_LEFT))
	{
		jx -= 1;
		mPlayer.move(Player::D_LEFT);
	}

	if (Input::isKeyDown(Input::K_RIGHT))
	{
		jx += 1;
		mPlayer.move(Player::D_RIGHT);
	}

	if (Input::isKeyDown(Input::K_UP))
	{
		jy -= 4;
	}

	mPlayer.update(deltaTimeMs);
	mPlayer.draw();

	std::stringstream strStream;

	strStream << "Draw calls: " << drawCalls;
	font.drawString(strStream.str(), 0, 50, BitmapFont::ALIGN_TL);

	if (Input::isButtonDown(Input::B_LEFT))
	{
		Vector2f A(static_cast<float>(mouseX), static_cast<float>(mouseY));
		Vector2f B(300, 500);
		B = A + ((B - A) * 1.5);

		Vector2f N(B - A);
		N.normalize();
		N.rotate(MathUtil::Numeric::deg2Rad(90));
		Vector2f P0 = B - (N * 150);

		for (int i = 0; i < 300; ++i)
		{
			Vector2f end(P0 + (N * (float)i));
			float L = (end - A).length();
			Vertex_Vector_XYZ_RGBA line;
			line.resize(2);
			line[0].setPosition(Vector2f(static_cast<float>(mouseX), static_cast<float>(mouseY)));
			line[0].setColor(1.0f, 1.0f, 1.0f, 1.0f);

			Vector2f intersection;
			q.intersect(Vector2f(static_cast<float>(mouseX), static_cast<float>(mouseY)), end, intersection);

			line[1].setPosition(intersection);
			float x = (L - (intersection - A).length());
			if (x < 0.01f) x = 0.0f;
			float col = x / L;
			line[1].setColor(col, col, col, 1.0f);

			Index_Vector ind;
			ind.resize(2);
			ind[0] = 0;
			ind[1] = 1;
			gl.setDrawMode(GL_LINES);
			gl.draw_XYZ_RGBA(line, ind);
		}
	}


	//mSandbox.update(deltaTimeMs);
	if (fabs(jx) > 0.0f || fabs(jy) > 0.0f)
	{
		mSandbox.jump(jx, jy);
	}
	mSandbox.draw();

	gl.endFrame();
	r += 0.5f;
}
