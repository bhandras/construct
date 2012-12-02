#include <sstream>

#include "construct.h"
#include "game.h"


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
	mAtlas = new TextureAtlas("player.xml");
	mAtlas->load();

	mPlayer.setAtlas(mAtlas);
	
	mPlayer.addKeyFrameImage("stand", "stand0");
	mPlayer.addKeyFrameImage("stand", "stand1");
	mPlayer.addKeyFrameImage("stand", "stand2");
	mPlayer.addKeyFrameImage("stand", "stand3");
	mPlayer.addKeyFrameImage("stand", "stand4");
	
	mPlayer.addKeyFrameImage("walk", "walk0");
	mPlayer.addKeyFrameImage("walk", "walk1");
	mPlayer.addKeyFrameImage("walk", "walk2");
	mPlayer.addKeyFrameImage("walk", "walk3");
	mPlayer.addKeyFrameImage("walk", "walk4");

	mPlayer.addKeyFrameImage("jump", "jump0");
	mPlayer.addKeyFrameImage("jump", "jump1");
	mPlayer.addKeyFrameImage("jump", "jump2");

	mPlayer.selectAnimation("walk");

	mPlayer.setFPS(5);
	mPlayer.setScale(10, 10);
	mPlayer.setLooped(true);

	Context::push();
	Context::setFillColor(Color4(0, 100, 0, 255));
	Context::setOutlineColor(Color4(0, 255, 0, 255));
	Context::setFilled(true);
}




void Game::step(unsigned mouseX, unsigned mouseY, bool mouseBtn1, bool mouseBtn2, unsigned deltaTimeMs)
{
	GL_Render& gl = GL_Render::get();
	int drawCalls = gl.getNumDrawCalls();

	gl.beginFrame();
	gl.setOrthoProjection(0.0f, static_cast<float>(mW), static_cast<float>(mH), 0.0f, 1.0f, -1.0f);


	mWorld.draw();

	Affine2df tr;
	tr.create(MathUtil::Numeric::deg2Rad(r), mouseX, mouseY, 1.0f, 1.0f);
	//pushVector.x = pushVector.y = 0.0f;
	t.setTransformation(tr);
	t.setSize(100);
	t.update();


	tr.create(MathUtil::Numeric::deg2Rad(r), 150, 150, 1.0f, 1.0f);
	c.setTransformation(tr);
	c.setRadius(100);
	c.update();

	Vector2f pushVector;

	if (t.intersects(c, pushVector))
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
	else
	{
		c.draw();
	}

	t.draw();

	q.setSize(100, 100);
	tr.create(MathUtil::Numeric::deg2Rad(r), 300, 500, 1.0f, 1.0f);

	q.setTransformation(tr);
	q.update();
	q.draw();


	//for (int i = 0; i < 100; ++i)
	//{
	//	sprite.setPosition(rand() % 1024, rand() % 1024);
	//	sprite.update(deltaMS);
	//	sprite.draw();
	//}

	mPlayer.setPosition(150, 150);
	mPlayer.update(deltaTimeMs);
	mPlayer.draw();

	std::stringstream strStream;

	strStream << "Draw calls: " << drawCalls;
	mFont.drawString(strStream.str(), 0, 50, BitmapFont::ALIGN_TL);

	if (mouseBtn1)
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
			Vector2f end(P0 + (N * i));
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
