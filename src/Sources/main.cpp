#include <SFML/Graphics.hpp>
using namespace sf;

#include <math.h>

constexpr auto pi = 3.1415926536;

RectangleShape player;
Vector2f pos, delta;
float angle;

Vector2i mapSize(8, 8);
int blockSize = 64;
int map[] =
{
	1,1,1,1,1,1,1,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,0,0,0,0,1,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1
};

void DrawPlayer(RenderWindow& app)
{
	player.setFillColor(Color::Yellow);
	player.setSize(Vector2f(8.f, 8.f));
	player.setPosition(pos);
	app.draw(player);

	Vertex line[] =
	{
		Vertex(Vector2f(pos.x + 3.75, pos.y + 3.75)),
		Vertex(Vector2f(pos.x + delta.x * 5 + 3.75, pos.y + delta.y * 5 + 3.75))
	};

	for (int i = 0; i < 2; i++) line[i].color = Color::Yellow;
	app.draw(line, 2, Lines);

}

void DrawMap2D(RenderWindow& app)
{
	Vector2i offset;
	RectangleShape block(Vector2f(blockSize, blockSize));

	for (int y = 0; y < mapSize.y; y++)
		for (int x = 0; x < mapSize.x; x++)
		{
			offset.x = x * blockSize; offset.y = y * blockSize;
			Vertex walls[] =
			{
				Vertex(Vector2f(offset.x + 1, offset.y + 1)),
				Vertex(Vector2f(offset.x + 1, offset.y + blockSize - 1)),
				Vertex(Vector2f(offset.x + blockSize - 1, offset.y + blockSize - 1)),
				Vertex(Vector2f(offset.x + blockSize - 1, offset.y + 1)),
			};

			if (map[y * mapSize.x + x] == 1) walls->color = Color::White;
			else { for (int i = 0; i < 4; i++) walls[i].color = Color::Black; }
			app.draw(walls, 4, PrimitiveType::Quads);

		}
}


void Keys()
{
	if (Keyboard::isKeyPressed(Keyboard::Z))
	{
		pos.x += delta.x;
		pos.y += delta.y;
	}
	if (Keyboard::isKeyPressed(Keyboard::Q))
	{
		angle -= 0.1;
		if (angle < 0) angle += 2 * pi;
		delta.x = cos(angle) * 5; delta.y = sin(angle) * 5;
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		pos.x -= delta.x;
		pos.y -= delta.y;
	}
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		angle += 0.1;
		if (angle < 2 * pi) angle -= 2 * pi;
		delta.x = cos(angle) * 5; delta.y = sin(angle) * 5;
	}
}

void DrawRays3D(RenderWindow& app)
{
	int mx, my, mp, dof; float rayAngle;
	Vector2f ray, offset;

	for (int r = 0; r < 1; r++)
	{
		dof = 0;
		float aTan = -1 / tan(rayAngle);

		if (rayAngle > pi)
		{
			ray.y = (((int)pos.y >> 6) << 6) - 0.0001;
			ray.x = (pos.y - ray.y) * aTan + pos.x;
			offset.y = -64; offset.x = -offset.y * aTan;
		}
		if (rayAngle < pi)
		{
			ray.y = (((int)pos.y >> 6) << 6) + 64;
			ray.x = (pos.y - ray.y) * aTan + pos.x;
			offset.y = 64; offset.x = -offset.y * aTan;
		}
		if (rayAngle == pi || rayAngle == 0)
		{
			ray.y = pos.y;
			ray.x = pos.x;
			dof = 8;
		}

		while (dof < 8)
		{
			mx = (int)(ray.x) >> 6; my = (int)(ray.y) >> 6; mp = my * mapSize.x + mx;
			if (mp < mapSize.x * mapSize.y && map[mp] == 1) dof = 8;
			else { ray.x += offset.x; ray.y += offset.y; dof++; }
		}

		Vertex rays[] =
		{
			Vertex(pos),
			Vertex(ray)
		};

		rays[0].color = Color::Green;
		rays[1].color = Color::Green;
		app.draw(rays, 2, Lines);
	}
}

void Display(RenderWindow& app)
{
	app.clear(Color(76.5, 76.5, 76.5, 255));

	DrawMap2D(app);
	DrawPlayer(app);
	//DrawRays3D(app);//
	Keys();
	
	app.display();
}

int main()
{
	RenderWindow app(VideoMode(1024, 512), "RayEngine");
	app.setFramerateLimit(30);
	
	pos.x = 300, pos.y = 300;
	delta.x = cos(angle) * 5; delta.y = sin(angle) * 5;

	while (app.isOpen())
	{
		Event e;
		while (app.pollEvent(e))
		{
			switch (e.type)
			{
				case Event::Closed:
					app.close();
					break;
				
				case Event::KeyReleased:
					if (e.key.code == Keyboard::LAlt + Keyboard::F4)
						app.close();
			}
		}

		Display(app);
	}

	return 0;
}