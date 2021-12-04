#include "simulation.h"

using namespace sf;
using namespace std;

//constructeur par default
Simulation::Simulation() {
	fractalType = 0;
	maxIteration = 100;
	limit = 16;
	zoomScale = 0.001;
	posX = -0.5;
	posY = 0;
	JuliaX = -0.5;
	JuliaY = -0.5;
	posJuliaX = 0; //postion du milieu de l'ecran sur la fractal
	posJuliaY = 0;
	zoomScaleJulia = 0.001;
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
	zoomScale = 0.01;
	posX = -0.5;
	posY = 0;
	JuliaX = 0;
	JuliaY = 0;
	posJuliaX = 0; //postion du milieu de l'ecran sur la fractal
	posJuliaY = 0;
	zoomScaleJulia = 0.001;
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
		string title;
		std::ostringstream streamObj;//for better precision to convert double into string
		if (fractalType == 0)
		{
			streamObj << std::fixed << std::setprecision(12) << "Mandelbrot : pos: " << posX << "; " << posY << "i; zoom : " << std::setprecision(3) << (1/(zoomScale*1000)) ;
			title = streamObj.str();
		}
		else if (fractalType == 1) {
			streamObj << std::fixed << std::setprecision(12) << "Julia : C: " << JuliaX << ";" << JuliaY << "i; pos: " << posJuliaX << "; " << posJuliaY << "i; zoom : " << std::setprecision(3) << (1 / (zoomScaleJulia * 1000));
			title = streamObj.str();
		}
		win.setTitle(title);
	}
}

void Simulation::updateColorList(int const& param, std::vector<sf::Color> const& list) { // 0: reset ans set; 1 add ; 2 set color with logic and automatic
	if (param == 0) {
		listColor.clear();
		for (int i = 0; i < list.size(); i++)
		{
			listColor.push_back(list[i]);
		}
	}
	if (param == 1)
	{
		for (int i = 0; i < list.size(); i++)
		{
			listColor.push_back(list[i]);
		}
	}
	if (param == 2) {
		//intercalage de couleur blanc et noir entre chaque couleur de facon logique
		listColor.clear();
		std::srand(std::time(nullptr));
		double lastHUE = 3.5;
		for (int i = 0; i < 10; i++)
		{
			double nextHue = lastHUE + (rand() % 110 - (double)35) / (double)100;
			lastHUE = nextHue;
			listColor.push_back(HSLtoRGB(nextHue, 0.000001, (rand() % 10 + (double)80) / (double)100));
			if (i % 2 == 1) {

				listColor.push_back(Color(20, 20, 20));
			}
			else {
				listColor.push_back(Color(220, 220, 220));
			}
		}
	}
}

void Simulation::simulate() {
	cout << "start simulate";

	if (fractalType == 0){ //Mandelbrot set
		for (int i = 0; i < win_width; i++)
		{

			for (int j = 0; j < win_height; j++) {
				double x = Simulation::mapScale(i , 0, win_width, (-zoomScale*static_cast<float>(win_width ) + posX), (zoomScale*static_cast<float>(win_width ) + posX));
				double y = Simulation::mapScale(j, 0, win_height, (-zoomScale * static_cast<float>(win_height) + posY) , (zoomScale * static_cast<float>(win_height) + posY) );

				double a = x; //partie reel
				double b = y; //partie imaginaire
				int n = 0; //nombre d'iteration

				double newb;
				while (n < maxIteration && a * a + b * b <= limit) {
					newb = (2 * a * b); //code degueu mais opti pour faire z^2 + c
					a = (a * a - b * b) + x;
					b = newb + y;

					n++;
				}

				if (n == maxIteration) {
					pixelNcount[i][j] = 0;
				}else {
					pixelNcount[i][j] = n;
				}
				pixelVal[i][j] = sqrt(a * a + b * b);

			}
		}
	}
	else if (fractalType == 1) { //Julia set
		for (int i = 0; i < win_width; i++)
		{
			for (int j = 0; j < win_height; j++) {
				double x = Simulation::mapScale(i, 0, win_width, (-zoomScaleJulia * static_cast<float>(win_width) + posJuliaX), (zoomScaleJulia * static_cast<float>(win_width) + posJuliaX));
				double y = Simulation::mapScale(j, 0, win_height, (-zoomScaleJulia * static_cast<float>(win_height) + posJuliaY), (zoomScaleJulia * static_cast<float>(win_height) + posJuliaY));

				double a = x ; //partie reel
				double b = y ; //partie imaginaire
				int n = 0; //nombre d'iteration


				while (n < maxIteration && a * a + b * b <= limit) {
					double newb = (2 * a * b); //code degueu mais opti pour faire z^2 + c
					a = (a * a - b * b) + JuliaX;
					b = newb + JuliaY;

					n++;
				}

				if (n == maxIteration) {
					pixelNcount[i][j] = 0;
				}else {
					pixelNcount[i][j] = n;
				}
				pixelVal[i][j] = sqrt(a * a + b * b);
			}
		}
	}
    //creat image and color with array compute
    for ( int i = 0; i < win_width; i++) {
        for ( int j = 0; j < win_height; j++)
        {
            image.setPixel(i, j, colorOfNumber(pixelNcount[i][j], pixelVal[i][j], listColor ) );
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
			if (fractalType == 0)
			{
				Mouse mouse;
				cout << "zoom " << 1 / zoomScale << endl;
				posX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, (-zoomScale * static_cast<float>(win_width) + posX), (zoomScale * static_cast<float>(win_width) + posX));
				posY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, (-zoomScale * static_cast<float>(win_height) + posY), (zoomScale * static_cast<float>(win_height) + posY));
				zoomScale /= 1.5;
			}
			else {
				Mouse mouse;
				cout << "zoom " << 1 / zoomScaleJulia << endl;
				posJuliaX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, (-zoomScaleJulia * static_cast<float>(win_width) + posJuliaX), (zoomScaleJulia * static_cast<float>(win_width) + posJuliaX));
				posJuliaY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, (-zoomScaleJulia * static_cast<float>(win_height) + posJuliaY), (zoomScaleJulia * static_cast<float>(win_height) + posJuliaY));
				zoomScaleJulia /= 1.5;
			}
			NeedToSimulate = true;
		}
		//zoom out
		if (event.mouseButton.button == Mouse::Right)
		{
			if (fractalType == 0)
			{
				Mouse mouse;
				cout << "zoom " << 1 / zoomScale << endl;
				posX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, (-zoomScale * static_cast<float>(win_width) + posX), (zoomScale * static_cast<float>(win_width) + posX));
				posY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, (-zoomScale * static_cast<float>(win_height) + posY), (zoomScale * static_cast<float>(win_height) + posY));
				zoomScale *= 1.5;
			}
			else {
				Mouse mouse;
				cout << "zoom " << 1 / zoomScaleJulia << endl;
				posJuliaX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, (-zoomScaleJulia * static_cast<float>(win_width) + posJuliaX), (zoomScaleJulia * static_cast<float>(win_width) + posJuliaX));
				posJuliaY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, (-zoomScaleJulia * static_cast<float>(win_height) + posJuliaY), (zoomScaleJulia * static_cast<float>(win_height) + posJuliaY));
				zoomScaleJulia *= 1.5;
			}
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
			if (fractalType == 0)
			{
				posJuliaX = 0; //postion du milieu de l'ecran sur la fractal
				posJuliaY = 0;
				zoomScaleJulia = 0.001;
				Mouse mouse;
				JuliaX = mapScale(mouse.getPosition(window).x, 0, window.getSize().x, (-zoomScale * static_cast<float>(win_width) + posX), (zoomScale * static_cast<float>(win_width) + posX));
				JuliaY = mapScale(mouse.getPosition(window).y, 0, window.getSize().y, (-zoomScale * static_cast<float>(win_height) + posY), (zoomScale * static_cast<float>(win_height) + posY));
				fractalType = 1;
			}
			else {
				fractalType = 0;
			}
			NeedToSimulate = true;
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
sf::Color Simulation::colorOfNumber(int const& valI, int const& val2I, vector<Color> const& listColor) {
	double val;
	if (valI == 0) {
		val = 0;
		return Color(0,0,0);
	}
	else {
		val = ((double)valI + 1 - log(log(val2I)) / 0.6931471805599) * 0.04;
	}

	int Index1color = floor(fmod(val, listColor.size()));

	int Index2color = floor(fmod(val, listColor.size())) + 1;
	if (Index2color >= listColor.size()) { Index2color = 0; }//pour éviter les out of range
	Color color;
	color = ColorSmoother(listColor[Index1color], listColor[Index2color], fmod(val, 1));
	//color = Simulation::HSLtoRGB(val, 0.1, 1);
	//color = Color(static_cast<unsigned int>(255*cos(val+1)), static_cast<unsigned int>(255 * cos(val+2)), static_cast<unsigned int>(255 * cos(val)), 255);

	return color;
}

sf::Color Simulation::ColorSmoother(Color const& c1, Color const& c2, double const& x){
	double val = 1 / (1 + pow(4, -7*(x - 0.5)));
	Color c;
	c.r = c1.r + (c2.r - c1.r) * val;
	c.g = c1.g + (c2.g - c1.g) * val;
	c.b = c1.b + (c2.b - c1.b) * val;
	return c;
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