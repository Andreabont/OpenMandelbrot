#ifndef LIBFUNCTIONS_H
#define LIBFUNCTIONS_H

#include <complex>
#include <SFML/Graphics.hpp>

inline std::complex<double> fractal_mandelbrot(std::complex<double> z, std::complex<double> c) {
   return (z * z) + c; 
}

inline std::complex<double> fractal_triple_mandelbrot(std::complex<double> z, std::complex<double> c) {
   return (z * z * z) + c; 
}

inline std::complex<double> fractal_quadruple_mandelbrot(std::complex<double> z, std::complex<double> c) {
   return (z * z * z * z) + c; 
}

inline std::complex<double> fractal_quintuple_mandelbrot(std::complex<double> z, std::complex<double> c) {
   return (z * z * z * z * z) + c; 
}

inline sf::Color render_linear(int iteration_number, int max_iterations) {
	int N = 256; // colors per element
	int N3 = N * N * N;
	double t = (double)iteration_number/(double)max_iterations;
	// expand n on the 0 .. 256^3 interval (integers)
	iteration_number = (int)(t * (double) N3);

	int b = iteration_number/(N * N);
	int nn = iteration_number - b * N * N;
	int r = nn/N;
	int g = nn - r * N;
	return sf::Color(r, g, b);
}

inline sf::Color render_smooth(int iteration_number, int max_iterations) {
	double t = (double)iteration_number/(double)max_iterations;
	// Use smooth polynomials for r, g, b
	int r = (int)(9*(1-t)*t*t*t*255);
	int g = (int)(15*(1-t)*(1-t)*t*t*255);
	int b =  (int)(8.5*(1-t)*(1-t)*(1-t)*t*255);	
	return sf::Color(r, g, b);
}

inline sf::Color render_black_and_white(int iteration_number, int max_iterations) {
	double t = (double)iteration_number/(double)max_iterations;
	return (t != 1)? sf::Color(0, 0, 0) : sf::Color(255, 255, 255);
}

inline sf::Color render_green_gradient(int iteration_number, int max_iterations) {
	double t = (double)iteration_number/(double)max_iterations;
    if(t == 1) return sf::Color(0, 0, 0);
	return (t > 0.5)? sf::Color(t*255, 255, t*255) : sf::Color(0, t*255, 0);
}

#endif //LIBFUNCTIONS_H
