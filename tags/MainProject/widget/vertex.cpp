#include "vertex.h"

Vertex::Vertex(const Point& coordenades)
: coord(coordenades)
{}

void Vertex::computeNormal(Vector faceNormal)
{
  normal += faceNormal;
}
