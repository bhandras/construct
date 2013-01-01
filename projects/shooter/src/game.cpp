#include <sstream>

#include "construct.h"
#include "game.h"

using namespace Construct;


Game::Game()
: mAtlas(0)
, mAccumlator(0.0f)
, mTimeStep(1000.0f / 120.0f)
, mFPS(0.0f)
{ }


void Game::init(unsigned w, unsigned h)
{
	mW = w;
	mH = h;

	GL_Render::get().init();
	mFont.load("vector_skin_font.bff");

	mPlayer.init();
	mPlayer.setPosition(150, 150);

	Context::push();
	Context::setFillColor(Color4(0, 100, 0, 255));
	Context::setOutlineColor(Color4(0, 255, 0, 255));
	Context::setFilled(true);
	Context::setOutlined(true);

	// pysics init
	mSpace.setGravity(Vector2f(0.0f, 10.0f));
	mGround.setSize(500, 50);

	Body* b = mSpace.addBody(Body::StaticBody);
	b->setShape(&mGround);
	b->setPosition(512, 700);
	b->setElasticity(0.3f);
	b->setFriction(0.5f);
	mBodies.push_back(b);

	mBoxes[0].setSize(50, 50);
	//mBox.transformation().create(MathUtil::Numeric::deg2Rad(45.0f), 0, 0, 1.0f, 1.0f);
	b = mSpace.addBody(Body::DynamicBody);
	b->setMass(1.0f);
	b->setFriction(0.1f);
	b->setShape(&mBoxes[0]);
	b->setPosition(512, 200);
	b->setContactCallback(createFunctor(this, &Game::playerContact));
	mBodies.push_back(b);


	mBoxes[1].setSize(50, 50);
	b = mSpace.addBody(Body::DynamicBody);
	b->setShape(&mBoxes[1]);
	b->setMass(1.0f);
	b->setPosition(512, 300);
	mBodies.push_back(b);


	mBoxes[2].setSize(50, 50);
	b = mSpace.addBody(Body::DynamicBody);
	b->setShape(&mBoxes[2]);
	b->setMass(1.0f);
	b->setPosition(512, 400);
	mBodies.push_back(b);

	mBoxes[3].setSize(50, 50);
	b = mSpace.addBody(Body::DynamicBody);
	b->setShape(&mBoxes[3]);
	b->setMass(1.0f);
	b->setPosition(512, 500);
	mBodies.push_back(b);


	mWalls[0].setSize(50, 100);
	b = mSpace.addBody(Body::StaticBody);
	b->setShape(&mWalls[0]);
	b->setPosition(700, 500);

	mWalls[1].setSize(100, 30);
	b = mSpace.addBody(Body::StaticBody);
	b->setShape(&mWalls[1]);
	b->setFriction(0.3f);
	b->setPosition(300, 500);

	mCircle.setRadius(50);
	b = mSpace.addBody(Body::StaticBody);
	b->setShape(&mCircle);
	b->setFriction(0.3f);
	b->setPosition(200, 600);

	mOnTheGround = false;


	Quad q1;
	q1.setSize(100, 100);
	q1.transformation().create(0, 100, 100, 1, 1);
	q1.update();

	Quad q2;
	q2.setSize(100, 100);
	q2.transformation().create(0, 200, 200, 1, 1);
	q2.update();

	Vector2f w1, w2, n1, n2;
	GJK2d::distance(q1, q2, w1, n1, w2, n2);
	int alma = 0;
}


void Game::jump(float x, float y)
{
	Vector2f velocity = mBodies[1]->getVelocity();

	if (mOnTheGround)
	{
		velocity.y += y*100.0f;
	}

	velocity.x += x*100.0f;

	if (velocity.x < -100)
	{
		velocity.x = -100;
	}

	if (velocity.x > 100)
	{
		velocity.x = 100;
	}

	mBodies[1]->setVelocity(velocity);
}


void Game::playerContact(Construct::Body* body, const Construct::Vector2f& pushVector)
{
	if (pushVector.dot(Vector2f(0, -1)) > 0.0f)
	{
		mOnTheGround = true;
	}
}

int frames = 0;
int numUpdates = 0;
void Game::step(float deltaTimeMs)
{
	++frames;
	
	mAccumlator += deltaTimeMs;
	while (mAccumlator >= mTimeStep)
	{
		mOnTheGround = false;
		mSpace.update(mTimeStep / 1000.0f);
		mAccumlator -= mTimeStep;

		++numUpdates;
	}


	

	GL_Render& gl = GL_Render::get();
	int drawCalls = gl.getNumDrawCalls();

	gl.beginFrame();
	gl.setOrthoProjection(0.0f, static_cast<float>(mW), static_cast<float>(mH), 0.0f, 1.0f, -1.0f);

	int mouseX, mouseY;
	Input::getPointer(mouseX, mouseY);

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
	mFont.drawString(strStream.str(), 0, 50, BitmapFont::ALIGN_TL);

	std::stringstream strStream2;
	mFPS += (1000.0f / (deltaTimeMs + 1.0f));
	strStream2 << "FPS: " << (int)mFPS / frames << "   updates: " << numUpdates/frames;
	mFont.drawString(strStream2.str(), 0, 20, BitmapFont::ALIGN_TL);

	//mSandbox.update(deltaTimeMs);
	if (fabs(jx) > 0.0f || fabs(jy) > 0.0f)
	{
		jump(jx, jy);
	}
	
	Context::push();
	Context::setFillColor(Color4(0, 170, 0, 255));
	Context::setOutlineColor(Color4(0, 255, 0, 255));
	Context::setFilled(true);
	Context::setOutlined(true);

	mGround.draw();
	mBoxes[0].draw();
	mBoxes[1].draw();
	mBoxes[2].draw();
	mBoxes[3].draw();
	mWalls[0].draw();
	mWalls[1].draw();
	mCircle.draw();
	Context::pop();
	mSpace.debug();

	gl.endFrame();
}



void Game::testRayCast(float mouseX, float mouseY)
{
	GL_Render& gl = GL_Render::get();

	Construct::Quad quad;

	quad.setSize(100, 100);
	quad.transformation().create(MathUtil::Numeric::deg2Rad(45.0f), 300.0f, 500.0f, 1.0f, 1.0f);
	quad.update();
	quad.draw();

	Construct::Triangle triangle;
	triangle.transformation().create(0.0f, mouseX, mouseY, 1.0f, 1.0f);
	triangle.setSize(100);
	triangle.update();
	triangle.draw();


	if (Input::isButtonDown(Input::B_LEFT))
	{
		Vector2f A(mouseX, mouseY);
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
			line[0].setPosition(Vector2f(mouseX, mouseY));
			line[0].setColor(1.0f, 1.0f, 1.0f, 1.0f);

			Vector2f intersection;
			quad.intersect(Vector2f(mouseX, mouseY), end, intersection);

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
}
