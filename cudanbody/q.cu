#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
using namespace sf;

int main()
{
	RenderWindow window(VideoMode(500, 500), "");
	window.setFramerateLimit(60);
	CircleShape circle(20, 250);
	circle.setFillColor(Color::White);
	Event event;
	double dt = 0.01;
	while (window.isOpen())
	{
		window.clear(Color::Black);
		circle.move(1, 1);
		window.draw(circle);
		window.display();
	}
	return 0;
}
