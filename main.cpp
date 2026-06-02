/*
===========================================================
                PROJECT: Dynamic City Lake View
                Instruction Manual
===========================================================

1. VEHICLE CONTROLS (CAR & BOAT)
-----------------------------------------------------------
[C] - Start Car: Initiates car movement on the road.
[S] - Stop Car: Halts car movement.
[B] - Start Boat: Moves the boat in the lake (Left to Right).
[N] - Stop Boat: Halts the boat movement.

2. FERRIS WHEEL (NAGORDOLA) CONTROLS
-----------------------------------------------------------
[Left Mouse Click]  - Start anticlockwise rotation.
[Right Mouse Click] - Stop rotation immediately.
[H] - Increase Speed: Accelerates the rotation.
[L] - Decrease Speed: Decelerates the rotation.

3. ENVIRONMENT TOGGLE
-----------------------------------------------------------
[1] - Overcast Scenario:
      * Switches to a overcast sky.
      * Starts the rain.
      * Turns on Building lights and Street Lampposts.

[2] - Clear Sky Scenario:
      * Returns to a bright blue sky.
      * Stops the rain.
      * Deactivates city lights.

===========================================================
*/

#include <windows.h>
#include <GL/glut.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <cmath>

#define MAX_DROPS 10000

const float groundY = -0.8f;

// Raindrop data
float dropX[MAX_DROPS];
float dropY[MAX_DROPS];
float dropSpeed[MAX_DROPS];
int dropCount = 0;

float boat_move_x = 0.0f; //Boat initial position
float boat_speed = 0.0f;; //Boat Initially Stop

float car_1_move_x = 0.0f; //Car 1 initial position
float car_2_move_x = 0.0f; //Car 2 initial position
float car_speed = 0.0f; //Initially Car Stop

float nagordola_angle = 0.0f;
float nagordola_speed = 0.0f; //Initially stop

float cloud_move = 0.0f; //Cloud initial position

bool is_overcast_sky = false; //Initially clear sky

void initGL() {
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void drawDrops() {
    //glColor3f(0.2f, 0.4f, 1.0f); // Rain color
    glColor3f(81.0f/255.0f, 245.0f/255.0f, 252.0f/255.0f);
    for (int i = 0; i < dropCount; ++i) {
        glBegin(GL_LINES);
        glVertex2f(dropX[i], dropY[i]);
        glVertex2f(dropX[i], dropY[i] - 0.05f);
        glEnd();
    }
}

void rain_animation(int value) {
    if (dropCount < MAX_DROPS && (rand() % 5) == 0) {
        dropX[dropCount] = (rand() % 2000 / 1000.0f) - 1.0f;  // -1 to 1 range
        dropY[dropCount] = 0.9f;
        dropSpeed[dropCount] = 0.01f + (rand() % 100 / 10000.0f);
        dropCount++;
    }

    for (int i = 0; i < dropCount; ) {
        dropY[i] -= dropSpeed[i];

        if (dropY[i] <= groundY) {
            dropX[i] = dropX[dropCount - 1];
            dropY[i] = dropY[dropCount - 1];
            dropSpeed[i] = dropSpeed[dropCount - 1];
            dropCount--;
        } else {
            i++;
        }
    }

    glutPostRedisplay();
    glutTimerFunc(10, rain_animation, 0);
}

void line(float a, float b, float c, float d)
{
    glLineWidth(1.0);
    glBegin(GL_LINES);
	glColor3f(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f); //Black Color
	glVertex2f(a, b);
	glVertex2f(c, d);
	glEnd();
}

void field()
{
    //Field associated with lake and road  //Obj 1 - Grass Field
    glBegin(GL_POLYGON);
	glColor3f(21.0f/255.0f, 173.0f/255.0f, 52.0f/255.0f); //Filed color
	glVertex2f(-1, -0.6); //B
	glVertex2f(-1, -0.5); //E
	glVertex2f(1, -0.5); //H
	glVertex2f(1, -0.6); //C
	glEnd();

	//Main Field
	glBegin(GL_POLYGON);
	glColor3f(21.0f/255.0f, 173.0f/255.0f, 52.0f/255.0f); //Filed color
	glVertex2f(-1, 0.52); //U
	glVertex2f(1, 0.52); //V
	glVertex2f(1, 0); //G
	glVertex2f(-1, 0); //F
	glEnd();
}

void lake()
{
    //Obj 2 - Lake
    glBegin(GL_POLYGON);
	glColor3f(125.0f/255.0f, 244.0f/255.0f, 255.0f/255.0f); //Water Color
	glVertex2f(-1, -1); //A
	glVertex2f(-1, -0.6); //B
	glVertex2f(1, -0.6); //C
	glVertex2f(1, -1); //D
	glEnd();
}

void road()
{
    //Road Structure  //Obj 3 - Road
    glBegin(GL_POLYGON);
	glColor3f(135.0f/255.0f, 135.0f/255.0f, 135.0f/255.0f); //Road color
	glVertex2f(-1, -0.5); //E
	glVertex2f(-1, 0); //F
	glVertex2f(1, 0); //G
	glVertex2f(1, -0.5); //H
	glEnd();

	//Road Divider 1
	glBegin(GL_POLYGON);
	glColor3f(255.0f/255.0f, 165.0f/255.0f, 31.0f/255.0f); //Road Divider Color
	glVertex2f(-0.8, -0.26); //I
	glVertex2f(-0.8, -0.24); //J
	glVertex2f(-0.5, -0.24); //K
	glVertex2f(-0.5, -0.26); //L
	glEnd();

	//Road Divider 2
	glBegin(GL_POLYGON);
	glColor3f(255.0f/255.0f, 165.0f/255.0f, 31.0f/255.0f); //Road Divider Color
	glVertex2f(-0.2, -0.26); //M
	glVertex2f(-0.2, -0.24); //N
	glVertex2f(0.1, -0.24); //O
	glVertex2f(0.1, -0.26); //P
	glEnd();

	//Road Divider 3
	glBegin(GL_POLYGON);
	glColor3f(255.0f/255.0f, 165.0f/255.0f, 31.0f/255.0f); //Road Divider Color
	glVertex2f(0.4, -0.26); //Q
	glVertex2f(0.4, -0.24); //R
	glVertex2f(0.7, -0.24); //S
	glVertex2f(0.7, -0.26); //T
	glEnd();
}

void boat()
{
    //Obj 4 - Boat

    glPushMatrix();
    glTranslatef(boat_move_x, 0.0f, 0.0f);

    //Boat_Base
    glBegin(GL_POLYGON);
	glColor3f(171.0f/255.0f, 69.0f/255.0f, 12.0f/255.0f); // Chocolate color
	glVertex2f(-0.8, -0.8); //A1
	glVertex2f(-0.72, -0.86); //W
	glVertex2f(-0.48, -0.86); //Z
	glVertex2f(-0.4, -0.8); //B1
	glEnd();

	//Boat room
	glBegin(GL_POLYGON);
	glColor3f(224.0f/255.0f, 182.0f/255.0f, 47.0f/255.0f); //
	glVertex2f(-0.7, -0.8); //C1
	glVertex2f(-0.7, -0.75); ////D1
	glVertex2f(-0.5, -0.75); //E1
	glVertex2f(-0.5, -0.8); //F1
	glEnd();

	//Boat half circle associated with left side
	glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            glColor3f(176.0f/255.0f, 176.0f/255.0f, 176.0f/255.0f);
            float pi=3.1416;
            float A=(i*pi)/200;
            float r=0.05;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.7, y-0.8); //C1
        }
	glEnd();

	//Boat half circle associated with right side
	glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            glColor3f(176.0f/255.0f, 176.0f/255.0f, 176.0f/255.0f);
            float pi=3.1416;
            float A=(i*pi)/200;
            float r=0.05;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.5, y-0.8); //F1
        }
	glEnd();

	//Boat base border
    line(-0.8, -0.8, -0.4, -0.8); //Boat upper - A1B1
    line(-0.72, -0.86, -0.48, -0.86); //Boat lower - WZ
    line(-0.8, -0.8, -0.72, -0.86); //Boat left - A1W
    line(-0.48, -0.86, -0.4, -0.8); //Boat right - ZB1

    //Boat room upper border
    line(-0.7, -0.75, -0.5, -0.75);

    //Circle border (Left Side)
	glLineWidth(1.5);
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<=200;i++)
        {
            glColor3f(0.0, 0.0, 0.0); //Black
            float pi=3.1416;
            float A=(i*pi)/200;
            float r=0.05;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.5, y-0.8); //F1
        }
	glEnd();

	//Circle border (Left Side)
	glLineWidth(1.5);
	glBegin(GL_LINE_STRIP);
	for(int i=0;i<=200;i++)
        {
            glColor3f(0.0, 0.0, 0.0); //Black
            float pi = 3.1416;
            float A = (i*pi)/200;
            float r = 0.05;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.7, y-0.8); //C1
        }
	glEnd();

	glPopMatrix();
}

void boat_animation(int value) {
    boat_move_x += boat_speed;

    if(boat_move_x > 1.8f) {
        boat_move_x = -1.03f;
    }

    glutPostRedisplay();

    glutTimerFunc(25, boat_animation, 0);
}

void vertical_railing(float a, float b, float c, float d, float e, float f, float g, float h)
	{
	    glBegin(GL_POLYGON);
	    glColor3f(87.0f/255.0f, 87.0f/255.0f, 87.0f/255.0f);
	    glVertex2f(a, b);
	    glVertex2f(c, d);
	    glVertex2f(e, f);
	    glVertex2f(g, h);
	    glEnd();
	}

void railing()
{
    //Obj 5 - Railing
    //Railing upper part
	glBegin(GL_POLYGON);
	glColor3f(87.0f/255.0f, 87.0f/255.0f, 87.0f/255.0f); //
	glVertex2f(-0.99, -0.53); // G1
	glVertex2f(-0.99, -0.54); // H1
	glVertex2f(0.99, -0.54); // I1
	glVertex2f(0.99, -0.53); // J1
	glEnd();

	//Railing lower part
	glBegin(GL_POLYGON);
	glColor3f(87.0f/255.0f, 87.0f/255.0f, 87.0f/255.0f); //
	glVertex2f(0.99, -0.56); // K1
	glVertex2f(0.99, -0.57); // L1
	glVertex2f(-0.99, -0.57); // M1
	glVertex2f(-0.99, -0.56); // N1
	glEnd();

	//Vertical railing beside lake
    vertical_railing(-0.98, -0.53, -0.98, -0.59, -0.99, -0.59, -0.99, -0.53); //O1 P1 Q1 G1
    vertical_railing(-0.9, -0.53, -0.9, -0.59, -0.89, -0.59, -0.89, -0.53); //R1 S1 T1 U1
    vertical_railing(-0.81, -0.53, -0.81, -0.59, -0.8, -0.59, -0.8, -0.53); // V1 W1 Z1 A2
    vertical_railing(-0.72, -0.53, -0.72, -0.59, -0.71, -0.59, -0.71, -0.53); //B2 C2 D2 E2
    vertical_railing(-0.63, -0.53, -0.63, -0.59, -0.62, -0.59, -0.62, -0.53); //F2 G2 H2 I2
    vertical_railing(-0.54, -0.53, -0.54, -0.59, -0.53, -0.59, -0.53, -0.53); //J2 K2 L2 M2
    vertical_railing(-0.45, -0.53, -0.45, -0.59, -0.44, -0.59, -0.44, -0.53); //N2 O2 P2 Q2
    vertical_railing(-0.36, -0.53, -0.36, -0.59, -0.35, -0.59, -0.35, -0.53); //R2 S2 T2 U2
    vertical_railing(-0.27, -0.53, -0.27, -0.59, -0.26, -0.59, -0.26, -0.53); //V2 W2 Z2 A3
    vertical_railing(-0.18, -0.53, -0.18, -0.59, -0.17, -0.59, -0.17, -0.53); //B3 C3 D3 E3
    vertical_railing(-0.09, -0.53, -0.09, -0.59, -0.08, -0.59, -0.08, -0.53); //F3 G3 H3 I3
    vertical_railing(0.0, -0.53, 0.0, -0.59, 0.01, -0.59, 0.01, -0.53); //J3 K3 L3 M3
    vertical_railing(0.09, -0.53, 0.09, -0.59, 0.1, -0.59, 0.1, -0.53); //N3 O3 P3 Q3
    vertical_railing(0.18, -0.53, 0.18, -0.59, 0.19, -0.59, 0.19, -0.53); //R3 S3 T3 U3
    vertical_railing(0.27, -0.53, 0.27, -0.59, 0.28, -0.59, 0.28, -0.53); //V3 W3 Z3 A4
    vertical_railing(0.36, -0.53, 0.36, -0.59, 0.37, -0.59, 0.37, -0.53); //B4 C4 D4 E4
    vertical_railing(0.45, -0.53, 0.45, -0.59, 0.46, -0.59, 0.46, -0.53); //F4 G4 H4 I4
    vertical_railing(0.54, -0.53, 0.54, -0.59, 0.55, -0.59, 0.55, -0.53); //J4 K4 L4 M4
    vertical_railing(0.63, -0.53, 0.63, -0.59, 0.64, -0.59, 0.64, -0.53); //N4 O4 P4 Q4
    vertical_railing(0.72, -0.53, 0.72, -0.59, 0.73, -0.59, 0.73, -0.53); //R4 S4 T4 U4
    vertical_railing(0.81, -0.53, 0.81, -0.59, 0.82, -0.59, 0.82, -0.53); //V4 W4 Z4 A5
    vertical_railing(0.9, -0.53, 0.9, -0.59, 0.91, -0.59, 0.91, -0.53); //B5 C5 D5 E5
    vertical_railing(0.98, -0.53, 0.98, -0.59, 0.99, -0.59, 0.99, -0.53); //F5 G5 H5 J1
}

//Right side car
void car_1()
{
    //Obj 6 - Car_1
    glPushMatrix();
    glTranslatef(car_1_move_x, 0.0f, 0.0f);

    //Car1
    glBegin(GL_POLYGON);
	glColor3f(128.0f/255.0f, 255.0f/255.0f, 51.0f/255.0f); //
	glVertex2f(0.84, -0.28); // I5
	glVertex2f(0.56, -0.28); // J5
	glVertex2f(0.56, -0.44); // K5
	glVertex2f(0.84, -0.44); // L5
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(255.0f/255.0f, 143.0f/255.0f, 112.0f/255.0f); //
	glVertex2f(0.4, -0.44); // M5
	glVertex2f(0.4, -0.38); // O5
	glVertex2f(0.56, -0.38); // N5
	glVertex2f(0.56, -0.44); // K5
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(255.0f/255.0f, 143.0f/255.0f, 112.0f/255.0f); //
	glVertex2f(0.56, -0.3); // P5
	glVertex2f(0.48, -0.3); // Q5
	glVertex2f(0.44, -0.38); // R5
	glVertex2f(0.56, -0.38); // N5
	glEnd();

	//Car 1 Window
	glBegin(GL_POLYGON);
	glColor3f(117.0f/255.0f, 227.0f/255.0f, 240.0f/255.0f); //Glass color
	glVertex2f(0.56, -0.32); // U5
	glVertex2f(0.5, -0.32); // S5
	glVertex2f(0.48, -0.38); // T5
	glVertex2f(0.56, -0.38); // N5
	glEnd();

	//Car 1 Head light
	glBegin(GL_POLYGON);
	glColor3f(247.0f/255.0f, 247.0f/255.0f, 0.0f/255.0f); //Head light color
	glVertex2f(0.4, -0.4); // V6
	glVertex2f(0.42, -0.4); // W6
	glVertex2f(0.42, -0.42); // Z6
	glVertex2f(0.4, -0.42); // A7
	glEnd();

	//Car 1 front wheel
	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r=0.04;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+0.52, y-0.44);
        }
	glEnd();

	//Car 1 Inside front wheel
	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(153.0f/255.0f, 153.0f/255.0f, 153.0f/255.0f);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r=0.023;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+0.52, y-0.44);
        }
	glEnd();

	//Car 1 back wheel
	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r=0.04;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+0.76, y-0.44);
        }
	glEnd();

	//Car 1 back right wheel
	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(153.0f/255.0f, 153.0f/255.0f, 153.0f/255.0f);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r=0.023;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+0.76, y-0.44);
        }
	glEnd();

	glPopMatrix();
}

void car_2()
{
    //Obj 7 - Car_2

    glPushMatrix();
    glTranslatef(car_2_move_x, 0.0f, 0.0f);

    //Car2
    glBegin(GL_POLYGON);
	glColor3f(255.0f/255.0f, 112.0f/255.0f, 51.0f/255.0f); //
	glVertex2f(-0.9, -0.16); // Z5
	glVertex2f(-0.48, -0.16); // A6
	glVertex2f(-0.48, -0.1); // B6
	glVertex2f(-0.9, -0.1); // C6
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(255.0f/255.0f, 112.0f/255.0f, 51.0f/255.0f); //
	glVertex2f(-0.82, -0.02); // D6
	glVertex2f(-0.86, -0.1); // E6
	glVertex2f(-0.56, -0.1); // F6
	glVertex2f(-0.6, -0.02); // G6
	glEnd();

	//Car 2 back side window
	glBegin(GL_POLYGON);
	glColor3f(117.0f/255.0f, 227.0f/255.0f, 240.0f/255.0f); //Glass color
	glVertex2f(-0.8, -0.04); // H6
	glVertex2f(-0.82, -0.1); // I6
	glVertex2f(-0.72, -0.1); // J6
	glVertex2f(-0.72, -0.04); // K6
	glEnd();

	//Car 2 front side window
	glBegin(GL_POLYGON);
	glColor3f(117.0f/255.0f, 227.0f/255.0f, 240.0f/255.0f); //Glass color
	glVertex2f(-0.7, -0.04); // L6
	glVertex2f(-0.7, -0.1); // M6
	glVertex2f(-0.6, -0.1); // N6
	glVertex2f(-0.62, -0.04); // O6
	glEnd();

	//Car 2 Head light
	glBegin(GL_POLYGON);
	glColor3f(247.0f/255.0f, 247.0f/255.0f, 0.0f/255.0f); //Head light color
	glVertex2f(-0.48, -0.12); // P6
	glVertex2f(-0.5, -0.12); // Q6
	glVertex2f(-0.5, -0.14); // R6
	glVertex2f(-0.48, -0.14); // S6
	glEnd();

	//Car 2 Back wheel
	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r=0.04;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.8, y-0.16);
        }
	glEnd();

	//Car 2 Inside Back wheel
	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(153.0f/255.0f, 153.0f/255.0f, 153.0f/255.0f);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r=0.023;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.8, y-0.16);
        }
	glEnd();

	//Car 2 front wheel
	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(0.0f/255.0f, 0.0f/255.0f, 0.0f/255.0f);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r=0.04;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.58, y-0.16);
        }
	glEnd();

	//Car 2 Inside front wheel
	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(153.0f/255.0f, 153.0f/255.0f, 153.0f/255.0f);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r=0.023;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.58, y-0.16);
        }
	glEnd();

	glPopMatrix();
}

void car_animation(int value)
{
    //Car 1 moves left
    car_1_move_x -= car_speed;
    if(car_1_move_x < -1.8) car_1_move_x = 1.05f;

    //Car 2 moves right
    car_2_move_x += car_speed;
    if(car_2_move_x > 1.8) car_2_move_x = -1.05;

    glutPostRedisplay();

    glutTimerFunc(25, car_animation, 0);
}

void border()
{
    line(-1, -0.6, 1, -0.6); // lake boundary
    line(-1, -0.5, 1, -0.5); // lower road line
    line(-1, 0, 1, 0);       // upper road line
    line(-1, 0.52, 1, 0.52);   //upper filed line (UV)
}

void nagordola_Wheel(float a, float b, float radius, float R, float G, float B)
{
    //Obj 8 - FERRIS WHEEL

    glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(R, G, B);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r = radius;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+a, y+b);
        }
	glEnd();
}

void nagordola_line(float a, float b, float c, float d, float e, float f, float g, float h)
{
    glBegin(GL_POLYGON);
	glColor3f(255.0/255.0, 255.0/255.0, 255.0/255.0);
	glVertex2f(a, b);
	glVertex2f(c, d);
	glVertex2f(e, f);
	glVertex2f(g, h);
	glEnd();
}
//Obj 9 - Upper Cabin
void cabin_upper(float a, float b, float c, float d, float e, float f, float g, float h)
{
    glBegin(GL_POLYGON);
	glColor3f(29.0/255.0, 0.0/255.0, 255.0/255.0); //Blue
	glVertex2f(a, b);
	glVertex2f(c, d);
	glVertex2f(e, f);
	glVertex2f(g, h);
	glEnd();
}

void cabin_lower(float a, float b, float c, float d, float e, float f, float g, float h)
{
    glBegin(GL_POLYGON);
	glColor3f(255.0/255.0, 0.0/255.0, 0.0/255.0); //Red
	glVertex2f(a, b);
	glVertex2f(c, d);
	glVertex2f(e, f);
	glVertex2f(g, h);
	glEnd();
}

void nagordola()
{
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    glTranslatef(-0.75, 0.35, 0.0);
    glRotatef(nagordola_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.75, -0.35, 0.0);

    // Outside wheel
    nagordola_Wheel(-0.75, 0.35, 0.15, 255.0/255.0, 255.0/255.0, 255.0/255.0);
    // Inside wheel
    nagordola_Wheel(-0.75, 0.35, 0.13, 21.0f/255.0f, 173.0f/255.0f, 52.0f/255.0f);

    // Nagordola main cross line
    nagordola_line(-0.89, 0.36, -0.89, 0.34, -0.61, 0.34, -0.61, 0.36);
    nagordola_line(-0.74, 0.49, -0.76, 0.49, -0.76, 0.21, -0.74, 0.21);

    // Top Cabin(W7, Z7, A8, B8 vertex)
    glPushMatrix();
    glTranslatef(-0.75, 0.49, 0.0);
    glRotatef(-nagordola_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.75, -0.49, 0.0);
    nagordola_line(-0.745, 0.49, -0.755, 0.49, -0.755, 0.425, -0.745, 0.425);
    cabin_upper(-0.72, 0.425, -0.78, 0.425, -0.78, 0.39, -0.72, 0.39);
    cabin_lower(-0.78, 0.39, -0.72, 0.39, -0.72, 0.365, -0.78, 0.365);
    glPopMatrix();

    // Right Cabin (O7, P7, Q7, R7 vertex)
    glPushMatrix();
    glTranslatef(-0.61, 0.35, 0.0);
    glRotatef(-nagordola_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.61, -0.35, 0.0);
    nagordola_line(-0.615, 0.35, -0.605, 0.35, -0.605, 0.285, -0.615, 0.285);
    cabin_upper(-0.58, 0.285, -0.64, 0.285, -0.64, 0.25, -0.58, 0.25);
    cabin_lower(-0.64, 0.25, -0.58, 0.25, -0.58, 0.225, -0.64, 0.225);
    glPopMatrix();

    // Left Cabin(S7, T7, U7, V7 vertex)
    glPushMatrix();
    glTranslatef(-0.89, 0.35, 0.0);
    glRotatef(-nagordola_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.89, -0.35, 0.0);
    nagordola_line(-0.885, 0.35, -0.895, 0.35, -0.895, 0.285, -0.885, 0.285);
    cabin_upper(-0.86, 0.285, -0.92, 0.285, -0.92, 0.25, -0.86, 0.25);
    cabin_lower(-0.92, 0.25, -0.86, 0.25, -0.86, 0.225, -0.92, 0.225);
    glPopMatrix();

    // Down Cabin (K7, L7, M7, N7 vertex)
    glPushMatrix();
    glTranslatef(-0.75, 0.21, 0.0);
    glRotatef(-nagordola_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.75, -0.21, 0.0);
    nagordola_line(-0.745, 0.21, -0.755, 0.21, -0.755, 0.145, -0.745, 0.145);
    cabin_upper(-0.72, 0.145, -0.78, 0.145, -0.78, 0.11, -0.72, 0.11);
    cabin_lower(-0.78, 0.11, -0.72, 0.11, -0.72, 0.085, -0.78, 0.085);
    glPopMatrix();

    glPopMatrix();
    glutSwapBuffers();

    //Nagordola Support stand 1
    glBegin(GL_POLYGON);
	glColor3f(122.0/255.0, 80.0/255.0, 75.0/255.0); // Nagordola Support stand color
	glVertex2f(-0.75, 0.35); //B7
	glVertex2f(-0.65, 0.05); //C9
	glVertex2f(-0.62, 0.05);//D9
	glEnd();

	//Nagordola Support stand 2
	glBegin(GL_POLYGON);
	glColor3f(122.0/255.0, 80.0/255.0, 75.0/255.0); // Nagordola Support stand color
	glVertex2f(-0.75, 0.35); //B7
	glVertex2f(-0.85, 0.05); //E9
	glVertex2f(-0.88, 0.05);//F9
	glEnd();

	glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            glColor3f(122.0/255.0, 80.0/255.0, 75.0/255.0);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r = 0.015;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x-0.75, y+0.35);
        }
	glEnd();
}

void nagordola_rotation(int value) {
    nagordola_angle += nagordola_speed;
	glutPostRedisplay();
	glutTimerFunc(20, nagordola_rotation, 0);
}

void bench()
{
    //Obj 13 - Bench
    //Left side leg
    glBegin(GL_POLYGON);
	glColor3f(217.0/255.0, 140.0/255.0, 0.0/255.0); // bench leg color
	glVertex2f(-0.27, 0.05); //Q9
	glVertex2f(-0.25, 0.05); //R9
	glVertex2f(-0.25, 0.11);//S9
	glVertex2f(-0.27, 0.11); //T9
	glEnd();
	//border
	line(-0.27, 0.11, -0.27, 0.05); //T9 Q9
	//line(-0.27, 0.05, -0.25, 0.05); //Q9 R9
	line(-0.25, 0.05, -0.25, 0.11); //R9 S9

	//Right side leg
    glBegin(GL_POLYGON);
	glColor3f(217.0/255.0, 140.0/255.0, 0.0/255.0); // bench leg color
	glVertex2f(-0.06, 0.05); //U9
	glVertex2f(-0.04, 0.05); //V9
	glVertex2f(-0.04, 0.11);//W9
	glVertex2f(-0.06, 0.11); //Z9
	glEnd();
	//border
	line(-0.06, 0.11, -0.06, 0.05); //Z9 U9
	//line(-0.06, 0.05, -0.04, 0.05); //U9 V9
	line(-0.04, 0.05, -0.04, 0.11); //V9 W9

	//Seat
    glBegin(GL_POLYGON);
	glColor3f(255.0/255.0, 208.0/255.0, 0.0/255.0); // bench seat color
	glVertex2f(-0.31, 0.11); //A10
	glVertex2f(0, 0.11); //B10
	glVertex2f(-0.02, 0.21);//C10
	glVertex2f(-0.29, 0.21); //D10
	glEnd();
	//Seat Border
	line(-0.31, 0.11, 0, 0.11); //A10 B10
	line(0, 0.11, -0.02, 0.21); //B10 C10
	line(-0.02, 0.21, -0.29, 0.21); //C10 D10
	line(-0.29, 0.21, -0.31, 0.11); //D10 A10

	//Back support left
    glBegin(GL_POLYGON);
	glColor3f(217.0/255.0, 140.0/255.0, 0.0/255.0); // bench leg color
	glVertex2f(-0.26, 0.21); //E10
	glVertex2f(-0.24, 0.21); //F10
	glVertex2f(-0.24, 0.24);//G10
	glVertex2f(-0.26, 0.24); //H10
	glEnd();
	//border
	line(-0.24, 0.21, -0.24, 0.24); //F10 G10
	line(-0.26, 0.24, -0.26, 0.21); //H10 E10

	//Back support right
    glBegin(GL_POLYGON);
	glColor3f(217.0/255.0, 140.0/255.0, 0.0/255.0); // bench leg color
	glVertex2f(-0.05, 0.21); //I10
	glVertex2f(-0.07, 0.21); //J10
	glVertex2f(-0.07, 0.24);//K10
	glVertex2f(-0.05, 0.24); //L10
	glEnd();
	//border
	line(-0.07, 0.21, -0.07, 0.24); //J10 K10
	line(-0.05, 0.24, -0.05, 0.21); //L10 I10

	//Back support
    glBegin(GL_POLYGON);
	glColor3f(189.0/255.0, 189.0/255.0, 189.0/255.0); // Back support color
	glVertex2f(-0.29, 0.24); //M10
	glVertex2f(-0.02, 0.24); //N10
	glVertex2f(-0.02, 0.33);//O10
	glVertex2f(-0.29, 0.33); //P10
	glEnd();
	//Back support border
	line(-0.29, 0.24, -0.02, 0.24); //M10 N10
	line(-0.02, 0.24, -0.02, 0.33); //N10 O10
	line(-0.02, 0.33, -0.29, 0.33); //O10P10
	line(-0.29, 0.33, -0.29, 0.24); //P10 M10

	//Back inside color
    glBegin(GL_POLYGON);
	glColor3f(255.0/255.0, 208.0/255.0, 0.0/255.0); // back inside color
	glVertex2f(-0.27, 0.26); //Q10
	glVertex2f(-0.04, 0.26); //R10
	glVertex2f(-0.04, 0.31);//S10
	glVertex2f(-0.27, 0.31); //T10
	glEnd();
	//Back inside color border
	line(-0.27, 0.26, -0.04, 0.26); //Q10 R10
	line(-0.04, 0.26, -0.04, 0.31); //R10 S10
	line(-0.04, 0.31, -0.27, 0.31); //S10 T10
	line(-0.27, 0.31, -0.27, 0.26); // T10 Q10
}

void lp(float a, float b, float c, float d, float e, float f, float g, float h)
{
    //box
    glBegin(GL_POLYGON);
	glColor3f(0.0/255.0, 0.0/255.0, 0.0/255.0); // back inside color
	glVertex2f(a, b);
	glVertex2f(c, d);
	glVertex2f(e, f);
	glVertex2f(g, h);
	glEnd();
}

void lp_light(float a, float b)
{
    glBegin(GL_POLYGON);
	for(int i=0;i<200;i++)
        {
            if(is_overcast_sky) glColor3f(255.0/255.0, 223.0/255.0, 0.0/255.0);
            else glColor3f(224.0/255.0, 224.0/255.0, 224.0/255.0);
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r = 0.02;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+a, y+b);
        }
	glEnd();
}

void lamp_post()
{
    //Left lamp post start from here
    glBegin(GL_POLYGON);
	glColor3f(245.0/255.0, 76.0/255.0, 0.0/255.0); //
	glVertex2f(-0.45, 0.07); //E11
	glVertex2f(-0.45, 0.05); //F11
	glVertex2f(-0.38, 0.05); //G11
	glVertex2f(-0.38, 0.07); //H11
	glEnd();
	//Border
	line(-0.45, 0.07, -0.45, 0.05); //E11 F11
	//line(-0.45, 0.05, -0.38, 0.05); //F11 G11
	line(-0.38, 0.05, -0.38, 0.07); //G11 H11
	line(-0.38, 0.07, -0.45, 0.07); //H11 E11

    lp(-0.4, 0.07, -0.43, 0.07, -0.43, 0.3, -0.4, 0.3); //U10 V10 W10 Z10 (stand)
    lp(-0.36, 0.3, -0.36, 0.31, -0.47, 0.31, -0.47, 0.30); //A11 B11 C11 D11
    lp(-0.47, 0.33, -0.47, 0.31, -0.46, 0.31, -0.46, 0.33); //N11 C11 L11 Q11
    lp(-0.36, 0.31, -0.37, 0.31, -0.37, 0.33, -0.36, 0.33); // B11 I11 Z11 W11
    lp(-0.41, 0.31, -0.42, 0.31, -0.42, 0.36, -0.41, 0.36); //P11 O11 F12 G12
    lp(-0.46, 0.34, -0.46, 0.33, -0.49, 0.33, -0.49, 0.34); //M11 Q11 R11 S11
    lp(-0.37, 0.34, -0.37, 0.33, -0.34, 0.33, -0.34, 0.34); //J11 Z11 A12 D12
    lp(-0.49, 0.34, -0.49, 0.36, -0.48, 0.36, -0.48, 0.34); //S11 U11 T11 V11;
    lp(-0.34, 0.34, -0.34, 0.36, -0.35, 0.36, -0.35, 0.34); // D12 B12 C12 E12
    lp_light(-0.485, 0.375); //J12
    lp_light(-0.415, 0.375); //H12
    lp_light(-0.345, 0.375); //L12
    //Left lamp post end here

    //Right lamp post start from here
    glBegin(GL_POLYGON);
	glColor3f(245.0/255.0, 76.0/255.0, 0.0/255.0); //
	glVertex2f(0.07, 0.05); //K11
	glVertex2f(0.14, 0.05); //N12
	glVertex2f(0.14, 0.07); //O12
	glVertex2f(0.07, 0.07); //P12
	glEnd();
	//Border
	line(0.07, 0.07, 0.14, 0.07); //P12 O12
	line(0.07, 0.07, 0.07, 0.05); //P12 K11
	line(0.14, 0.07, 0.14, 0.05); //O12 N12

	lp(0.12, 0.07, 0.09, 0.07, 0.09, 0.3, 0.12, 0.3); //Q12 R12 S12 T12
	lp(0.16, 0.3, 0.16, 0.31, 0.05, 0.31, 0.05, 0.3); //U12 V12 W12 Z12
	lp(0.06, 0.31, 0.06, 0.33, 0.05, 0.33, 0.05, 0.31); //A13 B13 C13 W12
    lp(0.15, 0.31, 0.15, 0.33, 0.16, 0.33, 0.16, 0.31); //D13 E13 F13 V12
    lp(0.11, 0.31, 0.1, 0.31, 0.1, 0.36, 0.11, 0.36); //H13 I13 J13 K13
    lp(0.06, 0.33, 0.06, 0.34, 0.03, 0.34, 0.03, 0.33); //B13 L13 M13 N13
    lp(0.15, 0.33, 0.15, 0.34, 0.18, 0.34, 0.18, 0.33); //E13 O13 P13 Q13
    lp(0.03, 0.34, 0.03, 0.36, 0.04, 0.36, 0.04, 0.34); //M13 R13 S13 T13
    lp(0.18, 0.34, 0.18, 0.36, 0.17, 0.36, 0.17, 0.34); //P13 U13 V13 W13
    lp_light(0.035, 0.375); //A14
    lp_light(0.105, 0.375); //B12
    lp_light(0.175, 0.375); //C14
    //Right lamp post end here
}

void c_trunk(float a, float b, float c, float d, float e, float f, float g, float h)
{
    //Trunk
    glBegin(GL_POLYGON);
	glColor3f(245.0/255.0, 76.0/255.0, 0.0/255.0); // Trunk color
	glVertex2f(a, b);
	glVertex2f(c, d);
	glVertex2f(e, f);
	glVertex2f(g, h);
	glEnd();
}

void c_leaf(float a, float b, float c, float d, float e, float f)
{
    glBegin(GL_POLYGON);
	glColor3f(58.0/255.0, 227.0/255.0, 57.0/255.0); // leaf color
	glVertex2f(a, b);
	glVertex2f(c, d);
	glVertex2f(e, f);
	glEnd();
}

void chrismas_tree()
{
    //Right Tree
    c_trunk(0.92, 0.05, 0.94, 0.05, 0.94, 0.18, 0.92, 0.18); //G14 H14 I14 J14
    c_leaf(0.87, 0.18, 0.99, 0.18, 0.93, 0.23); //K14 L14 M14
    c_leaf(0.88, 0.21, 0.98, 0.21, 0.93, 0.26); //N14 O14 P14
    c_leaf(0.89, 0.24, 0.97, 0.24, 0.93, 0.29); //Q14 R14 S14

    line(0.906, 0.21, 0.87, 0.18); //T14 K14
    line(0.87, 0.18, 0.99, 0.18); //K14 L14
    line(0.99, 0.18, 0.954, 0.21); //L14 U14
    line(0.906, 0.21, 0.88, 0.21); //T14 N14
    line(0.954, 0.21, 0.98, 0.21); //U14 O14
    line(0.88, 0.21, 0.91, 0.24); //N14 V14
    line(0.98, 0.21, 0.95, 0.24); //O14 W14
    line(0.91, 0.24, 0.89, 0.24); //V14 Q14
    line(0.95, 0.24, 0.97, 0.24); //W14 R14
    line(0.89, 0.24, 0.93, 0.29); //Q14 S14
    line(0.97, 0.24, 0.93, 0.29); //R14 S14
    line(0.92, 0.18, 0.92, 0.05); //J14 G14
    //line(0.92, 0.05, 0.94, 0.05); // G14 H14
    line(0.94, 0.05, 0.94, 0.18); // H14 I14

    //Left Tree
    c_trunk(0.38, 0.05, 0.36, 0.05, 0.36, 0.18, 0.38, 0.18); //D15 E15 F15 G15
    c_leaf(0.31, 0.18, 0.43, 0.18, 0.37, 0.23); //H15 I15 J15
    c_leaf(0.32, 0.21, 0.42, 0.21, 0.37, 0.26); //K15 L15 M15
    c_leaf(0.33, 0.24, 0.41, 0.24, 0.37, 0.29); //N15 O15 P15

    line(0.31, 0.18, 0.43, 0.18); //H15 I15
    line(0.31, 0.18, 0.346, 0.21); //H15 Q15
    line(0.43, 0.18, 0.394, 0.21); //I15 R15
    line(0.346, 0.21, 0.32, 0.21); //Q15 K15
    line(0.394, 0.21, 0.42, 0.21); //R15 L15
    line(0.32, 0.21, 0.35, 0.24); //K15 S15
    line(0.42, 0.21, 0.39, 0.24); //L15 T15
    line(0.35, 0.24, 0.33, 0.24); //S15 N15
    line(0.39, 0.24, 0.41, 0.24); //T15 O15
    line(0.33, 0.24, 0.37, 0.29); //N15 P15
    line(0.41, 0.24, 0.37, 0.29); //O15 P15
    line(0.36, 0.05, 0.36, 0.18); //E15 F15
    line(0.38, 0.05, 0.38, 0.18); //D15 G15

    //Left up tree
    c_trunk(0.402, 0.32, 0.418, 0.32, 0.418, 0.41, 0.402, 0.41); //U17 V17 Z17 W17
    c_leaf(0.37, 0.41, 0.45, 0.41, 0.41, 0.44); //A18 B18 C18
    c_leaf(0.38, 0.43, 0.44, 0.43, 0.41, 0.46); //D18 E18 F18
    c_leaf(0.39, 0.45, 0.43, 0.45, 0.41, 0.475); //G18 H18 I18

    line(0.37, 0.41, 0.45, 0.41); //A18 B18
    line(0.37, 0.41, 0.397, 0.43); //A18 J18
    line(0.45, 0.41, 0.4233333, 0.43); //B18 K18
    line(0.397, 0.43, 0.38, 0.43); //J18 D18
    line(0.4233333, 0.43, 0.44, 0.43); //K18 E18
    line(0.38, 0.43, 0.4, 0.45); //D18 L18
    line(0.44, 0.43, 0.42, 0.45); //E18 M18
    line(0.4, 0.45, 0.39, 0.45); //L18 G18
    line(0.42, 0.45, 0.43, 0.45); //M18 H18
    line(0.39, 0.45, 0.41, 0.475); //G18 I18
    line(0.43, 0.45, 0.41, 0.475); //H18 I18
    line(0.402, 0.32, 0.402, 0.41); //U17 W17
    line(0.418, 0.32, 0.418, 0.41); //V17 Z17

    //Right up tree
    c_trunk(0.898, 0.32, 0.882, 0.32, 0.882, 0.41, 0.898, 0.41); //K12 N18 O18 P18
    c_leaf(0.85, 0.41, 0.93, 0.41, 0.89, 0.44); //Q18 R18 S18
    c_leaf(0.86, 0.43, 0.92, 0.43, 0.89, 0.46); //T18 U18 V18
    c_leaf(0.87, 0.45, 0.91, 0.45, 0.89, 0.475); //W18 Z18 A19

    line(0.85, 0.41, 0.93, 0.41); //Q18 R18
    line(0.85, 0.41, 0.876666666667, 0.43); //Q18 B19
    line(0.93, 0.41, 0.903333333333, 0.43); //R18 C19
    line(0.876666666667, 0.43, 0.86, 0.43); //B19 T18
    line(0.903333333333, 0.43, 0.92, 0.43); //C19 U18
    line(0.86, 0.43, 0.88, 0.45); //T18 D19
    line(0.92, 0.43, 0.9, 0.45); //U18 E19
    line(0.88, 0.45, 0.87, 0.45); //D19 W18
    line(0.9, 0.45, 0.91, 0.45); //E19 Z18
    line(0.87, 0.45, 0.89, 0.475); //W18 A19
    line(0.91, 0.45, 0.89, 0.475); //Z18 A19
    line(0.882, 0.41, 0.882, 0.32); //O18 N18
    line(0.898, 0.41, 0.898, 0.32); //P18 K12
}

void park_road()
{
    glBegin(GL_POLYGON);
	glColor3f(217.0/255.0, 193.0/255.0, 167.0/255.0); // Park road color
	glVertex2f(0.88, 0); //Z14
	glVertex2f(0.42, 0); //A15
	glVertex2f(0.55, 0.52); //B15
	glVertex2f(0.75, 0.52); //C15
	glEnd();
}

void hedge(float a, float b, float c, float d, float e, float f, float g)
{
    glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            glColor3f(0.0/255.0, 145.0/255.0, 7.0/255.0); // leaf color
            float pi=3.1416;
            float A=(i*pi)/200;
            float r = g;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+a, y+b);
        }
	glEnd();

	glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            glColor3f(0.0/255.0, 145.0/255.0, 7.0/255.0); // leaf color
            float pi=3.1416;
            float A=(i*pi)/200;
            float r = g;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+c, y+d);
        }
	glEnd();

	glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            glColor3f(0.0/255.0, 145.0/255.0, 7.0/255.0); // leaf color
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r = g;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+e, y+f);
        }
	glEnd();
}

void hedge_tree()
{
    hedge(0.48, 0.04, 0.52, 0.04, 0.5, 0.06, 0.02); //U15 V15 W15 radius
    hedge(0.82, 0.04, 0.78, 0.04, 0.8, 0.06, 0.02); //C16 D16 E16 radius
    hedge(0.5, 0.14, 0.54, 0.14, 0.52, 0.16, 0.02); //I16 J16 K16 radius
    hedge(0.8, 0.14, 0.76, 0.14, 0.78, 0.16, 0.02); //O16 P16 Q16 radius
    hedge(0.52, 0.24, 0.55, 0.24, 0.535, 0.255, 0.015); //U16 V16 W16 radius
    hedge(0.78, 0.24, 0.75, 0.24, 0.765, 0.255, 0.015); //A17 B17 C17 radius
    hedge(0.55, 0.34, 0.58, 0.34, 0.565, 0.355, 0.015); //E17 F17 G17 radius
    hedge(0.72, 0.34, 0.75, 0.34, 0.735, 0.355, 0.015); //J17 H17 L17 radius
    hedge(0.575, 0.44, 0.595, 0.44, 0.585, 0.45, 0.010); //M17 N17 P17 radius
    hedge(0.725, 0.44, 0.705, 0.44, 0.715, 0.45, 0.010); //Q17 R17 T17 radius

    //Sky part hedge
    hedge(-0.96, 0.52, -0.88, 0.52, -0.92, 0.56, 0.04); //H19 I19 J19 radius
    hedge(-0.82, 0.52, -0.74, 0.52, -0.78, 0.56, 0.04); //L19 M19 O19 radius
    hedge(-0.67, 0.52, -0.61, 0.52, -0.64, 0.55, 0.03); //O20 Q20 S20 radius
    hedge(0.08, 0.52, 0.16, 0.52, 0.12, 0.56, 0.04); //P25 Q25 R25 radius
    hedge(0.24, 0.52, 0.32, 0.52, 0.28, 0.56, 0.04); //T25 U25 V25 radius
    hedge(0.505, 0.52, 0.535, 0.52, 0.52, 0.535, 0.015); //W27 V27 Z27 radius
    hedge(0.78, 0.52, 0.83, 0.52, 0.805, 0.545, 0.025); //O29 N29 P29 radius
}

void tree(float a, float b)
{
    glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            glColor3f(0.0/255.0, 145.0/255.0, 7.0/255.0); // leaf color
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r = 0.02;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+a, y+b);
        }
	glEnd();
}

void tree_structure()
{
    //P19 Q19 R19  S19 Z19 //B20 C20 D20//E20
    //1st tree from left side
    c_trunk(-0.71, 0.52, -0.69, 0.52, -0.69, 0.62, -0.71, 0.62); //F20 G20 P19 Q19
    c_trunk(-0.73, 0.65, -0.72, 0.65, -0.70, 0.62, -0.71, 0.62); //D20 E20 C20 Q19
    c_trunk(-0.67, 0.65, -0.68, 0.65, -0.70, 0.62, -0.69, 0.62); //H20 I20 C20 P19
    tree(-0.70, 0.66); //R19
    tree(-0.72, 0.66); //S19
    tree(-0.68, 0.66); //T19
    tree(-0.74, 0.66); //U19
    tree(-0.66, 0.66); //V19
    tree(-0.72, 0.68); //W19
    tree(-0.70, 0.68); //Z19
    tree(-0.68, 0.68); //A20
    tree(-0.70, 0.70); //B20

    //I26 J26 K26 L26 M26 //N26 O26 P26 //Q26
    //2nd tree from left side
    c_trunk(0.19, 0.52, 0.19, 0.62, 0.21, 0.62, 0.21, 0.52); //Z25 A26 B26 C26
    c_trunk(0.19, 0.62, 0.20, 0.62, 0.18, 0.65, 0.17, 0.65); //A26 D26 F26 E26
    c_trunk(0.20, 0.62, 0.22, 0.65, 0.23, 0.65, 0.21, 0.62); //D26 H26 G26 B26
    tree(0.16, 0.66); //I26
    tree(0.18, 0.66); //J26
    tree(0.20, 0.66); //K26
    tree(0.22, 0.66); //L26
    tree(0.24, 0.66); //M26
    tree(0.18, 0.68); //N26
    tree(0.20, 0.68); //O26
    tree(0.22, 0.68); //P26
    tree(0.20, 0.70); //Q26

    //R29 S29 T29 U29 V29 //W29 Z29 A30 //B30
    //3rd tree from left
    c_trunk(0.79, 0.52, 0.79, 0.62, 0.81, 0.62, 0.81, 0.52); //C30 D30 E30 F20
    c_trunk(0.79, 0.62, 0.77, 0.65, 0.78, 0.65, 0.80, 0.62); //D30 H30 I30 G30
    c_trunk(0.80, 0.62, 0.82, 0.65, 0.83, 0.65, 0.81, 0.62); //G30 K30 J30 E30
    tree(0.84, 0.66); //R29
    tree(0.82, 0.66); //S29
    tree(0.8, 0.66); //T29
    tree(0.78, 0.66); //U29
    tree(0.76, 0.66); //V29
    tree(0.82, 0.68); //W29
    tree(0.8, 0.68); //Z29
    tree(0.78, 0.68); //A30
    tree(0.8, 0.7); //B30
}

void building(float a, float b, float c, float d, float e, float f, float g, float h, float R, float G, float B)
{
    glBegin(GL_POLYGON);
	glColor3f(R/255.0, G/255.0, B/255.0);
	glVertex2f(a, b);
	glVertex2f(c, d);
	glVertex2f(e, f);
	glVertex2f(g, h);
	glEnd();
}

void window(float a, float b, float c, float d, float e, float f, float g, float h)
{
    glBegin(GL_POLYGON);
	if(is_overcast_sky) glColor3f(247.0/255.0, 238.0/255.0, 35.0/255.0);
    else glColor3f(230.0/255.0, 210.0/255.0, 122.0/255.0);
	glVertex2f(a, b);
	glVertex2f(c, d);
	glVertex2f(e, f);
	glVertex2f(g, h);
	glEnd();
}

void all_building()
{
    //Building count from left
    building(-0.58, 0.52, -0.58, 0.76, -0.44, 0.76, -0.44, 0.52, 160.0, 36.0, 212.0); //J20 K20 L20 M20 (Building 1)
    window(-0.56, 0.73, -0.53, 0.73, -0.53, 0.71, -0.56, 0.71); //N20 U20 V20 W20
    window(-0.46, 0.73, -0.46, 0.71, -0.49, 0.71, -0.49, 0.73); //Z20 A21 B21 C21
    window(-0.53, 0.68, -0.56, 0.68, -0.56, 0.66, -0.53, 0.66); //D21 E21 F21 G21
    window(-0.46, 0.68, -0.49, 0.68, -0.49, 0.66, -0.46, 0.66); //H21 I21 J21 K21
    window(-0.53, 0.63, -0.56, 0.63, -0.56, 0.61, -0.53, 0.61); //L21 M21 N21 O21
    window(-0.46, 0.63, -0.49, 0.63, -0.49, 0.61, -0.46, 0.61); //P21 Q21 R21 S21
    window(-0.53, 0.58, -0.56, 0.58, -0.56, 0.56, -0.53, 0.56); //T21 U21 V21 W21
    window(-0.46, 0.58, -0.49, 0.58, -0.49, 0.56, -0.46, 0.56); //Z21 A22 B22 C22
    line(-0.58, 0.52, -0.58, 0.76); //J20 K20
    line(-0.58, 0.76, -0.44, 0.76); //K20 L21
    line(-0.44, 0.76, -0.44, 0.52); //L21 M21


    building(-0.42, 0.52, -0.42, 0.71, -0.28, 0.71, -0.28, 0.52, 71, 122, 90); //D22 E22 F22 G22 (Building 2)
    window(-0.4, 0.68, -0.4, 0.66, -0.37, 0.66, -0.37, 0.68); //H22 I22 J22 K22
    window(-0.33, 0.68, -0.33, 0.66, -0.3, 0.66, -0.3, 0.68); //L22 M22 N22 O22
    window(-0.4, 0.63, -0.4, 0.61, -0.37, 0.61, -0.37, 0.63); //P22 Q22 R22 S22
    window(-0.33, 0.63, -0.33, 0.61, -0.3, 0.61, -0.3, 0.63); //T22 U22 V22 W22
    window(-0.4, 0.58, -0.4, 0.56, -0.37, 0.56, -0.37, 0.58); //Z22 A23 B23 C23
    window(-0.33, 0.58, -0.33, 0.56, -0.3, 0.56, -0.3, 0.58); //D23 E23 F23 G23
    line(-0.42, 0.52, -0.42, 0.71); //D22 E22
    line(-0.42, 0.71, -0.28, 0.71); //E22 F22
    line(-0.28, 0.71, -0.28, 0.52); //F22 G22

    building(-0.26, 0.52, -0.26, 0.66, -0.12, 0.66, -0.12, 0.52, 122, 121, 125); //H23 I23 J23 K23 (Building 3)
    window(-0.24, 0.63, -0.24, 0.61, -0.21, 0.61, -0.21, 0.63); //L23 M23 N23 O23
    window(-0.17, 0.63, -0.17, 0.61, -0.14, 0.61, -0.14, 0.63); //P23 Q23 R23 S23
    window(-0.24, 0.58, -0.24, 0.56, -0.21, 0.56, -0.21, 0.58); //T23 U23 V23 W23
    window(-0.17, 0.58, -0.17, 0.56, -0.14, 0.56, -0.14, 0.58); //Z23 A24 B24 C24
    line(-0.26, 0.52, -0.26, 0.66); //H23 I23
    line(-0.26, 0.66, -0.12, 0.66); //I23 J23
    line(-0.12, 0.66, -0.12, 0.52); //J23 K23

    building(-0.10, 0.52, -0.10, 0.76, 0.04, 0.76, 0.04, 0.52, 71, 122, 90); //D24 E24 F24 G24 (Building 4)
    window(-0.08, 0.73, -0.08, 0.71, -0.05, 0.71, -0.05, 0.73); //H24 I24 J24 K24
    window(-0.01, 0.73, -0.01, 0.71, 0.02, 0.71, 0.02, 0.73); //L24 M24 N24 O24
    window(-0.08, 0.68, -0.08, 0.66, -0.05, 0.66, -0.05, 0.68); //P24 Q24 R24 S24
    window(-0.01, 0.68, -0.01, 0.66, 0.02, 0.66, 0.02, 0.68); //T24 U24 V24 W24
    window(-0.08, 0.63, -0.08, 0.61, -0.05, 0.61, -0.05, 0.63); //Z24 A25 B25 C25
    window(-0.01, 0.63, -0.01, 0.61, 0.02, 0.61, 0.02, 0.63); //D25 E25 F25 G25
    window(-0.08, 0.58, -0.08, 0.56, -0.05, 0.56, -0.05, 0.58); //H25 I25 J25 K25
    window(-0.01, 0.58, -0.01, 0.56, 0.02, 0.56, 0.02, 0.58); //L25 M25 N25 O25
    line(-0.10, 0.52, -0.10, 0.76); //D24 E24
    line(-0.10, 0.76, 0.04, 0.76); //E24 F24
    line(0.04, 0.76, 0.04, 0.52); //F24 G24

    building(0.36, 0.52, 0.36, 0.71, 0.50, 0.71, 0.50, 0.52, 160, 36, 212); //R26 S26 T26 U26 (Building 5)
    window(0.38, 0.68, 0.38, 0.66, 0.41, 0.66, 0.41, 0.68); //V26 W26 Z26 A27
    window(0.45, 0.68, 0.45, 0.66, 0.48, 0.66, 0.48, 0.68); //B27 C27 D27 E27
    window(0.38, 0.63, 0.38, 0.61, 0.41, 0.61, 0.41, 0.63); //F27 G27 H27 I27
    window(0.45, 0.63, 0.45, 0.61, 0.48, 0.61, 0.48, 0.63); //J27 K27 L27 M27
    window(0.38, 0.58, 0.38, 0.56, 0.41, 0.56, 0.41, 0.58); //N27 O27 P27 Q27
    window(0.45, 0.58, 0.45, 0.56, 0.48, 0.56, 0.48, 0.58); //R27 S27 T27 U27
    line(0.36, 0.52, 0.36, 0.71); //R26 S26
    line(0.36, 0.71, 0.50, 0.71); //S26 T26
    line(0.50, 0.71, 0.50, 0.52); //T26 U26

    building(0.98, 0.52, 0.98, 0.76, 0.86, 0.76, 0.86, 0.52, 122, 121, 125); //B28 C28 D28 E28 (Building 6)
    window(0.88, 0.73, 0.88, 0.71, 0.91, 0.71, 0.91, 0.73); //F28 G28 H28 I28
    window(0.93, 0.73, 0.93, 0.71, 0.96, 0.71, 0.96, 0.73); //J28 K28 L28 M28
    window(0.88, 0.68, 0.88, 0.66, 0.91, 0.66, 0.91, 0.68); //N28 O28 P28 Q28
    window(0.93, 0.68, 0.93, 0.66, 0.96, 0.66, 0.96, 0.68); //R28 S28 T28 U28
    window(0.88, 0.63, 0.88, 0.61, 0.91, 0.61, 0.91, 0.63); //V28 W28 Z28 A29
    window(0.93, 0.63, 0.93, 0.61, 0.96, 0.61, 0.96, 0.63); //B29 C29 D29 E29
    window(0.88, 0.58, 0.88, 0.56, 0.91, 0.56, 0.91, 0.58); //F29 G29 H29 I29
    window(0.93, 0.58, 0.93, 0.56, 0.96, 0.56, 0.96, 0.58); //J29 K29 L29 M29
    line(0.98, 0.52, 0.98, 0.76); //B28 C28
    line(0.98, 0.76, 0.86, 0.76); //C28 D28
    line(0.86, 0.76, 0.86, 0.52); //D28 E28
}

void cloud(float a, float b, float c, float d, float e, float f)
{
    //Middle
    glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            //glColor3f(255.0/255.0, 255.0/255.0, 255.0/255.0); // Cloud color
            if(is_overcast_sky) glColor3f(140.0/255.0, 140.0/255.0, 140.0/255.0); // Cloud color
            else glColor3f(255.0/255.0, 255.0/255.0, 255.0/255.0); // Cloud color
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r = 0.04;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+a, y+b);
        }
	glEnd();

	//left
	glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            //glColor3f(255.0/255.0, 255.0/255.0, 255.0/255.0); // Cloud color
            if(is_overcast_sky) glColor3f(140.0/255.0, 140.0/255.0, 140.0/255.0); // Cloud color
            else glColor3f(255.0/255.0, 255.0/255.0, 255.0/255.0); // Cloud color
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r = 0.027;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+c, y+d);
        }
	glEnd();

	//right
	glBegin(GL_POLYGON);
	for(int i=0;i<=200;i++)
        {
            //glColor3f(255.0/255.0, 255.0/255.0, 255.0/255.0); // Cloud color
            if(is_overcast_sky) glColor3f(140.0/255.0, 140.0/255.0, 140.0/255.0); // Cloud color
            else glColor3f(255.0/255.0, 255.0/255.0, 255.0/255.0); // Cloud color
            float pi=3.1416;
            float A=(i*2*pi)/200;
            float r = 0.027;
            float x = r * cos(A);
            float y = r * sin(A);
            glVertex2f(x+e, y+f);
        }
	glEnd();
}

void cloud_group()
{
    cloud(-0.88, 0.94, -0.92, 0.94, -0.84, 0.94); //M30 L30 N30
    cloud(-0.76, 0.86, -0.80, 0.86, -0.72, 0.86); //Q30 R30 S30
    cloud(-0.52, 0.92, -0.56, 0.92, -0.48, 0.92); //V30 W30 Z30
    cloud(-0.28, 0.86, -0.32, 0.86, -0.24, 0.86); //C31 D31 E31
    cloud(0.02, 0.86, -0.02, 0.86, 0.06, 0.86); //H31 I31 J31
    cloud(0.32, 0.92, 0.28, 0.92, 0.36, 0.92); //M31 N31 O31
    cloud(0.64, 0.86, 0.6, 0.86, 0.68, 0.86); //R31 S31 U31
    cloud(0.86, 0.92, 0.82, 0.92, 0.90, 0.92); //W31 Z31 A32
}

void all_cloud()
{
    glPushMatrix();
    glTranslatef(cloud_move, 0.0f, 0.0f);
    cloud_group();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(cloud_move - 2.0, 0.0f, 0.0f);
    cloud_group();
    glPopMatrix();
}

void cloud_animation(int value) {
    cloud_move += 0.0008f;

    if(cloud_move > 2.0f) {
        cloud_move = 0.0f;
    }

    glutPostRedisplay();
    glutTimerFunc(25, cloud_animation, 0);
}

void clear_sky()
{
    glBegin(GL_POLYGON);
	glColor3f(74.0/255.0, 244.0/255.0, 256.0/255.0); // sky color
	glVertex2f(-1, 0.52); //U
	glVertex2f(1, 0.52); //V
	glVertex2f(1, 1); //F19
	glVertex2f(-1, 1); //G19
	glEnd();
}

void overcast_sky()
{
    glBegin(GL_POLYGON);
	glColor3f(173.0/255.0, 173.0/255.0, 173.0/255.0); // sky color
	glVertex2f(-1, 0.52); //U
	glVertex2f(1, 0.52); //V
	glVertex2f(1, 1); //F19
	glVertex2f(-1, 1); //G19
	glEnd();
}

void handleKeypress(unsigned char key, int x, int y) {
    switch (key) {
        case 'c': //Car move
            car_speed = 0.01f;
            break;
        case 's': // Car Stop
            car_speed = 0.0f;
            break;

        case 'h': // Nagordola High speed
            nagordola_speed += 0.5f;
            break;
        case 'l': // Nagordola Low speed
            nagordola_speed -= 0.5f;
            if (nagordola_speed < 0) nagordola_speed = 0;
            break;

        case 'b': //Boat move
            boat_speed = 0.002;
            break;
        case 'n': //Boat stop
            boat_speed = 0.0f;
            break;

        case '1': //Overcast sky
            is_overcast_sky = true;
            break;
        case '2': //Clear sky
            is_overcast_sky = false;
            break;
    }
    glutPostRedisplay();
}

void handleMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
	{
	    nagordola_speed = 1.0f;
    }
if (button == GLUT_RIGHT_BUTTON)
	{
	    nagordola_speed = 0.0f;
	}
}

void City_Lake_View() {
	glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
	lake();
	road();
	field();
	boat();
	railing();
	car_1();
	car_2();
	nagordola();
	bench();
	lamp_post();
	chrismas_tree();
	park_road();

	//sky();
	if(is_overcast_sky){
        overcast_sky();
	}
	else{
        clear_sky();
	}

	tree_structure();
	hedge_tree();
	all_building();
	all_cloud();

	if(is_overcast_sky){
        drawDrops();
	}

	border();
	glFlush();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);          // Initialize GLUT
	glutInitWindowSize(900, 800);   // Set the window's initial width & height
	glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
	glutCreateWindow("Dynamic City Lake View");  // Create window with the given title
	//glutFullScreen();
	glutDisplayFunc(City_Lake_View);       // Register callback handler for window re-paint event
	initGL();                       // Our own OpenGL initialization
	glutTimerFunc(25, boat_animation, 0);
	glutTimerFunc(25, car_animation, 0);
	glutTimerFunc(20, nagordola_rotation, 0);
	glutTimerFunc(25, cloud_animation, 0);
	glutTimerFunc(10, rain_animation, 0);
	glutKeyboardFunc(handleKeypress);
	glutMouseFunc(handleMouse);
	glutMainLoop();                 // Enter the event-processing loop
	return 0;
}
