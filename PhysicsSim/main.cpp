#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <chrono>

int main()
{
    float mass1 = 1.f;
    float mass2 = 1.f;

	std::cout << "Enter the mass of the second cube: \n";
	std::cin >> mass2;

    sf::RenderWindow window(sf::VideoMode({ 1280, 720 }), "Colliding Blocks");
    window.setFramerateLimit(500);

    sf::RectangleShape rect1; //yellow
    sf::RectangleShape rect2; //blue

    float wpos_x = window.getSize().x / 2.f;
    float wpos_y = window.getSize().y / 2.f;

    rect1.setSize(sf::Vector2f(50, 50));
    rect1.setOutlineColor(sf::Color(205, 170, 0));
    rect1.setOutlineThickness(5.f);
    rect1.setFillColor(sf::Color::Yellow);
    rect1.setPosition({ wpos_x - 60.f, wpos_y + 80.f });

    rect2.setSize(sf::Vector2f(50, 50));
    rect2.setOutlineColor(sf::Color(0, 0, 139));
    rect2.setOutlineThickness(5.f);
    rect2.setFillColor(sf::Color::Blue);
    rect2.setPosition({wpos_x + 200.f, wpos_y + 80.f});

    sf::RectangleShape border_x;
    sf::RectangleShape border_y;

    border_y.setSize(sf::Vector2f(5, 340));
    border_y.setOutlineColor(sf::Color::White);
    border_y.setOutlineThickness(1.f);
    border_y.setPosition({ wpos_x - 400.f, wpos_y - 200.f });

    border_x.setSize(sf::Vector2f(1280, 5));
    border_x.setOutlineColor(sf::Color::White);
    border_x.setOutlineThickness(1.f);
    border_x.setPosition({ wpos_x - 400.f, wpos_y + 136.f });

    sf::Clock clock;
    float speed2 = 100.f;
    float speed1 = 0.f;

    sf::Font font;
    if (!font.openFromFile("opensans.ttf"))
    {
        std::cerr << "Error loading font\n";
        return -1;
    }

	int collCounter = 0;

    sf::Text actualCounterText(font);
    actualCounterText.setString(std::to_string(collCounter));
    actualCounterText.setCharacterSize(48);
    actualCounterText.setFillColor(sf::Color::White);
    actualCounterText.setPosition(sf::Vector2f(wpos_x - 60.f, wpos_y + 250.f));

    sf::Text collCounterText(font);
    collCounterText.setString("Collisions: ");
    collCounterText.setCharacterSize(48);
    collCounterText.setFillColor(sf::Color::White);
    collCounterText.setPosition(sf::Vector2f(wpos_x - 300.f, wpos_y + 250.f));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float deltaTime = clock.restart().asSeconds();
        float movement1 = speed1 * deltaTime;
        float movement2 = speed2 * deltaTime;
        const float velocity1 = -movement1;
        const float velocity2 = -movement2;

        sf::Vector2f previousPos1 = rect1.getPosition();
        sf::Vector2f previousPos2 = rect2.getPosition();
        rect2.move(sf::Vector2f(velocity2, 0.f));
        rect1.move(sf::Vector2f(velocity1, 0.f));

        sf::FloatRect rect2Bounds = rect2.getGlobalBounds();
        sf::FloatRect rect1Bounds = rect1.getGlobalBounds();
        sf::FloatRect wallBounds = border_y.getGlobalBounds();

        if (rect1Bounds.findIntersection(wallBounds).has_value())
        {
            rect1.setPosition(previousPos1);
            //rect2.move(sf::Vector2f(velocity, 0.f));
            speed1 = -speed1;
			collCounter++;
            actualCounterText.setString(std::to_string(collCounter));
        }
        if (rect2Bounds.findIntersection(rect1Bounds).has_value())
        {
            rect1.setPosition(previousPos1);
            rect2.setPosition(previousPos2);
            
            float newSpeed1 = (speed1 * (mass1 - mass2) + 2 * mass2 * speed2) / (mass1 + mass2);
            float newSpeed2 = (speed2 * (mass2 - mass1) + 2 * mass1 * speed1) / (mass1 + mass2);

            speed1 = newSpeed1;
            speed2 = newSpeed2;

			collCounter++;
            actualCounterText.setString(std::to_string(collCounter));
        }
        
        window.clear();
        window.draw(rect1);
        window.draw(rect2);
        window.draw(border_x);
        window.draw(border_y);
        window.draw(collCounterText);
        window.draw(actualCounterText);
        window.display();
    }
}