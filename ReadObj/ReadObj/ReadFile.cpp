#include "StdAfx.h"
#include "ReadFile.h"

ReadFile::ReadFile(void)
: vertexNumber(0)
, faceNumber(0)
, normalNumber(0)
{
	for (int i =0; i<2 ;i++)
	{
		boundingBox[i].p[0] = 0.0;
		boundingBox[i].p[1] = 0.0;
		boundingBox[i].p[2] = 0.0;
	}
}

ReadFile::~ReadFile(void)
{
}

void ReadFile::ReadData(int kind, char* fileName)
{
	switch(kind)
	{
	case 1://kind ==1 open a .obj file and display it
		ReadOBJ(fileName);
		break;
	case 2://kind==2 open a off file and display it 
		printf("Perpare in ReadOFF method.\n");
		ReadOFF(fileName);
		printf("ReadOFF method used correctly.\n");
		break;
	case 3:
		ReadPLY(fileName);
		break;
	default:
		printf("There is no method for this kind of file.\n");
		exit(1);

	}

	//compute the bounding box of the model
	for (int i = 0;i < vertexNumber ; i++)
	{
		if (modelData.pointList[i].p[0] < boundingBox[0].p[0]) //xmin
			boundingBox[0].p[0] = modelData.pointList[i].p[0];  

		if (modelData.pointList[i].p[0] > boundingBox[1].p[0] ) //xmax
			boundingBox[1].p[0] = modelData.pointList[i].p[0];

		if (modelData.pointList[i].p[1] < boundingBox[0].p[1] ) //ymin
			boundingBox[0].p[1] = modelData.pointList[i].p[1];

		if (modelData.pointList[i].p[1] > boundingBox[1].p[1] ) //ymax
			boundingBox[1].p[1] = modelData.pointList[i].p[1];

		if (modelData.pointList[i].p[2] < boundingBox[0].p[2] ) //zmin
			boundingBox[0].p[2] = modelData.pointList[i].p[2];

		if (modelData.pointList[i].p[2] > boundingBox[1].p[2] ) //zmax
			boundingBox[1].p[2] = modelData.pointList[i].p[2];
	}
	
}

void ReadFile::ReadOBJ(char* fileName)
{
	//a file point
	FILE* filepoint;
	//
	fopen_s( &filepoint, fileName, "r");
	if (filepoint == NULL)
	{
		printf("Can't Open file %s\n", fileName);
		exit(1);
	}
	//use to store scan data adjective.
	GLfloat f1, f2, f3;//temp data
	int vertexInIndexInOneFace = 0;
	char buffer[512];
	//fgets:从流中读取n-1个字符，除非读完一行
	//把独到的数据放入buffer.下面是对每一行数据单独进行处理。
	fgets( buffer, 511, filepoint);
	while( !feof(filepoint) )
	{
		//忽略注释行，空行等情况。
		while ( '#' == buffer[0] || ' ' == buffer[0] || '\t' == buffer[0] || '\n' == buffer[0] )
		{
			fgets( buffer, 255, filepoint );
		}
		//以'v'开头的顶点数据读入。
		if ( 'v' == buffer[0] && ' ' == buffer[1])
		{
			//read Vertex infomation
			sscanf_s(buffer, "v %f %f %f",&f1, &f2, &f3);
			modelData.pointList[vertexNumber].p[0] = f1;
			modelData.pointList[vertexNumber].p[1] = f2;
			modelData.pointList[vertexNumber].p[2] = f3;
			vertexNumber++;
		}
		else if ('v' == buffer[0] && 'n' == buffer[1])
		{
			sscanf_s(buffer, "vn %f %f %f", &f1, &f2, &f3);
			modelData.normalList[normalNumber].p[0] = f1;
			modelData.normalList[normalNumber].p[1] = f2;
			modelData.normalList[normalNumber].p[2] = f3;
			normalNumber++;
		}
		//读入面信息：
		else if ('f' == buffer[0])
		{
			//创建三个缓冲区
			char tempbuf[256];
			int pointIndex[50];
			int normalIndex[50];

			vertexInIndexInOneFace = 0;
			//将buffer中的数据，即本行的数据复制到tempbuf中。
			//从buffer[2]开始因为第一个字符存"f"
			strcpy_s( tempbuf,&buffer[2] );
			//计算tempbuf中有多少数据。就是这一行有多少数据。
			int num=strlen(tempbuf);
			//建一个变量记录处理掉了多少。
			int many=0;
			//用一个循环来处理掉一行的所有数据。
			while (many < num-2)
			{
				//扫描处理
				//	A/B/C：顶点索引/贴图UV索引/法线索引
				int temint = sscanf_s( tempbuf, "%d//%d", &pointIndex[vertexInIndexInOneFace], &normalIndex[vertexInIndexInOneFace] );
				//vertexInIndexInOneFace:形成一个每三个点组成一个面的顶点数组。
				vertexInIndexInOneFace ++ ;

				//下面是在一行里从下一个"%d//%d"开始读
				int i =0;
				while ( tempbuf[i] != ' ' && i<(num-many) )
					i++;
				many = many+i+1;
				strcpy_s(tempbuf, &buffer[many+2]);
				
			}
			//注意这下面还是只对单行数据处理。
			//从上面所得到的vertexInIndexInOneFace数组中每个面使用三个顶点。
			//下面确定每个点的位置。
			for (int i = 2; i < vertexInIndexInOneFace; i++)
			{
				modelData.triList[faceNumber].position[0] = pointIndex [0]-1 ;
				modelData.triList[faceNumber].normal[0]  =  normalIndex[0]-1 ;
				modelData.triList[faceNumber].position[1] = pointIndex [i-1]-1 ;
				modelData.triList[faceNumber].normal[1]  =  normalIndex[i-1]-1 ;
				modelData.triList[faceNumber].position[2] = pointIndex [i]-1 ;
				modelData.triList[faceNumber].normal[2]  =  normalIndex[i]-1 ;
				faceNumber++;
			}
		}
		else
		{}
		//得到下一行
		fgets( buffer, 255, filepoint);
	}
	fclose(filepoint);
}

void ReadFile::ReadOFF(char* fileName)
{
	printf("ReadOFF position one.\n");
	FILE* filepoint;
	fopen_s( &filepoint, fileName, "r");
	if (filepoint == NULL)
	{
		printf("Can't Open file %s\n", fileName);
		exit(1);
	}
	GLfloat f1, f2, f3;//temp data
	int vertexInIndexInOneFace = 0;
	char buffer[256];

	printf("ReadOFF position two.\n");
	fgets( buffer, 255, filepoint);
	//use to record how many point,face data read in.
	int vertexNum = 0;
	int faceNum = 0;

	printf("ReadOFF position three.\n");
	while( !feof(filepoint) )
	{
		//此处读写正确。但是执行不下去了？？？
		while ( '#' == buffer[0] || ' ' == buffer[0] || '\t' == buffer[0] || '\n' == buffer[0] )
		{
			//从流中读取n-1个字符，除非读完一行，参数s是来接收字符串，如果成功则返回s的指针，否则返回NULL。
			fgets( buffer, 255, filepoint );
		}
		if ( 'O' == buffer[0] || 'F' == buffer[1] || 'F' == buffer[2] ){
			sscanf_s(buffer, "OFF %f %f %f",&f1, &f2, &f3);
			vertexNum = f1;
			faceNum = f2;
			printf("%d",vertexNum);
			printf("\n");
			printf("%d",faceNum);
			printf("\n");
			printf("\n");
			
		}
		if ( vertexNumber < vertexNum )
		{
			//read Vertex infomation
			sscanf_s(buffer, "%f %f %f",&f1, &f2, &f3);
			modelData.pointList[vertexNumber].p[0] = f1;
			modelData.pointList[vertexNumber].p[1] = f2;
			modelData.pointList[vertexNumber].p[2] = f3;
			vertexNumber++;
			/*printf("%d",f1);
			printf("\n");*/
		}
		else if ( faceNumber < faceNum )
		{
			sscanf_s(buffer, "3 %f %f %f",&f1, &f2, &f3);
			modelData.triList[faceNumber].position[0] = f1;
			modelData.triList[faceNumber].position[1] = f2;
			modelData.triList[faceNumber].position[2] = f3;
			faceNumber++;
			/*printf("%d",faceNumber);
			printf("\n");*/
		}
		else
		{}
		fgets( buffer, 255, filepoint);
	}
	printf("ReadOFF position four.\n");
	fclose(filepoint);
}

void ReadFile::ReadPLY(char* fileName)
{
	//a file point
	printf("in the ReadPly method.\n");
	FILE* filepoint;
	fopen_s( &filepoint, fileName, "r");
	if (filepoint == NULL)
	{
		printf("Can't Open file %s\n", fileName);
		exit(1);
	}
	//use to store scan data adjective.
	GLfloat f1, f2, f3, f4, f5, f6;//temp data
	int vertexInIndexInOneFace = 0;
	char buffer[512];
	int vertexNum = 0;
	int faceNum = 0;
	//fgets:从流中读取n-1个字符，除非读完一行
	//把独到的数据放入buffer.下面是对每一行数据单独进行处理。
	fgets( buffer, 511, filepoint);
	while( !feof(filepoint) )
	{
		printf("in the read char while.\n");
		while ( sscanf_s(buffer, "comment") )
		{
			//从流中读取n-1个字符，除非读完一行，参数s是来接收字符串，如果成功则返回s的指针，否则返回NULL。
			fgets( buffer, 255, filepoint );
		}
		if ( 'e'==buffer[0] && 'l'==buffer[1] && 'e'==buffer[2] && 'v'==buffer[8] ){
			vertexNum = f1;
			printf("%d",vertexNum );
		}
		if ( 'e'==buffer[0] && 'l'==buffer[1] && 'e'==buffer[2] && 'f'==buffer[8]  ){
			faceNum = f2;
			printf("%d",faceNum );
		}
		//if (  )
		fgets( buffer, 511, filepoint);
	}
}






