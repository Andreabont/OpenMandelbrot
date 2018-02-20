#ifndef LIBFRACTAL_H
#define LIBFRACTAL_H

#include <functional>
#include <complex>
#include <SFML/Graphics.hpp>
#include "functions.hpp"

struct Domain {
    
    double x_min, x_max, y_min, y_max;
    
    Domain(double x_min, double x_max, double y_min, double y_max) : x_min(x_min), x_max(x_max), y_min(y_min), y_max(y_max) {}
    
    double width() {
        return x_max - x_min;
    }
    
    double height() {
        return y_max - y_min;
    }
    
    double size() {
        return width() * height();
    }
    
    double ratio() {
        return width() / height();
    }
    
    void setRatio(double ratio) {
        double factor = (width() / ratio) / 2;
        double w = (y_max + y_min) / 2;
        y_min = w - factor;
        y_max = w + factor;
    }
    
    void zoom(double factor) {
        
        double x_frac = width() / 2;
        double y_frac = height() / 2;

        double x_factor = x_frac - (x_frac * factor);
        double y_factor = y_frac - (y_frac * factor);

        x_min -= x_factor;
        y_min -= y_factor;
        
        x_max += x_factor;
        y_max += y_factor;
        
    }
    
    void move(double x, double y) {
        
        double deltax = x * (x_max - x_min);
        x_min += deltax;
        x_max += deltax;
        
        double deltay = y * (y_max - y_min);
        y_min += deltay;
        y_max += deltay;
        
    }
    
    void centralize(std::complex<double> point) {

        double x_frac = width() / 2;
        double y_frac = height() / 2;
        
        x_min = point.real() - x_frac;
        y_min = point.imag() - y_frac;
        
        x_max = point.real() + x_frac;
        y_max = point.imag() + y_frac;
        
    }
    
};

class Fractal {
    
private:
    
    int image_width, image_height;
    std::function<std::complex<double>(std::complex<double>, std::complex<double>)> fractal_function;
    std::function<sf::Color(int iteration_number, int max_iterations)> render_function;
    sf::Image frame;
    bool hasChanged;
    Domain domain;
    
    std::complex<double> scale_point(std::complex<double> point);
    int compute_point(std::complex<double> point, int max_iterations);
    int compute_max_iterations(int window_width, double domain_width);
    
public:
    
    Fractal(int image_width,
            int image_height,
            Domain domain,
            std::function<std::complex<double>(std::complex<double>, std::complex<double>)> fractal_function = fractal_mandelbrot,
            std::function<sf::Color(int iteration_number, int max_iterations)> render_function = render_smooth
           );
    
    void setFractalFunction(std::function<std::complex<double>(std::complex<double>, std::complex<double>)> fractal_function);
    
    void setRenderFunction(std::function<sf::Color(int iteration_number, int max_iterations)> render_function);
    
    void moveTo(int x, int y);
    
    void moveBy(double x, double y);
    
    void zoom(double factor, bool invert = false);
    
    sf::Image getFrame();
    
    Domain& getDomain() {
        return this->domain;
    }
    
};

#endif //LIBFRACTAL_H
