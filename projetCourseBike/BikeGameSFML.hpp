#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <functional>

const float WINDOW_WIDTH = 700.0f;
const float WINDOW_HEIGHT = 1000.0f;
const float ROAD_START_X = WINDOW_WIDTH / 3.0f;
const float ROAD_WIDTH = WINDOW_WIDTH / 3.0f;

enum GameState { MENU, PLAYING, GAME_OVER, ABOUT };

class Wall {
public:
    sf::RectangleShape rect;
    static sf::Texture sharedTexture;
    static bool textureLoaded;

    Wall(float x, float y, float w, float h, const std::string& texturePath);
    void update(float speed);
    void render(sf::RenderWindow& window);
};

class Bike {
public:
    sf::RectangleShape rect;
    sf::Texture texture;
    Bike(float x, float y, float w, float h, const std::string& texturePath);
    void handleInput();
    void render(sf::RenderWindow& window);
};

class Token {
public:
    sf::Sprite sprite;
    static sf::Texture sharedTexture;
    static bool textureLoaded;
    float width, height;
    bool collected = false;

    Token(float x, float y, float w, float h);
    void update(float speed);
    void render(sf::RenderWindow& window);
};



class Tree {
public:
    sf::Sprite sprite;
    static sf::Texture sharedTexture;
    static bool textureLoaded;
    float speed;

    Tree(float x, float y, float spd);
    void update();
    void render(sf::RenderWindow& window);
};

class Button {
public:
    Button(float x, float y, const std::string& text, const std::function<void()>& action, sf::Font& font);
    void render(sf::RenderWindow& window);
    bool isClicked(sf::Event& event);

private:
    sf::RectangleShape rect;
    sf::Text buttonText;
    std::function<void()> onClickAction;
};

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();

private:
    sf::Music backgroundMusic;
    sf::RenderWindow window;
    sf::Texture backgroundTexture, menuBackgroundTexture;
    sf::Sprite backgroundSprite, menuBackgroundSprite;
    sf::Font font;
    sf::Text scoreText, timeText, aboutText;
    Bike* bike;
    Token* token;
    std::vector<Wall> walls;
    std::vector<Tree> leftTrees, rightTrees;
    bool running;
    bool gameOver;
    int score;
    float scrollY = 0;
    GameState currentState;

    Button* playButton;
    Button* aboutButton;
    Button* quitButton;
    Button* retryButton;
    Button* backButton;

    sf::Clock gameClock;
    sf::Time elapsedTime;

    void handleEvents();
    void update();
    void render();
    void cleanup();
    void resetGame();
    void renderMenu();
    void renderGameOver();
    void renderAbout();

    bool checkCollision(const sf::RectangleShape& rectShape, const sf::RectangleShape& otherShape);
    bool checkCollision(const sf::RectangleShape& rectShape, const sf::Sprite& sprite);

};
