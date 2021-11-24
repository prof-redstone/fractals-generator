#include "simulation.h"

using namespace sf;
using namespace std;

//constructeur par default
Simulation::Simulation() {
	fractalType = 0;
	maxIteration = 100;
	limit = 16;
	zoomScale = 1;
	posX = -0.5;
	posY = 0;
	JuliaX = -0.5;
	JuliaY = -0.5;
	win_width = 100;//use update window to change
	win_height = 100;//use update window to change
	image.create(win_width, win_height, Color::Cyan);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture);

	//creat 2dimension vector for 2 variables
	vector<int> v1(win_width, 0);
	vector<vector<int>> v11(win_height, v1);
	pixelNcount = v11;
	vector<double> v2(win_width, 0);
	vector<vector<double>> v22(win_height, v2);
	pixelVal = v22;
}
//constructeur avec la window (pour la taille de la simulation)
Simulation::Simulation(sf::RenderWindow& win) {
	fractalType = 1;
	maxIteration = 100;
	limit = 16;
	zoomScale = 1;
	posX = -0.5;
	posY = 0;
	JuliaX = 0;
	JuliaY = 0;
    win_width = win.getSize().x; //ici
    win_height = win.getSize().y;
	image.create(win_width, win_height, Color::Black);
    texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture);
	
	//creat 2dimension vector for 2 variables
	vector<int> v1(win_width, 0);
	vector<vector<int>> v11(win_height, v1);
	pixelNcount = v11;
	vector<double> v2(win_width, 0);
	vector<vector<double>> v22(win_height, v2);
	pixelVal = v22;
	ready = true;
}

//mettre à jour la taille de l'image avec la taille de la fenetre
void Simulation::updateWindow(sf::RenderWindow& win){
	win_width = win.getSize().x;
	win_height = win.getSize().y;
	image.create(win_width, win_height, Color::Black);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture,true);

	//creat 2dimension vector for 2 variables
	vector<int> v1(win_height, 0);
	vector<vector<int>> v11(win_width, v1);
	pixelNcount = v11;
	vector<double> v2(win_height, 0);
	vector<vector<double>> v22(win_width, v2);
	pixelVal = v22;
	ready = true;
}

//mettre à jour la taille de l'image avec la taille donné
void Simulation::updateWindow(int x, int y) {
	win_width = x;
	win_height = y;
	image.create(win_width, win_height, Color::Black);
	texture.create(win_width, win_height);
	texture.update(image);
	sprite.setTexture(texture, true);

	//creat 2dimension vector for 2 variables
	vector<int> v1(win_height, 0);
	vector<vector<int>> v11(win_width, v1);
	pixelNcount = v11;
	vector<double> v2(win_height, 0);
	vector<vector<double>> v22(win_width, v2);
	pixelVal = v22;
	ready = true;
}

void Simulation::render(sf::RenderWindow& win) {
	if (ready){
		//creation de la texture par defaut
		//image dans la texture
		texture.create(win_width, win_height);
		texture.update(image);
    
		//texture dans le sprite
		sprite.setTexture(texture,false);

		win.draw(sprite);
	}
}


void Simulation::simulate() {
	cout << "start simulate";

	if (fractalType == 0){
		for (int i = 0; i < win_width; i++)
		{

			for (int j = 0; j < win_height; j++) {
				double x = Simulation::mapScale(i, 0, win_width, (-zoomScale + posX) * (static_cast<float>(win_width) / static_cast<float>(win_height)), (zoomScale  + posX) * (static_cast<float>(win_width) / static_cast<float>(win_height)));
				double y = Simulation::mapScale(j, 0, win_height, (-zoomScale + posY) , (zoomScale + posY) );

				double a = x; //partie reel
				double b = y; //partie imaginaire
				int n = 0; //nombre d'iteration


				while (n < maxIteration && a * a + b * b <= limit) {
					double newb = (2 * a * b); //code degueu mais opti pour faire z^2 + c
					a = (a * a - b * b) + x;
					b = newb + y;

					n++;
				}

				pixelNcount[i][j] = n;
				pixelVal[i][j] = sqrt(a * a + b * b);

			}
		}
	}
	else if (fractalType == 1) {
		for (int i = 0; i < win_width; i++)
		{
			for (int j = 0; j < win_height; j++) {
				double x = Simulation::mapScale(j, 0, win_height, -zoomScale + posX, zoomScale + posX);
				double y = Simulation::mapScale(i, 0, win_width, -zoomScale + posY, zoomScale + posY);

				double a = x + JuliaX; //partie reel
				double b = y + JuliaY; //partie imaginaire
				int n = 0; //nombre d'iteration


				while (n < maxIteration && a * a + b * b <= limit) {
					double newb = (2 * a * b); //code degueu mais opti pour faire z^2 + c
					a = (a * a - b * b) + JuliaX;
					b = newb + JuliaY;

					n++;
				}

				pixelNcount[i][j] = n;
				pixelVal[i][j] = sqrt(a * a + b * b);

			}
		}
	}
	image.create(win_width, win_height, Color::Black);
    //creat image and color with array compute
    for ( int i = 0; i < win_width; i++) {
        for ( int j = 0; j < win_height; j++)
        {

            image.setPixel(i, j, colorOfNumber(pixelNcount[i][j], pixelVal[i][j]));
        }
    }

	cout << " finished " << endl;
}

int Simulation::inputHandler(Event event, sf::RenderWindow& window){
	bool NeedToSimulate = false;
	if (event.type == Event::Closed) {
		window.close();
	}
	if (event.type == Event::MouseButtonPressed) {
		//zoom in 
		if (event.mouseButton.button == Mouse::Left)
		{
			zoomScale /= 1.5;
			Mouse mouse;
			cout << "zoom " << 1 / zoomScale << endl;
			posX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, -zoomScale + posX, zoomScale + posX);
			posY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, -zoomScale + posY, zoomScale + posY);
			NeedToSimulate = true;
		}
		//zoom out
		if (event.mouseButton.button == Mouse::Right)
		{
			zoomScale *= 1.5;
			Mouse mouse;
			cout << "zoom " << 1 / zoomScale << endl;
			posX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, -zoomScale + posX, zoomScale + posX);
			posY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, -zoomScale + posY, zoomScale + posY);
			NeedToSimulate = true;
		}
	}
	if (event.type == Event::KeyPressed) {
		//nb iteration max
		if (event.key.code == Keyboard::Left)//increase iteration
		{
			maxIteration += 50;
			cout << "Iteration " << to_string(maxIteration) << endl;
			NeedToSimulate = true;
		}
		if (event.key.code == Keyboard::Right)//decrease iteration
		{
			maxIteration += -50;
			cout << "Iteration " << to_string(maxIteration) << endl;
			NeedToSimulate = true;
		}
		//change limit to pretend to tend to infinity
		if (event.key.code == Keyboard::Up)//incredase limit
		{
			limit *= 2;
			cout << "limite " << to_string(limit) << endl;
			NeedToSimulate = true;
		}
		if (event.key.code == Keyboard::Down)//decrease limit
		{
			limit /= 2;
			cout << "limite " << to_string(limit) << endl;
			NeedToSimulate = true;
		}
		//change fractal type
		if (event.key.code == Keyboard::LControl)
		{
			cout << "controle !" << endl;
		}
	}
	//window change size event to adapte image to window
	if (event.type == sf::Event::Resized)
	{
		cout << "taille " << typeid(window.getSize().x).name() << " " << window.getSize().y << endl;
		this->updateWindow(event.size.width, event.size.height);
		//marche pas sans les 2 lignes en dessous si on change la taille de la fenetre ça fait des trucs nimp
		sf::FloatRect view(0, 0, event.size.width, event.size.height);
		window.setView(sf::View(view));
		NeedToSimulate = true;
	}


	if (NeedToSimulate == true) {
		return 1; //besoin de re simuler
	}
	return 0;
}

//assigne color base on a float number
sf::Color Simulation::colorOfNumber(int const& valI, int const& val2I) {
	double val = valI + 1 - log(log(val2I)) / log(2);
	//double val = valI ;
	Color color;
	color = Simulation::HSLtoRGB(val * 0.02, 0.1, 1);
	//color = Color(static_cast<unsigned int>(255*cos(val+1)), static_cast<unsigned int>(255 * cos(val+2)), static_cast<unsigned int>(255 * cos(val)), 255);

	return color;
}

//simply converte color to HSL mode to RGB mode
sf::Color Simulation::HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI)
{
	//hue : 0 : red  1 : yellow  2 : green  3 : cyan  4 : blue  5 : purple  6 : red
	//hue  0 == 6   6 is one cycle rotation
	//saturation [0;1]
	//darkness [0;1]
	//alpha [0;1]

    double red = 0;
    double green = 0;
    double blue = 0;
    double hue = fmod(hueI, 6);

    if (hue >= 0 && hue < 1) {
        red = 255; green = hue * 255; blue = 0;
    }
    else if (hue >= 1 && hue < 2) {
        green = 255; red = 255 - ((hue - 1) * 255); blue = 0;
    }
    else if (hue >= 2 && hue < 3) {
        green = 255; blue = (hue - 2) * 255; red = 0;
    }
    else if (hue >= 3 && hue < 4) {
        blue = 255; green = 255 - ((hue - 3) * 255); red = 0;
    }
    else if (hue >= 4 && hue < 5) {
        blue = 255; red = (hue - 4) * 255; green = 0;
    }
    else if (hue >= 5 && hue < 6) {
        red = 255; blue = 255 - ((hue - 5) * 255); green = 0;
    }

    red = red + (255 - red) * satI;
    green = green + (255 - green) * satI;
    blue = blue + (255 - blue) * satI;

    red = red * darkI;
    green = green * darkI;
    blue = blue * darkI;

    return Color(static_cast<Uint8>(red), static_cast<Uint8>(green), static_cast<Uint8>(blue), static_cast<Uint8>(alphaI * 255));
}