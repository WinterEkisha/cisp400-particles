#include "Engine.h"

Engine::Engine() {
	m_Window.create(sf::VideoMode(1920, 1080), "fuck this shit");

}

void Engine::run(){
	sf::Clock frame;

	Particle test(m_Window, 26, sf::Vector2i( 100, 100));
	cout << "Starting Particle unit tests..." << endl;
    Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
    p.unitTests();
    cout << "Unit tests complete.  Starting engine..." << endl;
	while(m_Window.isOpen()){
		auto dt = frame.restart().asSeconds();
		input();
		update(dt);
		draw();
	}
}

void Engine::input() {


    Event event;
    while (m_Window.pollEvent(event)) {
      if (event.type == Event::Closed) {
        m_Window.close();
      }
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {

			for (int i = 0; i < 5; i++){
				//target, numPoints, click postion
				m_particles.push_back(Particle(m_Window,
				getRandInt(25, 50),
				Vector2i (event.mouseButton.x, event.mouseButton.y)));

			}
        }
      }
    }
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
      m_Window.close();
    }

}

void Engine::update(float dtAsSeconds){
	auto iter = m_particles.begin(); //m_particles[0]
	while(iter != m_particles.end()){
		if(iter->getTTL() > 0.0){
			iter->update(dtAsSeconds);
			iter++;
		}
		else{
			iter = m_particles.erase(iter);
		}
	}
}

void Engine::draw(){
	m_Window.clear();
	for(auto particle : m_particles){
		m_Window.draw(particle);
	}
	m_Window.display();
}

