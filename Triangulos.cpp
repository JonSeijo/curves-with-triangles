/*
Shapes experiment
Needs SFML for compiling

JonSeijo
*/

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

using std::vector;
typedef sf::Vector2<double> punto;
typedef sf::Vector2f puntoF;

void debug(vector<punto> vs) {
    for (auto v : vs) {
        std::cout << "(" << v.x << ", " << v.y << ")  ";
    }
    std::cout << "\n";
}

void dibujarTriangulo(punto p1, punto p2, punto p3, sf::RenderWindow &window, sf::Color fillColor=sf::Color(21, 29, 45)) {
    sf::ConvexShape triangulo;
    triangulo.setPointCount(3);

    triangulo.setPoint(0, puntoF(p1));
    triangulo.setPoint(1, puntoF(p2));
    triangulo.setPoint(2, puntoF(p3));

    triangulo.setFillColor(fillColor);
    triangulo.setOutlineColor(sf::Color(255, 255, 255));
    triangulo.setOutlineThickness(1);

    window.draw(triangulo);
}

// Estilo 1 del dibujo, curvas suaves
void dibujarFigura_1(vector<punto> AB, vector<punto> BC, vector<punto> CA,
        sf::RenderWindow &window, vector<sf::Color> colorScale=vector<sf::Color>()) {
    int n = AB.size() - 1;
    int colorScaleSize = colorScale.size();

    if (colorScaleSize != 0 && colorScaleSize != 3*n) {
        std::cerr << "Color Scale does not have the correct size: "
            << colorScaleSize << " n: " << n << "\n";
    }

    for (int i = 1; i <= n; i++) {
        if (colorScaleSize) {
            dibujarTriangulo(AB[i], AB[n], BC[i], window, colorScale[i]);
            dibujarTriangulo(BC[i], BC[n], CA[i], window, colorScale[i+1]);
            dibujarTriangulo(CA[i], CA[n], AB[i], window, colorScale[i+2]);
        } else {
            dibujarTriangulo(AB[i], AB[n], BC[i], window);
            dibujarTriangulo(BC[i], BC[n], CA[i], window);
            dibujarTriangulo(CA[i], CA[n], AB[i], window);
        }
    }
}


void crearPunteado(punto inicial, punto final, vector<punto> &punteado, float intervalos) {
    punteado.push_back(inicial);

    punto director = final - inicial;
    director.x /= intervalos;
    director.y /= intervalos;

    punto secuencial = inicial;

    for (int cant = 1; cant < intervalos; cant++) {
        secuencial.x += director.x;
        secuencial.y += director.y;
        punteado.push_back(secuencial);
    }

    punteado.push_back(final);
}

int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(404, 404), "Figuras", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(20);
    sf::Color backgroundColor = sf::Color(21, 29, 45);


    punto a0(0, 0);
    punto b0(400, 0);
    punto c0(0, 400);
    punto d0(400, 400);

    vector<punto> AB;
    vector<punto> BC;
    vector<punto> CA;

    vector<punto> BD;
    vector<punto> DC;
    vector<punto> CB;

    float n = 35;

    crearPunteado(a0, b0, AB, n);
    crearPunteado(b0, c0, BC, n);
    crearPunteado(c0, a0, CA, n);

    crearPunteado(b0, d0, BD, n);
    crearPunteado(d0, c0, DC, n);
    crearPunteado(c0, b0, CB, n);

    // Genero una escala de grises
    vector<sf::Color> grayScale;
    for (float i = 0; i < 3*n; i++) {
        float value = (i*255.0f) / (3*n);
        grayScale.push_back(sf::Color(value, value, value));
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        window.clear(backgroundColor);

        dibujarFigura_1(AB, BC, CA, window, grayScale);
        dibujarFigura_1(BD, DC, CB, window, grayScale);

        window.display();
    }
}