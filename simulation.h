#ifndef SIMULATION_H
#define SIMULATION_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

class Simulation {
public:
	bool ready = false;
	sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	std::vector<std::vector<int>> pixelNcount; //nb d'iteration avant que ça diverge
	std::vector<std::vector<double>> pixelVal; //modulo de la valeur à la divergence (pour avoir un truc plus lisse)
	uint8_t fractalType; //juste mandelbrot for the moment (0)
	int maxIteration;
	float limit;
	double zoomScale;
	double posX;
	double posY;
	int win_width;
	int win_height;
	double JuliaX;
	double JuliaY;

	Simulation();
	Simulation(sf::RenderWindow& win);

	void simulate();

	void render(sf::RenderWindow& win);

	void updateWindow(sf::RenderWindow& win);
	void updateWindow(int x, int y);

	int inputHandler(sf::Event event, sf::RenderWindow& win);

	//map number to a scale to another 
	static double mapScale(double const& val, double const& x0, double const& x1, double const& y0, double const& y1) {
		return ((y1 - y0) / (x1 - x0)) * (val - x0) + y0;
	}

	//tools function
	static sf::Color colorOfNumber(int const& valI, int const& val2I);
	static sf::Color HSLtoRGB(double hueI, double const satI, double const darkI, double const alphaI=1);
};





#endif