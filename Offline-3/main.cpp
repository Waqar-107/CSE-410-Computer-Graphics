/*** from dust i have come, dust i will be ***/

#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
#define rotateVal 3
#define clkwise 1
#define anticlkwise -1

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

class shape
{
public:
    string name;
    double r, g, b;
    double ambient_coeff, diffuse_coeff, specular_coeff, reflection_coeff;
    double specular_exponent;

    double cx, cy, cz;
    double radius;

    double x, y, z;
    double base, height;

    shape(string name){
        this->name = name;
    }
};
//===============================================
// variables
point pos, U, R, L;

int level_of_recursion, pixels, n_objs;
vector<shape> vec;
//===============================================


//===============================================
float degreeToRadian(float deg) {
    return (pi * deg) / 180;
}

point add(point u, point v) {
    return point(u.x + v.x, u.y + v.y, u.z + v.z);
}

point subtract(point u, point v) {
    return point(u.x - v.x, u.y - v.y, u.z - v.z);
}

point cross_product(point u, point v) {
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

void move_forward() {
    pos = add(pos, L);
}

void move_backward() {
    pos = subtract(pos, L);
}

void move_right() {
    pos = add(pos, R);
}

void move_left() {
    pos = subtract(pos, R);
}

void move_up() {
    pos = add(pos, U);
}

void move_down() {
    pos = subtract(pos, U);
}

void look_left() {
    //rotate l and r
    L = rotation3D(L, U, anticlkwise);
    R = rotation3D(R, U, anticlkwise);
}

void look_right() {
    L = rotation3D(L, U, clkwise);
    R = rotation3D(R, U, clkwise);
}

void look_up() {
    L = rotation3D(L, R, anticlkwise);
    U = rotation3D(U, R, anticlkwise);
}

void look_down() {
    L = rotation3D(L, R, clkwise);
    U = rotation3D(U, R, clkwise);
}

void tilt_clockwise() {
    R = rotation3D(R, L, anticlkwise);
    U = rotation3D(U, L, anticlkwise);
}

void tilt_counter_clockwise() {
    R = rotation3D(R, L, clkwise);
    U = rotation3D(U, L, clkwise);
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

void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;

	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
		for(j=0;j<slices;j++)
		{
			glBegin(GL_QUADS);{
			    //upper hemisphere
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}

//===============================================
void drawCheckerBoard()
{
    /*
    (-1000, 1000)  --------  (1000, 1000)
    |                                                       |
    |                                                       |
    |                                                       |
    (-1000, -1000) -------- (1000, -1000)
    */

    int X = -1000, Y = 1000;
    int th = 20;

    bool white = true, white_in;
    for(int r = Y; r >= -Y + th; r -= th)
    {
        white_in = white;
        for(int c = X; c <= -X + th; c += th)
        {
            if(white_in)
                glColor3f(1.0, 1.0, 1.0);
            else
                glColor3f(0.0, 0.0, 0.0);

            white_in = !white_in;

            //left corner of the cell is r,c

            glBegin(GL_QUADS);
            {
                glVertex3f(r, c, 0);
                glVertex3f(r + th, c, 0);
                glVertex3f(r + th, c - th, 0);
                glVertex3f(r, c - th, 0);
            }
            glEnd();
        }

        white = !white;
    }

}

drawPyramid(shape p)
{
    //we shall draw a square as the base then 4 triangles
    glBegin(GL_QUADS);
    {
        p.z += 0.01;
        glVertex3f(p.x, p.y, p.z);    //leftmost corner
        glVertex3f(p.x + p.base, p.y, p.z);
        glVertex3f(p.x + p.base, p.y + p.base, p.z);
        glVertex3f(p.x, p.y + p.base, p.z);
    }
    glEnd();

    point top(p.x + p.base / 2, p.y + p.base / 2, p.z + p.height);

    //1st triangle
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p.x, p.y, p.z);    //leftmost corner
        glVertex3f(p.x + p.base, p.y, p.z);
        glVertex3f(top.x, top.y, top.z);
    }
    glEnd();

    //2nd triangle
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p.x, p.y, p.z);    //leftmost corner
        glVertex3f(p.x, p.y + p.base, p.z);
        glVertex3f(top.x, top.y, top.z);
    }
    glEnd();

    //3rd triangle
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p.x + p.base, p.y, p.z);
        glVertex3f(p.x + p.base, p.y + p.base, p.z);
        glVertex3f(top.x, top.y, top.z);
    }
    glEnd();

    //4th triangle
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p.x + p.base, p.y + p.base, p.z);
        glVertex3f(p.x, p.y + p.base, p.z);
        glVertex3f(top.x, top.y, top.z);
    }
    glEnd();
}

void drawShapes()
{
    for(int i = 0; i < vec.size(); i++)
    {
        if(vec[i].name == "sphere")
        {
            glPushMatrix();
            {
                glTranslated(vec[i].cx, vec[i].cy, vec[i].cz);
                glColor3f(vec[i].r, vec[i].g, vec[i].b);
                drawSphere(vec[i].radius, 50, 50);
            }
            glPopMatrix();
        }

        else if(vec[i].name == "pyramid")
        {
            glPushMatrix();
            {
                glColor3f(vec[i].r, vec[i].g, vec[i].b);
                drawPyramid(vec[i]);
            }
            glPopMatrix();
        }
    }
}
//===============================================

void keyboardListener(unsigned char key, int x, int y)
{
    switch (key)
    {
    case '1':
        look_right();
        break;
    case '2':
        look_left();
        break;
    case '3':
        look_up();
        break;
    case '4':
        look_down();
        break;
    case '5':
        tilt_counter_clockwise();
        break;
    case '6':
        tilt_clockwise();
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

    //position of camera, coordinate where the camera is looking at, up vector
    gluLookAt(pos.x, pos.y, pos.z, pos.x + L.x, pos.y + L.y, pos.z + L.z, U.x, U.y, U.z);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    drawAxes();
    drawCheckerBoard();
    drawShapes();

    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}

void animate()
{
    angle += 0.05;
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

void parseData()
{
    cin >> level_of_recursion;
    cin >> pixels;
    cin >> n_objs;

    string str;
    for(int i = 0; i < n_objs; i++)
    {
        cin >> str;

        if(str == "sphere")
        {
            shape x("sphere");

            cin >> x.cx >> x.cy >> x.cz;
            cin >> x.radius;
            cin >> x.r >> x.g >> x.b;
            cin >> x.ambient_coeff >> x.diffuse_coeff >> x.specular_coeff >> x.reflection_coeff;
            cin >> x.specular_exponent;

            vec.push_back(x);
        }

        else if(str == "pyramid")
        {
            shape x("pyramid");

            cin >> x.x >> x.y >> x.z;
            cin >> x.base >> x.height;
            cin >> x.r >> x.g >> x.b;
            cin >> x.ambient_coeff >> x.diffuse_coeff >> x.specular_coeff >> x.reflection_coeff;
            cin >> x.specular_exponent;

            vec.push_back(x);
        }

        else
            break;
    }
}
int main(int argc, char **argv)
{
    freopen("description.txt", "r", stdin);
    parseData();

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
