#include "scene.h"
#include "XML.h"
#include <math.h>
#include "box.h"

int Scene::DRETA=0;
int Scene::ESQUERRA=1;
int Scene::RECTA=2;
int Scene::ARBRE=3;
int Scene::PEDRES=4;
int Scene::CASA=5;
int Scene::DOBLE=6;
int Scene::TRIPLE=7;
int Scene::FANAL=8;

MaterialLib Scene::matlib;

Scene::Scene():stopped(0), speed(0), smooth(false)
{}

void Scene::Init()
{
  XML xmlloader;
  xmlloader.LoadSceneDescription(this, "../data/escena.xml");
  
  int n = lobjects.size();
  
  for (int i = 0; i < n; i++)
    lobjects[i].CalculaNormals();
  
  for (int i = 0; i < n; i++)
    lobjects[i].CalculaNormalsVertex();
}


// Modifica un punt donat aplicant-li la transformació que es té guardada
// a la matriu MODELVIEW
void Scene::transformPoint(Point &p)
{
  GLfloat m[16];
  GLfloat p1[16];

  int i; 
  for (i=4; i<16; ++i) p1[i]=0.0f;

  p1[0] = p.x; 
  p1[1] = p.y; 
  p1[2] = p.z; 
  p1[3] = 1;

  glPushMatrix();
  glMultMatrixf(p1);
  glGetFloatv(GL_MODELVIEW_MATRIX, m);
  p.x = m[0];
  p.y = m[1];
  p.z = m[2];
  glPopMatrix();
}

// Mètode que pinta l'escena: tots els objectes i el vehicle
void Scene::Render(bool carLoaded, bool firstPersonView)
{
  // Cal pintar tots els objectes de l'escena que es troben als vectors
  // Cal pintar també el vehicle, si s'escau

  int n = lreferencies.size();
  for (int i = 0; i < n; i++) lreferencies[i].Render(lobjects, smooth);
  
  n = circuit.size();
  for (int i = 0; i < n; i++) circuit[i].Render(lobjects, smooth);
  
  if (carLoaded && !firstPersonView) veh.Render(smooth);
}

void Scene::RenderLampposts()
{
  int n = lreferencies.size();
  for (int i = 0; i < n; i++)
    if (lreferencies[i].getObjectId() == FANAL)
    {
      glPushName(i);
      lreferencies[i].Render(lobjects, false);
      glPopName();
    }
}

void Scene::AddObject(Object &o)
{
  lobjects.push_back(o);
}

void Scene::AddObject(Referencia &oref)
{
  lreferencies.push_back(oref);
}

void Scene::AddObject(Tram &tref)
{
  circuit.push_back(tref);
}

void Scene::CalculaEsfera (double &radi, Point &centreEscena)
{
  Box capsaEscena = CalculaCapsa();
  centreEscena =  (capsaEscena.minb + capsaEscena.maxb)/2.;
  radi = (capsaEscena.maxb-capsaEscena.minb).length()/2.f;
}

Box Scene::CalculaCapsa()
{
  // creem la capsa inicialment amb els vèrtexs mínim i míxim del terra
  Box capsaEscena(Point (0.0, 0.0, 0.0), Point (0.0, 0.0, 0.0));
  Box bobj;
  // actualitzem la capsa amb tots els objectes de l'escena
  std::vector<Referencia>::iterator ito;
  for (ito=lreferencies.begin(); ito!=lreferencies.end(); ito++)
  {
    bobj=(*ito).computeTransformedBox(lobjects[(*ito).getObjectId()]);

    capsaEscena.update(bobj.minb);
    capsaEscena.update(bobj.maxb);
  }
  std::vector<Tram>::iterator itt;
  for (itt=circuit.begin(); itt!=circuit.end(); itt++)
  {
    bobj=(*itt).computeTransformedBox(lobjects[(*itt).getObjectId()]);

    capsaEscena.update(bobj.minb);
    capsaEscena.update(bobj.maxb);
  }

  return capsaEscena;
}

void Scene::openCar(const char* filename)
{
  veh.llegirModel(filename);
  
  Point tramPosition = circuit[0].getPosition();
  Vector tramSize = circuit[0].getSize();
  float tramOrientation = circuit[0].getOrientation();
  
  Point carPosition(tramPosition.x, tramPosition.y + tramSize.y, tramPosition.z);
  veh.setPos(carPosition);
  
  veh.setSize(tramSize.x/2.0);
  
  veh.setOrientation(tramOrientation);
  
  veh.turn(false);
  
  nextTram = circuit[0].getNextTram(direction);
  direction = circuit[0].getDirection();
  steps = STEPS / 2;
  stepSize = circuit[0].getSize().x / STEPS;
  doTurn = false;
  turnPoint = Point(0, 0, 0);
  nextPosition = carPosition;
  nextOrientation = tramOrientation;
}

void Scene::orientCar(float alpha)
{
  veh.setInitialOrientation(alpha);
}

void Scene::animation()
{
  if(!stopped)
    for (int speedCount = 0; speedCount < speed; speedCount++)
    {
      if (doTurn) turnAnimation();
      else normalAnimation();
    }
}

void Scene::normalAnimation()
{
  Point actualPos = veh.getPos();
  nextPosition = actualPos + stepSize*direction;
  
  if (++steps >= STEPS)
  {
    if (steps > STEPS) nextPosition = actualPos;
    Vector newDir;
    nextTram = circuit[nextTram].getNextTram(newDir);
    
    if (newDir.x != direction.x)
    {
      double sizeX = circuit[nextTram].getSize().x / 2.0;
      double sizeZ = circuit[nextTram].getSize().z / 2.0;
      turnPoint =  nextPosition + Point(newDir.x*sizeX, 0.0, newDir.z*sizeZ);
      nextPosition = turnPoint + Point(direction.x*sizeX, 0.0, direction.z*sizeZ);
      
      double angle = (-direction.x*newDir.z + direction.z*newDir.x)*90.0;
      nextOrientation = veh.getOrientation() + angle;
      
      direction = newDir;
      
      doTurn = true;
      stepSize = angle / STEPS;
    }
    else
    {
      veh.setOrientation(nextOrientation);
      veh.setPos(nextPosition);
    }
    steps = 0;
  }
  else
  {
    veh.setOrientation(nextOrientation);
    veh.setPos(nextPosition);
  }
}

void Scene::turnAnimation()
{
  veh.turn(true, steps*stepSize, turnPoint);
  if (++steps == STEPS)
  {
    doTurn = false;
    veh.turn(false);
    
    if (direction.x != 0) stepSize = circuit[nextTram].getSize().x / STEPS;
    else if (direction.z != 0) stepSize = circuit[nextTram].getSize().z / STEPS;
    
    veh.setOrientation(nextOrientation);
    veh.setPos(nextPosition);
  }
}

void Scene::stopAnimation(bool stop)
{
  stopped = stop;
}

bool Scene::animationIsStopped()
{
  return stopped;
}

void Scene::setCarSpeed(int newSpeed)
{
  speed = newSpeed;
}

Point Scene::getCarCenter()
{
  return veh.getCenter();
}

Vector Scene::getCarDirection()
{
  if (!doTurn) return direction;

  double radius = direction.length();
  double angle = steps*stepSize + veh.getOrientation();
  Vector dir;
  dir.x = -cos((angle*M_PI)/180.0)*radius;
  dir.y = direction.y;
  dir.z = sin((angle*M_PI)/180.0)*radius;
  
  return dir;
}

void Scene::activeSmooth(bool active)
{
  smooth = active;
}

int Scene::getNLampposts()
{
  int m = 0;
  int n = lreferencies.size();
  for (int i = 0; i < n; i++)
    if (lreferencies[i].getObjectId() == FANAL) m++;
    
  return m;
}

int Scene::getLightID(int lamppost)
{
  return lreferencies[lamppost].getLightID();
}

void Scene::turnOff(int lamppost)
{
  lreferencies[lamppost].turnOff();
}

Point Scene::turnOn(int lamppost, int lightID)
{
  return lreferencies[lamppost].turnOn(lightID);
}

double Scene::getTramSize()
{
  return circuit[0].getSize().x;
}
