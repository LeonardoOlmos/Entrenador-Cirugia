/*
 * Camaras2.c
 *
 *  Created on: 24/05/2012
 *      Author: 123
 */

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <cv.h>
#include <highgui.h>

FILE *fileCoordinates;
time_t t_inicial, t_final, t_final_muestra;

IplImage *SegmentacionColor(const IplImage *imgSource, int Cam);

double px1_v=0.0,py1_v=0.0,px2_v=0.0,py2_v=0.0,angxy_v=0.0,angxz_v=0.0;
double X_v=0.0,Y_v=0.0,Z_v=0.0;

double px1_a=0.0,py1_a=0.0,px2_a=0.0,py2_a=0.0,angxy_a=0.0,angxz_a=0.0;
double X_a=0.0,Y_a=0.0,Z_a=0.0;

double seg=0.0,seg_muestra=0.0;

char fNameCoord[20];
char key;
int fileCounter = 0;

int main(int argc, char **argv)
{
	int Cam=0;
	char Eti1[10];
	char Eti2[10];

	/* Cargamos imagen */
	IplImage *imagen1 = 0;
	IplImage *imagen2 = 0;
	CvCapture *captura1 = 0;
	CvCapture *captura2 = 0;
	CvFont font1;
	cvInitFont(&font1,CV_FONT_HERSHEY_SIMPLEX, 0.55, 0.55, 0,1,8);
	CvFont font2;
	cvInitFont(&font2,CV_FONT_HERSHEY_SIMPLEX, 0.55, 0.55, 0,1,8);
	//CvScalar textColor = CV_RGB(139,0,0);

	/* Valores para calibrar las camaras web*/
	double intrinsecos[] = { 569.12027, 0.0, 312.68577,
							 0.0, 567.76584, 242.96958,
							 0.0, 0.0, 1.0 };

	double distorcion[] = { 0.01436,
						   -0.04911,
						    0.00164,
						    0.00124,
							0.0 };

	CvMat intrinsic=cvMat(3, 3, CV_64FC1, intrinsecos);//Matriz de parametros intrinsecos 1

	CvMat distortion=cvMat(5,1,CV_64FC1, distorcion);//Vector de distorsion, Radial y Tangencial 1

	CvMat intrinsic1=cvMat(3, 3, CV_64FC1, intrinsecos);//Matriz de parametros intrinsecos 2

	CvMat distortion1=cvMat(5,1,CV_64FC1, distorcion);//Vector de distorsion, Radial y Tangencial 2

	captura1 = cvCreateCameraCapture(0);//Tomamos la captura de la camara 1

	if(!captura1)
	{
	printf("ERROR: No hay Dispositivo Conectado");
	return (-1);
	}

	cvSetCaptureProperty(captura1, CV_CAP_PROP_FRAME_WIDTH,640);//Colocamos las propiedades del tamaño de la imagen
	cvSetCaptureProperty(captura1, CV_CAP_PROP_FRAME_HEIGHT,480);

	imagen1=cvQueryFrame(captura1);//Tomamos un frame para calibrar

	IplImage *mapx = cvCreateImage( cvGetSize(imagen1), IPL_DEPTH_32F, 1 );
	IplImage *mapy = cvCreateImage( cvGetSize(imagen1), IPL_DEPTH_32F, 1 );

	cvInitUndistortMap(&intrinsic,&distortion,mapx,mapy);

	captura2 = cvCreateCameraCapture(1);//Tomamos la captura de la camara 2

	if(!captura2)
	{
	printf("ERROR: No hay Dispositivo Conectado");
	return (-1);
	}

	cvSetCaptureProperty(captura2, CV_CAP_PROP_FRAME_WIDTH,640);//Colocamos las propiedades del tamaño de la imagen
	cvSetCaptureProperty(captura2, CV_CAP_PROP_FRAME_HEIGHT,480);

	imagen2=cvQueryFrame(captura2);//Tomamos un frame para calibrar

	IplImage *mapx1 = cvCreateImage( cvGetSize(imagen2), IPL_DEPTH_32F, 1 );
	IplImage *mapy1 = cvCreateImage( cvGetSize(imagen2), IPL_DEPTH_32F, 1 );

	cvInitUndistortMap(&intrinsic1,&distortion1,mapx1,mapy1);

	cvNamedWindow( "WebCam_1",CV_WINDOW_AUTOSIZE );
	cvNamedWindow( "WebCam_2",CV_WINDOW_AUTOSIZE );
	//cvNamedWindow( "WebCam_3",CV_WINDOW_AUTOSIZE );
	//cvNamedWindow( "WebCam_4",CV_WINDOW_AUTOSIZE );

	//Inicializamos el archivo de coordenadas
	do{
		// crea el nombre del archivo de coordenadas como Coord000X.txt **
		sprintf(fNameCoord,"C:/Users/Laboratorio Cirugía/Entrenador-Cirugia/api-cirugia/%s_%04d.txt",argv[1],fileCounter);

		// trata de abrir el archivo de coordenadas con dicho nombre y número en modo de lectura
		fileCoordinates = fopen(fNameCoord,"r");
		printf("%s\n",fNameCoord);

		// si no se abrió significa que no existe ese nombre de archivo
		if(!fileCoordinates){
		// por lo que utilizará ese nombre para guardar las coordenadas
		fileCoordinates = fopen(fNameCoord, "w");
		}
		else{
		// si existe el archivo, se cierra dicho archivo,...
		fclose(fileCoordinates);
		// se pone el puntero en nulo ...
		fileCoordinates = NULL;
		}
		// y se repite el proceso con un nuevo número
		fileCounter++;
	  }while(fileCoordinates == NULL );


	//t_inicial_v = clock();
	//t_inicial_a = clock();
	do
	{
	key=cvWaitKey(10);

	}while(key!='q');

	t_inicial = clock(); //Inicializamos el Tiempo

	while(1)
	{

	IplImage *Camara1;
	IplImage *Camara2;
	IplImage *temporal;
	IplImage *temporal1;

		imagen1 = cvQueryFrame(captura1);

		//Calibramos la imagen de la camara 1
		temporal = cvCloneImage(imagen1);
		cvRemap( temporal, imagen1, mapx, mapy, CV_INTER_CUBIC | CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
		cvReleaseImage(&temporal);

		//cvSaveImage("IncioVA1.jpg",imagen1,NULL);

		Cam=1;
		Camara1=SegmentacionColor(imagen1,Cam);

		imagen2 = cvQueryFrame(captura2);

		//Calibramos la imagen de la camara 2
		temporal1 = cvCloneImage(imagen2);
		cvRemap( temporal1, imagen2, mapx1, mapy1, CV_INTER_CUBIC | CV_WARP_FILL_OUTLIERS, cvScalarAll(0) );
		cvReleaseImage(&temporal1);

		//cvSaveImage("IncioVA2.jpg",imagen2,NULL);

		Cam=2;
		Camara2=SegmentacionColor(imagen2,Cam);

		//Dibujamos en la imagen 1
		cvLine(Camara1,cvPoint(305,240),cvPoint(335,240),CV_RGB(0,255,255),1,8,0);
		cvLine(Camara1,cvPoint(320,225),cvPoint(320,255),CV_RGB(0,255,255),1,8,0);

		sprintf(Eti1,"WebCam_1");
		cvPutText(Camara1,Eti1,cvPoint(530,440),&font1,CV_RGB(255,128,0));
		cvShowImage("WebCam_1",Camara1);
		//cvSaveImage("FinalA1.jpg",Camara1,NULL);

		//Dibujamos en la imagen 2
		cvLine(Camara2,cvPoint(305,240),cvPoint(335,240),CV_RGB(0,255,255),1,8,0);
		cvLine(Camara2,cvPoint(320,225),cvPoint(320,255),CV_RGB(0,255,255),1,8,0);

		sprintf(Eti2,"WebCam_2");
		cvPutText(Camara2,Eti2,cvPoint(530,440), &font1,CV_RGB(255,128,0));
		cvShowImage("WebCam_2",Camara2);
		//cvSaveImage("FinalA2.jpg",Camara2,NULL);

		if (cvWaitKey(8)==27)
		{
		break;
		}

		cvReleaseImage( &Camara1 );
		cvReleaseImage( &Camara2 );
	}

	//Toma del Tiempo Final de la tarea
	t_final = clock();
	seg = (t_final-t_inicial)/(double)CLOCKS_PER_SEC;
	printf("\n Tiempo de la tarea: %f segundos \n",seg);

	cvReleaseCapture( &captura1 );
	cvReleaseCapture( &captura2 );
	cvReleaseImage( &imagen1 );
	cvReleaseImage( &imagen2 );
	cvDestroyAllWindows();
	fclose(fileCoordinates);
	return (0);
}

IplImage *SegmentacionColor(const IplImage *imgSource, int Cam)
{


		IplImage *imgDest;
		IplImage *HSV_v;
		IplImage *Bin_v;
		IplImage *HSV_a;
		IplImage *Bin_a;
		IplImage *tempo;
		CvSeq *cont_v,*cont_a;
		CvBox2D Box_v,Box_a;
		CvScalar down_v,down_a;
		CvScalar up_v,up_a;
		CvFont font_v,font_a;

		CvMemStorage *Storage_v=cvCreateMemStorage(0);
		CvMemStorage *BoxStorage_v=cvCreateMemStorage(0);
		CvMemStorage *Storage_a=cvCreateMemStorage(0);
		CvMemStorage *BoxStorage_a=cvCreateMemStorage(0);

		CvMoments momentsv;
		CvMoments momentsa;

		int C_v,C_a;
		char Eti_v[5],Eti_a[5];

		cvInitFont(&font_v,CV_FONT_HERSHEY_DUPLEX , 0.5, 0.5, 0, 1, 8);
		cvInitFont(&font_a,CV_FONT_HERSHEY_DUPLEX , 0.5, 0.5, 0, 1, 8);

		imgDest = cvCloneImage(imgSource);

		tempo = cvCloneImage(imgDest);
		cvSmooth(imgDest,tempo,CV_GAUSSIAN,1,1,0,0);
		//imgDest=cvCreateImage(cvGetSize(imgSource),8,3);
		//cvCopy(imgSource,imgDest,NULL);

		//Valores para la segmentacion del Color Verde Abierto
		down_v=cvScalar(50,25,35,0);
	  	up_v=cvScalar(105,255,255,0);

		//Valores para la segmentacion del Color Azul Abierto
		down_v=cvScalar(100,90,95,0);
		up_v=cvScalar(120,255,255,0);

		//Valores para la segmentacion del Color Verde
		//down_v=cvScalar(38,45,58,0);
		//up_v=cvScalar(85,255,255,0);

		//Valores para la segmentacion del Color Azul//
		//down_a=cvScalar(95.525,75,70.25,0);
		//up_a=cvScalar(120,255,255,0);

		HSV_v=cvCreateImage(cvGetSize(imgSource),8,3);
		HSV_a=cvCreateImage(cvGetSize(imgSource),8,3);
		Bin_v=cvCreateImage(cvGetSize(imgSource),8,1);
		Bin_a=cvCreateImage(cvGetSize(imgSource),8,1);

		cvCvtColor(tempo,HSV_v,CV_BGR2HSV);
		cvCvtColor(tempo,HSV_a,CV_BGR2HSV);
		cvInRangeS(HSV_v,down_v,up_v,Bin_v);
		cvInRangeS(HSV_a,down_a,up_a,Bin_a);
		cvReleaseImage(&tempo);

		//Operaciones Morfologicas para el Marcador Verde
		cvErode(Bin_v,Bin_v,NULL,2);
		cvDilate(Bin_v,Bin_v,NULL,4);

		//Operaciones Morfologicas para el Marcador Azul
		cvErode(Bin_a,Bin_a,NULL,2);
		cvDilate(Bin_a,Bin_a,NULL,4);


		C_v=cvFindContours(Bin_v,Storage_v,&cont_v,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

		C_a=cvFindContours(Bin_a,Storage_a,&cont_a,sizeof(CvContour),CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));

		if (C_v>0)
		{
		cont_v=cvApproxPoly(cont_v,sizeof(CvContour),Storage_v,CV_POLY_APPROX_DP,1,1);
		}

		if (C_a>0)
		{
		cont_a=cvApproxPoly(cont_a,sizeof(CvContour),Storage_a,CV_POLY_APPROX_DP,1,1);
		}

		for(;cont_v!=0;cont_v=cont_v->h_next)
		{

			double angulo=0.0,ang=0.0,x=0.0,y=0.0;
			double u11=0.0,u20=0.0,u02=0.0;
			int x1=0,x2=0,y1=0,y2=0;// x, y;

			cvMoments(cont_v,&momentsv,0);//Obtencion de momentos geometricos

			u11=cvGetCentralMoment(&momentsv,1,1);//Obtencion de momentos centrales
			u20=cvGetCentralMoment(&momentsv,2,0);
			u02=cvGetCentralMoment(&momentsv,0,2);

			x=(momentsv.m10/momentsv.m00);
			y=(momentsv.m01/momentsv.m00);

			ang=(0.5)*(atan2(2.0*(u11),(u02-u20)));// Angulo en radianes
			angulo=(((ang)*180)/(3.141592));// Lo pasamos a grados

			if (Cam==1)
			{

			px1_v=x;
			py1_v=y;
			angxy_v=angulo;
			//printf("\n %f \t %f \t %f",x,y,angulo);

			}

			else if (Cam==2)
			{

			px2_v=x;
			py2_v=y;
			angxz_v=angulo;
			//printf("\n %f \t %f \t %f",x,y,angulo);

			//Estimacion de la posicion de la Pinza Verde

			X_v=(-4218042.572964758 - 35046.782735411056*px1_v + (138.2829291531299)*(pow(px1_v,2.0)) + 11976.27356141024*py1_v - 34.967027389427464*px1_v*py1_v + (61001.98163565843 + px1_v*(-325.04583809598637 + 0.4290161636740012*px1_v + 0.15360600571426627*py1_v) - 52.610349876804854*py1_v)*py2_v)/
			    (-3681172.682386461 + 10579.648523146354*px1_v + (0.49121207647312665)*(pow(px1_v,2.0)) + 2259.508750994723*py1_v - 6.597069111485799*px1_v*py1_v + (1976.4929504069964 + px1_v*(-6.0108370796733555 + 0.0007009796673850188*px1_v + 0.027261560798726096*py1_v) - 9.337136560120587*py1_v)*py2_v);

			Y_v=(-5.405737179150725e9 +(pow(px1_v,3.0))*(-13.641342009875881 - 0.027461422173199274*py2_v) + (6.800377335691601e7)*py2_v + (pow(px1_v,2.0))*(-36458.97742793409 + py1_v*(-607.6140135310608 - 2.780067702426688*py2_v) + 595.3897772807168*py2_v) + py1_v*(-7.15106827144821e7 - 329716.07250046456*py2_v + py1_v*(1923.2673077288346 + 8.800710192541828*py2_v)) + px1_v*(2.987058815947646e7 - 399250.75938182353*py2_v + py1_v*(416898.11033562664 + py1_v*(-5.615347735813202 - 0.02569536114644222*py2_v) + 1914.8477938763172*py2_v)))/
			    ((1854.4933353525018 - 5.3848637714815855*px1_v - 0.049775282172777116*py1_v)*(-3681172.682386461 + 10579.648523146354*px1_v + 0.49121207647312665*(pow(px1_v,2.0)) + 2259.508750994723*py1_v - 6.597069111485799*px1_v*py1_v + (1976.4929504069964 + px1_v*(-6.0108370796733555 + 0.0007009796673850188*px1_v + 0.027261560798726096*py1_v) - 9.337136560120587*py1_v)*py2_v));

			Z_v=(-6.917825201948971e11 + (pow(px1_v,3.0))*(17283.2281363905 + py1_v*(-33.7096989151921 + 0.14865741974709462*py2_v) - 150.65871107510267*py2_v) + (pow(px1_v,4.0))*(-0.15514794166823132 - 0.004939936127903132*py2_v) + (6.29198172439953e9)*py2_v + (pow(px1_v,2.0))*(-1.768164641850239e7 + py1_v*(34858.344458671374 + py1_v*(-0.31158418541571764 + 0.0013745450574731137*py2_v) - 154.48238358898652*py2_v) + 158576.89491208375*py2_v) + py1_v*(1.380366476701965e9 - 6176416.933199026*py2_v + py1_v*(-36551.17936271451 + 161.24452167172828*py2_v)) + px1_v*(6.054569635890569e9 - (5.481165389814905e7)*py2_v + py1_v*(-1.201489169398355e7 + py1_v*(213.43635536351522 - 0.9415686067589351*py2_v) + 53505.10604553184*py2_v)))/
			    ((-33.07535000864 + 0.09656982731202979*px1_v)*(1854.4933353525018 - 5.3848637714815855*px1_v - 0.049775282172777116*py1_v)*(-3681172.682386461 + 10579.648523146354*px1_v + (0.49121207647312665)*(pow(px1_v,2.0)) + 2259.508750994723*py1_v - 6.597069111485799*px1_v*py1_v + (1976.4929504069964 + px1_v*(-6.0108370796733555 + 0.0007009796673850188*px1_v + 0.027261560798726096*py1_v) - 9.337136560120587*py1_v)*py2_v));

//			//Toma del Tiempo de la estimacion para la pinza verde
//			t_final_v = clock();
//			seg_v = (t_final_v-t_inicial_v)/(double)CLOCKS_PER_SEC;

			}

			//Dibujamos el contorno de verde
			//cvDrawContours(imgDest,cont_v,CV_RGB(0,255,0),CV_RGB(0,255,0),-1,1,8,cvPoint(0,0));
			CvRect rect_v = cvBoundingRect( cont_v, 0 );
			Box_v = cvMinAreaRect2(cont_v, BoxStorage_v );
			//Dibujamos un rectangulo de color verde sobre el objeto
			cvRectangle(imgDest, cvPoint(rect_v.x, rect_v.y + rect_v.height),cvPoint(rect_v.x + rect_v.width, rect_v.y), CV_RGB(0, 255, 0), 1, 8, 0 );

			sprintf(Eti_v,"P1");
			cvPutText(imgDest,Eti_v,cvPoint(rect_v.x+10, rect_v.y ), &font_v, CV_RGB(0,0,0));

			//Dibujamos un circulo y una elipse de color verde
			cvCircle(imgDest, cvPoint (Box_v.center.x, Box_v.center.y), 5, CV_RGB(255,255,0), -4,8,0);
			//cvEllipse(imgDest, cvPoint(Box_v.center.x, Box_v.center.y), cvSize(Box_v.size.height/2, Box_v.size.width/2), Box_v.angle, 0, 360, CV_RGB(128, 128, 0), 1, 8, 0 );

			//Obtenemos el angulo de la pinza para dibujar una linea
			x1=(int)(x-35*sin(ang));
			y1=(int)(y-35*cos(ang));
			x2=(int)(x+35*sin(ang));
			y2=(int)(y+35*cos(ang));

			cvLine(imgDest,cvPoint(x1,y1),cvPoint(x2,y2),CV_RGB(255,255,255),2,8,0);

			//Dibujamos el centro de masa de la pinza
			cvLine(imgDest,cvPoint(Box_v.center.x-10,Box_v.center.y-10),cvPoint(Box_v.center.x+10,Box_v.center.y+10),CV_RGB(0,0,0),1,8,0);
			cvLine(imgDest,cvPoint(Box_v.center.x-10,Box_v.center.y+10),cvPoint(Box_v.center.x+10,Box_v.center.y-10),CV_RGB(0,0,0),1,8,0);

		}

		for(;cont_a!=0;cont_a=cont_a->h_next)
		{

			double x=0.0,y=0.0,angulo=0.0,ang=0.0;
			double u11=0.0,u20=0.0,u02=0.0;
			int x1=0,x2=0,y1=0,y2=0;

			cvMoments(cont_a,&momentsa,0);//Obtencion de momentos geometricos

			u11=cvGetCentralMoment(&momentsa,1,1);//Obtencion de momentos centrales
			u20=cvGetCentralMoment(&momentsa,2,0);
			u02=cvGetCentralMoment(&momentsa,0,2);

			x=momentsa.m10/momentsa.m00;
			y=momentsa.m01/momentsa.m00;

			ang=(0.5)*(atan2(2.0*(u11),(u02-u20)));// Angulo en radianes
			angulo=(((ang)*180)/(3.141592))+90;// Lo pasamos a grados

			if (Cam==1)
				{

				px1_a=x;
				py1_a=y;
				angxy_a=angulo;

				}

			else if (Cam==2)
				{

				px2_a=x;
				py2_a=y;
				angxz_a=angulo;

				//Estimacion de la posicion de la Pinza Azul
				X_a=(-4218042.572964758 - 35046.782735411056*px1_a + (138.2829291531299)*(pow(px1_a,2.0)) + 11976.27356141024*py1_a - 34.967027389427464*px1_a*py1_a + (61001.98163565843 + px1_a*(-325.04583809598637 + 0.4290161636740012*px1_a + 0.15360600571426627*py1_a) - 52.610349876804854*py1_a)*py2_a)/
					(-3681172.682386461 + 10579.648523146354*px1_a + (0.49121207647312665)*(pow(px1_a,2.0)) + 2259.508750994723*py1_a - 6.597069111485799*px1_a*py1_a + (1976.4929504069964 + px1_a*(-6.0108370796733555 + 0.0007009796673850188*px1_a + 0.027261560798726096*py1_a) - 9.337136560120587*py1_a)*py2_a);

				Y_a=(-5.405737179150725e9 +(pow(px1_a,3.0))*(-13.641342009875881 - 0.027461422173199274*py2_a) + (6.800377335691601e7)*py2_a + (pow(px1_a,2.0))*(-36458.97742793409 + py1_a*(-607.6140135310608 - 2.780067702426688*py2_a) + 595.3897772807168*py2_a) + py1_a*(-7.15106827144821e7 - 329716.07250046456*py2_a + py1_a*(1923.2673077288346 + 8.800710192541828*py2_a)) + px1_a*(2.987058815947646e7 - 399250.75938182353*py2_a + py1_a*(416898.11033562664 + py1_a*(-5.615347735813202 - 0.02569536114644222*py2_a) + 1914.8477938763172*py2_a)))/
			        ((1854.4933353525018 - 5.3848637714815855*px1_a - 0.049775282172777116*py1_a)*(-3681172.682386461 + 10579.648523146354*px1_a + 0.49121207647312665*(pow(px1_a,2.0)) + 2259.508750994723*py1_a - 6.597069111485799*px1_a*py1_a + (1976.4929504069964 + px1_a*(-6.0108370796733555 + 0.0007009796673850188*px1_a + 0.027261560798726096*py1_a) - 9.337136560120587*py1_a)*py2_a));

				Z_a=(-6.917825201948971e11 + (pow(px1_a,3.0))*(17283.2281363905 + py1_a*(-33.7096989151921 + 0.14865741974709462*py2_a) - 150.65871107510267*py2_a) + (pow(px1_a,4.0))*(-0.15514794166823132 - 0.004939936127903132*py2_a) + (6.29198172439953e9)*py2_a + (pow(px1_a,2.0))*(-1.768164641850239e7 + py1_a*(34858.344458671374 + py1_a*(-0.31158418541571764 + 0.0013745450574731137*py2_a) - 154.48238358898652*py2_a) + 158576.89491208375*py2_a) + py1_a*(1.380366476701965e9 - 6176416.933199026*py2_a + py1_a*(-36551.17936271451 + 161.24452167172828*py2_a)) + px1_a*(6.054569635890569e9 - (5.481165389814905e7)*py2_a + py1_a*(-1.201489169398355e7 + py1_a*(213.43635536351522 - 0.9415686067589351*py2_a) + 53505.10604553184*py2_a)))/
					((-33.07535000864 + 0.09656982731202979*px1_a)*(1854.4933353525018 - 5.3848637714815855*px1_a - 0.049775282172777116*py1_a)*(-3681172.682386461 + 10579.648523146354*px1_a + (0.49121207647312665)*(pow(px1_a,2.0)) + 2259.508750994723*py1_a - 6.597069111485799*px1_a*py1_a + (1976.4929504069964 + px1_a*(-6.0108370796733555 + 0.0007009796673850188*px1_a + 0.027261560798726096*py1_a) - 9.337136560120587*py1_a)*py2_a));

				}

			//Dibujamos el contorno de azul
			//cvDrawContours(imgDest,cont_a,CV_RGB(0,0,255),CV_RGB(0,0,255),-1,1,8,cvPoint(0,0));
			CvRect rect_a = cvBoundingRect( cont_a, 0 );
			Box_a = cvMinAreaRect2(cont_a, BoxStorage_a );
			cvRectangle(imgDest, cvPoint(rect_a.x, rect_a.y + rect_a.height),cvPoint(rect_a.x + rect_a.width, rect_a.y), CV_RGB(0, 0, 255), 1, 8, 0 );

			sprintf(Eti_a,"P2");
			cvPutText(imgDest,Eti_a,cvPoint(rect_a.x+10, rect_a.y ), &font_a, CV_RGB(0,0,0));

			//Dibujamos un circulo y una elipse de color verde
			cvCircle(imgDest, cvPoint (Box_a.center.x, Box_a.center.y), 5, CV_RGB(255,255,0), -4,8,0);
			//cvEllipse(imgDest, cvPoint(Box_a.center.x, Box_a.center.y), cvSize(Box_a.size.height/2, Box_a.size.width/2), Box_a.angle, 0, 360, CV_RGB(0, 128, 128), 1, 8, 0 );

			//Obtenemos el angulo de la pinza para dibujar una linea
			x1=(int)(x-35*sin(ang));
			y1=(int)(y-35*cos(ang));
			x2=(int)(x+35*sin(ang));
			y2=(int)(y+35*cos(ang));

			cvLine(imgDest,cvPoint(x1,y1),cvPoint(x2,y2),CV_RGB(255,255,255),2,8,0);

			//Dibujamos el centro de masa de la pinza
			cvLine(imgDest,cvPoint(Box_a.center.x-10,Box_a.center.y-10),cvPoint(Box_a.center.x+10,Box_a.center.y+10),CV_RGB(0,0,0),1,8,0);
			cvLine(imgDest,cvPoint(Box_a.center.x-10,Box_a.center.y+10),cvPoint(Box_a.center.x+10,Box_a.center.y-10),CV_RGB(0,0,0),1,8,0);

		}

		//Toma del Tiempo de la estimacion
		t_final_muestra = clock();
		seg_muestra = (t_final_muestra-t_inicial)/(double)CLOCKS_PER_SEC;

		fprintf(fileCoordinates,"%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",X_v,Y_v,Z_v,X_a,Y_a,Z_a,angxy_v,angxz_v,angxy_a,angxz_a,seg_muestra);
		//printf("\nEstimacion: %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \t %f \n",X_v,Y_v,Z_v,X_a,Y_a,Z_a,angxy_v,angxz_v,angxy_a,angxz_a);

		cvReleaseMemStorage(&Storage_v);
		cvReleaseMemStorage(&BoxStorage_v);
		cvReleaseMemStorage(&Storage_a);
		cvReleaseMemStorage(&BoxStorage_a);

		cvReleaseImage(&HSV_v);
		cvReleaseImage(&Bin_v);
		cvReleaseImage(&HSV_a);
		cvReleaseImage(&Bin_a);

		return imgDest;

}

