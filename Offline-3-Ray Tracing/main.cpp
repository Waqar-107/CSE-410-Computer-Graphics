/*** from dust i have come, dust i will be ***/

#include<bits/stdc++.h>
#include <windows.h>
#include <glut.h>
#include "bitmap_image.hpp"

#define pi (2*acos(0.0))
#define rotateVal 3
#define clkwise 1
#define anticlkwise -1

#define window_height 500
#define window_width 500

#define dbg printf("in\n")
#define nl printf("\n")

#define EPSILON  0.0000001

using namespace std;

double cameraHeight;
double cameraAngle;
int drawgrid, drawaxes, display_light_source;
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

    void normalize(){
        double sq = sqrt(x * x + y * y + z * z);

        x /= sq; x *= 1.0;
        y /= sq; y *= 1.0;
        z /= sq; z *= 1.0;
    }

    void print(){
        cout << x << " " << y << " " << z << endl;
    }
};

vector<point> light_sources;

float degreeToRadian(float deg) {
    return (pi * deg) / 180;
}

void pf(point p) {
    printf("%.10f %.10f %.10f\n", p.x, p.y, p.z);
}

point add(point u, point v) {
    return point(u.x + v.x, u.y + v.y, u.z + v.z);
}

point subtract(point u, point v) {
    return point(u.x - v.x, u.y - v.y, u.z - v.z);
}

point multiplyWithScaler(point p, double sc) {
    point temp;

    temp.x = p.x * sc;
    temp.y = p.y * sc;
    temp.z = p.z * sc;

    return temp;
}

point cross_product(point u, point v) {
    point temp;
    temp.x = u.y * v.z - u.z * v.y;
    temp.y = u.z * v.x - u.x * v.z;
    temp.z = u.x * v.y - u.y * v.x;

    return temp;
}

double dot_product(point u, point v) {
    double res = u.x * v.x + u.y * v.y + u.z * v.z;
    return res;
}

double distance_between_points(point u, point v) {
    double x = u.x - v.x;
    double y = u.y - v.y;
    double z = u.z - v.z;

    return sqrt(x * x + y * y + z * z);
}

point rotation3D(point v, point reff, int dir) {
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

// r = (a.n)n - a
point getReflection(point original_vec, point normal)
{
    double coeff = dot_product(original_vec, normal) * 2;
    point reflected_vec = subtract(multiplyWithScaler(normal, coeff), original_vec);

    reflected_vec.normalize();

    return reflected_vec;
}

struct Ray
{
    point start, dir;
    Ray()
    {
        start = point(0, 0, 0);
        dir = point(0, 0, 0);
    }

    Ray(point start, point dir)
    {
        this->start = start;
        this->dir = dir;
    }

    void normalizeDirection() {
        dir.normalize();
    }
};

class shape
{
public:
    double color[3];
    double ambient_coeff, diffuse_coeff, specular_coeff, reflection_coeff;
    double specular_exponent;

    point center;
    double radius;

    point leftCorner;
    double base, height;

    point A, B, C;

    shape(){}
    virtual void draw(){}
    virtual double intersect(Ray ray, double *current_color, int level, int idx){
        return -1;
    }

    virtual double intersecting_point(Ray ray){
        return -1;
    }
};


//===============================================
// variables
point pos, U, R, L;
int light_src_quantity;
int level_of_recursion, pixels, n_objs;
double fovY;
shape *board;
vector<shape*> vec;
//===============================================

class sphere : public shape
{
public:
    sphere(){}

    void draw()
    {
        glPushMatrix();
        glTranslated(center.x, center.y, center.z);
        glColor3f(color[0], color[1], color[2]);
        glutSolidSphere(radius, 100, 100);
        glPopMatrix();
    }

     point getNormal(point x, point y)
    {
        //normal in that point is (P - C)
        point temp = subtract(x, y);
        temp.normalize();

        return temp;
    }

    //using algebraic way
    //(P - C).(P - C) = r ^ 2 where P = R0 + t * Rd
    double intersecting_point(Ray ray)
    {
        point r0 = ray.start, rd = ray.dir;

        double a = 1.0;     //as rd is unit
        double b = 2 * dot_product(rd, subtract(r0, center));
        double c = dot_product(subtract(r0, center), subtract(r0, center)) - (radius * radius);

        double d = (b * b) - (4 * a * c);

        if(d < 0)
            return -1;

        d = sqrt(d);
        double t1 = (- b - d) / (2 * a);
        double t2 = (- b + d) / (2 * a);

        if(t2 > 0)
        {
            if(t1 < 0)
                return t1;
            else
                return t2;
        }
    }

    double intersect(Ray ray, double *current_color, int level, int idx)
    {
        double t = intersecting_point(ray);

        if(t <= 0) return -1;
        if(level == 0) return t;

        for(int i = 0; i < 3; i++)
            current_color[i] = color[i] * ambient_coeff;

        //intersection point is => (r0 + t * rd)
        point intersectionPoint(add(ray.start, multiplyWithScaler(ray.dir, t)));

        point normal = getNormal(intersectionPoint, center);
        point reflection = getReflection(ray.dir, normal);

        //Illumination
        for(int i = 0; i < light_sources.size(); i++)
        {
            point L = subtract(light_sources[i], intersectionPoint);
            L.normalize();

            point N = getNormal(intersectionPoint, center);
            N.normalize();

            point R = getReflection(L, N);
            R.normalize();

            point V = subtract(intersectionPoint, pos);
            V.normalize();

            Ray sunLight(intersectionPoint, L);

            //check if obscured
            bool obscured = false;
            double dist = distance_between_points(light_sources[i], intersectionPoint);

            for(int j = 0; j < vec.size(); j++)
            {
                //no need to check this object
                if(j == idx)
                    continue;

                double temp = vec[j]->intersecting_point(sunLight);

                //if less than dist than nearest object to sunlight is not this particular object, so it is obscured
                if(temp <= 0 || temp > dist) continue;
                obscured = true;

                break;
            }

            if(!obscured)
            {
                double lambart = max(0.0, dot_product(L, N));
                double phong = max(0.0, pow(dot_product(R, V), specular_exponent));

                for(int c = 0; c < 3; c++)
                    current_color[c] += (lambart * diffuse_coeff * color[c]) + (phong * specular_coeff * color[c]);
            }
        }

        int nearest, t_min, t2;
        double *dummy_color = new double[3];

        //Reflection
        /*if(level < level_of_recursion)
        {
            point start = add(intersectionPoint, multiplyWithScaler(reflection, 1.0));
            Ray reflectionRay(start, reflection);

            nearest = -1; t_min = 1e9 * 1.0;
            double *reflected_color = new double[3];

            for(int k = 0; k < vec.size(); k++)
            {
                t2 = vec[k]->intersect(reflectionRay, reflected_color, 0, k);

                if(t2 > 0 && t2 < t_min)
                    t_min = t2, nearest = k;
            }

            if(nearest != -1)
            {
                t2 = vec[nearest]->intersect(reflectionRay, reflected_color, level + 1, idx);

                for(int c = 0; c < 3; c++)
                    current_color[c] += (reflected_color[c] * reflection_coeff);
            }

            delete[] reflected_color;
        }
*/
        return t;
    }
};

class triangle : public shape
{
public:
    triangle(){}
    void draw()
    {
        glColor3f(color[0], color[1], color[2]);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(A.x, A.y, A.z);
            glVertex3f(B.x, B.y, B.z);
            glVertex3f(C.x, C.y, C.z);
        }
        glEnd();
    }

    // (b - a) * (c - a);
    point getNormal()
    {
        point temp = cross_product(subtract(B, A), subtract(C, A));
        temp.normalize();

        return temp;
    }

    //Möller–Trumbore intersection algorithm
    double intersecting_point(Ray ray)
    {
        //considering C = 0, A = 1, B = 2;
        point edge1 = subtract(A, C);
        point edge2 = subtract(B, C);

        point h = cross_product(ray.dir, edge2);
        double a = dot_product(h, edge1);

        //this ray is parallel to the triangle
        if(a > -EPSILON && a < EPSILON)
            return -1;

        double f = 1.0 / a;
        point s = subtract(ray.start, C);
        double u = f * dot_product(s, h);

        if(u < 0.0 || u > 1.0) return -1;

        point q = cross_product(s, edge1);
        double v = f * dot_product(ray.dir, q);

        if(v < 0.0 || u + v > 1.0) return -1;

        //now we compute t
        float t = f * dot_product(edge2, q);
        if(t > EPSILON)
            return t;
        else
            return -1;
    }

    double intersect(Ray ray, double *current_color, int level, int idx)
    {
        double t = intersecting_point(ray);

        if(t <= 0) return -1;
        if(level == 0) return t;

        for(int i = 0; i < 3; i++)
            current_color[i] = color[i] * ambient_coeff;

        //intersection point is => (r0 + t * rd)
        point intersectionPoint(add(ray.start, multiplyWithScaler(ray.dir, t)));
        point normal = getNormal();
        point reflection = getReflection(ray.dir, normal);

        //Illumination
        for(int i = 0; i < light_sources.size(); i++)
        {
            point L = subtract(light_sources[i], intersectionPoint);
            L.normalize();

            point N = getNormal();
            N.normalize();

            point R = getReflection(L, N);
            R.normalize();

            point V = subtract(intersectionPoint, pos);
            V.normalize();

            Ray sunLight(intersectionPoint, L);

            //check if obscured
            bool obscured = false;
            double dist = distance_between_points(light_sources[i], intersectionPoint);

            for(int j = 0; j < vec.size(); j++)
            {
                //no need to check this object
                if(j == idx)
                    continue;

                double temp = vec[j]->intersecting_point(sunLight);

                //if less than dist than nearest object to sunlight is not this particular object, so it is obscured
                if(temp <= 0 || temp > dist) continue;
                obscured = true;

                break;
            }

            if(!obscured)
            {
                double lambart = max(0.0, dot_product(L, N));
                double phong = max(0.0, pow(dot_product(R, V), specular_exponent));

                for(int c = 0; c < 3; c++)
                    current_color[c] += (lambart * diffuse_coeff * color[c]) + (phong * specular_coeff * color[c]);
            }
        }

        int nearest, t_min, t2;
        double *dummy_color = new double[3];

        //Reflection
      /*  if(level < level_of_recursion)
        {
            point start = add(intersectionPoint, multiplyWithScaler(reflection, 1.0));
            Ray reflectionRay(start, reflection);

            nearest = -1; t_min = 1e9 * 1.0;
            double *reflected_color = new double[3];

            for(int k = 0; k < vec.size(); k++)
            {
                t2 = vec[k]->intersect(reflectionRay, reflected_color, 0, k);

                if(t2 > 0 && t2 < t_min)
                    t_min = t2, nearest = k;
            }

            if(nearest != -1)
            {
                t2 = vec[nearest]->intersect(reflectionRay, reflected_color, level + 1, nearest);

                for(int c = 0; c < 3; c++)
                    current_color[c] += (reflected_color[c] * reflection_coeff);
            }

            delete[] reflected_color;
        }
*/
        return t;
    }
};

class checkerBoard : public shape
{
public:
    checkerBoard(){
        ambient_coeff = 0.4;
        diffuse_coeff = specular_coeff = reflection_coeff = 0.2;
        specular_exponent = 1.0;
    }

    void draw()
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

                //left corner of the cell is r, c
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

    point getNormal() {
        return point(0, 0, 1);
    }

    bool onSurface(point p) {
        double th = 1000;
        return (p.x >= -th && p.x <= th && p.y >= -th && p.y <= th && p.z == 0);
    }

    void setColor(point p)
    {
        //point not in plane
        if(p.z != 0)
        {
            color[0] = color[1] = color[2] = 0.0;
            return;
        }

        int x = (p.x + 1000) / 20;
        int y = (p.y + 1000) / 20;

        if((x + y) % 2 == 0)
            color[0] = color[1] = color[2] = 1.0;
        else
            color[0] = color[1] = color[2] = 0.0;
    }

    double intersecting_point(Ray ray)
    {
        point r0 = ray.start, rd = ray.dir;
        point n = getNormal();

        //a point in the plane
        point p0 = point(0, 0, 0);

        double denominator = dot_product(ray.dir, n);
        double numerator = dot_product(subtract(p0, r0), n);

        double t = -1.0;
        if(denominator != 0)
            t  = numerator / denominator;

        return t;
    }

    double intersect(Ray ray, double *current_color, int level, int idx)
    {
         double t = intersecting_point(ray);

        if(t <= 0) return -1;
        if(level == 0) return t;

        //check if the intersection plane is on the checker-board, then determine its color
        //intersection point is => (r0 + t * rd)
        point intersectionPoint(add(ray.start, multiplyWithScaler(ray.dir, t)));

        //check if not out of the plane
        if(!onSurface(intersectionPoint))return -1;

        setColor(intersectionPoint);

        for(int c = 0; c < 3; c++)
            current_color[c] = color[c] * ambient_coeff;


        point normal = getNormal();
        point reflection = getReflection(ray.dir, normal);

        //Illumination
        for(int i = 0; i < light_sources.size(); i++)
        {
            point L = subtract(light_sources[i], intersectionPoint);
            L.normalize();

            point N = getNormal();
            N.normalize();

            point R = getReflection(L, N);
            R.normalize();

            point V = subtract(intersectionPoint, pos);
            V.normalize();

            Ray sunLight(intersectionPoint, L);

            //check if obscured
            bool obscured = false;
            double dist = distance_between_points(light_sources[i], intersectionPoint);

            for(int j = 0; j < vec.size(); j++)
            {
                //no need to check this object
                if(j == idx)
                    continue;

                double temp = vec[j]->intersecting_point(sunLight);

                //if less than dist than nearest object to sunlight is not this particular object, so it is obscured
                if(temp <= 0 || temp > dist) continue;
                obscured = true;

                break;
            }

            if(!obscured)
            {
                double lambart = max(0.0, dot_product(L, N));
                double phong = max(0.0, pow(dot_product(R, V), specular_exponent));

                for(int c = 0; c < 3; c++)
                    current_color[c] += (lambart * diffuse_coeff * color[c]) + (phong * specular_coeff * color[c]);
            }
        }

        int nearest, t_min, t2;

         //Reflection
        /*if(level < level_of_recursion)
        {
            point start = add(intersectionPoint, multiplyWithScaler(reflection, 1.0));
            Ray reflectionRay(start, reflection);

            nearest = -1; t_min = 1e9 * 1.0;
            double *reflected_color = new double[3];

            for(int k = 0; k < vec.size(); k++)
            {
                t2 = vec[k]->intersect(reflectionRay, reflected_color, 0, k);

                if(t2 > 0 && t2 < t_min)
                    t_min = t2, nearest = k;
            }

            if(nearest != -1)
            {
                t2 = vec[nearest]->intersect(reflectionRay, reflected_color, level + 1, nearest);

                for(int c = 0; c < 3; c++)
                    current_color[c] += (reflected_color[c] * reflection_coeff);
            }

            delete[] reflected_color;
        }
*/
        return t;
    }
};

//===============================================
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

//===============================================
void drawLightSources()
{
    if(!display_light_source)return;

    glColor3f(1, 1, 1);
    for(int i = 0; i < light_sources.size(); i++)
    {
        glPushMatrix();
            glTranslated(light_sources[i].x, light_sources[i].y, light_sources[i].z);
            glutSolidSphere(1, 90, 90);
        glPopMatrix();
    }
}


void drawShapes()
{
    drawLightSources();
    for(int i = 0; i < vec.size(); i++)
        vec[i]->draw();
}

void capture()
{
    bitmap_image image(pixels, pixels);

    double plane_dist = (window_height / 2) * tan(degreeToRadian(fovY / 2));
    point topLeft, l, r, u;

    l = multiplyWithScaler(L, plane_dist);
    r = multiplyWithScaler(R, window_width / 2);
    u = multiplyWithScaler(U, window_height / 2);

    topLeft = subtract(add(add(pos, l), u), r);

    double du = (double)window_width / pixels;
    double dv = (double)window_height / pixels;

    int nearest;
    double t, t_min;
    point corner;
    double *dummy_color = new double[3];

    for(int i = 0; i < pixels; i++)
    {
        for(int j = 0; j < pixels; j++)
        {
            corner.x = topLeft.x + (j * du * R.x) - (i * dv * U.x);
            corner.y = topLeft.y + (j * du * R.y) - (i * dv * U.y);
            corner.z = topLeft.z + (j * du * R.z) - (i * dv * U.z);

            Ray ray(pos, subtract(corner, pos));
            ray.normalizeDirection();

            nearest = -1; t_min = 1e9 * 1.0;
            for(int k = 0; k < vec.size(); k++)
            {
                //by giving level 0 we denote that we  only want to know the nearest object
                t = vec[k]->intersect(ray, dummy_color, 0, k);

                if(t > 0 && t < t_min)
                    t_min = t, nearest = k;
            }

            if(nearest != -1)
            {
                t = vec[nearest]->intersect(ray, dummy_color, 1, nearest);
                image.set_pixel(j, i, 255 * dummy_color[0], 255 * dummy_color[1], 255 * dummy_color[2]);
            }
        }
    }

    image.save_image("1505107_rayTracing.bmp");
    image.clear();

    cout << "image captured\n";
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
    case '0':
        capture();
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
            display_light_source = 1 - display_light_source;
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

    //--------------------------------------------
    display_light_source = 1;
    fovY = 80.0;
    //--------------------------------------------

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
    gluPerspective(fovY, 1, 1, 1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance and far distance

    pos = point(100, 90, 20);
    U = point(0, 0, 1), R = point(-1/sqrt(2.0),  1/sqrt(2.0), 0), L = point(-1/sqrt(2.0),  -1/sqrt(2.0), 0);
}

void parseData()
{
    board = new checkerBoard();
    //vec.push_back(board);

    shape *x;

    cin >> level_of_recursion;
    cin >> pixels;
    cin >> n_objs;

    string str;
    for(int i = 0; i < n_objs; i++)
    {
        cin >> str;

        if(str == "sphere")
        {
            x = new sphere();
            cin >> x->center.x >> x->center.y >> x->center.z;
            cin >> x->radius;
        }

        else if(str == "pyramid")
        {
            x = new triangle();
            cin >> x->leftCorner.x >> x->leftCorner.y >> x->leftCorner.z;
            cin >> x->base >> x->height;
        }

        cin >> x->color[0] >> x->color[1] >> x->color[2];
        cin >> x->ambient_coeff >> x->diffuse_coeff >> x->specular_coeff >> x->reflection_coeff;
        cin >> x->specular_exponent;

        vec.push_back(x);

        if(str == "sphere") continue;

        for(int k = 0; k < 3; k++)
        {
            vec.push_back(new triangle());

            for(int j = 0; j < 3; j++)
                vec.back()->color[j] = x->color[j];

            vec.back()->ambient_coeff = x->ambient_coeff;
            vec.back()->diffuse_coeff = x->diffuse_coeff;
            vec.back()->specular_coeff = x->specular_coeff;
            vec.back()->reflection_coeff = x->reflection_coeff;
            vec.back()->specular_exponent = x->specular_exponent;
        }

         point top(x->leftCorner.x + x->base / 2, x->leftCorner.y + x->base / 2, x->leftCorner.z + x->height);
         for(int j = vec.size() - 1; j >= vec.size() - 4; j--)
            vec[j]->C = top;

         int jdx = vec.size() - 1;

        vec[jdx]->A = point(x->leftCorner.x, x->leftCorner.y, x->leftCorner.z);
        vec[jdx]->B = point(x->leftCorner.x + x->base, x->leftCorner.y, x->leftCorner.z);
        jdx--;

        vec[jdx]->A = point(x->leftCorner.x, x->leftCorner.y, x->leftCorner.z);
        vec[jdx]->B = point(x->leftCorner.x, x->leftCorner.y + x->base, x->leftCorner.z);
        jdx--;

        vec[jdx]->A = point(x->leftCorner.x + x->base, x->leftCorner.y, x->leftCorner.z);
        vec[jdx]->B = point(x->leftCorner.x + x->base, x->leftCorner.y + x->base, x->leftCorner.z);
        jdx--;

        vec[jdx]->A = point(x->leftCorner.x + x->base, x->leftCorner.y + x->base, x->leftCorner.z);
        vec[jdx]->B = point(x->leftCorner.x, x->leftCorner.y + x->base, x->leftCorner.z);

        //now the base
        for(int k = 0; k < 2; k++)
        {
            vec.push_back(new triangle());

            for(int j = 0; j < 3; j++)
                vec.back()->color[j] = x->color[j];

            vec.back()->ambient_coeff = x->ambient_coeff;
            vec.back()->diffuse_coeff = x->diffuse_coeff;
            vec.back()->specular_coeff = x->specular_coeff;
            vec.back()->reflection_coeff = x->reflection_coeff;
            vec.back()->specular_exponent = x->specular_exponent;
        }

        jdx = vec.size() - 1;
        vec[jdx]->A = point(x->leftCorner.x, x->leftCorner.y, x->leftCorner.z);
        vec[jdx]->B = point(x->leftCorner.x + x->base, x->leftCorner.y, x->leftCorner.z);
        vec[jdx]->C = point(x->leftCorner.x + x->base, x->leftCorner.y + x->base, x->leftCorner.z);

        jdx--;
        vec[jdx]->A = point(x->leftCorner.x, x->leftCorner.y, x->leftCorner.z);
        vec[jdx]->B = point(x->leftCorner.x + x->base, x->leftCorner.y, x->leftCorner.z);
        vec[jdx]->C = point(x->leftCorner.x + x->base, x->leftCorner.y + x->base, x->leftCorner.z);
    }

    cin >> light_src_quantity;

    point temp;

    cin >> temp.x >> temp.y >> temp.z;
    light_sources.push_back(temp);

    cin >> temp.x >> temp.y >> temp.z;
    light_sources.push_back(temp);
}

int main(int argc, char **argv)
{
    freopen("description.txt", "r", stdin);
    parseData();

    glutInit(&argc, argv);
    glutInitWindowSize(window_height, window_width);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);    //Depth, Double buffer, RGB color

    glutCreateWindow("Ray-Tracing");

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
