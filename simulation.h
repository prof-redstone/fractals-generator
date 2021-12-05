#ifndef SIMULATION_H
#define SIMULATION_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <sstream>//for larger precision with to_string(double)
#include <iomanip>//for larger precision with to_string(double)
#include <cstdlib>//to generate color
#include <ctime>  //to initialize random

#include <complex>

class Simulation {
public:
	bool ready = false;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	std::vector<std::vector<int>> pixelNcount; //nb d'iteration avant que ça diverge
	std::vector<std::vector<double>> pixelVal; //modulo de la valeur à la divergence (pour avoir un truc plus lisse)
	uint8_t fractalType; //mandelbrot 0; Julia set 1; Burning ship 2;
	int win_width;//taille de la window
	int win_height;
	int maxIteration;
	float limit;
	double zoomScale; //niveau de zoom
	double posX; //postion du milieu de l'ecran sur la fractal
	double posY;
	double JuliaX; //coordonné de fractal Julia
	double JuliaY;
	double posJuliaX; //postion du milieu de l'ecran sur la fractal
	double posJuliaY;
	double zoomScaleJulia;


	std::vector<sf::Color> listColor = { 
		sf::Color(25, 7, 26),
		sf::Color(0, 7, 100), 
		sf::Color(211, 236, 248), 
		sf::Color(255, 170, 0), 
		sf::Color(66, 30, 15) 
	};

	Simulation();
	Simulation(sf::RenderWindow& win);

	void simulate();

	void render(sf::RenderWindow& win);

	void updateWindow(sf::RenderWindow& win);
	void updateWindow(int x, int y);
	void updateColorList(int const& param, std::vector<sf::Color> const& list = {});

	int inputHandler(sf::Event event, sf::RenderWindow& win);

	//map number to a scale to another 
	static double mapScale(double const& val, double const& x0, double const& x1, double const& y0, double const& y1) {
		return ((y1 - y0) / (x1 - x0)) * (val - x0) + y0;
	}

	//tools function
	static sf::Color colorOfNumber(int const& valI, int const& val2I, std::vector<sf::Color> const& listColor);
	static sf::Color ColorSmoother(sf::Color const& c1, sf::Color const& c2, double const& x);
	static sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI=1);
};





#endif