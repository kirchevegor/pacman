#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>

using namespace sf;

const int H = 21;  // Высота карты
const int W = 19;  // Ширина карты
const int ts = 25; // Размер тайла

int q = 0;           // Количество собранных очков
bool life = true;    // Состояние пакмена (жив или мёртв)
bool isPaused = false; // Состояние паузы

// Карта игры
String TileMap[H] = {
    "AAAAAAAAAAAAAAAAAAA",
    "A1       A       2A",
    "A AA AAA A AAA AA A",
    "A        A        A",
    "A AA A AAAAA A AA A",
    "A    A   A   A    A",
    "AAAA AAA A AAA AAAA",
    "BBBA A       A ABBB",
    "AAAA A AAAAA A AAAA",
    "BBBB   ABBBA   BBBB",
    "AAAA A AAAAA A AAAA",
    "BBBA A       A ABBB",
    "AAAA A AAAAA A AAAA",
    "A        A        A",
    "A AA AAA A AAA AA A",
    "A  A     C     A  A",
    "AA A A AAAAA A A AA",
    "A    A   A   A    A",
    "A AAAAAA A AAAAAA A",
    "A3               4A",
    "AAAAAAAAAAAAAAAAAAA",
};

class Player {
public:
    float frame = 0;  // Кадр анимации
    int x = 9, y = 15;  // Координаты игрока
    int newx = 0, newy = 0;  // Новые координаты игрока
    int rotate = 1, ti = 0;  // Направление и счётчик времени
    int points = 0; // Счетчик собранных кружочков для игрока

    void update() {
        frame += 0.01;
        if (frame > 5)
            frame -= 5;

        ti++;
        if (ti >= 300) {
            switch (rotate)
            {
            case 1:
                if (TileMap[y][newx + 1] != 'A')
                    newx += 1;
                break;
            case 2:
                if (TileMap[y][newx - 1] != 'A')
                    newx -= 1;
                break;
            case 3:
                if (TileMap[newy - 1][x] != 'A')
                    newy -= 1;
                break;
            case 4:
                if (TileMap[newy + 1][x] != 'A')
                    newy += 1;
                break;
            }

            ti = 0;
        }

        if (TileMap[newy][newx] == ' ' || TileMap[newy][newx] == 'B') {
            if (TileMap[newy][newx] == ' ') {
                q++;
                points++; // Увеличение счетчика для первого игрока
            }

            if (TileMap[newy][newx] == '1'
                || TileMap[newy][newx] == '2' || TileMap[newy][newx] == '3' || TileMap[newy][newx] == '4')
                life = false;

            TileMap[y][x] = 'B';
            TileMap[newy][newx] = 'C';

            x = newx;
            y = newy;
        }

        if (newy == 9 && (newx == 0 || newx == 18)) {
            if (newx == 0)
                newx = 17;
            else
                newx = 1;

            TileMap[y][x] = 'B';
            TileMap[newy][newx] = 'C';

            x = newx;
            y = newy;
        }
    }
};

class Enemy {
public:
    int x[4] = { 1, 17 , 1, 17 };  // Координаты врагов
    int y[4] = { 1, 1, 19, 19 };
    int newx[4] = { 0 , 0 , 0, 0 };  // Новые координаты врагов
    int newy[4] = { 0, 0, 0, 0 };
    int rotate[4] = { 1, 1, 1, 1 };  // Направление врагов и счётчик времени
    int ti = 0;

    void update() {
        ti++;

        if (ti >= 300) {
            for (int i = 0; i < 4; i++) {
                rotate[i] = rand() % 4 + 1;

                newx[i] = x[i];
                newy[i] = y[i];

                switch (rotate[i])
                {
                case 1:
                    if (TileMap[y[i]][newx[i] + 1] != 'A')
                        newx[i] += 1;
                    break;
                case 2:
                    if (TileMap[y[i]][newx[i] - 1] != 'A')
                        newx[i] -= 1;
                    break;
                case 3:
                    if (TileMap[newy[i] - 1][x[i]] != 'A')
                        newy[i] -= 1;
                    break;
                case 4:
                    if (TileMap[newy[i] + 1][x[i]] != 'A')
                        newy[i] += 1;
                    break;
                }
            }

            ti = 0;
        }

        for (int i = 0; i < 4; i++) {
            if (TileMap[newy[i]][newx[i]] == ' ' || TileMap[newy[i]][newx[i]] == 'B' ||
                TileMap[newy[i]][newx[i]] == 'C') {
                if (TileMap[newy[i]][newx[i]] == 'B')
                    TileMap[y[i]][x[i]] = 'B';
                else if (TileMap[newy[i]][newx[i]] == ' ')
                    TileMap[y[i]][x[i]] = ' ';
                else if (TileMap[newy[i]][newx[i]] == 'C')
                    life = false;

                if (i == 0)
                    TileMap[newy[i]][newx[i]] = '1';
                if (i == 1)
                    TileMap[newy[i]][newx[i]] = '2';
                if (i == 2)
                    TileMap[newy[i]][newx[i]] = '3';
                if (i == 3)
                    TileMap[newy[i]][newx[i]] = '4';

                x[i] = newx[i];
                y[i] = newy[i];
            }

            if (newy[i] == 9 && (newx[i] == 0 || newx[i] == 18)) {
                if (newx[i] == 0)
                    newx[i] = 17;
                else
                    newx[i] = 1;

                TileMap[y[i]][x[i]] = 'B';

                if (i == 0)
                    TileMap[newy[i]][newx[i]] = '1';
                if (i == 1)
                    TileMap[newy[i]][newx[i]] = '2';
                if (i == 2)
                    TileMap[newy[i]][newx[i]] = '3';
                if (i == 3)
                    TileMap[newy[i]][newx[i]] = '4';

                x[i] = newx[i];
                y[i] = newy[i];
            }
        }
    }
};

void restartGame(Player& p, Player& p2, Enemy& en) {
    q = 0;
    life = true;
    isPaused = false;

    // Вернуть начальное состояние карты
    String initialTileMap[H] = {
        "AAAAAAAAAAAAAAAAAAA",
        "A1       A       2A",
        "A AA AAA A AAA AA A",
        "A        A        A",
        "A AA A AAAAA A AA A",
        "A    A   A   A    A",
        "AAAA AAA A AAA AAAA",
        "BBBA A       A ABBB",
        "AAAA A AAAAA A AAAA",
        "BBBB   ABBBA   BBBB",
        "AAAA A AAAAA A AAAA",
        "BBBA A       A ABBB",
        "AAAA A AAAAA A AAAA",
        "A        A        A",
        "A AA AAA A AAA AA A",
        "A  A     C     A  A",
        "AA A A AAAAA A A AA",
        "A    A   A   A    A",
        "A AAAAAA A AAAAAA A",
        "A3               4A",
        "AAAAAAAAAAAAAAAAAAA",
    };

    for (int i = 0; i < H; ++i) {
        TileMap[i] = initialTileMap[i];
    }

    // Вернуть начальные координаты игроков
    p.x = 9;
    p.y = 15;
    p.newx = 0;
    p.newy = 0;
    p.rotate = 1;
    p.frame = 0;
    p.points = 0;

    p2.x = 9;  // Начальные координаты второго игрока
    p2.y = 17;
    p2.newx = 0;
    p2.newy = 0;
    p2.rotate = 1;
    p2.frame = 0;
    p2.points = 0;

    // Вернуть начальные координаты врагов
    en.x[0] = 1; en.y[0] = 1;
    en.x[1] = 17; en.y[1] = 1;
    en.x[2] = 1; en.y[2] = 19;
    en.x[3] = 17; en.y[3] = 19;
    for (int i = 0; i < 4; ++i) {
        en.newx[i] = 0;
        en.newy[i] = 0;
        en.rotate[i] = 1;
    }
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(W * ts, H * ts), "Maze!");

    // Загрузка текстур
    Texture t;
    t.loadFromFile("C:/Pacmanlab/Paint/title.png");
    Sprite plat(t);

    Texture yw;
    yw.loadFromFile("C:/Pacmanlab/Paint/youwin.png");
    Sprite youwin(yw);
    youwin.setPosition(100, 210);

    Texture yl;
    yl.loadFromFile("C:/Pacmanlab/Paint/youlose.png");
    Sprite youlose(yl);
    youlose.setPosition(100, 210);

    Player p;
    Player p2; // Второй игрок
    Enemy en;

    Font font;  // Загрузка шрифта для текста счетчика
    if (!font.loadFromFile("C:/Pacmanlab/Paint/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return 1;
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            // Управление первым игроком (стрелки)
            if (event.type == Event::KeyPressed && !isPaused && q < 171 && life) {
                p.newx = p.x;
                p.newy = p.y;

                if (event.key.code == Keyboard::Right)
                    p.rotate = 1;
                if (event.key.code == Keyboard::Left)
                    p.rotate = 2;
                if (event.key.code == Keyboard::Up)
                    p.rotate = 3;
                if (event.key.code == Keyboard::Down)
                    p.rotate = 4;
            }

            // Управление вторым игроком (WASD)
            if (event.type == Event::KeyPressed && !isPaused && q < 171 && life) {
                p2.newx = p2.x;
                p2.newy = p2.y;

                if (event.key.code == Keyboard::D)
                    p2.rotate = 1;
                if (event.key.code == Keyboard::A)
                    p2.rotate = 2;
                if (event.key.code == Keyboard::W)
                    p2.rotate = 3;
                if (event.key.code == Keyboard::S)
                    p2.rotate = 4;
            }
        }

        if (!isPaused && q < 171 && life) {
            p.update();
            p2.update(); // Обновление второго игрока
            en.update();
        }

        window.clear(Color::Black);

        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++) {
                if (TileMap[i][j] == 'A')
                    plat.setTextureRect(IntRect(0, 0, ts, ts));
                if (TileMap[i][j] == 'C')
                    plat.setTextureRect(IntRect(ts * int(p.frame), ts * p.rotate, ts, ts));
                if (TileMap[i][j] == 'D') // Для второго игрока
                    plat.setTextureRect(IntRect(ts * int(p2.frame), ts * p2.rotate, ts, ts));
                if (TileMap[i][j] == ' ')
                    plat.setTextureRect(IntRect(ts, 0, ts, ts));
                if (TileMap[i][j] == '1')
                    plat.setTextureRect(IntRect(ts * 5, ts * en.rotate[0], ts, ts));
                if (TileMap[i][j] == '2')
                    plat.setTextureRect(IntRect(ts * 5, ts * en.rotate[1], ts, ts));
                if (TileMap[i][j] == '3')
                    plat.setTextureRect(IntRect(ts * 5, ts * en.rotate[2], ts, ts));
                if (TileMap[i][j] == '4')
                    plat.setTextureRect(IntRect(ts * 5, ts * en.rotate[3], ts, ts));
                if (TileMap[i][j] == 'B')
                    continue;

                plat.setPosition(j * ts, i * ts);
                window.draw(plat);
            }

        // Отображение счетчика для первого игрока
        sf::Text player1Score;
        player1Score.setFont(font);
        player1Score.setCharacterSize(20);
        player1Score.setFillColor(sf::Color::White);
        player1Score.setPosition(10, 10);
        player1Score.setString("Player 1 Score: " + std::to_string(p.points));

        // Отображение счетчика для второго игрока
        sf::Text player2Score;
        player2Score.setFont(font);
        player2Score.setCharacterSize(20);
        player2Score.setFillColor(sf::Color::White);
        player2Score.setPosition(10, 40);  // Позиция для второго игрока
        player2Score.setString("Player 2 Score: " + std::to_string(p2.points));

        window.draw(player1Score);
        window.draw(player2Score);

        if (q == 171)
            window.draw(youwin);
        if (!life)
            window.draw(youlose);

        window.display();

        // Проверка условий для перезапуска игры
        if (q == 171 || !life) {
            restartGame(p, p2, en);
        }
    }

    return 0;
}
