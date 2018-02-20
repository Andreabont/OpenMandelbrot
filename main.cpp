#include <iostream>
#include <tuple>
#include <string>
#include <SFML/Graphics.hpp>
#include <boost/program_options.hpp>

#include <iomanip>
#include <ctime>
#include <chrono>

#include "functions.hpp"
#include "Fractal.hpp"

const double DOMAIN_X_MIN = -2.0;
const double DOMAIN_X_MAX = 2.0;
const double DOMAIN_Y_MIN = -1.7;
const double DOMAIN_Y_MAX = 1.7;
const int IMAGE_WIDTH = 1920;
const int IMAGE_HEIGHT = 1080;
const int FRAMERATE = 20;

int main(int argc, char **argv) {
    
    boost::program_options::options_description po ("Mandelbrot");

    po.add_options()
        ("help", "Help")
        ("noratio", "Don't force the window ratio on domain")
        ("framerate", boost::program_options::value<int>()->default_value(FRAMERATE), "Framerate")
        ("width", boost::program_options::value<int>()->default_value(IMAGE_WIDTH), "Window width")
        ("height", boost::program_options::value<int>()->default_value(IMAGE_HEIGHT), "Window height")
        ("domXmin", boost::program_options::value<double>()->default_value(DOMAIN_X_MIN), "Domain X (Min)")
        ("domXmax", boost::program_options::value<double>()->default_value(DOMAIN_X_MAX), "Domain X (Max)")
        ("domYmin", boost::program_options::value<double>()->default_value(DOMAIN_Y_MIN), "Domain Y (Min)")
        ("domYmax", boost::program_options::value<double>()->default_value(DOMAIN_Y_MAX), "Domain Y (Max)")
        ("filename", boost::program_options::value<std::string>(), "Filename")
    ;

    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, po), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << po << std::endl;
        return 0;
    }
    
    int screen_width = vm["width"].as<int>();
    int screen_height = vm["height"].as<int>();

    double windowRatio = (double)screen_width / (double)screen_height;
    
    Fractal mandelbrot(
        screen_width, 
        screen_height, 
        Domain(
            vm["domXmin"].as<double>(), 
            vm["domXmax"].as<double>(), 
            vm["domYmin"].as<double>(), 
            vm["domYmax"].as<double>()
        )
    );
    
    if(!vm.count("noratio")){
        std::cerr << "Force ratio " << windowRatio << std::endl;
        mandelbrot.getDomain().setRatio(windowRatio);
    }
    
    if(vm.count("filename")) {
        mandelbrot.getFrame().saveToFile(vm["filename"].as<std::string>());
        std::cerr << "Screenshot stored in file " << vm["filename"].as<std::string>() << std::endl;
        return 0;
    }
    
    sf::RenderWindow window(sf::VideoMode(screen_width, screen_height), "Mandelbrot");
    window.setFramerateLimit(vm["framerate"].as<int>());

    sf::Texture texture;
    sf::Sprite sprite;
    
    while (window.isOpen()) {
                
        sf::Event event;
        while (window.pollEvent(event)) {
            
            switch (event.type) {
                
                case sf::Event::Closed:
                    window.close();
                    break;
                    
                case sf::Event::MouseButtonReleased: {
                    
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
                    
                    switch (event.mouseButton.button) {
                        
                        case sf::Mouse::Left:
                            mandelbrot.moveTo(mousePosition.x, mousePosition.y);
                            break;
                            
                        case sf::Mouse::Right:
                            mandelbrot.moveTo(mousePosition.x, mousePosition.y);
                            break;
                            
                        default:
                            break;
                        
                    }
                    
                }
                    break;
                    
                case sf::Event::MouseWheelMoved:
                    
                    if(event.mouseWheel.delta > 0) {
                        mandelbrot.zoom(1.05);
                    } else {
                        mandelbrot.zoom(1.05, true);
                    }
                    
                    break;
                            
                    
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        
                        case sf::Keyboard::Escape:
                            window.close();
                            break;

                        case sf::Keyboard::Key::Add:
                            mandelbrot.zoom(1.5);
                            break;
                            
                        case sf::Keyboard::Key::Subtract:
                            mandelbrot.zoom(1.5, true);
                            break;
                            
                        case sf::Keyboard::Key::Right:
                            mandelbrot.moveBy(0.01, 0);
                            break;
                            
                        case sf::Keyboard::Key::Left:
                            mandelbrot.moveBy(-0.01, 0);
                            break;
                            
                        case sf::Keyboard::Key::Up:
                            mandelbrot.moveBy(0, -0.01);
                            break;
                            
                        case sf::Keyboard::Key::Down:
                            mandelbrot.moveBy(0, 0.01);
                            break;

                        case sf::Keyboard::Key::S: {
                            std::time_t t = std::time(nullptr);
                            std::tm tm = *std::localtime(&t);
                            std::stringstream filename;
                            filename << "Screenshot_" << std::put_time(&tm, "%Y%m%d%H%M%S") << ".jpeg";
                            mandelbrot.getFrame().saveToFile(filename.str());
                            std::cerr << "Screenshot stored in file " << filename.str() << std::endl;
                        }
                            break;
                            
                        case sf::Keyboard::Key::D: {
                            std::cerr  << std::scientific << std::setprecision(10) << "domXmin=" << mandelbrot.getDomain().x_min << ", domXmax=" << mandelbrot.getDomain().x_max << ", domYmin=" << mandelbrot.getDomain().y_min << ", domYmax=" << mandelbrot.getDomain().y_max << std::endl;
                        }
                            break;
                            
                        // Renderer selection
                        case sf::Keyboard::Key::Q:
                            std::cerr << "Selected smooth renderer" << std::endl;
                            mandelbrot.setRenderFunction(render_smooth);
                            break;
                            
                        case sf::Keyboard::Key::W:
                            std::cerr << "Selected linear renderer" << std::endl;
                            mandelbrot.setRenderFunction(render_linear);
                            break;
                            
                        case sf::Keyboard::Key::E:
                            std::cerr << "Selected black&white renderer" << std::endl;
                            mandelbrot.setRenderFunction(render_black_and_white);
                            break;

                        case sf::Keyboard::Key::R:
                            std::cerr << "Selected green gradient renderer" << std::endl;
                            mandelbrot.setRenderFunction(render_green_gradient);
                            break;                            
                            
                        // Fractal selection
                        case sf::Keyboard::Key::Num1:
                            std::cerr << "Selected mandelbrot algorithm" << std::endl;
                            mandelbrot.setFractalFunction(fractal_mandelbrot);
                            break;
                            
                        case sf::Keyboard::Key::Num2:
                            std::cerr << "Selected triple mandelbrot algorithm" << std::endl;
                            mandelbrot.setFractalFunction(fractal_triple_mandelbrot);
                            break;
                            
                        case sf::Keyboard::Key::Num3:
                            std::cerr << "Selected quadruple mandelbrot algorithm" << std::endl;
                            mandelbrot.setFractalFunction(fractal_quadruple_mandelbrot);
                            break;
                            
                        case sf::Keyboard::Key::Num4:
                            std::cerr << "Selected quintuple mandelbrot algorithm" << std::endl;
                            mandelbrot.setFractalFunction(fractal_quintuple_mandelbrot);
                            break;
                            
                        default:
                            break;
                            
                    }
                    
                default:
                    break;

            }
        }

        window.clear(sf::Color::Black);
        texture.loadFromImage(mandelbrot.getFrame());
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
        
    }
    
    return 0;

}
