#include "Fractal.hpp"
#include <chrono>
#include <cmath>
#include <iostream>

Fractal::Fractal(int image_width, int image_height, Domain domain, std::function<std::complex<double> (std::complex<double>, std::complex<double>)> fractal_function, std::function<sf::Color(int iteration_number, int max_iterations)> render_function) : domain(domain) {
    this->fractal_function = fractal_function;
    this->render_function = render_function;
    this->image_height = image_height;
    this->image_width = image_width;
    this->frame.create(image_width, image_height, sf::Color(0, 0, 0));
    this->hasChanged = true;
}

void Fractal::setFractalFunction(std::function<std::complex<double> (std::complex<double>, std::complex<double>)> fractal_function) {
    this->fractal_function = fractal_function;
    this->hasChanged = true;
}

void Fractal::setRenderFunction(std::function<sf::Color (int iteration_number, int max_iterations)> render_function) {
    this->render_function = render_function;
    this->hasChanged = true;
}

int Fractal::compute_point(std::complex<double> point, int max_iterations) {
    
    std::complex<double> z(0);
    int iter = 0;
    
    while (abs(z) < 2.0 && iter < max_iterations) {
        z = this->fractal_function(z, point);
        iter++;
    }

    return iter;
    
}

std::complex<double> Fractal::scale_point(std::complex<double> point) {
    std::complex<double> aux(point.real() / (double)this->image_width * this->domain.width() + this->domain.x_min, point.imag() / (double)this->image_height * this->domain.height() + domain.y_min);
    return aux;
}

sf::Image Fractal::getFrame(){
    
    if (this->hasChanged) {
        
        std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
        
        int max_iterations = compute_max_iterations(this->image_width, this->domain.width());
        
        #pragma omp parallel for
        for(int y = 0; y < this->image_height; y++) {
            for(int x = 0; x < this->image_width; x++) {
                std::complex<double> point(x, y);
                point = scale_point(point);
                int iterations = compute_point(point, max_iterations);
                sf::Color color = this->render_function(iterations, max_iterations);
                this->frame.setPixel(x, y, color);
            }  
        }
        
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        
        auto t = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        std::cerr << "Frame> Time: " << t << "ms, Max iterations: " << max_iterations << std::endl;
        
        this->hasChanged = false;
    }
    
    return this->frame;
    
}

void Fractal::moveTo(int x, int y) {
    std::complex<double> point(x, y);
    point = this->scale_point(point);
    this->domain.centralize(point);
    this->hasChanged = true;
}

void Fractal::moveBy(double x, double y) {
    this->domain.move(x, y);
    this->hasChanged = true;    
}

void Fractal::zoom(double factor, bool invert) {
    this->domain.zoom((invert)?1/factor:factor);
    this->hasChanged = true;
}

int Fractal::compute_max_iterations(int window_width, double domain_width) {
    int max = 50 * std::pow(std::log10(window_width / domain_width), 1.25);
    return (max > 0)? max : 0;
}



