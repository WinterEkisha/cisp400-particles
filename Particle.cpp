#include "Particle.h"
#include "Matrices.h"




int getRandInt(const int min, const int max) {
  return (std::rand() % (max - min + 1)) + min;
}


sf::Color getRandColor() {
  return sf::Color(getRandInt(0, 255), getRandInt(0, 255), getRandInt(0, 255));
}


Particle::Particle(RenderTarget &target, int numPoints, Vector2i mouseClickPosition)
    : m_ttl(TTL), m_numPoints(numPoints), m_radiansPerSec(getRandInt(0, 1) * M_PI), m_vx(getRandInt(100, 500)),
      m_vy(getRandInt(100, 500)), m_color1(WHITE), m_color2(getRandColor()), m_A(2, numPoints) {

  m_cartesianPlane.setCenter(0, 0);
  m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
  m_centerCoordinate =
      target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

  double theta = getRandInt(0, 1) * M_PI / 2;
  const float dTheta = 2 * M_PI / (numPoints - 1);

  for (size_t i = 0; i < numPoints; i++) {
    float r = getRandInt(20, 80);
    float dx = r * cos(theta);
    float dy = r * sin(theta);

    m_A(0, i) = m_centerCoordinate.x + dx;
    m_A(1, i) = m_centerCoordinate.y + dy;
    theta += dTheta;
  }
};

void Particle::draw(RenderTarget &target, RenderStates states) const{
	sf::VertexArray lines(sf::TriangleFan, m_numPoints + 1);                                              
	sf::Vector2f center = target.mapPixelToCoords((sf::Vector2i) m_centerCoordinate, m_cartesianPlane);   
	//sf::Vector2f center = target.mapPixelToCoords(sf::Vector2i (1, 1), m_cartesianPlane);
	lines[0].position = center;
	lines[0].color = m_color2;
	for (int i = 1; i <= m_numPoints; i++){
		sf::Vector2i worldPosition(m_A(0, i-1), m_A(1, i-1));
		lines[i].position = target.mapPixelToCoords(worldPosition, m_cartesianPlane);
		lines[0].color = m_color2;
	}
	target.draw(lines);
}

void Particle::update(float dt) {
	m_ttl -= dt;
	rotate(dt* m_radiansPerSec);
	scale(SCALE);
	float dx = (m_vx * dt), dy;
	m_vy -= G * dt;
	dy = m_vy * dt;
	translate(dx, dy);	
}

void Particle::rotate(double theta) {
  const Matrices::RotationMatrix rotateMatrix(theta);
  const Vector2f tempCoord = m_centerCoordinate;

  translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
  m_A = rotateMatrix * m_A;
  translate(tempCoord.x, tempCoord.y);
};


void Particle::scale(double c) {
  const Matrices::ScalingMatrix scaleMatrix(c);
  const Vector2f tempCoord = m_centerCoordinate;

  translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
  m_A = scaleMatrix * m_A;
  translate(tempCoord.x, tempCoord.y);
};


void Particle::translate(double xShift, double yShift) {
  const Matrices::TranslationMatrix transMatrix(xShift, yShift, m_numPoints);

  m_A = transMatrix + m_A;
  m_centerCoordinate.x += xShift;
  m_centerCoordinate.y += yShift;
};


bool Particle::almostEqual(double a, double b, double eps) {
  return fabs(a - b) < eps;
}


void Particle::unitTests() {
  int score = 0;

  cout << "Testing RotationMatrix constructor...";
  double theta = M_PI / 4.0;
  RotationMatrix r(M_PI / 4);
  if (r.getRows() == 2 && r.getCols() == 2 &&
      almostEqual(r(0, 0), cos(theta)) && almostEqual(r(0, 1), -sin(theta)) &&
      almostEqual(r(1, 0), sin(theta)) && almostEqual(r(1, 1), cos(theta))) {
    cout << "Passed.  +1" << endl;
    score++;
  } else {
    cout << "Failed." << endl;
  }

  cout << "Testing ScalingMatrix constructor...";
  ScalingMatrix s(1.5);
  if (s.getRows() == 2 && s.getCols() == 2 && almostEqual(s(0, 0), 1.5) &&
      almostEqual(s(0, 1), 0) && almostEqual(s(1, 0), 0) &&
      almostEqual(s(1, 1), 1.5)) {
    cout << "Passed.  +1" << endl;
    score++;
  } else {
    cout << "Failed." << endl;
  }

  cout << "Testing TranslationMatrix constructor...";
  TranslationMatrix t(5, -5, 3);
  if (t.getRows() == 2 && t.getCols() == 3 && almostEqual(t(0, 0), 5) &&
      almostEqual(t(1, 0), -5) && almostEqual(t(0, 1), 5) &&
      almostEqual(t(1, 1), -5) && almostEqual(t(0, 2), 5) &&
      almostEqual(t(1, 2), -5)) {
    cout << "Passed.  +1" << endl;
    score++;
  } else {
    cout << "Failed." << endl;
  }


  cout << "Testing Particles..." << endl;
  cout << "Testing Particle mapping to Cartesian origin..." << endl;
  if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0) {
    cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x
         << "," << m_centerCoordinate.y << ")" << endl;
  } else {
    cout << "Passed.  +1" << endl;
    score++;
  }

  cout << "Applying one rotation of 90 degrees about the origin..." << endl;
  Matrix initialCoords = m_A;
  rotate(M_PI / 2.0);
  bool rotationPassed = true;
  for (int j = 0; j < initialCoords.getCols(); j++) {
    if (!almostEqual(m_A(0, j), -initialCoords(1, j)) ||
        !almostEqual(m_A(1, j), initialCoords(0, j))) {
      cout << "Failed mapping: ";
      cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j)
           << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
      rotationPassed = false;
    }
  }
  if (rotationPassed) {
    cout << "Passed.  +1" << endl;
    score++;
  } else {
    cout << "Failed." << endl;
  }

  cout << "Applying a scale of 0.5..." << endl;
  initialCoords = m_A;
  scale(0.5);
  bool scalePassed = true;
  for (int j = 0; j < initialCoords.getCols(); j++) {
    if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) ||
        !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j))) {
      cout << "Failed mapping: ";
      cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j)
           << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
      scalePassed = false;
    }
  }
  if (scalePassed) {
    cout << "Passed.  +1" << endl;
    score++;
  } else {
    cout << "Failed." << endl;
  }

  cout << "Applying a translation of (10, 5)..." << endl;
  initialCoords = m_A;
  translate(10, 5);
  bool translatePassed = true;
  for (int j = 0; j < initialCoords.getCols(); j++) {
    if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) ||
        !almostEqual(m_A(1, j), 5 + initialCoords(1, j))) {
      cout << "Failed mapping: ";
      cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j)
           << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
      translatePassed = false;
    }
  }
  if (translatePassed) {
    cout << "Passed.  +1" << endl;
    score++;
  } else {
    cout << "Failed." << endl;
  }

  cout << "Score: " << score << " / 7" << endl;
}
