#include "glwidget.h"

#include <stdlib.h>
#define _USE_MATH_DEFINES 1
#include <cmath>
#include <utility>
#include <queue>

typedef pair<unsigned int, unsigned int> Pair2uInts;
typedef priority_queue<Pair2uInts> PQueue2uInts;

// Constructora amb format per defecte
GLWidget::GLWidget(QWidget * parent):QGLWidget(parent)
{
  // per rebre events de teclat
  setFocusPolicy(Qt::ClickFocus);
  
  DoingInteractive = NONE;
  
  timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(timerDone()));
  
  carLoaded = false;
  firstPersonView = false;
  carLampposts = false;
}


// initializeGL() - Aqui incluim les inicialitzacions del contexte grafic.
void GLWidget::initializeGL()
{
  glClearColor(0.4f, 0.4f, 0.8f, 1.0f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_NORMALIZE);
  glEnable(GL_LIGHTING);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  initializeLights();
  // inicialitzem tambe l'escena
  scene.Init();
  // dimensions escena i camera inicial
  computeDefaultCamera();
  
  Box capsaEscena = scene.CalculaCapsa();
  Point a(capsaEscena.minb);
  Point b(capsaEscena.maxb.x, capsaEscena.minb.y, capsaEscena.maxb.z);
  zFarFPV = maxFPVzFar = (b - a).length();
  NLampposts = scene.getNLampposts();
  
  timer->start(40);
}

void GLWidget::setDefaultCamera()
{
  computeDefaultCamera();
  int viewPort[4];
  glGetIntegerv(GL_VIEWPORT, viewPort);
  resizeGL(viewPort[2], viewPort[3]);
  emit activateFirstPersonView(false);
}

void GLWidget::computeDefaultCamera()
{
  // Cal inicialitzar els parametres necessaris per a definir la camera
  // amb els valors que ens permetin obtenir la camera inicial desitjada

  activeFirstPersonView(false);
  
  double radius;
  Point center;
  scene.CalculaEsfera(radius, center);
  
  fovy = DEFAULT_FOVY;
  initialZNear = zNear = zNearAux = radius;
  initialZFar = zFar = zFarAux = 3.0*radius;
  
  VRP = center;
  double vCoord = (2.0*radius)/sqrt(3.0);
  OBS = VRP + Vector(vCoord, vCoord, vCoord);
  UP = Vector(0.0, 1.0, 0.0);
  
  distOBS = (VRP-OBS).length();
  
  setModelview();
}

void GLWidget::setModelview()
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(OBS.x, OBS.y, OBS.z, VRP.x, VRP.y, VRP.z, UP.x, UP.y, UP.z);
}

void GLWidget::setProjection()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(fovy, aspect, zNear, zFar);
}

// paintGL() - callback cridat cada cop que cal refrescar la finestra. 
void GLWidget::paintGL( void )
{ 
  // Esborrem els buffers
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  // dibuixar eixos aplicacio
  glBegin(GL_LINES);
  glColor3f(1,0,0); glVertex3f(0,0,0); glVertex3f(20,0,0); // X
  glColor3f(0,1,0); glVertex3f(0,0,0); glVertex3f(0,20,0); // Y
  glColor3f(0,0,1); glVertex3f(0,0,0); glVertex3f(0,0,20); // Z
  glEnd();
  
  updateLightsPosition();
  
  // pintem l'escena
  scene.Render(carLoaded, firstPersonView);
}

// resizeGL() - Cridat quan es canvia el tamany del viewport.
void GLWidget::resizeGL (int width, int height)
{
  glViewport (0, 0, width, height);

  aspect = (double)width / (double)height;
  fovy = DEFAULT_FOVY;
  if (firstPersonView) fovy = FPV_FOVY;
  
  // Si height > width recalculem fovy segons fovx = DEFAULT_FOVY
  if (aspect < 1) fovy = ((atan(tan((fovy*M_PI)/360.0)/aspect))*360.0)/M_PI;
  
  setProjection();
}

// help() - Surt per la terminal des de la que hem 
// engegat el programa. En la versio amn interficie 
// hauria de crear una nova finestra amb la informacio...
void GLWidget::help( void ){
  cout<<"Tecles definides: \n";
  cout<<"f                     Pinta en filferros\n";
  cout<<"s                     Pinta amb omplert de poligons\n";
  cout<<"h,H,?                 Imprimir aquesta ajuda\n";
  // Completar amb altres tecles que definiu...
  //
  cout <<"Left_Click            Rotar l'escena\n";
  cout <<"SHIFT + Left_Click    Fer zoom a l'escena\n";
  cout <<"SHIFT + Right_Click   Fer pan a l'escena\n";
  cout << "SPACE                 Activar/Desactivar l'animacio\n";
}

/*--------------------
*
*  teclat()
*
*  callback per quan hom prem una tecla.
*
*/
void GLWidget::keyPressEvent(QKeyEvent *e)
{
  switch( e->key() ) 
  {	    
    case Qt::Key_F: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case Qt::Key_S: glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
    case Qt::Key_Space:
      if (carLoaded)
      {
        if (scene.animationIsStopped())
        {
          scene.stopAnimation(false);
          emit activateAnimation("ANIMACIO ACTIVADA");
        }
        else
        {
          scene.stopAnimation(true);
          emit activateAnimation("ANIMACIO DESACTIVADA");
        }
      }
      break;
    case  'h'  : case  'H'  : case  '?'  :  help();
      break;
    default: e->ignore(); // el propaguem cap al pare...
  }

}

/*--------------------
* mousePressEvent()
*/
void GLWidget::mousePressEvent( QMouseEvent *e){
  xClick = e->x();
  yClick = e->y();
  
  if (e->button()&Qt::LeftButton && e->modifiers()&Qt::ShiftModifier && e->modifiers()&Qt::ControlModifier)
  {
    onOffLamppost(e->x(), e->y());
  }
  else if (e->button()&Qt::LeftButton && 
      ! (e->modifiers()&(Qt::ShiftModifier|Qt::AltModifier|Qt::ControlModifier)))
  {
    DoingInteractive = ROTATE;
  }
  else if (e->button()&Qt::LeftButton &&  e->modifiers() &Qt::ShiftModifier)
  {
    DoingInteractive = ZOOM;
  }
  else if (e->button()&Qt::RightButton &&  e->modifiers() &Qt::ShiftModifier)
  {
    DoingInteractive = PAN;
  }
}

/*--------------------
*
* mouseReleaseEvent()
*
* Callback Qt cridat quan es deixa anar el boto del
* ratoli
*
*/
void GLWidget::mouseReleaseEvent( QMouseEvent *){
  DoingInteractive = NONE;
}

/*--------------------
*
* mouseMoveEvent()
*
* Callback Qt cridat quan s'arrosega el ratoli amb
* algun boto premut.
*
*/
void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
  // Aqui cal que es calculi i s'apliqui la rotacio, el zoom o el pan
  // com s'escaigui...
  if (firstPersonView) return;
  
  double incx = (xClick - e->x());
  double incy = (yClick - e->y());
  
  /****************************************************************************/
  /****************************************************************************/
  /* Totes les tranformacions aplicades en Sistema de Coordenades d'Observador*/
  /****************************************************************************/
  /****************************************************************************/
  
  float modelview[4][4];
  glGetFloatv(GL_MODELVIEW_MATRIX , &modelview[0][0]);
  
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotacio
    
    incx *= ROTATION_FACTOR;
    incy *= ROTATION_FACTOR;
    
    glTranslatef(0.0, 0.0, -distOBS);
    glRotatef(-incx, 0.0, 1.0, 0.0);
    glRotatef(-incy, 1.0, 0.0, 0.0);
    glTranslatef(0.0, 0.0, distOBS);
  }
  else if (DoingInteractive == ZOOM)
  {
    // Fem el zoom
    
    incy *= ZOOM_FACTOR;
    
    distOBS -= incy;
    
    glTranslatef(0.0, 0.0, incy);
    
    
    /***********************************/
    /* Recalcul dels plans de retallat */
    /***********************************/
    
    zNearAux -= incy;
    zFarAux -= incy;
    
    computeCuttingPlanes();
    
    initialZNear -= incy;
    initialZFar -= incy;
  }
  else if (DoingInteractive==PAN)
  {  
    // Fem el pan
    
    incx *= PAN_FACTOR;
    incy *= PAN_FACTOR;
    
    glTranslatef(-incx, incy, 0.0);
    
    
    /***********************************/
    /* Recalcul dels plans de retallat */
    /***********************************/
    
    Vector xobs(modelview[0][0], modelview[1][0], modelview[2][0]);
    Vector yobs(modelview[0][1], modelview[1][1], modelview[2][1]);
    
    VRP += (incx*xobs - incy*yobs);
    
    double radius;
    Point center;
    scene.CalculaEsfera(radius, center);
    
    double distance = (VRP - center).length();
    zNearAux = initialZNear - distance;
    zFarAux = initialZFar + distance;
    zFarAux = initialZFar + distance;
    
    computeCuttingPlanes();
  }   
  xClick = e->x();
  yClick = e->y();
  
  glMatrixMode(GL_MODELVIEW);
  glMultMatrixf(&modelview[0][0]);
}

void GLWidget::computeCuttingPlanes()
{
  if (zNearAux <= 0.0)
  {
    zNear = 0.001;
    zFar = (zFarAux - zNearAux) + zNear;
  }
  else
  {
    zNear = zNearAux;
    zFar = zFarAux;
  }
  
  setProjection();
}

void GLWidget::openCar()
{
  scene.stopAnimation(true);
  emit activateAnimation("ANIMACIO DESACTIVADA");
  
  // Obrir el dialeg per seleccionar un model pel vehicle
  QString filename = QFileDialog::getOpenFileName(this, "Selecciona un model per al cotxe...", "../data", "object (*.obj)");
  
  // Si s'ha seleccionat alg�n model, carregar el vechicle
  if (filename != "")
  {
    scene.openCar(filename.toLatin1().data());
    carLoaded = true;
    emit carOpened(true);
    emit dialValueChanged(0);
  }
}

void GLWidget::orientCar(int alpha)
{
  scene.orientCar((float)(-alpha));
}

void GLWidget::initializeLights()
{
  L[0] = new llum(GL_LIGHT0, "Llum de camera");
  L[0]->activada = true;
  configure(L[0]);
  
  lightsWindow.iniciar(L, 1);
  connect(&lightsWindow, SIGNAL(llumModificada(llum *)), this, SLOT(configureLight(llum *)));
  
  L[1] = new llum(GL_LIGHT1, "Lamppost1");
  L[2] = new llum(GL_LIGHT2, "Lamppost2");
  L[3] = new llum(GL_LIGHT3, "Lamppost3");
  L[4] = new llum(GL_LIGHT4, "Lamppost4");
  L[5] = new llum(GL_LIGHT5, "Lamppost5");
  L[6] = new llum(GL_LIGHT6, "Lamppost6");
  L[7] = new llum(GL_LIGHT7, "Lamppost7");
  for (int i = 1; i < 8; i++)
  {
    L[i]->activada = false;
    
    L[i]->referencia = ESCENA;
    
    L[i]->ambient[0]= 0;
    L[i]->ambient[1]= 0;
    L[i]->ambient[2]= 0;
    L[i]->ambient[3]= 0;

    L[i]->difos[0] = 1;
    L[i]->difos[1] = 0;
    L[i]->difos[2] = 0;
    L[i]->difos[3] = 1;

    L[i]->especular[0] = 0;
    L[i]->especular[1] = 0;
    L[i]->especular[2] = 0;
    L[i]->especular[3] = 0;
    
    L[i]->spotDireccio[0] = 0;
    L[i]->spotDireccio[1] = -1;
    L[i]->spotDireccio[2] = 0;
    L[i]->spotExponent = 0;
    L[i]->spotCutoff= 60;
    configure(L[i]);
  }
  
  for (int i = 4; i < 8; i++)
  {
    L[i]->difos[1] = 1;
    L[i]->difos[2] = 1;
    configure(L[i]);
  }
}

void GLWidget::timerDone()
{
  if (carLoaded)
  {
    scene.animation();
    if (carLampposts) onOffCarLampposts();
    if (firstPersonView) updateModelView();
  }
  updateGL();
}

void GLWidget::setCarSpeed(int speed)
{
  scene.setCarSpeed(speed);
}

void GLWidget::configureLight(llum* light)
{
  configure(light);
}

void GLWidget::configure(llum* light)
{
  if (light->referencia == OBSERVADOR)
  {
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glLightfv(light->id, GL_POSITION, light->posicio);
    glPopMatrix();
  }
  else glLightfv(light->id, GL_POSITION, light->posicio);
  
  glLightfv(light->id, GL_AMBIENT, light->ambient);
  glLightfv(light->id, GL_DIFFUSE, light->difos);
  glLightfv(light->id, GL_SPECULAR, light->especular);
  
  glLightfv(light->id, GL_SPOT_DIRECTION, light->spotDireccio);
  glLightf(light->id, GL_SPOT_EXPONENT, light->spotExponent);
  glLightf(light->id, GL_SPOT_CUTOFF, light->spotCutoff);
  
  glLightf(light->id, GL_CONSTANT_ATTENUATION, light->grau[0]);
  glLightf(light->id, GL_LINEAR_ATTENUATION, light->grau[1]);
  glLightf(light->id, GL_QUADRATIC_ATTENUATION, light->grau[2]);
  
  if (light->activada) glEnable(light->id);
  else glDisable(light->id);
}

// Torna a enviar a opengl la configuracio dels llums per actualitzar la posicio d'aquests
void GLWidget::updateLightsPosition()
{
  for(int i = 0; i < 8; i++) configure(L[i]);
}

void GLWidget::onOffLamppost(int xClick, int yClick)
{
  int lamppost = getSelectedLamppost(xClick, yClick);
  if (lamppost == -1) return;
  
  int lightID = scene.getLightID(lamppost);
  if (lightID >= 1 && lightID <= 3)
  {
    scene.turnOff(lamppost);
    L[lightID]->activada = false;
    configure(L[lightID]);
    onLampposts.remove(lamppost);
  }
  else
  {
    if (onLampposts.size() >= 3)
    {
      int oldLamppost = onLampposts.front();
      onLampposts.pop_front();
      lightID = scene.getLightID(oldLamppost);
      scene.turnOff(oldLamppost);
    }
    else
    {
      bool found = false;
      for (int i = 1; i < 4 && !found; i++)
        if (!(L[i]->activada))
        {
          lightID = i;
          found = true;
        }
    }
    Point lamppostPosition = scene.turnOn(lamppost, lightID);
    onLampposts.push_back(lamppost);
    L[lightID]->posicio[0] = lamppostPosition.x;
    L[lightID]->posicio[1] = lamppostPosition.y;
    L[lightID]->posicio[2] = lamppostPosition.z;
    L[lightID]->activada = true;
    configure(L[lightID]);
  }
}

int GLWidget::getSelectedLamppost(int xClick, int yClick)
{
  float projection[4][4];
  glMatrixMode(GL_PROJECTION);
  glGetFloatv(GL_PROJECTION_MATRIX, &projection[0][0]);
  glPushMatrix();
  glLoadIdentity();
  int viewPort[4];
  glGetIntegerv(GL_VIEWPORT, viewPort);
  gluPickMatrix(xClick, viewPort[3] - yClick, 3, 3, viewPort);
  glMultMatrixf(&projection[0][0]);
  
  unsigned int selectionBuffer[4*NLampposts];
  glSelectBuffer(4*NLampposts, selectionBuffer);
  
  glDisable(GL_LIGHTING);
  
  glRenderMode(GL_SELECT);
  
  glInitNames();
  
  scene.RenderLampposts();
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  
  glEnable(GL_LIGHTING);
  
  int hits = glRenderMode(GL_RENDER);
  if (!hits) return -1;
  int lamppostID = selectionBuffer[3];
  unsigned int minZ = selectionBuffer[1];
  hits--;
  for(int i = 5; hits > 0; i+=4, hits--)
    if (selectionBuffer[i] < minZ)
    {
      minZ = selectionBuffer[i];
      lamppostID = selectionBuffer[i+2];
    }
  
  return lamppostID;
}

void GLWidget::onOffCarLampposts()
{
  while (!onCarLampposts.empty())
  {
    int lightID = scene.getLightID(onCarLampposts.front());
    if (lightID >= 4 && lightID <= 7)
      scene.turnOff(onCarLampposts.front());
    onCarLampposts.pop_front();
  }
  for (int i = 4; i < 8; i++) L[i]->activada = false;
  
  onCarLampposts = getVisibleLampposts();
  int j = 4;
  if (!onCarLampposts.empty())
    for(list<int>::iterator i = onCarLampposts.begin(); i != onCarLampposts.end(); i++)
    {
      if (scene.getLightID(*i) == -1)
      {
        Point lamppostPosition = scene.turnOn(*i, j);
        L[j]->posicio[0] = lamppostPosition.x;
        L[j]->posicio[1] = lamppostPosition.y;
        L[j]->posicio[2] = lamppostPosition.z;
        L[j]->activada = true;
        configure(L[j]);
        j++;
      }
    }
  for(; j < 8; j++) configure(L[j]);
}

list<int> GLWidget::getVisibleLampposts()
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  Point center = scene.getCarCenter();
  Vector direction = scene.getCarDirection();
  gluLookAt(center.x, center.y, center.z,
    center.x + direction.x, center.y + direction.y, center.z + direction.z,
    UP.x, UP.y, UP.z);
  
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluPerspective(75.0, 1.0, 0.001, maxFPVzFar);
  
  unsigned int selectionBuffer[4*NLampposts];
  glSelectBuffer(4*NLampposts, selectionBuffer);
  
  glDisable(GL_LIGHTING);
  
  glRenderMode(GL_SELECT);
  
  glInitNames();
  
  scene.RenderLampposts();
  
  glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  
  glEnable(GL_LIGHTING);
  
  int hits = glRenderMode(GL_RENDER);
  PQueue2uInts pQueue;
  for(int i = 1; hits > 0; i+=4, hits--)
  {
    pQueue.push(Pair2uInts(selectionBuffer[i], selectionBuffer[i+2]));
    if (pQueue.size() > 4) pQueue.pop();
  }
  
  list<int> lampposts;
  while(!pQueue.empty())
  {
    lampposts.push_back(pQueue.top().second);
    pQueue.pop();
  }
  
  return lampposts;
}

void GLWidget::showLightsWindow()
{
  lightsWindow.show();
}

void GLWidget::activeSmooth(bool active)
{
  scene.activeSmooth(active);
}

void GLWidget::activeLocalViewer(bool active)
{
  if (active) glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  else glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
}

void GLWidget::activeCullFace(bool active)
{
  if (active) glEnable(GL_CULL_FACE);
  else glDisable(GL_CULL_FACE);
}

void GLWidget::activeFirstPersonView(bool active)
{
  if (active && !firstPersonView)
  {
    firstPersonView = true;
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    saveCurrentCamera();

    fovy = FPV_FOVY;
    zNear = 0.1*scene.getTramSize();
    zFar = zFarFPV;
    
    Point OBS = scene.getCarCenter();
    Vector direction = scene.getCarDirection();
    VRP.x = OBS.x + direction.x;
    VRP.y = OBS.y + direction.y;
    VRP.z = OBS.z + direction.z;
    
    int viewPort[4];
    glGetIntegerv(GL_VIEWPORT, viewPort);
    resizeGL(viewPort[2], viewPort[3]);
    
    setModelview();
    setProjection();
    
    emit activateFirstPersonView(true);
  }
  else if (!active && firstPersonView)
  {
    firstPersonView = false;
    
    restoreOldCamera();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    int viewPort[4];
    glGetIntegerv(GL_VIEWPORT, viewPort);
    resizeGL(viewPort[2], viewPort[3]);
    
    emit activateFirstPersonView(false);
  }
}

void GLWidget::activeCarLampposts(bool active)
{
  if (!carLampposts && active) carLampposts = true;
  else if (carLampposts && !active)
  {
      while (!onCarLampposts.empty())
      {
        int lightID = scene.getLightID(onCarLampposts.front());
        if (lightID >= 4 && lightID <= 7)
          scene.turnOff(onCarLampposts.front());
        onCarLampposts.pop_front();
      }
      for (int i = 4; i < 8; i++)
      {
        L[i]->activada = false;
        configure(L[i]);
      }
      
      carLampposts = false;
  }
}

void GLWidget::setFPVzFar(int value)
{
  zFar = zFarFPV = ((double)value*maxFPVzFar)/100.0;
  setProjection();
}

// En cas d'estar en primera persona, torna a situar la camara en el lloc adient
void GLWidget::updateModelView()
{
  OBS = scene.getCarCenter();
  Vector direction = scene.getCarDirection();
  VRP.x = OBS.x + direction.x;
  VRP.y = OBS.y + direction.y;
  VRP.z = OBS.z + direction.z;
  setModelview();
}

void GLWidget::saveCurrentCamera()
{
  initialZNearLast = initialZNear;
  zNearLast = zNear;
  zNearAuxLast = zNearAux;
  initialZFarLast = initialZFar;
  zFarLast = zFar;
  zFarAuxLast = zFarAux;
  VRPLast = VRP;
  OBSLast = OBS;
  UPLast = UP;
  distOBSLast = distOBS;
}

void GLWidget::restoreOldCamera()
{
  initialZNear = initialZNearLast;
  zNear = zNearLast;
  zNearAux = zNearAuxLast;
  initialZFar = initialZFarLast;
  zFar = zFarLast;
  zFarAux = zFarAuxLast;
  VRP = VRPLast;
  OBS = OBSLast;
  UP = UPLast;
  distOBS = distOBSLast;
}
