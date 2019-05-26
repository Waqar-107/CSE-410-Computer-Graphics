/*** from dust i have come, dust i will be ***/

#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
#define rotateVal 3
#define clkwise 1
#define anticlkwise -1

#define dbg printf("in\n")
#define nl printf("\n")
#define N 1000100
#define inf 1e18

#define sf(n) scanf("%d", &n)
#define sff(n,m) scanf("%d%d",&n,&m)
#define sfl(n) scanf("%I64d", &n)
#define sffl(n,m) scanf("%I64d%I64d",&n,&m)

#define pf(n) printf("%d",n)
#define pff(n,m) printf("%d %d",n,m)
#define pffl(n,m) printf("%I64d %I64d",n,m)
#define pfl(n) printf("%I64d",n)
#define pfs(s) printf("%s",s)

#define pb push_back
#define pp pair<int, int>

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
    double x, y, z;
    point(){}
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
float degreeToRadian(float deg){
    return (pi * deg) / 180;
}

void printPoint(point x){
    cout<<x.x<<" "<<x.y<<" "<<x.z;nl;
}

point add(point u, point v) {
    return point(u.x + v.x, u.y + v.y, u.z + v.z);
}

point subtract(point u, point v){
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

void move_forward(){
    pos = add(pos, L);
}

void move_backward(){
    pos = subtract(pos, L);
}

void move_right(){
    pos = add(pos, R);
}

void move_left(){
    pos = subtract(pos, R);
}

void move_up(){
    pos = add(pos, U);
}

void move_down(){
    pos = subtract(pos, U);
}

void look_left(){
    //rotate l and r
    L = rotation3D(L, U, anticlkwise);
    R = rotation3D(R, U, anticlkwise);
}

void look_right(){
    L = rotation3D(L, U, clkwise);
    R = rotation3D(R, U, clkwise);
}

void look_up(){
    L = rotation3D(L, R, anticlkwise);
    U = rotation3D(U, R, anticlkwise);
}

void look_down(){
    L = rotation3D(L, R, clkwise);
    U = rotation3D(U, R, clkwise);
}

void tilt_clockwise(){
    R = rotation3D(R, L, anticlkwise);
    U = rotation3D(U, L, anticlkwise);
}

void tilt_counter_clockwise(){
    R = rotation3D(R, L, clkwise);
    U = rotation3D(U, L, clkwise);
}
//===============================================


void drawAxes()
{
    if (drawaxes == 1)
    {
        glColor3f(1.0, 1.0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f(100, 0, 0);
            glVertex3f(-100, 0, 0);

            glVertex3f(0, -100, 0);
            glVertex3f(0, 100, 0);

            glVertex3f(0, 0, 100);
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
        glColor3f(0.6, 0.6, 0.6);    //grey
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

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f(a, a, 2);
        glVertex3f(a, -a, 2);
        glVertex3f(-a, -a, 2);
        glVertex3f(-a, a, 2);
    }
    glEnd();
}

void drawCircle(double radius, int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7, 0.7, 0.7);
    //generate points
    for (i = 0; i <= segments; i++)
    {
        points[i].x = radius * cos(((double) i / (double) segments) * 2 * pi);
        points[i].y = radius * sin(((double) i / (double) segments) * 2 * pi);
    }
    //draw segments using generated points
    for (i = 0; i < segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x, points[i].y, 0);
            glVertex3f(points[i + 1].x, points[i + 1].y, 0);
        }
        glEnd();
    }
}

void drawCone(double radius, double height, int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for (i = 0; i <= segments; i++)
    {
        points[i].x = radius * cos(((double) i / (double) segments) * 2 * pi);
        points[i].y = radius * sin(((double) i / (double) segments) * 2 * pi);
    }
    //draw triangles using generated points
    for (i = 0; i < segments; i++)
    {
        //create shading effect
        if (i < segments / 2)
            shade = 2 * (double) i / (double) segments;
        else
            shade = 2 * (1.0 - (double) i / (double) segments);
        glColor3f(shade, shade, shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0, 0, height);
            glVertex3f(points[i].x, points[i].y, 0);
            glVertex3f(points[i + 1].x, points[i + 1].y, 0);
        }
        glEnd();
    }
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
            points[i][j].x = r * cos(((double) j / (double) slices) * 2 * pi);
            points[i][j].y = r * sin(((double) j / (double) slices) * 2 * pi);
            points[i][j].z = h;
        }
    }
    //draw quads using generated points
    for (i = 0; i < stacks; i++)
    {
        glColor3f((double) i / (double) stacks, (double) i / (double) stacks, (double) i / (double) stacks);
        for (j = 0; j < slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, points[i + 1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x, points[i][j].y, -points[i][j].z);
                glVertex3f(points[i][j + 1].x, points[i][j + 1].y, -points[i][j + 1].z);
                glVertex3f(points[i + 1][j + 1].x, points[i + 1][j + 1].y, -points[i + 1][j + 1].z);
                glVertex3f(points[i + 1][j].x, points[i + 1][j].y, -points[i + 1][j].z);
            }
            glEnd();
        }
    }
}

void drawSS()
{
    glColor3f(1, 0, 0);
    drawSquare(20);

    glRotatef(angle, 0, 0, 1);
    glTranslatef(110, 0, 0);
    glRotatef(2 * angle, 0, 0, 1);
    glColor3f(0, 1, 0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle, 0, 0, 1);
        glTranslatef(60, 0, 0);
        glRotatef(2 * angle, 0, 0, 1);
        glColor3f(0, 0, 1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3 * angle, 0, 0, 1);
    glTranslatef(40, 0, 0);
    glRotatef(4 * angle, 0, 0, 1);
    glColor3f(1, 1, 0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        look_left();
        break;
    case '2':
        look_right();
        break;
    case '3':
        look_up();
        break;
    case '4':
        look_down();
        break;
    case '5':
        tilt_clockwise();
        break;
    case '6':
        tilt_counter_clockwise();
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
        move_backward();
        break;

    case GLUT_KEY_UP:        // up arrow key
        move_forward();
        break;

    case GLUT_KEY_RIGHT:
        move_right();
        break;

    case GLUT_KEY_LEFT:
        move_left();
        break;

    case GLUT_KEY_PAGE_UP:
        move_up();
        break;

    case GLUT_KEY_PAGE_DOWN:
        move_down();
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

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?
    //gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
    gluLookAt(pos.x, pos.y, pos.z, pos.x + L.x, pos.y + L.y, pos.z + L.z, U.x, U.y, U.z);


    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    //drawSphere(30,24,20);




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

    pos = point(100, 100, 0);
    U = point(0, 0, 1), R = point(-1/sqrt(2.0),  1/sqrt(2.0), 0), L = point(-1/sqrt(2.0),  -1/sqrt(2.0), 0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

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
