
//les includes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>


//constantes
const int WIN_WIDTH = 900;
const int WIN_HEIGHT = 900;
//include
#include "main.h"
#include "simulation.h"


//name space
using namespace sf;
using namespace std;


//prtotypes des fonctions
void InputHandler(sf::Event event, sf::RenderWindow& window);
void loadFont();
void SetText(Text& txt, string str, Color col, int size);
void UpdateImage(RenderWindow& window);
Image renderMandelbrotSet(RenderWindow& window);
double mapScale(double const& val, double const& x0, double const& x1, double const& y0, double const& y1);
Color colorOfNumber(int const& valI, int const& val2I);
Color colorWheel(double hueI, double const satI =1, double const darkI =1, double const alphaI = 1);

//variable
Font font; //from sfml

int FractalType = 1;//1 = mandelbrot 2=Julia

int maxIteration = 150;
float limit = 4;
double zoomScale = 1;
double centerX = -0.5;
double centerY = 0;

int main()
{
    cout << "demarrage de l'application" << endl;
    //creation de la fenetre
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Fractal Generator");
    //activation Vsinc
    window.setVerticalSyncEnabled(true);
    //chargement de la font pour pouvoir afficher des chose
    loadFont();

    

    while (window.isOpen()) {
        //gestion des inputs et des events
        Event event;
        while (window.pollEvent(event)) {
            InputHandler(event, window);
        }
        //couleur du font de la fenetre
        window.clear(Color::Black);

        UpdateImage(window);

        window.display();
    }
    return 0;
}


//gestion des inputs et des evenements
void InputHandler(Event event, RenderWindow& window) {
    if (event.type == Event::Closed) {
        window.close();
    }
    if (event.type == Event::MouseButtonPressed) {
        if (event.mouseButton.button == Mouse::Left)
        {
            zoomScale/=1.5;
            Mouse mouse;
            cout << "zoom " << 1/zoomScale << endl;
            centerX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, -zoomScale + centerX, zoomScale + centerX);
            centerY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, -zoomScale + centerY, zoomScale + centerY);
        }
        if (event.mouseButton.button == Mouse::Right)
        {
            zoomScale *= 1.5;
            Mouse mouse;
            cout << "zoom " << 1 / zoomScale << endl;
            centerX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, -zoomScale + centerX, zoomScale + centerX);
            centerY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, -zoomScale + centerY, zoomScale + centerY);
        }
    }
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Left)
        {
            maxIteration += 50;
            cout << "Iteration " << to_string(maxIteration) << endl;
        }
        if (event.key.code == Keyboard::Right)
        {
            maxIteration += -50;
            cout << "Iteration " << to_string(maxIteration) << endl;
        }
        if (event.key.code == Keyboard::Up)
        {
            limit *=2;
            cout << "limite " << to_string(limit) << endl;
        }
        if (event.key.code == Keyboard::Down)
        {
            limit /=2;
            cout << "limite " << to_string(limit) << endl;
        }
    }
}

//charger la police d'ecriture
void loadFont() {
    if (!font.loadFromFile("res/poppins.ttf")) {
        cout << "Ereur de charment de la font " << "res/poppins.ttf" << endl;
    }
}

void SetText(Text& txt, string str = "", Color col = Color::White, int size = 26) {
    /*Text txt;
    SetText(txt, "Hello World", Color::Green, 50);
    txt.setStyle(Text::Bold | Text::Underlined);*/
    txt.setFont(font);
    txt.setString(str);
    txt.setFillColor(col);
    txt.setCharacterSize(size);
}

void UpdateImage(RenderWindow& window) {
    //image compose la texture qui compose le sprite, puis on affiche le sprite
    Image image;
    Texture texture;
    Sprite sprite;

    

    image = renderMandelbrotSet(window);
    cout << "render mandel " << endl;
    

    //creation de la texture par defaut
    texture.create(window.getSize().x, window.getSize().y);
    //image dans la texture
    texture.update(image);
    //texture dans le sprite
    sprite.setTexture(texture);

    window.draw(sprite);
}

Image renderMandelbrotSet(RenderWindow& window) {
    int width = window.getSize().x;
    int height = window.getSize().y;
    int default_value = 0;
    vector<int> v(width, default_value);
    vector<vector<int>> array(height, v);
    double default_value2 = 0;
    vector<int> v2(width, default_value);
    vector<vector<int>> array2(height, v);

    Image image;
    //création d'une image avec uin fond noir par defaut
    image.create(width, height, Color::Black);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++) {
            double x = mapScale(i, 0, width, -zoomScale + centerX, zoomScale + centerX);
            double y = mapScale(j, 0, height, -zoomScale + centerY, zoomScale + centerY);

            double a = x;
            double b = y;
            int n = 0;


            while (n < maxIteration && a*a + b*b <= limit) {
                //double newa = ;
                double newb = (2 * a * b);

                a = (a * a - b * b) + x;
                b = newb + y;

                //smoothcolor += exp(-(a * a + b * b));

                n++;
            }

            array[i][j] = n;
            array2[i][j] = sqrt(a * a + b * b);
            
            /*
            double bright = mapScale(n, 1, maxIteration, 0, 1);
            bright = mapScale(bright, 0, 1, 0, 255);


            if (n == maxIteration) {
                bright = 0;
            }
            array[i][j] = bright;
            */
        }
    }

    //creat image and color with array compute
    for (unsigned int i = 0; i < window.getSize().x; i++) {
        for (unsigned int j = 0; j < window.getSize().y; j++)
        {
            image.setPixel(i, j, colorOfNumber(array[i][j], array2[i][j] ));
        }
    }

    return image;
}


double mapScale(double const &val, double const &x0, double const &x1, double const &y0, double const &y1) {
    return ((y1 - y0) / (x1 - x0)) *(val - x0) + y0;
}


Color colorOfNumber(int const &valI, int const& val2I) {
    double val = valI + 1 - log(log(val2I)) / log(2);
    //double val = valI ;
    Color color;
    color = colorWheel(val*0.02, 0.1, 1);
    //color = Color(static_cast<unsigned int>(255*cos(val+1)), static_cast<unsigned int>(255 * cos(val+2)), static_cast<unsigned int>(255 * cos(val)), 255);

    return color;
}

Color colorWheel(double hueI , double const satI , double const darkI , double const alphaI) {
    /*
    hue :
    0 : red
    1 : yellow
    2 : green
    3 : cyan
    4 : blue
    5 : purple
    6 : red
    hue  0 == 6   6 is one cycle rotation

    saturation [0;1]
    darkness [0;1]
    alpha [0;1]

    */

    double red=0;
    double green=0;
    double blue=0;
    double hue = fmod(hueI,6);

    if (hue >= 0 && hue < 1) {
        red = 255;
        green = hue * 255;
        blue = 0;
    }
    else if (hue >= 1 && hue < 2) {
        green = 255;
        red = 255 - ((hue - 1) * 255);
        blue = 0;
    }
    else if (hue >= 2 && hue < 3) {
        green = 255;
        blue = (hue - 2) * 255;
        red = 0;
    }
    else if (hue >= 3 && hue < 4) {
        blue = 255;
        green = 255 - ((hue - 3) * 255);
        red = 0;
    }
    else if (hue >= 4 && hue < 5) {
        blue = 255;
        red = (hue - 4) * 255;
        green = 0;
    }
    else if (hue >= 5 && hue < 6) {
        red = 255;
        blue = 255 - ((hue - 5) * 255);
        green = 0;
    }

    red = red + (255 - red) * satI;
    green = green + (255 - green) * satI;
    blue = blue + (255 - blue) * satI;

    red = red * darkI;
    green = green * darkI;
    blue = blue * darkI;

    return Color(static_cast<Uint8>(red), static_cast<Uint8>(green), static_cast<Uint8>(blue), static_cast<Uint8>(alphaI*255));
}
