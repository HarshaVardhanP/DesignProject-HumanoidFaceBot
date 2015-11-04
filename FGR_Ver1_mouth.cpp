 #include "opencv2/objdetect/objdetect.hpp"
 #include "opencv2/highgui/highgui.hpp"
 #include "opencv2/imgproc/imgproc.hpp"

 #include <iostream>
 #include <stdio.h>

 using namespace std;
 using namespace cv; 
     
 /** Function Headers */
 void detectAndDisplay( Mat frame );
 
 /** Global variables */
 String face_cascade_name = "../../data/haarcascades/haarcascade_frontalface_alt.xml";
 String eyes_cascade_name = "../../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
 String mouth_cascade_name = "../../data/haarcascades/haarcascade_mcs_mouth.xml";

 CascadeClassifier face_cascade;
 CascadeClassifier eyes_cascade;
 CascadeClassifier mouth_cascade;

 int flag;
 string window_name = "Capture - Face detection";
 RNG rng(12345);

 /** @function main */
 int main( int argc, const char** argv )
 {
   
   CvCapture* capture;
   Mat frame;

   //-- 1. Load the cascades
   if( !face_cascade.load(face_cascade_name) ){ printf("--(!)Error loading\n"); return -1; };
   if( !eyes_cascade.load(eyes_cascade_name) ){ printf("--(!)Error loading\n"); return -1; };
   if( !mouth_cascade.load(mouth_cascade_name) ){ printf("--(!)Error loading\n"); return -1; };


   //-- 2. Read the video stream
   capture = cvCaptureFromCAM( -1 );
   if( capture )
   {
     while( true )
     {
       frame = cvQueryFrame( capture );

       //-- 3. Apply the classifier to the frame
       if( !frame.empty() )
         { detectAndDisplay( frame ); }
       else
         { printf(" --(!) No captured frame -- Break!"); break; }

       int c = waitKey(10);
       if( (char)c == 'c' ) { break; }
      }
   }
  return 0;
 }

/** @function detectAndDisplay */
void detectAndDisplay( Mat frame )
{ 
  std::vector<Rect> faces;
  Mat frame_gray;
  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );

  //-- Detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
  for( size_t i = 0; i < faces.size(); i++ )
  {
    Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
       Point pt_1(faces[i].x,faces[i].y);
       Point pt_2(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
    rectangle(frame, pt_1, pt_2, Scalar( 0, 0, 255 ), 4, 8, 0);
    //ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    
    Mat faceROI = frame_gray( faces[i] );
    std::vector<Rect> eyes;
    std::vector<Rect> mouth;

    //-- In each face, detect eyes
/*    eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) ); 
    for( size_t j = 0; j < eyes.size(); j++)
     {	
       Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
       int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
       circle( frame, center, radius, Scalar( 0, 255, 0 ), 4, 8, 0 );
     } */

//----------------Mouth Cascade -----------------------------

    mouth_cascade.detectMultiScale( faceROI, mouth, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) ); 
    for( size_t j = 0; j < mouth.size(); j++ )
     {
       	
       Point center( faces[i].x + mouth[j].x + mouth[j].width*0.5, faces[i].y + mouth[j].y + mouth[j].height*0.5 );
       Point pt1(faces[i].x + mouth[j].x,faces[i].y + mouth[j].y);
       Point pt2(faces[i].x + mouth[j].x+mouth[j].width,faces[i].y + mouth[j].y + mouth[j].height);
       int radius = cvRound( (mouth[j].width + mouth[j].height)*0.25 );
       if( faces[i].y + mouth[j].y + mouth[j].height*0.5 > faces[i].y + faces[i].height*0.75 )
       {
       float ratio = ((double)(mouth[j].height*mouth[j].width))/((double)(faces[i].height*faces[i].width));
  
      if((0.1>ratio)&&(ratio>0.06))
       {    
       printf("m = %12d \n",mouth[j].height*mouth[j].width); 
       printf("f = %12d \n",faces[i].height*faces[i].width); 
       printf("m/f = %f \n",ratio);
            flag = 1;
            rectangle(frame, pt1, pt2, Scalar( 255, 0, 0 ), 4, 8, 0);
          //circle( frame, center, radius, Scalar( 255, 0, 0 ), 4, 8, 0 );
       }
       else
            flag = 0;
       }      
     } 
     printf("\n %d \n",flag);

//------------------------------------------------------------------------------*/

    /*------------Arduino Transfer writing into dev file directly
    FILE *file;
    file = fopen("/dev/ttyUSB0","w");  //Opening device file
    fprintf(file,"%d",flag); //Writing to the fileq
    fprintf(file,"%c",','); //To separate digits
    fclose(file);
    //---------------------------------------------------*/ 
  }
  //-- Show what you got
  imshow( window_name, frame );
 }
