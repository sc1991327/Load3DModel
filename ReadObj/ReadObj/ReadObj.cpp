// ReadObj.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Render.h"
Render gEngine;
int main(int argc, char** argv)
{
	gEngine.init(&argc, argv, "display OBJ model", "off/model.off");
	gEngine.start();
	return 0;
}

