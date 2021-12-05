
//les includes
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include "main.h"
#include "simulation.h"
//name space
using namespace sf;
using namespace std;

//prototypes des fonctions
void loadFont();
void SetText(Text& txt, string str, Color col, int size);

//constantes
const int WIN_WIDTH = 900;
const int WIN_HEIGHT = 900;

//variable
Font font; //from sfml

Simulation simulation;

int main()
{
    cout << "demarrage de l'application" << endl;
    //creation de la fenetre
    RenderWindow window(VideoMode(WIN_WIDTH, WIN_HEIGHT, 32), "Fractal Generator");
    window.setKeyRepeatEnabled(false);//pour éviter les trucs bisard quand on utilise sur le clavier
    //activation Vsinc
    window.setVerticalSyncEnabled(true);
    //chargement de la font pour pouvoir afficher des chose
    loadFont();

    
    simulation.updateWindow(window);
    vector<Color> listColor = {
        sf::Color(0, 215, 255),
        sf::Color(0, 110, 255),
        sf::Color(0, 0, 255),
        sf::Color(160, 0, 255),
        sf::Color(0, 0, 255),
    };
    simulation.updateColorList(0,listColor);
    simulation.simulate();

    while (window.isOpen()) {
        //gestion des inputs et des events
        Event event;
        bool needToReSimulate = false;
        while (window.pollEvent(event)) { //tant qu'on a un evenement on le boucle
            if (simulation.inputHandler(event, window) == 1) //gère les evenement et en fonction des evenement resumuler la fractal
            {
                needToReSimulate = true;
            }
        }
        if (needToReSimulate) { simulation.simulate(); }
        //couleur du font de la fenetre
        window.clear(Color::Black);

        simulation.render(window);

        window.display();
    }
    return 0;
}


//charger la police d'ecriture
void loadFont() {
    if (!font.loadFromFile("res/poppins.ttf")) {
        cout << "Ereur de charment de la font " << "res/poppins.ttf" << endl;
    }
}

//si y'a besoins d'afficher un texte c'est près avec une police d'écriture 
void SetText(Text& txt, string str = "", Color col = Color::White, int size = 26) {
    /*Text txt;
    SetText(txt, "Hello World", Color::Green, 50);
    txt.setStyle(Text::Bold | Text::Underlined);*/
    txt.setFont(font);
    txt.setString(str);
    txt.setFillColor(col);
    txt.setCharacterSize(size);
}

