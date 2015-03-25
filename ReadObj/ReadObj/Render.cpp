#include "stdafx.h"
#include "Render.h"

Render::Render(void)
: kind(0)
, isBegin(false)
, showDistance(true)
, showHyperplane(true)
, showInterpolationResult(true)
, showSquareVertex(true)
, squareSizeChange(0)
, inputDataName(NULL)
, zoom(0)
, wire(false)
, pointModel(false)
, scale(1.0)
, xTranslate(0)
, yTranslate(0)
, zTranslate(0)
, leftButtonDown(false)
, PreviousX(0)
, PreviousY(0)
, rotateAngle(0)
{
	data= new ReadFile;
	zoom = 12.5;

	inputDataName =new char[256];
}

Render::~Render(void)
{
	free(data);
	free(inputDataName);
}


void Render::init(int *argc, char** argv, const char *windowName, char* fileName )
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowPosition(200,100);
	glutInitWindowSize(WIDTH,HEIGHT);
	opengl_init(fileName);
	glutCreateWindow( windowName );
	printInfo(0);
	glutDisplayFunc ( Render::displayCallBack );
	glutReshapeFunc ( Render::reshapeCallBack );
	glutKeyboardFunc( Render::keyboardCallBack );
	glutSpecialFunc ( Render::specialKeyCallBack );
	glutMouseFunc ( Render::mouseCallBack );
	glutMotionFunc(Render::mouseMoveCallBack);
	
}

void Render::start(void)
{
	glutMainLoop();
}

void Render::display(void)
{
	//draw the point cloud
	glClearColor(0.9, 0.9, 0.9, 1.0);
	glClearDepth(1.0f);	
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//Matrials(1.0);
	glRotatef(rotateAngle, rotateValue.p[0], rotateValue.p[1], rotateValue.p[2]);
	glPushMatrix();
	glScalef(scale,scale,scale);
 	glTranslatef(xTranslate,yTranslate,zTranslate);

	DrawBounding();

	if (wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else if ( pointModel )
	{
		glPointSize(2.0);
		glPolygonMode( GL_FRONT_AND_BACK, GL_POINT);
	}
	else
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
	
	glBegin(GL_LINES);
	Point tempPoint;
	printf("\n");
	printf("\n");
	for (int i = 0; i < data->faceNumber; i++)
	{	
		for (int j= 0; j< 3;j++)
		{
			/*tempPoint.p[0] = data->modelData.normalList[data->modelData.triList[i].normal[j]].p[0];
			tempPoint.p[1] = data->modelData.normalList[data->modelData.triList[i].normal[j]].p[1];
			tempPoint.p[2] = data->modelData.normalList[data->modelData.triList[i].normal[j]].p[2];
			glNormal3fv(tempPoint.p);*/
			tempPoint.p[0] = data->modelData.pointList[data->modelData.triList[i].position[j]].p[0];
			tempPoint.p[1] = data->modelData.pointList[data->modelData.triList[i].position[j]].p[1];
			tempPoint.p[2] = data->modelData.pointList[data->modelData.triList[i].position[j]].p[2];
			//cout <<  << endl;
			glVertex3fv( tempPoint.p);
		}
	}
	glEnd();
	glPopMatrix();
	glFlush();
}

void Render::keyboard(unsigned char key, int x , int y)
{
	//once inter this function, no hypeplane compution
	switch (key)
	{
	case 'w':
		if (wire)
		{
			wire = false;
			pointModel=false;
		}
		else{
			wire = true;
			pointModel =  false;
		}
		glutPostRedisplay();
		break;
	case 'p':
		if (pointModel)
		{
			pointModel= false;
			wire = false;
		}
		else {
			pointModel= true;
			wire= false;
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
	
}

void Render::mouse(int button, int state, int x, int y)
{
	if( button == GLUT_LEFT_BUTTON && state == GLUT_DOWN )
	{ 
		PreviousX = x;
		PreviousY = y;
		leftButtonDown = true;
	}
	if( button == GLUT_LEFT_BUTTON && state == GLUT_UP )
	{ 
		leftButtonDown = false;
	}
	if ( button ==  GLUT_WHEEL_UP )
	{
		scale *= 1.01;
		glutPostRedisplay();
	}
	if ( button ==  GLUT_WHEEL_DOWN )
	{
		scale *=0.99;
		glutPostRedisplay();
	}
	
}

void Render::specialKey(int key, int x , int y)
{
	switch ( key )
	{
	case GLUT_KEY_UP:
		xTranslate += 0.0;
		yTranslate += 0.01;
		zTranslate += 0.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		xTranslate -= 0.0;
		yTranslate -= 0.01;
		zTranslate -= 0.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		xTranslate += 0.01;
		yTranslate += 0.00;
		zTranslate += 0.0;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		xTranslate -= 0.01;
		yTranslate -= 0.0;
		zTranslate -= 0.0;
		glutPostRedisplay();
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void Render::reshape(int width, int height)
{
	glViewport(0,0, width, height);
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, width/height,0.0, 1000);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt(0.0,0.0,data->boundingBox[1].p[2]*zoom, 0.0,0.0,0.0, 0.0,1.0,0.0);
}

void Render::printInfo(int x)
{
	if ( x==0 ){
		printf("window is opened.\n");
	}
	if ( x==1 ){
		printf("opengl_init is loaded.\n");
	}
	if ( x==2 ){
		printf("ReadData is loaded.\n");
	}
 	//printf( "Input the file name:" );
 	//scanf_s( "%s", inputDataName );
	
}
void Render::mouseMove(int x, int y)
{
	
	if (leftButtonDown)
	{
		Point3 startV;  
		Screen2Shpere(WIDTH, HEIGHT, PreviousX, PreviousY, &startV);  

		Point3    dragV;  
		Screen2Shpere(WIDTH, HEIGHT, x, y, &dragV);
		VectorCross(&startV, &dragV);  

		// 而旋转角度就是m1和m2之间的夹角a, 点积得到:  
		VectorDot(&startV, &dragV);
		rotateAngle = acosf(rotateAngle);
		glutPostRedisplay();
	}
	else
	{
		rotateAngle = 0.0;
	}
}
void Render::opengl_init(char* fileName)
{
	//initialize the input data
	printInfo(1);
	/////////////////////////////////////////////////////////////////////////////
	data->ReadData( 2, fileName);
	printInfo(2);
	/////////////////////////////////////////////////////////////////////////////
	//viewpoint 	
	glMatrixMode( GL_PROJECTION);
	glLoadIdentity( );
	gluPerspective( 75, WIDTH/HEIGHT,0.0, 1000 );
	glMatrixMode  ( GL_MODELVIEW );
	glLoadIdentity( );
	gluLookAt     ( 0.0,0.0,data->boundingBox[1].p[2]*zoom, 0.0,0.0,0.0, 0.0,1.0,0.0 );
	glPushMatrix();
}
void Render::displayCallBack()
{	
	gEngine.display();
}
void Render::keyboardCallBack(unsigned char k, int mouseX, int mouseY) 
{
	gEngine.keyboard(k, mouseX, mouseY);
}
void Render::specialKeyCallBack(int k, int mouseX, int mouseY)
{
	gEngine.specialKey(k, mouseX, mouseY);
}
void Render::mouseCallBack(int button, int state, int x, int y)
{
	gEngine.mouse( button, state, x ,y);
}

void Render::reshapeCallBack(int width, int height)
{
	gEngine.reshape(width, height);
}
void Render::mouseMoveCallBack(int x, int y)
{
	gEngine.mouseMove(x,y );
}
void Render::DrawBounding(void)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[1].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[1].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[0].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[0].p[1], data->boundingBox[0].p[2]);

	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[1].p[1], data->boundingBox[1].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[1].p[1], data->boundingBox[1].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[0].p[1], data->boundingBox[1].p[2]);
	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[0].p[1], data->boundingBox[1].p[2]);

	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[1].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[0].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[0].p[1], data->boundingBox[1].p[2]);
	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[1].p[1], data->boundingBox[1].p[2]);

	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[1].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[0].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[0].p[1], data->boundingBox[1].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[1].p[1], data->boundingBox[1].p[2]);

	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[1].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[1].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[1].p[1], data->boundingBox[1].p[2]);
	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[1].p[1], data->boundingBox[1].p[2]);

	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[0].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[0].p[1], data->boundingBox[0].p[2]);
	glVertex3f(data->boundingBox[1].p[0],data->boundingBox[0].p[1], data->boundingBox[1].p[2]);
	glVertex3f(data->boundingBox[0].p[0],data->boundingBox[0].p[1], data->boundingBox[1].p[2]);

	glEnd();
}

void Render::Matrials(float zoom)
{

	GLfloat mat_ambient[]= {0.0,1.0,0.0,0.0}; 
	GLfloat mat_diffuse[]= {1.0,.0,0.0,0.0}; 
	GLfloat mat_specular[]={0.8,0.8,0.8,0.0}; 
	GLfloat mat_shininess[]={60.0}; 
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,mat_ambient); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,mat_diffuse); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,mat_specular); 
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,mat_shininess); 

	GLfloat light_position[]={0, 0, 1000,1.0};
	glLightfv(GL_LIGHT0,GL_POSITION,light_position); 
	GLfloat light_ambient [] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse [] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT , light_ambient );
	glLightfv(GL_LIGHT0, GL_DIFFUSE , light_diffuse );
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glEnable(GL_LIGHTING); 
	glEnable(GL_LIGHT0); 
	glDepthFunc(GL_LESS); 
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LESS);										
	

}

void Render::Screen2Shpere(float w, float h, float x, float y,  Point3 *v )
{
	if (w<2) w=2;           // 防止被 0 除
	if (h<2) h=2;  

	x = ((x/((w-1)/2))-1);  
	y = -((y/((h-1)/2))-1); // OGL需要, Y 轴反向  
	float d2 = x*x + y*y;  
	float d = sqrt(d2);  

	if (d2>1.0f){  
		v->p[0] = x/d;  
		v->p[1] = y/d;  
		v->p[2] = 0.f;  
	}  
	else{  
		v->p[0] = x;  
		v->p[1] = y;  
		v->p[2] = sqrt(1.0f-d2);  
	}     
}

void Render::makeNormal(Point3* v)
{
	float m = 1.0f/sqrt(v->p[0]*v->p[0] + v->p[1]*v->p[1] + v->p[2]*v->p[2]);  
	v->p[0] *= m;   
	v->p[1] *= m;  
	v->p[2] *= m;  
}

void Render::VectorCross( const Point3 *a, const Point3 *b )
{
	rotateValue.p[0] = a->p[1]*b->p[2] - b->p[1]*a->p[2];  
	rotateValue.p[1] = a->p[2]*b->p[0] - b->p[2]*a->p[0];  
	rotateValue.p[2] = a->p[0]*b->p[1] - b->p[0]*a->p[1];  
	makeNormal(&rotateValue);  
}

void Render::VectorDot( const Point3 *a, const Point3 *b )
{
	rotateAngle = a->p[0]*b->p[0]+a->p[1]*b->p[1]+a->p[2]*b->p[2];
}
