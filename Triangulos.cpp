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
void dibujarFigura(vector<vector<punto> > figura,
        sf::RenderWindow &window, vector<sf::Color> colorScale=vector<sf::Color>()) {

    int n = figura[0].size() - 1;
    int colorScaleSize = colorScale.size();

    if (colorScaleSize != 0 && colorScaleSize != 3*n) {
        std::cerr << "Color Scale does not have the correct size: "
            << colorScaleSize << " n: " << n << "\n";
    }

    for (int i = 1; i <= n; i++) {
        if (colorScaleSize) {
            dibujarTriangulo(figura[0][i], figura[0][n], figura[1][i], window, colorScale[i]);
            dibujarTriangulo(figura[1][i], figura[1][n], figura[2][i], window, colorScale[i+1]);
            dibujarTriangulo(figura[2][i], figura[2][n], figura[0][i], window, colorScale[i+2]);
        } else {
            dibujarTriangulo(figura[0][i], figura[0][n], figura[1][i], window);
            dibujarTriangulo(figura[1][i], figura[1][n], figura[2][i], window);
            dibujarTriangulo(figura[2][i], figura[2][n], figura[0][i], window);
        }
    }
}


vector<punto> crearPunteado(punto inicial, punto final, float intervalos) {
    vector<punto> punteado;
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
    return punteado;
}

vector<vector<punto> > crearFigura(punto a0, punto b0, punto c0, float n) {
    vector<vector<punto> > figura(3, vector<punto>());

    figura[0] = crearPunteado(a0, b0, n);
    figura[1] = crearPunteado(b0, c0, n);
    figura[2] = crearPunteado(c0, a0, n);

    return figura;
}

int main() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(404, 404), "", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(20);
    sf::Color backgroundColor = sf::Color(21, 29, 45);


    punto a0(0, 0);
    punto b0(400, 0);
    punto c0(0, 400);
    punto d0(400, 400);

    float n = 35;

    vector<vector<punto> > figura1 = crearFigura(a0, b0, c0, n);
    vector<vector<punto> > figura2 = crearFigura(b0, d0, c0, n);

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

        dibujarFigura(figura1, window, grayScale);
        dibujarFigura(figura2, window, grayScale);

        window.display();
    }
}