#ifndef _Vehicle_H_
#define _Vehicle_H_
#include "object.h"
#include <QtOpenGL/qgl.h>

class Vehicle
{
  friend class Scene;

 private:
  Object obj;
  Point pos;         // posici� del centre de la base del vehicle
  double escalat;    // escalat (homogeni) que cal aplicar-li en pintar-lo
  float initialOrient;        // orientaci� inicial (en graus) respecte l'eix Y
  float orient;        // orientaci� (en graus) respecte l'eix Y
 
  bool doTurn;
  float turnAlpha;
  Point turnPoint;
  
public:
  Vehicle();

  // carrega l'objecte
  void llegirModel (const char* filename); 

  // assigna la posici� del vehicle
  void setPos (Point);
  Point getPos();
  Point getCenter();
 
  // assigna l'orientaci� del vehicle
  void setInitialOrientation (float);
  float getInitialOrientation ();
  void setOrientation (float);
  float getOrientation();

  // assinga el tamany del vehicle
  void setSize(double size);
  double getSize();

  void turn(bool doTurn, double alpha = 0.0, Point point = Point(0, 0, 0));

  void Render (bool smooth);
  
};

#endif
