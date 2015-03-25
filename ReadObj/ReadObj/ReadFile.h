#ifndef __READFILE_H__
#define __READFILE_H__

#include "ModelContext.h"
#pragma once

//Use to read different model file.
class ReadFile
{
public:
	ReadFile(void);
	~ReadFile(void);
	
private:
	void ReadOBJ(char* fileName);
	void ReadOFF(char* fileName);
	void ReadPLY(char* fileName);
	int normalNumber;

public:
	//ReadData method use the ReadOBJ,ReadOFF,ReadPLY method to express model file.
	void ReadData(int kind, char* fileName);
	//Use to record vertex,face numbers.
	int vertexNumber;
	int faceNumber;
	Point boundingBox[2];
	//Model Object: all data use to draw models.
	Model modelData;
};

#endif