#include <sstream>

#include "construct.h"
#include "game.h"

using namespace Construct;

float r = 0.0f;
Triangle t;
Quad q;
Circle c;


Game::Game()
: mAtlas(0)
{

}


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
}




void Game::step(unsigned deltaTimeMs)
{
	GL_Render& gl = GL_Render::get();
	int drawCalls = gl.getNumDrawCalls();

	gl.beginFrame();
	gl.setOrthoProjection(0.0f, static_cast<float>(mW), static_cast<float>(mH), 0.0f, 1.0f, -1.0f);


	mWorld.draw();

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


	//for (int i = 0; i < 100; ++i)
	//{
	//	sprite.setPosition(rand() % 1024, rand() % 1024);
	//	sprite.update(deltaMS);
	//	sprite.draw();
	//}

	if (Input::isKeyDown(Input::K_LEFT))
	{
		mPlayer.move(Player::D_LEFT);
	}

	if (Input::isKeyDown(Input::K_RIGHT))
	{
		mPlayer.move(Player::D_RIGHT);
	}

	mPlayer.update(deltaTimeMs);
	mPlayer.draw();

	std::stringstream strStream;

	strStream << "Draw calls: " << drawCalls;
	mFont.drawString(strStream.str(), 0, 50, BitmapFont::ALIGN_TL);

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
			q.intersect(Vector2f(mouseX, mouseY), end, intersection);

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

	gl.endFrame();
	r += 0.5f;
}
