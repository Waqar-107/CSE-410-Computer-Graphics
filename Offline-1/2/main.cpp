/*** from dust i have come, dust i will be ***/

#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

#define rotateVal 3
#define camRotateVal 1

#define clkwise 1
#define anticlkwise -1

#define camUpDownVal 1

#define nl printf("\n")

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
point pos, U, R, L;
//===============================================


//===============================================
void drawCircle(double radius, int segments)
{
    struct point points[100];

    //generate points to draw in x-z plane
    for(int i = 0; i < segments; i++)
    {
        // x = r * cos(theta)
        // y = r * sin(theta)
        points[i].x = radius * cos(((double) i / (double) segments) * 2 * pi);
        points[i].z = radius * sin(((double) i / (double) segments) * 2 * pi);
    }

    //glColor3f (0.5, 0.5, 0.5);
    glColor3f (0.0, 1.0, 1.0);

    //draw segments using generated points
    for (int i = 0; i < segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x, 0, points[i].z);
            glVertex3f(points[(i + 1) % segments].x, 0, points[i + 1].z);
        }
        glEnd();
    }
}

void drawSides(double radius, int segments, int d)
{
     struct point points[100];

    //generate points to draw in x-z plane
    for(int i = 0; i < segments; i++)
    {
        points[i].x = radius * cos(((double) i / (double) segments) * 2 * pi);
        points[i].z = radius * sin(((double) i / (double) segments) * 2 * pi);
    }

    //glColor3f (0.5, 0.5, 0.5);
    glColor3f (0.0, 1.0, 1.0);

    for(int i = 0; i < segments; i++)
    {
        glBegin(GL_QUADS);
        {
            glVertex3f(points[i].x, d, points[i].z);
            glVertex3f(points[i].x, -d, points[i].z);
            glVertex3f(points[(i + 1) % segments].x, -d, points[(i + 1) % segments].z);
            glVertex3f(points[(i + 1) % segments].x, d, points[(i + 1) % segments].z);
        }
        glEnd();
    }
}

void drawWheel()
{
    int segments = 100;
    double r = 25, d = 5;

    glPushMatrix();
    glTranslated(0, 0, r);

    //-------------------------------------------
    //draw circles
    glPushMatrix();
    glTranslated(0, d, 0);
    drawCircle(r, segments);
    glPopMatrix();

    glPushMatrix();
    glTranslated(0, -d, 0);
    drawCircle(r, segments);
    glPopMatrix();
    //-------------------------------------------

    //-------------------------------------------
    //draw the cylindrical part of the circle
    drawSides(r, segments, d);
    //-------------------------------------------

    //-------------------------------------------
    //the rims inside the wheel
    //glColor3f (0.5, 0.5, 0.5);
    glColor3f (0.0, 1.0, 1.0);
    glBegin(GL_QUADS);
    {
        glVertex3f(0, -d / 2, r);
        glVertex3f(0, d / 2, r);
        glVertex3f(0, d / 2, -r);
        glVertex3f(0, -d / 2, -r);
    }
    glEnd();

    glBegin(GL_QUADS);
    {
        glVertex3f(r, d / 2, 0);
        glVertex3f(r, -d / 2, 0);
        glVertex3f(-r, -d / 2, 0);
        glVertex3f(-r, d / 2, 0);
    }
    glEnd();
    //-------------------------------------------

    glPopMatrix();
}

float degreeToRadian(float deg)
{
    return (pi * deg) / 180;
}

void pfp(point x){
    cout<<x.x<<" "<<x.y<<" "<<x.z<<endl;
}

point add(point u, point v)
{
    return point(u.x + v.x, u.y + v.y, u.z + v.z);
}

point subtract(point u, point v)
{
    return point(u.x - v.x, u.y - v.y, u.z - v.z);
}

point cross_product(point u, point v)
{
    point temp;
    temp.x = u.y * v.z - u.z * v.y;
    temp.y = u.z * v.x - u.x * v.z;
    temp.z = u.x * v.y - u.y * v.x;

    return temp;
}

point rotation3D(point v, point reff, int dir)
{
    //first determine a vector that is perpendicular to both \
    the reference and the vector we are rotating
    point p = cross_product(v, reff);
    point temp;

    //scale v by cos and p by sine and take their sum
    double ang = dir * degreeToRadian(rotateVal);
    temp.x = v.x * cos(ang) + p.x * sin(ang);
    temp.y = v.y * cos(ang) + p.y * sin(ang);
    temp.z = v.z * cos(ang) + p.z * sin(ang);

    return temp;
}

void move_forward()
{

}

void move_backward()
{

}

void move_right()
{

}

void move_left()
{

}

void camera_move_left()
{
    //2D rotation -> x` = x*cos(theta) - y*sin(theta) ; y` = x*sin(theta) + y*cos(theta) ;
    double x2 = pos.x * cos(degreeToRadian(camRotateVal * anticlkwise)) - pos.y * sin(degreeToRadian(camRotateVal * anticlkwise));
    double y2 = pos.x * sin(degreeToRadian(camRotateVal * anticlkwise)) + pos.y * cos(degreeToRadian(camRotateVal * anticlkwise));

    pos.x = x2;
    pos.y = y2;
}

void camera_move_right()
{
    //2D rotation -> x` = x*cos(theta) - y*sin(theta) ; y` = x*sin(theta) + y*cos(theta) ;
    double x2 = pos.x * cos(degreeToRadian(camRotateVal * clkwise)) - pos.y * sin(degreeToRadian(camRotateVal * clkwise));
    double y2 = pos.x * sin(degreeToRadian(camRotateVal * clkwise)) + pos.y * cos(degreeToRadian(camRotateVal * clkwise));

    pos.x = x2;
    pos.y = y2;
}

void camera_move_up(){
    pos.z += camUpDownVal;
}

void camera_move_down(){
    pos.z -= camUpDownVal;
}
//===============================================


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

void drawGrid()
{
    int i;
    if (drawgrid == 1)
    {
        glColor3f(0.5, 0.5, 0.5);    //grey
        glBegin(GL_LINES);
        {
            for (i = -8; i <= 8; i++)
            {
                if (i == 0)
                    continue;    //SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i * 10, -90, 0);
                glVertex3f(i * 10, 90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i * 10, 0);
                glVertex3f(90, i * 10, 0);
            }
        }
        glEnd();
    }
}

//===============================================

//===============================================


void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
        move_forward();
        break;
    case 'a':
        move_left();
        break;
    case 's':
        move_backward();
        break;
    case 'd':
        move_right();
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
        camera_move_down();
        break;

    case GLUT_KEY_UP:        // up arrow key
        camera_move_up();
        break;

    case GLUT_KEY_RIGHT:
        camera_move_right();
        break;

    case GLUT_KEY_LEFT:
        camera_move_left();
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
        if(state == GLUT_DOWN)
            drawaxes = 1 - drawaxes;
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
    gluLookAt(pos.x, pos.y, pos.z,  0, 0, 0 ,	 0, 0, 1);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    drawAxes();
    drawGrid();
    drawWheel();

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
    drawgrid = 1;
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

    //pos = point(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight);
    pos = point(70.0, 70.0, 100.0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

    glutCreateWindow("wheel movement");

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
