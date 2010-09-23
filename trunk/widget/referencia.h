#ifndef _Referencia_H_
#define _Referencia_H_
#include <vector>
#include "point.h"
#include "object.h"
#include "box.h"


using namespace std;

class Referencia
{
 protected:
  int object;   // identificador de l'objecte
  Point pos;    // posicio sobre el terra
  Vector size;	   // mida en unitats
  float orientation;  //orientació respecte Y
  int lightID; // Llum associada en cas de ser de tipus FANAL

 public:
  Referencia(int idob, Point p, Vector sz, float ori);
  ~Referencia(void);

  void Render(std::vector<Object> &, bool smooth);
  int getObjectId();
  Box& computeTransformedBox(Object &obj);
  Point getPosition();
  Vector getSize();
  float getOrientation();
 
 /* Nomes s'utilitzen en cas de ser de tipus FANAL */
  int getLightID();
  void turnOff();
  Point turnOn(int light);
};

#endif
