#ifndef _GLWIDGET_H_
#define _GLWIDGET_H_

#include <QtOpenGL/qgl.h>
#include <QKeyEvent>
#include <iostream>
#include <qstring.h>
#include <qfiledialog.h>
#include <qtimer.h>
#include <QtDesigner/QDesignerExportWidget>

#include "material_lib.h"
#include "point.h"
#include "scene.h"
#include "llum.h"
#include "finestraLlums.h"
#include <list>

#define ROTATION_FACTOR 0.2
#define ZOOM_FACTOR 0.1
#define PAN_FACTOR 0.03

#define DEFAULT_FOVY 60.0
#define FPV_FOVY 90.0

class QDESIGNER_WIDGET_EXPORT GLWidget : public QGLWidget
{
  Q_OBJECT

 public:
  GLWidget(QWidget * parent);

 signals:
  
  void carOpened(bool);
  void dialValueChanged(int);
  void activateAnimation(QString);
  void activateFirstPersonView(bool);
 
 public slots:

  // help - Ajuda per la terminal des de la que hem  engegat el programa.
  void help(void);

  // Afegiu aquí la declaració dels slots que necessiteu
  
  // Reestablir la camera per defecte
  void setDefaultCamera();
 
  // Obrir un dialeg per seleccionar un model pel vehicle i carregar-lo
  void openCar();
 
  // Establir una novia orientación pel vehicle
  void orientCar(int alpha);
 
  void timerDone();
  void setCarSpeed(int speed);
  void configureLight(llum* light);
  void showLightsWindow();
  void activeSmooth(bool active);
  void activeLocalViewer(bool active);
  void activeCullFace(bool active);
  
  void activeFirstPersonView(bool active);
  void setFPVzFar(int value);
  void activeCarLampposts(bool active);

 protected:
  // initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
  virtual void initializeGL();

  // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
  // Tot el que es dibuixa es dibuixa aqui.
  virtual void paintGL( void );
 
  // resizeGL() - Es cridat quan canvi la mida del widget
  virtual void resizeGL (int width, int height);

  // keyPressEvent() - Cridat quan es prem una tecla
  virtual void keyPressEvent(QKeyEvent *e);

  // mousePressEvent() i mouseReleaseEvent()
  virtual void mousePressEvent( QMouseEvent *e);
  virtual void mouseReleaseEvent( QMouseEvent *);
 
  // mouseMoveEvent() - Cridat quan s'arrosega el ratoli amb algun botó premut.
  virtual void mouseMoveEvent(QMouseEvent *e);
 
  // Calcular el parametres de la camera per defecte
  void computeDefaultCamera();
  
  // Configurar la matriu modelview segons els parametres de la camera
  void setModelview();
  
  // Configurar la matriu de projecció segons els parametres de la camera
  void setProjection();
  
  // Recalcular els plans de retallat de l'escena
  void computeCuttingPlanes();
  
  void saveCurrentCamera();
  void restoreOldCamera();
  
  // Configuracio dels llums
  void initializeLights();
  void configure(llum* light);
  void updateLightsPosition();
  
  void onOffLamppost(int xClick, int yClick); // Realitza el proces de seleccio i tractament del fanal seleccionat per l'usuari
  int getSelectedLamppost(int xClick, int yClick); // Retorna el fanal seleccionat per l'usuari mes proper al obs
  void onOffCarLampposts(); // Realitza el proces de seleccio i tractament dels fanals visibles pel vehicle
  list<int> getVisibleLampposts(); // Retorna com a maxim 4 fanals, els mes propers i visibles pel vehicle
  
  void updateModelView();
 
 private:

  // interaccio
  typedef  enum {NONE, ROTATE, ZOOM, PAN} InteractiveAction;
  InteractiveAction DoingInteractive;
 
  int   xClick, yClick;
  
  Scene scene;  // Escena a representar en el widget

  // Afegiu els atributs que necessiteu
  
  QTimer* timer;
  
  llum* L[8];
  finestraLlums lightsWindow;
  
  double fovy;
  double aspect;
  double initialZNear;
  double initialZNearLast;
  double zNear;
  double zNearLast;
  double zNearAux;
  double zNearAuxLast;
  double initialZFar;
  double initialZFarLast;
  double zFar;
  double zFarLast;
  double zFarAux;
  double zFarAuxLast;
  double maxFPVzFar;
  double zFarFPV;
  Point VRP;
  Point VRPLast;
  Point OBS;
  Point OBSLast;
  Vector UP;
  Vector UPLast;
  double distOBS;
  double distOBSLast;
  
  bool carLoaded;
  bool firstPersonView;
  int NLampposts;
  list<int> onLampposts; // Llista amb els fanals encesos per l'usuario en el moment actual
  list<int> onCarLampposts; // Llista amb els fanals encesos davant del vehicle (no inclou els encesos manualment)
  bool carLampposts; // Indica si cal o no activar els fanals automatics davant del vehicle
};

#endif
