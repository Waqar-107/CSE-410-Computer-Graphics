/*** from dust i have come, dust i will be ***/

#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
#define rotateVal 3

#define nl printf("\n")
#define sqr_side 20.0

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
    double x, y, z;
    point() {}
    point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
};

//===============================================
// variables
point pos;
double red, blue, green, yellow, cyan, magenta, temp;
//===============================================

float degreeToRadian(float deg) {
    return (pi * deg) / 180;
}

void drawAxes()
{
    if (drawaxes == 1)
    {
        glBegin(GL_LINES);
        {
            glColor3f(1.0, 1.0, 1.0);
            glVertex3f(100, 0, 0);
            glColor3f (0.0, 1.0, 1.0);
            glVertex3f(-100, 0, 0);

            glColor3f (0.0, 0.0, 1.0);
            glVertex3f(0, -100, 0);
            glColor3f (1.0, 1.0, 0.0);
            glVertex3f(0, 100, 0);

            glColor3f (0.0, 1.0, 0.0);
            glVertex3f(0, 0, 100);
            glColor3f (1.0, 0.0, 1.0);
            glVertex3f(0, 0, -100);
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(a, a, 0);
        glVertex3f(a, -a, 0);
        glVertex3f(-a, -a, 0);
        glVertex3f(-a, a, 0);
    }
    glEnd();
}

void drawSphere(double radius, int slices, int stacks)
{
    struct point points[100][100];
    int i, j;
    double h, r;

    //generate points
    for (i = 0; i <= stacks; i++)
    {
        h = radius * sin(((double) i / (double) stacks) * (pi / 2));
        r = radius * cos(((double) i / (double) stacks) * (pi / 2));
        for (j = 0; j <= slices; j++)
        {
            points[i][j].x = r * cos(((double) j / (double) slices) * (pi / 2));
            points[i][j].y = r * sin(((double) j / (double) slices) * (pi / 2));
            points[i][j].z = h;
        }
    }

    //draw quads using generated points
    for (i = 0; i < stacks; i++)
    {
        for (j = 0; j < slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
            }
            glEnd();
        }
    }
}

void drawCylinder(double radius, double height)
{
    point points[300];
    int segments = 300;

    //generate points
    for(int i = 0; i < segments; i++)
    {
        points[i].x = radius * cos(((double) i / (double) segments) * (pi / 2 + 0.01));
        points[i].y = radius * sin(((double) i / (double) segments) * (pi / 2 + 0.01));
    }

    for(int i = 0; i < segments - 1; i++)
    {
        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x, points[i].y, 0);
            glVertex3f(points[i].x, points[i].y, height);
            glVertex3f(points[i + 1].x, points[i + 1].y, height);
            glVertex3f(points[i + 1].x, points[i + 1].y, 0);
        }
        glEnd();
    }
}


//===============================================
void online()
{
    //-------------------------------------------
    //red
    glPushMatrix();
    glColor3f (1.0, 0.0, 0.0);
    drawSquare(sqr_side);
    glPopMatrix();
    //-------------------------------------------

    //-------------------------------------------
    //magenta
    glPushMatrix();
    glColor3f (1.0, 0.0, 1.0);

    glTranslated(sqr_side + sqr_side * cos(degreeToRadian(magenta)), 0, 0);
    glTranslated(0, 0, sqr_side * sin(degreeToRadian(magenta)));

    glRotated(-magenta, 0, 1, 0);

    drawSquare(sqr_side);
    glPopMatrix();
    //-------------------------------------------

    //-------------------------------------------
    //blue
    glPushMatrix();
    glColor3f (0.0, 0.0, 1.0);

    glTranslated(-sqr_side - sqr_side * cos(degreeToRadian(blue)), 0, 0);
    glTranslated(0, 0, sqr_side * sin(degreeToRadian(blue)));

    glRotated(blue, 0, 1, 0);

    drawSquare(sqr_side);
    glPopMatrix();
    //-------------------------------------------

    //-------------------------------------------
    //cyan
    glPushMatrix();
    glColor3f (0.0, 1.0, 1.0);

    //----------------------------------------------------------------------------
    //self rotation
    temp = sqr_side * (1 - cos(degreeToRadian(cyan)));
    glTranslated(-temp, 0, 0);

    temp = sqr_side * sin(degreeToRadian(cyan));
    glTranslated(0, 0, temp);
    //----------------------------------------------------------------------------

    //----------------------------------------------------------------------------
    //stuck with magenta
    temp = 3 * sqr_side * (1 - cos(degreeToRadian(magenta)));
    glTranslated(sqr_side * 4 - temp, 0, 0);
    glTranslated(0, 0, 3 * sqr_side * sin(degreeToRadian(magenta)));
    //----------------------------------------------------------------------------

    glRotated(-magenta, 0, 1, 0);
    glRotated(-cyan, 0, 1, 0);

    drawSquare(sqr_side);
    glPopMatrix();
    //-------------------------------------------

    //-------------------------------------------
    //yellow
    glPushMatrix();
    glColor3f (1.0, 1.0, 0.0);

    glTranslated(0, -sqr_side - sqr_side * cos(degreeToRadian(yellow)), 0);
    glTranslated(0, 0, sqr_side * sin(degreeToRadian(yellow)));

    glRotated(-yellow, 1, 0, 0);

    drawSquare(sqr_side);
    glPopMatrix();
    //-------------------------------------------

    //-------------------------------------------
    //green
    glPushMatrix();
    glColor3f (0.0, 1.0, 0.0);

    glTranslated(0, sqr_side + sqr_side * cos(degreeToRadian(green)), 0);
    glTranslated(0, 0, sqr_side * sin(degreeToRadian(green)));

    glRotated(green, 1, 0, 0);

    drawSquare(sqr_side);
    glPopMatrix();
    //-------------------------------------------
}
//===============================================


void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        green = min(90.0, green + rotateVal);
        break;
    case '2':
        green = max(0.0, green - rotateVal);
        break;
    case '3':
        blue = min(90.0, blue + rotateVal);
        break;
    case '4':
        blue = max(0.0, blue - rotateVal);
        break;
    case '5':
        yellow = min(90.0, yellow + rotateVal);
        break;
    case '6':
        yellow = max(0.0, yellow - rotateVal);
        break;
    case '7':
        magenta = min(90.0, magenta + rotateVal);
        break;
    case '8':
        magenta = max(0.0, magenta - rotateVal);
        break;
    case '9':
        cyan = min(90.0, cyan + rotateVal);
        break;
    case '0':
        cyan = max(0.0, cyan - rotateVal);
        break;

    default:
        break;
    }
}

void specialKeyListener(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN:        //down arrow key
        break;

    case GLUT_KEY_UP:        // up arrow key
        break;

    case GLUT_KEY_RIGHT:
        break;

    case GLUT_KEY_LEFT:
        break;

    case GLUT_KEY_PAGE_UP:
        break;

    case GLUT_KEY_PAGE_DOWN:
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        break;

    case GLUT_KEY_END:
        break;

    default:
        break;
    }
}

void mouseListener(int button, int state, int x, int y)      //x, y is the x-y of the screen (2D)
{
    switch (button)
    {
    case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN)          // 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes = 1 - drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}

void display()
{
    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);    //color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //position of camera, coordinate where the camera is looking at, up vector
    gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    drawAxes();
    online();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate()
{
    angle += 0.05;
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid = 0;
    drawaxes = 1;
    cameraHeight = 150.0;
    cameraAngle = 1.0;
    angle = 0;

    //clear the screen
    glClearColor(0, 0, 0, 0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80, 1, 1, 1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance and far distance

    red = blue = green = yellow = cyan = magenta = 0.0;
    pos = point(100, 100, 0);

}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

    glutCreateWindow("qube to sphere");

    init();

    glEnable(GL_DEPTH_TEST);    //enable Depth Testing

    glutDisplayFunc(display);    //display callback function
    glutIdleFunc(animate);        //what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();        //The main loop of OpenGL

    return 0;
}

/*
https://stackoverflow.com/questions/19170778/glrotateangle-x-y-z-what-is-x-y-z-in-this-case
*/
