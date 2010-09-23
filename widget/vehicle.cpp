#include "vehicle.h"
#include "scene.h"

#define _USE_MATH_DEFINES 1
#include <cmath>

using namespace std;

Vehicle::Vehicle():obj("VEHICLE"), orient(0)
{}

void Vehicle::llegirModel (const char* filename)
{
  obj.readObj(filename, Scene::matlib);
  obj.updateBoundingBox();
  Box box=obj.boundingBox();
  
  obj.CalculaNormals();
  obj.CalculaNormalsVertex();

   // Aqui cal que inicialitzeu correctament la resta d'atributs del vehicle

  pos = (box.minb + box.maxb)/2.0;
  escalat = 1.0;
  initialOrient = 0.0;
  orient = 0.0;
}

void Vehicle::setPos (Point p)
{
  pos=p;
}

Point Vehicle::getPos()
{
  return pos;
}

Point Vehicle::getCenter()
{
  Box box=obj.boundingBox();
  Point center(pos);
  center.y += escalat*(box.maxb.y - box.minb.y)/2.0;
  
  if (!doTurn) return center;
  
  double clockwise = 1.0;
  if (turnAlpha > 0.0) clockwise = -1.0;
  double radius = (pos - turnPoint).length();
  double angle = turnAlpha + getOrientation();
  center.x = turnPoint.x + clockwise*sin((angle*M_PI)/180.0)*radius;
  center.z = turnPoint.z + clockwise*cos((angle*M_PI)/180.0)*radius;
  
  return center;
}

void Vehicle::setInitialOrientation(float o)
{
  orient -= initialOrient;
  initialOrient=o;
  orient += initialOrient;

  if (initialOrient >= 360.0) initialOrient -= 360.0;
  else if (initialOrient < 0.0) initialOrient += 360.0;
  
  if (orient >= 360.0) orient -= 360.0;
  else if (orient < 0.0) orient += 360.0;
}

void Vehicle::setOrientation(float o)
{
  orient=initialOrient + o;
  
  if (orient >= 360.0) orient -= 360.0;
  else if (orient < 0.0) orient += 360.0;
}

float Vehicle::getOrientation()
{ 
  double orientation = orient-initialOrient;

  if (orientation >= 360.0) orientation -= 360.0;
  else if (orientation < 0.0) orientation += 360.0;

  return orientation;
}

void Vehicle::setSize(double size)
{
  obj.updateBoundingBox();
  Box box=obj.boundingBox();
  
  escalat = size / (box.maxb.x - box.minb.x);
}

double Vehicle::getSize()
{
  obj.updateBoundingBox();
  Box box=obj.boundingBox();
  
  return escalat*(box.maxb.x - box.minb.x);
}

void Vehicle::turn(bool doTurn, double alpha, Point point)
{
  this->doTurn = doTurn;
  turnAlpha = (float)alpha;
  turnPoint = point;
}

void Vehicle::Render (bool smooth)
{
  // Cal aplicar les transformacions de model necessàries i pintar l'objecte
  Box box=obj.boundingBox();
  
  Point initialPos = (box.minb + box.maxb)/2.0;
  initialPos.y = box.minb.y;
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  if (doTurn)
  {
    glTranslatef(turnPoint.x, turnPoint.y, turnPoint.z);
    glRotatef(turnAlpha, 0.0, 1.0, 0.0);
    glTranslatef(-turnPoint.x, -turnPoint.y, -turnPoint.z);
  }
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(orient, 0.0, 1.0, 0.0);
  glScalef(escalat, escalat, escalat);
  glTranslatef(-initialPos.x, -initialPos.y, -initialPos.z);
  obj.Render(smooth);
  glPopMatrix();
}

