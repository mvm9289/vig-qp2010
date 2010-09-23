#ifndef _FACE_H_
#define _FACE_H_

#include <vector>
#include "point.h"
#include "vertex.h"

using namespace std;

// Face - representa una cara d'un objecte 3D
class Face
{
 public:
	Face();

	// constructor a partir de 3 o 4 indexs a vertex
	Face(int i1, int i2, int i3, int i4=-1); 
 
	void computeNormal(vector<Vertex> &v);
	void computeNormalVertex(vector<Vertex> &v);

	// atributs
	int material;	       // ID del material (index del material)
	Vector normal;		// Vector normal de la cara
	vector<int> vertices;  // vector amb els indexs dels vertexs de la cara
};



#endif
