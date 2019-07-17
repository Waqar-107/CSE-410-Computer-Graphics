/***from dust i have come, dust i will be***/

#include<bits/stdc++.h>

#define dbg printf("in\n")
#define nl printf("\n")
#define pfs(s) printf("%s",s)

#define pi acos(-1.0)
#define pp pair<matrix, bool>

using namespace std;

struct point {
    double x, y, z, w;
    point(){
        w = 1;
    }
    point(double x, double y, double z)
    {
        this->x = x;
        this->y = y;
        this->z = z;

        this->w = 1;
    }

    void print() {
        cout << x << " " << y << " " << z << " " << w << endl;
    }
};

struct matrix {
    double mat[4][4];
    matrix(){
        resetMatrix();
    }

    void print()
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
                cout << fixed << setprecision(7) << mat[i][j] << " ";

            nl;
        }
    }

    void print_in_file(FILE *f)
    {
         for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                fprintf(f, "%.7f", mat[j][i] / mat[3][i]);
                if(j < 2)
                    fprintf(f, " ");
            }

            fprintf(f, "\n");
        }

        fprintf(f, "\n");
    }

    void resetMatrix()
    {
        memset(mat, 0, sizeof(mat));
        for(int i = 0; i < 4; i++)
            mat[i][i] = 1;
    }
};

//===============================================
FILE *stage1, *stage2, *stage3;
point eye, look, up;
double fovY, aspectRatio, near, far;
matrix mTop, V, P;

stack<pp> stk;
//===============================================


//===============================================
//returns a * b
matrix multiply(matrix a, matrix b)
{
    matrix mult;

    //4 columns of b
    for(int c = 0; c < 4; c++)
    {
        for(int i = 0; i < 4; i++)
        {
            double sum = 0;
            for(int j = 0; j < 4; j++)
                sum += a.mat[i][j] * b.mat[j][c];

            mult.mat[i][c] = sum;
        }
    }

    return mult;
}

double degreeToRadian(double deg) {
    return (pi * deg) / 180;
}

point normalized(point p)
{
    double sq = sqrt(p.x * p.x + p.y * p.y + p.z * p.z);

    p.x /= sq;
    p.y /= sq;
    p.z /= sq;

    return p;
}

matrix scaleW(matrix m)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j  = 0; j < 4; j++)
            m.mat[i][j] /= m.mat[3][i];
    }

    return m;
}

point cross_product(point u, point v) {
    point temp;
    temp.x = u.y * v.z - u.z * v.y;
    temp.y = u.z * v.x - u.x * v.z;
    temp.z = u.x * v.y - u.y * v.x;

    return temp;
}

void init()
{
    stage1 = fopen("stage1.txt", "w");
    stage2 = fopen("stage2.txt", "w");
    stage3 = fopen("stage3.txt", "w");

    stk.push({mTop, false});

    //for view transformation
    point l = point(look.x - eye.x, look.y - eye.y, look.z - eye.z);
    l = normalized(l);

    point r = cross_product(l, up);
    r = normalized(r);

    point u = cross_product(r, l);

    matrix T;
    T.mat[0][3] = -eye.x;
    T.mat[1][3] = -eye.y;
    T.mat[2][3] = -eye.z;

    matrix R;
    R.mat[0][0] = r.x;
    R.mat[0][1] = r.y;
    R.mat[0][2] = r.z;

    R.mat[1][0] = u.x;
    R.mat[1][1] = u.y;
    R.mat[1][2] = u.z;

    R.mat[2][0] = -l.x;
    R.mat[2][1] = -l.y;
    R.mat[2][2] = -l.z;

    V = multiply(R, T);

    //projection
    double fovX = fovY * aspectRatio;

    double t = near * tan(degreeToRadian(fovY / 2));
    double r2 = near * tan(degreeToRadian(fovX / 2));

    P.mat[0][0] = near / r2;
    P.mat[1][1] = near / t;
    P.mat[2][2] = -(far + near) / (far - near);
    P.mat[3][3] = 0;

    P.mat[3][2] = -1;
    P.mat[2][3] = -(2 * far * near) / (far - near);
}

point Rodrigues(point x, point a, double theta)
{
    double ca = cos(degreeToRadian(theta));
    double sa = sin(degreeToRadian(theta));

    double b = (1 - ca) * (x.x * a.x + x.y * a.y + x.z * a.z);

    point A = point(x.x * ca, x.y * ca, x.z * ca);
    point B = point(a.x * b, a.y * b, a.z * b);

    point C = cross_product(a, x);

    C.x *= sa;
    C.y *= sa;
    C.z *= sa;

    return point(A.x + B.x + C.x, A.y + B.y + C.y, A.z + B.z + C.z);
}
//===============================================

int main()
{
    freopen("scene.txt", "r", stdin);

    int i, j, k;
    int n, m;
    double x, y, z, angle, sq;

    point c;
    matrix temp, temp2, temp3;
    string cmd;

    //gluLookAt
    cin >> eye.x >> eye.y >> eye.z;
    cin >> look.x >> look.y >> look.z;
    cin >> up.x >> up.y >> up.z;

    //gluPerspective
    cin >> fovY >> aspectRatio >> near >> far;

    init();

    while(true)
    {
        cin >> cmd;

        if(cmd == "end")
            break;

        else if(cmd == "triangle")
        {
            temp.resetMatrix();

            cin >> temp.mat[0][0] >> temp.mat[1][0] >> temp.mat[2][0];
            cin >> temp.mat[0][1] >> temp.mat[1][1] >> temp.mat[2][1];
            cin >> temp.mat[0][2] >> temp.mat[1][2] >> temp.mat[2][2];

            temp.mat[0][3] = temp.mat[1][3] = temp.mat[2][3] = 1;
            temp.mat[3][0] = temp.mat[3][1] = temp.mat[3][2] = 1;

            if(stk.empty())
                cout << "error during triangle op, stack empty\n";

            else
                temp = multiply(stk.top().first, temp), temp.print_in_file(stage1);

            //view
            temp2 = multiply(V, temp);
            temp2.print_in_file(stage2);

            //projection
            temp3 = multiply(P, temp2);
            temp3.print_in_file(stage3);
        }

        else if(cmd == "translate")
        {
            cin >> x >> y >> z;

            temp.resetMatrix();
            temp.mat[0][3] = x;
            temp.mat[1][3] = y;
            temp.mat[2][3] = z;

            if(stk.empty())
                cout << "error during translation, stack empty\n";

            else
                mTop = multiply(stk.top().first, temp), stk.push({mTop, false});
        }

        else if(cmd == "scale")
        {
            cin >> x >> y >> z;

            temp.resetMatrix();
            temp.mat[0][0] = x;
            temp.mat[1][1] = y;
            temp.mat[2][2] = z;

            if(stk.empty())
                cout << "error during scaling, stack empty\n";

            else
                mTop = multiply(stk.top().first, temp), stk.push({mTop, false});
        }

        else if(cmd == "rotate")
        {
            cin >> angle >> x >> y >> z;

            sq = sqrt(x * x + y * y + z * z);

            x /= sq;
            y /= sq;
            z /= sq;

            temp.resetMatrix();

            c = Rodrigues(point(1, 0, 0), point(x, y, z), angle);
            temp.mat[0][0] = c.x;
            temp.mat[1][0] = c.y;
            temp.mat[2][0] = c.z;

            c = Rodrigues(point(0, 1, 0), point(x, y, z), angle);
            temp.mat[0][1] = c.x;
            temp.mat[1][1] = c.y;
            temp.mat[2][1] = c.z;

            c = Rodrigues(point(0, 0, 1), point(x, y, z), angle);
            temp.mat[0][2] = c.x;
            temp.mat[1][2] = c.y;
            temp.mat[2][2] = c.z;
cout<<"rotate "<<angle<<" "<<x<<" "<<y<<" "<<z;nl;
temp.print();nl;
stk.top().first.print();nl;nl;
            if(stk.empty())
                cout << "error during rotation, stack empty\n";

            else
                mTop = multiply(stk.top().first, temp), stk.push({mTop, false});
        }

        else if(cmd == "push")
            stk.push({stk.top().first, true});

        else if(cmd == "pop")
        {
            while(!stk.empty() && !stk.top().second)
                stk.pop();

            if(!stk.empty())
                stk.pop();
            else
                cout<<"unable to pop\n";
        }

        //cout<<cmd<<" "<<stk.size()<<endl;
    }

    fclose(stage1);
    fclose(stage2);
    fclose(stage3);

    return 0;
}
