#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

class Rocket {
private:
    sf::Sprite spriteRocket;
    sf::Texture texture;
    sf::Image image;
public:
    Rocket() {
        if (!image.loadFromFile("images/rocket.jpg")) {
            std::cerr << "Ошибка загрузки текстуры астероида!" << std::endl;
            exit(1);
        }
        image.createMaskFromColor(sf::Color(0, 0, 0));
        texture.loadFromImage(image);
        spriteRocket.setTexture(texture);
        spriteRocket.setScale(0.25f, 0.25f);
        spriteRocket.setRotation(90);
    }
    sf::Sprite& getSpriteRocket() {
        return spriteRocket;
    }
};

class Asteroid {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Image image;
public:
    Asteroid() {
        if (!image.loadFromFile("images/asteroid.png")) {
            std::cerr << "Ошибка загрузки текстуры астероида!" << std::endl;
            exit(1);
        }
        this->image = image;
        image.createMaskFromColor(sf::Color(13, 17, 132));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setScale(0.25f, 0.25f);
    }
    sf::Sprite& getSprite() {
        return sprite;
    }
};

class Star {
private:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Image image;
public:
    Star() {
        if (!image.loadFromFile("images/star.png")) {
            std::cerr << "Ошибка загрузки текстуры астероида!" << std::endl;
            exit(1);
        }
        this->image = image;
        image.createMaskFromColor(sf::Color(0,0,0));
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setScale(0.99f, 0.99f);
    }
    sf::Sprite& getSprite() {
        return sprite;
    }
};

bool intersectsAny(const sf::Sprite& sprite, const std::vector<sf::Sprite>& others) {
    for (const auto& other : others) {
        if (sprite.getGlobalBounds().intersects(other.getGlobalBounds())) {
            return true;
        }
    }
    return false;
}

int main() {
    setlocale(LC_ALL, "RUS");
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML Example");
    srand(static_cast<unsigned>(time(nullptr)));

    // Создание ракеты
    Rocket rocket;
    sf::Sprite spriteRocket;
    bool startRocket = false;
    std::vector<sf::Sprite> rockets;

    // Загрузка корабля
    sf::Texture heroTexture;
    if (!heroTexture.loadFromFile("images/starship.png")) {
        std::cerr << "Ошибка загрузки текстуры корабля!" << std::endl;
        return -1;
    }

    sf::Sprite heroSprite;
    heroSprite.setTexture(heroTexture);
    heroSprite.setScale(0.25f, 0.25f);
    heroSprite.setRotation(90);
    heroSprite.setPosition(250.f, 250.f);

    // Загрузка фона
    sf::Texture spaceTexture;
    if (!spaceTexture.loadFromFile("images/galaxy.png")) {
        std::cerr << "Ошибка загрузки текстуры фона!" << std::endl;
        return -1;
    }

    sf::RectangleShape background1(sf::Vector2f(1920, 1080));
    background1.setTexture(&spaceTexture);
    background1.setPosition(0.f, 0.f);
    sf::RectangleShape background2(sf::Vector2f(1920, 1080));
    background2.setTexture(&spaceTexture);
    background2.setPosition(1920.f, 0.f);
    
    // Астероиды
    std::vector<sf::Sprite> stars(5);
    Star star;
    for (int i = 0; i < 5; i++) {
        stars[i] = star.getSprite();
        stars[i].setPosition(1920.f + i * 400.f, static_cast<float>(rand() % 800 + 100)); // Начальная позиция астероидов
    }

    std::vector<sf::Sprite> asteroids(3); // Список для астероидов
    Asteroid asteroid;
    for (int i = 0; i < 3; i++) {
        sf::Sprite s = asteroid.getSprite();
        bool safePosition = false;
        while (!safePosition) {
            s.setPosition(1920.f + i * 400.f, static_cast<float>(rand() % 800 + 100));

            // Проверка, что астероид не пересекается с любой звездой
            if (!intersectsAny(s, stars)) {
                safePosition = true; // Если не пересекается, это безопасная позиция
            }
        }
        asteroids[i] = s; // Добавление безопасного астероида в вектор
    }

    sf::Clock clock;
    sf::Font font;
   
    // Загрузка шрифта
    if (!font.loadFromFile("C:\\Windows\\Fonts\\Arial.ttf")) {
        std::cerr << "Ошибка загрузки шрифта!" << std::endl;
        return -1;
    }
    
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f);

    int score = 0;
    bool gameOver = false;
    bool inMenu = true;
    bool ignoreNextClick = true;

    sf::Text diedButton;
    diedButton.setFont(font);
    diedButton.setCharacterSize(100);
    diedButton.setFillColor(sf::Color::Blue);
    diedButton.setString("You died!");
    diedButton.setPosition(730.f, 300.f);

    sf::Text playButton;
    playButton.setFont(font);
    playButton.setCharacterSize(60);
    playButton.setFillColor(sf::Color::Green);
    playButton.setString("Play");
    playButton.setPosition(850.f, 400.f);
    
    sf::Text restartButton;
    restartButton.setFont(font);
    restartButton.setCharacterSize(50);
    restartButton.setFillColor(sf::Color::Green);
    restartButton.setString("Restart?");
    restartButton.setPosition(850.f, 400.f);

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setString("SPACE SURVIVOR");
    titleText.setPosition(600.f, 200.f);

    sf::Text exitButton;
    exitButton.setFont(font);
    exitButton.setCharacterSize(50);
    exitButton.setFillColor(sf::Color::Red);
    exitButton.setString("Exit?");
    exitButton.setPosition(850.f, 500.f);

    sf::Text exitMenuButton;
    exitMenuButton.setFont(font);
    exitMenuButton.setCharacterSize(60);
    exitMenuButton.setFillColor(sf::Color::Red);
    exitMenuButton.setString("Exit");
    exitMenuButton.setPosition(850.f, 500.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (ignoreNextClick) {
                if (inMenu && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (playButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        inMenu = false;
                        clock.restart();
                        ignoreNextClick = false;
                    }
                    else if (exitMenuButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        window.close();
                    }
                }
            }

            else {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                    if (rockets.size() < 3) {
                        spriteRocket = rocket.getSpriteRocket();
                        spriteRocket.setPosition(heroSprite.getPosition().x, heroSprite.getPosition().y + 80.f);
                        
                        rockets.push_back(spriteRocket);
                    }
                }
            }
        }
        window.clear();
        
        if (inMenu) {
            window.draw(titleText);
            window.draw(playButton);
            window.draw(exitMenuButton);
            window.display();
            continue;
        }

        else {
            float time = clock.restart().asSeconds();
            if (!gameOver) {
                
                // Движение корабля
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                    heroSprite.move(0.f, -700.f * time);
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    heroSprite.move(0.f, 700.f * time);
                }
                
                // Движение ракеты
                for (int i = 0; i < rockets.size(); i++) {
                    rockets[i].move(200 * time, 0);
                    
                    // Если ракета вышла за экран — удаляем
                    if (rockets[i].getPosition().x > 1920) {
                        rockets.erase(rockets.begin() + i);
                        i--;
                    }
                }

                for (int i = 0; i < asteroids.size(); i++) {
                    for (int j = 0; j < rockets.size(); j++) {
                        rockets[j].move(200.f * time, 0);

                        if (asteroids[i].getGlobalBounds().intersects(rockets[j].getGlobalBounds())) {
                            asteroids[i].setPosition(1920.f, static_cast<float>(rand() % 800 + 100));
                            rockets.erase(rockets.begin() + j);
                            j--;
                        }
                    }
                }
                
                // Движение фона
                background1.move(-300.f * time, 0.f);
                background2.move(-300.f * time, 0.f);

                if (background1.getPosition().x <= -1920) {
                    background1.setPosition(background2.getPosition().x + 1920, 0.f);
                }
                if (background2.getPosition().x <= -1920) {
                    background2.setPosition(background1.getPosition().x + 1920, 0.f);
                }
                
                // Движение астероидов
                for (int i = 0; i < 5; i++) {
                    stars[i].move(-200.f * time, 0);

                    if (stars[i].getPosition().x <= -100) {
                        stars[i].setPosition(1920.f, static_cast<float>(rand() % 800 + 100)); // Возврат астероида на правую сторону экрана
                    }

                    if (heroSprite.getGlobalBounds().intersects(stars[i].getGlobalBounds())) {
                        score += 10;
                        stars[i].setPosition(1920.f, static_cast<float>(rand() % 800 + 100));
                    }
                }

                // Движение комет
                for (int i = 0; i < 3; i++) {
                    asteroids[i].move(-200.f * time, 0);

                    if (asteroids[i].getPosition().x <= -100) {
                        asteroids[i].setPosition(1920.f, static_cast<float>(rand() % 800 + 100)); // Возврат астероида на правую сторону экрана
                    }

                    if (heroSprite.getGlobalBounds().intersects(asteroids[i].getGlobalBounds())) {
                        gameOver = true;
                    }
                }
            }

            // Обновление текста с очками
            window.clear();
            scoreText.setString("Scores: " + std::to_string(score));
            window.draw(background1);
            window.draw(background2);
            window.draw(heroSprite);

            // Отрисовка текста
            window.draw(scoreText);

            // Рисуем все ракеты из вектора
            for (auto& rocketSprite : rockets) {
                window.draw(rocketSprite);
            }

            for (const auto& star : stars) {
                window.draw(star);
            }
                
            for (const auto& asteroid : asteroids) {
                window.draw(asteroid);
            }
                
            for (int i = 0; i < rockets.size(); i++) {
                sf::FloatRect rocketBounds = rockets[i].getGlobalBounds();
                rocketBounds.left += 5;
                rocketBounds.top += 5;
                rocketBounds.width -= 10;
                rocketBounds.height -= 10;

                sf::RectangleShape debugRocket;
                debugRocket.setPosition(rocketBounds.left, rocketBounds.top);
                debugRocket.setSize(sf::Vector2f(rocketBounds.width, rocketBounds.height));
                debugRocket.setOutlineColor(sf::Color::Red);
                debugRocket.setOutlineThickness(1);
                debugRocket.setFillColor(sf::Color::Transparent);
                window.draw(debugRocket);
            }

            for (int i = 0; i < asteroids.size(); i++) {
                sf::FloatRect asteroidBounds = asteroids[i].getGlobalBounds();
                asteroidBounds.left += 5;
                asteroidBounds.top += 5;
                asteroidBounds.width -= 10;
                asteroidBounds.height -= 10;

                sf::RectangleShape debugAsteroid;
                debugAsteroid.setPosition(asteroidBounds.left, asteroidBounds.top);
                debugAsteroid.setSize(sf::Vector2f(asteroidBounds.width, asteroidBounds.height));
                debugAsteroid.setOutlineColor(sf::Color::Green);
                debugAsteroid.setOutlineThickness(1);
                debugAsteroid.setFillColor(sf::Color::Transparent);
                window.draw(debugAsteroid);
            }

            if (gameOver) {
                window.draw(diedButton);
                window.draw(restartButton);
                window.draw(exitButton);
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (restartButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        gameOver = false;
                        score = 0;
                        heroSprite.setPosition(250.f, 250.f);
                        rockets.clear();

                        for (int i = 0; i < 3; i++) {
                            asteroids[i].setPosition(1920.f + i * 400.f, static_cast<float>(rand() % 800 + 100));
                        }
                        for (int i = 0; i < 5; i++) {
                            stars[i].setPosition(1920.f + i * 400.f, static_cast<float>(rand() % 800 + 100));
                        }
                    }

                    else if (exitButton.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                        window.close();
                    }
                }
            }
            window.display();
        }
    }
    return 0;
}


