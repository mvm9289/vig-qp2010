#ifndef _Scene_H_
#define _Scene_H_
#include "referencia.h"
#include "object.h"
#include "vehicle.h"
#include "tram.h"
#include <QtOpenGL/qgl.h>

#define STEPS 100

class Scene
{
  friend class XML;
  friend class Tram;
 private:
  // Això és per a millorar la cerca dels objectes dins del vector.
  static int DRETA;
  static int ESQUERRA;
  static int RECTA;
  static int ARBRE;
  static int PEDRES;
  static int CASA;
  static int DOBLE;
  static int TRIPLE;
  static int FANAL;

  
  // Tindrem un vector amb els models geomètrics dels objectes de l'escena
  // i els altres dos amb instàncies seves (instàncies o referències a 
  // objectes).
  // El vector circuit conté els trams de la carretera i el vector 
  // lreferencies tota la resta d'instàncies d'objectes de l'escena 
  // (arbres, cases, fanals, etc...)

  std::vector<Object> lobjects;
  std::vector<Referencia> lreferencies;
  std::vector<Tram> circuit;

  Vehicle veh;
  
  bool stopped;
  int nextTram;
  Vector direction;
  int steps;
  double stepSize;
  int speed;
  bool doTurn;
  Point turnPoint;
  Point nextPosition;
  double nextOrientation;
  
  bool smooth;
  
  
  void normalAnimation();
  void turnAnimation();

 public:
  static MaterialLib matlib;	  // col·lecció de materials 
  static void transformPoint(Point &p);

  Scene();

  void Init();
  void Render(bool carLoaded, bool firstPersonView);
  void RenderLampposts();
  void AddObject(Object &);
  void AddObject(Referencia &);
  void AddObject(Tram &);

  void CalculaEsfera (double &radi, Point &centreEscena);
  Box CalculaCapsa();

  // Carregar un nou model per al vehicle
  void openCar(const char* filename);

  // Establir una nova orientació al vehicle
  void orientCar(float alpha);
 
  void animation();
  void stopAnimation(bool stop);
  bool animationIsStopped();
  void setCarSpeed(int newSpeed);
  Point getCarCenter();
  Vector getCarDirection();
 
  void activeSmooth(bool active);
  
  double getTramSize();
  
  /* Tractament de fanals */
  int getNLampposts();
  int getLightID(int lamppost);
  void turnOff(int lamppost);
  Point turnOn(int lamppost, int lightID);
};

#endif

