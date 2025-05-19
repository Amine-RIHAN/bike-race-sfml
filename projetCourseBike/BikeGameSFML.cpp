#include "BikeGameSFML.hpp"
#include <iostream>
#include <ctime>
#include <cstdlib>

// Wall implementation
sf::Texture Wall::sharedTexture;
bool Wall::textureLoaded = false;

Wall::Wall(float x, float y, float w, float h, const std::string& texturePath) {
    rect.setPosition(x, y);
    rect.setSize(sf::Vector2f(w, h));

    if (!textureLoaded) {
        if (!sharedTexture.loadFromFile(texturePath)) {
            rect.setFillColor(sf::Color(150, 150, 150));
            std::cerr << "Failed to load wall texture: " << texturePath << std::endl;
        } else {
            textureLoaded = true;
        }
    }

    if (textureLoaded) {
        rect.setTexture(&sharedTexture);
    }
}

void Wall::update(float speed) {
    rect.move(0, speed);
    if (rect.getPosition().y > WINDOW_HEIGHT) {
        float newX = ROAD_START_X + (rand() % static_cast<int>(ROAD_WIDTH - rect.getSize().x));
        rect.setPosition(newX, -100);
    }
}

void Wall::render(sf::RenderWindow& window) {
    window.draw(rect);
}

// Tree implementation
sf::Texture Tree::sharedTexture;
bool Tree::textureLoaded = false;

Tree::Tree(float x, float y, float spd) : speed(spd) {
    if (!textureLoaded) {
        if (!sharedTexture.loadFromFile("assets/arbre.png")) {
            std::cerr << "Failed to load tree texture!" << std::endl;
        } else {
            textureLoaded = true;
        }
    }
    sprite.setTexture(sharedTexture);
    sprite.setPosition(x, y);
    sprite.setScale(0.7f, 0.7f);
}

void Tree::update() {
    sprite.move(0, speed);
    if (sprite.getPosition().y > WINDOW_HEIGHT) {
        sprite.setPosition(sprite.getPosition().x, -200);
    }
}

void Tree::render(sf::RenderWindow& window) {
    window.draw(sprite);
}

// Bike implementation
Bike::Bike(float x, float y, float w, float h, const std::string& texturePath) {
    texture.loadFromFile(texturePath);
    rect.setPosition(x, y);
    rect.setSize(sf::Vector2f(w, h));
    rect.setTexture(&texture);
}

void Bike::handleInput() {
    const float speed = 8.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        rect.move(-speed, 0);
        if (rect.getPosition().x < ROAD_START_X)
            rect.setPosition(ROAD_START_X, rect.getPosition().y);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        rect.move(speed, 0);
        if (rect.getPosition().x + rect.getSize().x > ROAD_START_X + ROAD_WIDTH)
            rect.setPosition(ROAD_START_X + ROAD_WIDTH - rect.getSize().x, rect.getPosition().y);
    }
}

void Bike::render(sf::RenderWindow& window) {
    window.draw(rect);
}

// Token implementation
sf::Texture Token::sharedTexture;
bool Token::textureLoaded = false;

Token::Token(float x, float y, float w, float h) : width(w), height(h) {
    if (!textureLoaded) {
        if (!sharedTexture.loadFromFile("assets/score.png")) {
            std::cerr << "Failed to load token texture!" << std::endl;
        } else {
            textureLoaded = true;
        }
    }
    sprite.setTexture(sharedTexture);
    sprite.setPosition(x, y);
    sprite.setScale(w / sharedTexture.getSize().x, h / sharedTexture.getSize().y);
}

void Token::update(float speed) {
    if (!collected) {
        sprite.move(0, speed);
        if (sprite.getPosition().y > WINDOW_HEIGHT) {
            // Positionner le token hors de l'écran, puis le réinitialiser en haut
            float newX = ROAD_START_X + (rand() % static_cast<int>(ROAD_WIDTH - width));
            sprite.setPosition(newX, -100);
        }
    }
}


void Token::render(sf::RenderWindow& window) {
    if (!collected) {
        window.draw(sprite);
    }
}



// Button implementation
Button::Button(float x, float y, const std::string& text, const std::function<void()>& action, sf::Font& font)
    : onClickAction(action) {
    rect.setSize(sf::Vector2f(200, 50));
    rect.setPosition(x, y);
    rect.setFillColor(sf::Color::Green);

    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(x + 50, y + 10);
}

void Button::render(sf::RenderWindow& window) {
    window.draw(rect);
    window.draw(buttonText);
}

bool Button::isClicked(sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed &&
        rect.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
        onClickAction();
        return true;
    }
    return false;
}

// Game implementation
Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Bike Race"), bike(nullptr), token(nullptr),
               running(false), gameOver(false), score(0), scrollY(0), currentState(MENU) {
    srand(static_cast<unsigned>(time(nullptr)));
}

Game::~Game() { cleanup(); }

bool Game::init() {
    if (!menuBackgroundTexture.loadFromFile("assets/menu_background.png")) {
        std::cerr << "Failed to load menu background texture!" << std::endl;
        return false;
    }
    menuBackgroundSprite.setTexture(menuBackgroundTexture);

    // Charger la musique de fond
    if (!backgroundMusic.openFromFile("assets/moto_background.wav")) {
        std::cerr << "Erreur de chargement du son!" << std::endl;
        return false;
    }
    backgroundMusic.setLoop(true);  // Musique en boucle
    backgroundMusic.setVolume(100);  // Volume à 100

    if (!backgroundTexture.loadFromFile("assets/road_background.png")) {
        std::cerr << "Failed to load background texture!" << std::endl;
        return false;
    }
    backgroundSprite.setTexture(backgroundTexture);

    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return false;
    }

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    timeText.setFont(font);
    timeText.setCharacterSize(24);
    timeText.setFillColor(sf::Color::White);
    timeText.setPosition(10, 40);

    aboutText.setFont(font);
    aboutText.setCharacterSize(18);
    aboutText.setFillColor(sf::Color::White);
    aboutText.setPosition(50, 200);

    bike = new Bike(ROAD_START_X + (ROAD_WIDTH - 50) / 2, 500, 50, 80, "assets/bike.png");

    for (int i = 0; i < 3; i++) {
        float offsetX = (i % 2 == 0 ? -60.f : 60.f);  // alterne gauche/droite
        walls.emplace_back(
            ROAD_START_X + (ROAD_WIDTH - 50) / 2 + offsetX,
            -i * 200,
            50, 50,
            "assets/obstacle.png"
        );
    }

    token = new Token(ROAD_START_X + (ROAD_WIDTH - 30) / 2, -150, 30, 30);

    for (int i = 0; i < 5; i++) {
        leftTrees.emplace_back(10, -i * 450, 1.0f);
        rightTrees.emplace_back(620, -i * 200, 1.0f);
    }

    playButton = new Button(WINDOW_WIDTH / 2 - 100, 300, "Jouer", [this]() { currentState = PLAYING; resetGame(); backgroundMusic.play(); }, font);
    aboutButton = new Button(WINDOW_WIDTH / 2 - 100, 400, "A propos", [this]() { currentState = ABOUT; backgroundMusic.stop(); }, font);
    quitButton = new Button(WINDOW_WIDTH / 2 - 100, 500, "Quitter", [this]() { window.close(); }, font);
    retryButton = new Button(WINDOW_WIDTH / 2 - 100, 300, "Rejouer", [this]() { resetGame(); currentState = PLAYING; backgroundMusic.play(); }, font);
    backButton = new Button(WINDOW_WIDTH / 2 - 100, 400, "Retour", [this]() { currentState = MENU; backgroundMusic.play(); }, font);

    return true;
}

void Game::run() {
    while (window.isOpen()) {
        handleEvents();
        if (currentState == PLAYING && !gameOver) update();
        render();
    }
}

void Game::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }

        if (currentState == MENU) {
            playButton->isClicked(event);
            aboutButton->isClicked(event);
            quitButton->isClicked(event);
        } else if (currentState == ABOUT) {
            backButton->isClicked(event);
        } else if (currentState == GAME_OVER) {
            retryButton->isClicked(event);
            quitButton->isClicked(event);
            aboutButton->isClicked(event);
        }
    }
}

void Game::update() {
    if (gameOver) return;

    bike->handleInput();
    scrollY += 5;
    if (scrollY >= 600) scrollY = 0;

    score++;
    scoreText.setString("Score: " + std::to_string(score));

    elapsedTime = gameClock.getElapsedTime();
    int seconds = static_cast<int>(elapsedTime.asSeconds());
    timeText.setString("Temps: " + std::to_string(seconds));

    for (auto& wall : walls) {
        wall.update(0.5f);
        if (checkCollision(bike->rect, wall.rect)) {
            gameOver = true;
            currentState = GAME_OVER;
        }
    }

    token->update(0.5f);
    if (!token->collected && bike->rect.getGlobalBounds().intersects(token->sprite.getGlobalBounds())) {
        token->collected = true;  // Marquer le token comme collecté
    }

    // Réinitialiser le token une fois qu'il est collecté
    if (token->collected) {
        token->sprite.setPosition(ROAD_START_X + (ROAD_WIDTH - 30) / 2, -150);  // Nouvelle position
        token->collected = false;
    }

    for (auto& tree : leftTrees) tree.update();
    for (auto& tree : rightTrees) tree.update();
}


void Game::render() {
    window.clear();
    if (currentState == MENU) renderMenu();
    else if (currentState == PLAYING) {
        window.draw(backgroundSprite);
        bike->render(window);
        for (auto& wall : walls) wall.render(window);
        token->render(window);
        for (auto& tree : leftTrees) tree.render(window);
        for (auto& tree : rightTrees) tree.render(window);
        window.draw(scoreText);
        window.draw(timeText);
    }
    else if (currentState == GAME_OVER) renderGameOver();
    else if (currentState == ABOUT) renderAbout();
    window.display();
}

void Game::renderMenu() {
    window.draw(menuBackgroundSprite);
    playButton->render(window);
    aboutButton->render(window);
    quitButton->render(window);
}

void Game::renderGameOver() {
    window.draw(menuBackgroundSprite);
    window.draw(scoreText);
    window.draw(timeText);
    retryButton->render(window);
    quitButton->render(window);
    aboutButton->render(window);
}

void Game::renderAbout() {
    aboutText.setString("Ce jeu a été réalisé dans le cadre d'un projet du module POO en c++,\nsous l'encadrement de Mme R.HANNANE.\nCe projet a été développé par AMINE RIHAN et ADHAM EL WARARI.");
    window.draw(aboutText);
    backButton->render(window);
}

void Game::cleanup() {
    delete bike;
    delete token;
    delete playButton;
    delete aboutButton;
    delete quitButton;
    delete retryButton;
    delete backButton;
}




void Game::resetGame() {
    gameOver = false;
    walls.clear();
    leftTrees.clear();
    rightTrees.clear();
    token->sprite.setPosition(ROAD_START_X + (ROAD_WIDTH - 30) / 2, -150);
    token->sprite.setPosition(ROAD_START_X + (ROAD_WIDTH - 30) / 2, -150);
    token->collected = false;

    for (int i = 0; i < 3; i++) {
        float offsetX = (i % 2 == 0 ? -60.f : 60.f);  // alterne gauche/droite
        walls.emplace_back(
            ROAD_START_X + (ROAD_WIDTH - 50) / 2 + offsetX,
            -i * 200,
            50, 50,
            "assets/obstacle.png"
        );
    }

    for (int i = 0; i < 5; i++) {
        leftTrees.emplace_back(10, -i * 450, 1.0f);
        rightTrees.emplace_back(620, -i * 200, 1.0f);
    }
    score = 0;
    scrollY = 0;
    gameClock.restart();
}
bool Game::checkCollision(const sf::RectangleShape& rectShape, const sf::RectangleShape& otherShape) {
    return rectShape.getGlobalBounds().intersects(otherShape.getGlobalBounds());
}

