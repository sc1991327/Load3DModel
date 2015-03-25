#ifndef __RENDER_H__
#define __RENDER_H__

#pragma once

#include "stdafx.h"
#include "ReadFile.h"


class Render
{
public:
	Render(void);
	~Render(void);
	//initial opengl scene.
	void init(int *argc, char** argv, const char *windowName, char* fileName);
	//start glutMainLoop();
	void start(void);

private:
	void printInfo(int x);
	void display(void);
	void DrawBounding(void);
	void opengl_init(char* fileName);
	void reshape(int width, int height);
	void specialKey(int key, int x , int y);
	void keyboard(unsigned char key, int x , int y);
	void mouse(int button, int state, int x, int y);
	void mouseMove(int x, int y);

	//glut call function
	static void displayCallBack();
	static void keyboardCallBack(unsigned char k, int mouseX, int mouseY) ;
	static void specialKeyCallBack(int k, int mouseX, int mouseY) ;
	static void mouseCallBack(int button, int state, int x, int y);
	static void reshapeCallBack(int width, int height);
	static void mouseMoveCallBack(int x, int y );


private:
	ReadFile* data;
	int kind;
	// click left mouse button to start render the hyperplane
	bool isBegin;
	bool showDistance;
	bool showHyperplane;
	bool showInterpolationResult;
	bool showSquareVertex;
	float squareSizeChange;
	float kernelSizeChange;
	char* inputDataName;
	float zoom;
	bool wire;

	void Matrials(float zoom);
	bool pointModel;
	float scale;
	float xTranslate;
	float yTranslate;
	float zTranslate;
	
public:
	void Screen2Shpere(float     w,     // 窗口区域宽度(像素)  
		float     h,     // 窗口区域高度(像素)  
		float     x,     // 鼠标点的位置(x, y)  
		float     y,       
		Point3 *v      // 球面坐标单位矢量 
		);
private:
	bool leftButtonDown;
	int PreviousX;
	int PreviousY;
	Point3 rotateValue;
	void makeNormal(Point3* value);
	void VectorCross( const Point3 *a, const Point3 *b);
	void VectorDot( const Point3 *a, const Point3 *b);
	float rotateAngle;
};
extern Render gEngine;
#endif