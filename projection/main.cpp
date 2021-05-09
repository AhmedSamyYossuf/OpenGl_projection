#include<windows.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>
#include<iostream>
#include <cmath>
using namespace std;

#define PI 3.14159265

/*
    *assume that
        1- center of cuboid in the origin point

*/
struct point
{
    int x;
    int y;
};

struct color
{
     float red   ;
     float blue  ;
     float green ;
};


int length , width , height , x_viewer , y_viewer , z_viewer ;
float Composite_matrix[4][4] , viewer_origin[4] , center_cuboid[4] , center_after_Composite[4];
float point_1[4] , point_2[4] , point_3[4] , point_4[4] , point_5[4] , point_6[4] , point_7[4] , point_8[4] ;
float Par_point_1[4] , Par_point_2[4] , Par_point_3[4] , Par_point_4[4] , Par_point_5[4] , Par_point_6[4] , Par_point_7[4] , Par_point_8[4] ;
float persp_point_1[4] , persp_point_2[4] , persp_point_3[4] , persp_point_4[4] , persp_point_5[4] , persp_point_6[4] , persp_point_7[4] , persp_point_8[4] ;
float Z_Projection_Plane;
void input();
void viewer_to_origin_center_Composite();
void mult_2matrix_4x4(float A[4][4], float B[4][4] ,float (&out_mat)[4][4]);
void mult_2matrix_4x4_4x1(float A[4][4], float B[4] , float *out_mat);
void print_matrix(float mat[4][4] );
void print_vector(float mat[4] );
void point_of_cuboid(float center[4]);  // function to determine all point for cuboid
void parallel_projection();
void perspective_projection();

void initial_window(int initPos_X ,int initPos_Y ,int Size_x ,int Size_y ,char name[] , int min_x , int max_x , int min_y , int max_y );
void Draw_Point(int x , int y);
void Draw_Point_color(int x , int y , color point_color);
void line(int x1 , int y1 , int x2 , int y2 , point arr_point[] , int &arrSize);
bool compare(point i , point j);
void Fill(point *arr_point , int arrSize , color point_color);
void polyline(int *x , int *y , int Size , point arr_point[] , int &arrSize);
void polygon(int *x , int *y , int Size , point arr_point[] , int &arrSize);
void show_shape_fill_parallel();

int main()
{

    input();
    viewer_to_origin_center_Composite();
    point_of_cuboid(center_after_Composite);
    parallel_projection();
    perspective_projection();
    print_vector(Par_point_1);
    print_vector(Par_point_2);
    print_vector(Par_point_3);
    print_vector(Par_point_4);
    print_vector(Par_point_5);
    print_vector(Par_point_6);
    initial_window(100 , 100 , 800 , 400 ,"Draw and color" ,-100 , 100 , -100 , 100);
    glutDisplayFunc(show_shape_fill_parallel);
    glutMainLoop();
    return 0 ;
}

void input()
{
    cout<<"Enter the dimension of the cuboid"<<endl;
	cout<<"length :";
	cin>>length;
	cout<<"width :";
	cin>>width;
    cout<<"height :";
    cin>>height;
	cout<<"Enter the position of the viewer "<<endl;
    cout<<"x :";
    cin>>x_viewer;
    cout<<"y :";
    cin>>y_viewer;
    cout<<"z :";
    cin>>z_viewer;

}

void viewer_to_origin_center_Composite()
{

	//translation matrix to move  the viewer to the origin
    float viewer_to_origin[4][4]={ {1,0,0,-x_viewer} , {0,1,0,-y_viewer} , {0,0,1,-z_viewer} , {0,0,0,1} } ;

    //VisionPoint
    float v[4]={x_viewer , y_viewer , z_viewer , 1 };

    // center cuboid
    float center_cob[4]={0 , 0 , 0 , 1 };

    mult_2matrix_4x4_4x1( viewer_to_origin , v , viewer_origin);          // to move viewer to origin
    mult_2matrix_4x4_4x1( viewer_to_origin , center_cob , center_cuboid); // to move center with viewer

    //calculate tan-1 for x angle and y angle
    float y_Radian = atan(-x_viewer/-z_viewer);
    float x_Radian = atan(-y_viewer/-z_viewer);
    float seta_x = x_Radian *PI/180;
    float seta_y = y_Radian *PI/180;

    //RotateAround X Axis
    float Rotate_X_axis[4][4] = { {1,0,0,0} , {0,cos(-seta_y),-sin(-seta_y),0}
                                  , {0,sin(-seta_y),cos(-seta_y),0} , {0,0,0,1} };

    // RotateAround Y Axis

    float  Rotate_Y_axis[4][4]={ {cos(-seta_x),0,sin(-seta_x),0} , {0,1,0,0}
                                 , {-sin(-seta_x),0,cos(-seta_x),0} , {0,0,0,1} };

    mult_2matrix_4x4( Rotate_X_axis , Rotate_Y_axis , Composite_matrix );
    mult_2matrix_4x4_4x1(Composite_matrix , center_cuboid ,center_after_Composite);

}

void point_of_cuboid(float center[4])
{
    float w = .5 * width ;
    float h = .5 * height ;
    float L = .5 * length ;

    point_1[3] = point_2[3] = point_3[3] = point_4[3] = point_5[3] = point_6[3] =point_7[3] = point_8[3] = 1;

    point_1[0] = center_after_Composite[0] + w ;
    point_1[1] = center_after_Composite[1] + L ;
    point_1[2] = center_after_Composite[2] + h ;

    point_2[0] = center_after_Composite[0] + w ;
    point_2[1] = center_after_Composite[1] + L ;
    point_2[2] = center_after_Composite[2] - h ;

    point_3[0] = center_after_Composite[0] + w ;
    point_3[1] = center_after_Composite[1] - L ;
    point_3[2] = center_after_Composite[2] + h ;

    point_4[0] = center_after_Composite[0] + w ;
    point_4[1] = center_after_Composite[1] - L ;
    point_4[2] = center_after_Composite[2] - h ;

    point_5[0] = center_after_Composite[0] - w ;
    point_5[1] = center_after_Composite[1] + L ;
    point_5[2] = center_after_Composite[2] + h ;

    point_6[0] = center_after_Composite[0] - w ;
    point_6[1] = center_after_Composite[1] + L ;
    point_6[2] = center_after_Composite[2] - h ;

    point_7[0] = center_after_Composite[0] - w ;
    point_7[1] = center_after_Composite[1] - L ;
    point_7[2] = center_after_Composite[2] + h ;

    point_8[0] = center_after_Composite[0] - w ;
    point_8[1] = center_after_Composite[1] - L ;
    point_8[2] = center_after_Composite[2] - h ;
}

void parallel_projection()
{
    Z_Projection_Plane = center_after_Composite[2] / 2;

    Par_point_1[0] = point_1[0] ;
    Par_point_1[1] = point_1[1] ;
    Par_point_1[2] = Z_Projection_Plane;

    Par_point_2[0] = point_2[0] ;
    Par_point_2[1] = point_2[1] ;
    Par_point_2[2] = Z_Projection_Plane;

    Par_point_3[0] = point_3[0] ;
    Par_point_3[1] = point_3[1] ;
    Par_point_3[2] = Z_Projection_Plane;

    Par_point_4[0] = point_4[0] ;
    Par_point_4[1] = point_4[1] ;
    Par_point_4[2] = Z_Projection_Plane;

    Par_point_5[0] = point_5[0] ;
    Par_point_5[1] = point_5[1] ;
    Par_point_5[2] = Z_Projection_Plane;

    Par_point_6[0] = point_6[0] ;
    Par_point_6[1] = point_6[1] ;
    Par_point_6[2] = Z_Projection_Plane;

    Par_point_7[0] = point_7[0] ;
    Par_point_7[1] = point_7[1] ;
    Par_point_7[2] = Z_Projection_Plane;

    Par_point_8[0] = point_8[0] ;
    Par_point_8[1] = point_8[1] ;
    Par_point_8[2] = Z_Projection_Plane;
}

void perspective_projection()
{
    Z_Projection_Plane = center_after_Composite[2] / 2;

    persp_point_1[0] = point_1[0]*Z_Projection_Plane / point_1[2] ;
    persp_point_1[1] = point_1[1]*Z_Projection_Plane / point_1[2]  ;
    persp_point_1[2] = Z_Projection_Plane;

    persp_point_2[0] = point_2[0]*Z_Projection_Plane / point_2[2]  ;
    persp_point_2[1] = point_2[1]*Z_Projection_Plane / point_2[2]  ;
    persp_point_2[2] = Z_Projection_Plane;

    persp_point_3[0] = point_3[0]*Z_Projection_Plane / point_3[2]  ;
    persp_point_3[1] = point_3[1]*Z_Projection_Plane / point_3[2]  ;
    persp_point_3[2] = Z_Projection_Plane;

    persp_point_4[0] = point_4[0]*Z_Projection_Plane / point_4[2]  ;
    persp_point_4[1] = point_4[1]*Z_Projection_Plane / point_4[2]  ;
    persp_point_4[2] = Z_Projection_Plane;

    persp_point_5[0] = point_5[0]*Z_Projection_Plane / point_5[2]  ;
    persp_point_5[1] = point_5[1]*Z_Projection_Plane / point_5[2]  ;
    persp_point_5[2] = Z_Projection_Plane;

    persp_point_6[0] = point_6[0]*Z_Projection_Plane / point_6[2]  ;
    persp_point_6[1] = point_6[1]*Z_Projection_Plane / point_6[2]  ;
    persp_point_6[2] = Z_Projection_Plane;

    persp_point_7[0] = point_7[0]*Z_Projection_Plane / point_7[2]  ;
    persp_point_7[1] = point_7[1]*Z_Projection_Plane / point_7[2]  ;
    persp_point_7[2] = Z_Projection_Plane;

    persp_point_8[0] = point_8[0]*Z_Projection_Plane / point_8[2]  ;
    persp_point_8[1] = point_8[1]*Z_Projection_Plane / point_8[2]  ;
    persp_point_8[2] = Z_Projection_Plane;
}

void mult_2matrix_4x4(float A[4][4], float B[4][4] , float (&out_mat)[4][4])
{

    float num;

    for (int i=0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            num = 0;
            for(int k=0; k<4; k++)
            {
                num += A[i][k]*B[k][j];
            }
            out_mat[i][j]=num;
        }
    }
}

void mult_2matrix_4x4_4x1(float A[4][4], float B[4] , float *out_mat)
{
    float  sum ;
    for(int i = 0 ; i < 4 ; i++)
    {
        sum = 0 ;
        for(int j = 0 ; j < 4 ; j++ )
        {
            sum = sum + (A[i][j]*B[j]);
        }
        out_mat[i] = sum ;
    }
}

void print_matrix(float mat[4][4] )
{
    for(int i = 0 ; i < 4 ; i++)
    {
        for(int j = 0 ; j < 4 ; j++ )
        {
           cout << mat[i][j] << "  ";
        }
        cout<<endl;
    }
}
void print_vector(float mat[4] )
{
    for(int j = 0 ; j < 4 ; j++ )
    {
        cout << mat[j]<<"  ";
    }
}




void initial_window(int initPos_X ,int initPos_Y ,int Size_x ,int Size_y ,char name[] , int min_x , int max_x , int min_y , int max_y )
{
    glutInitWindowPosition(initPos_X , initPos_Y);
    glutInitWindowSize(Size_x , Size_y);
    glutCreateWindow(name);
    glClearColor(1.0,1.0,1.0,0.0);
    glLoadIdentity();
    gluOrtho2D(min_x , max_x , min_y , max_y);
}

void Draw_Point(int x , int y)
{
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0,0,0);
    glVertex2i(x , y) ;
    glEnd();
}

void Draw_Point_color(int x , int y , color point_color)
{
    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f( point_color.red , point_color.blue , point_color.green );
    glVertex2i(x , y) ;
    glEnd();
}

void line(int x1 , int y1 , int x2 , int y2 , point arr_point[] , int &arrSize)
{
    int minX , minY , maxX , maxY ;

    if(x1 > x2)
    {
        maxX = x1 ;
        minX = x2 ;
    }
    else
    {
        maxX = x2 ;
        minX = x1 ;
    }

    if(y1 > y2)
    {
        maxY = y1 ;
        minY = y2 ;
    }
    else
    {
        maxY = y2 ;
        minY = y1 ;
    }

    if(x1 == x2)
    {
       for(int y = minY ; y < maxY ; y++)
       {
           Draw_Point(x1 , y);
           arr_point[arrSize].x = x1  ;
           arr_point[arrSize].y = y ;
           arrSize++;
       }
       return ;
    }

    if(y1 == y2)
    {
       for(int x = minX ; x < maxX ; x++)
       {
           Draw_Point(x , y1);
           arr_point[arrSize].x = x  ;
           arr_point[arrSize].y = y1 ;
           arrSize++;
       }
       return ;
    }

    float m = (float)(y2 - y1)/(x2 - x1);
    float c = y2 - (float)m * x2 ;

    if (abs(int(m)) <= 1)
    {
        int y ;
        for(int x = minX ; x <= maxX ; x++)
        {
            y = (int)((m * x) + c + 0.5);
            Draw_Point(x , y);
            arr_point[arrSize].x = x ;
            arr_point[arrSize].y = y ;
            arrSize++;
        }
    }
    else
    {
        int x ;
        for(int y = minY ; y <= maxY ; y++)
        {
            x = (int)((( y - c ) / m ) + 0.5);
            Draw_Point(x , y);
            arr_point[arrSize].x = x ;
            arr_point[arrSize].y = y ;
            arrSize++;
        }
    }
}

bool compare(point i , point j)
{
    if(i.y < j.y)
    {
        return true ;
    }
    else if(i.y > j.y)
    {
        return false ;
    }
    else if(i.x < j.x)
    {
        return true ;
    }
    else
    {
        return false ;
    }
}

void Fill(point *arr_point , int arrSize , color point_color)
{
    std::sort(arr_point , arr_point + arrSize , compare) ; // sort on y
    for(long i =0 ; i < arrSize - 1 ; i++ )
    {
        if(arr_point[i].y == arr_point[i + 1 ].y)
        {
            for(int j = arr_point[i].x+1 ; j < arr_point[i + 1].x ; j++)
            {
                Draw_Point_color(j , arr_point[i].y , point_color);
            }
        }
    }
}

void polyline(int *x , int *y , int Size , point arr_point[] , int &arrSize)
{
    for (int i = 0 ; i < (Size - 1 ) ; i++)
    {
        line(x[i] , y[i] , x[i + 1 ] , y[i + 1] ,arr_point ,arrSize);
    }
}

void polygon(int *x , int *y , int Size , point arr_point[] , int &arrSize)
{
    polyline(x , y , Size , arr_point , arrSize);
    line(x[0] , y[0] , x[Size - 1 ] , y[Size - 1] , arr_point , arrSize);
}

void show_shape_fill_parallel()
{
    glClear(GL_COLOR_BUFFER_BIT);
    int arrSize = 0 ;
    point arr_point[1500];
    color point_color = {0.6 , 0.8 , 0};
    int x[] = {Par_point_1[0] , Par_point_3[0] , Par_point_7[0] , Par_point_5[0]};
    int y[] = {Par_point_1[1] , Par_point_3[1] , Par_point_7[1] , Par_point_5[1]};
    polygon(x , y , 4 , arr_point , arrSize );
    Fill(arr_point , arrSize , point_color);
    glFlush();
}



