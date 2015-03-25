#include "stdafx.h"

//define model architecture.
typedef struct Point
{
	GLfloat p[3];
}Point3;

typedef struct Triangle
{
	int position[3];
	int normal[3];
}Triangle;

typedef struct ModelCon
{
	Point pointList[MAX_VERTEX];
	Point normalList[MAX_VERTEX];
	Triangle triList[MAX_TRIANGLE];
}Model;