#include "referencia.h"
#include "box.h"
#include <QtOpenGL/qgl.h>
#include "scene.h"

Referencia::Referencia(int ob, Point p, Vector sz, float ori):
  object(ob), pos(p), size(sz), orientation(ori), lightID(-1)
{}

Referencia::~Referencia(void)
{}

void Referencia::Render(std::vector<Object> &lobjects, bool smooth)
{
  //float sx, sy, sz;

  Object o=lobjects[this->object];
  Box box=o.boundingBox();

  // A partir d'aquí, cal aplicar les transformacions de model necessàries 
  // i pintar l'objecte

  Point initialPos = (box.minb + box.maxb)/2.0;
  initialPos.y = box.minb.y;
  float sx = size.x / (box.maxb.x - box.minb.x);
  float sy = size.y / (box.maxb.y - box.minb.y);
  float sz = size.z / (box.maxb.z - box.minb.z);
  
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(orientation, 0.0, 1.0, 0.0);
  glScalef(sx, sy, sz);
  glTranslatef(-initialPos.x, -initialPos.y, -initialPos.z);
  o.Render(smooth);
  glPopMatrix();
}

int Referencia::getObjectId()
{
  return object;
}

// Calculem la caixa transformada a partir de la caixa original
// de l'objecte.

Box& Referencia::computeTransformedBox(Object &obj)
{
  Box b = obj.boundingBox();
  Box *r=new Box(); // Retornem un Box, així que ens cal al heap.
  Box &result = *r;
  float sa, sb, sc;
  sa=b.maxb.x-b.minb.x;	
  sb=b.maxb.y-b.minb.y;	
  sc = b.maxb.z - b.minb.z;	
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();
  glTranslatef(pos.x, pos.y, pos.z);
  glRotatef(orientation, 0, 1, 0);
  glScalef(size.x/sa, size.y/sb, size.z/sc);
  glTranslatef(-(b.maxb.x+b.minb.x)/2, -b.minb.y, -(b.maxb.z+b.minb.z)/2);

  vector<Vertex> &verts = obj.vertices;
  Point punt(verts[0].coord.x, verts[0].coord.y, verts[0].coord.z);
  Scene::transformPoint(punt);
  result.init(punt);

  for (unsigned int i=1; i<verts.size(); ++i)
  {
    Point punt(verts[i].coord.x, verts[i].coord.y, verts[i].coord.z);
    Scene::transformPoint(punt);
    result.update(punt);
  }
  glPopMatrix();
  return result;
}

Point Referencia::getPosition()
{
   return pos;
}

Vector Referencia::getSize()
{
   return size;
}

float Referencia::getOrientation()
{
   return orientation;
}

 /* Nomes s'utilitzen en cas de ser de tipus FANAL */

// Retorna el ID del llum associat. -1 indica que no hi te llum.
int Referencia::getLightID()
{
  return lightID;
}

// Elimina la relacio amb el llum
void Referencia::turnOff()
{
  lightID = -1;
}

// Crea la relacio amb el llum "light" i retorna la posicio que ha d'ocupar el llum
Point Referencia::turnOn(int light)
{
  lightID = light;
  
  Point position(pos);
  position.y += size.y*0.8;
  
  return position;
}
