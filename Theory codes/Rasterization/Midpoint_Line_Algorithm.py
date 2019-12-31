# from dust i have come, dust i will be

'''
Midpoint-Line Algorithm
1. at each iteration the algorithm chooses between two pixels E and NE
depending on d calculated on previous iteration

2. then d is updated depending on selected pixels

3. at start midpoint is (x0 + 1, y0 + 1 / 2)
F(x0 + 1, y0 + 1 / 2) = a(x0 + 1) + b(y0 + 1 / 2) + c
                      = ax0 + by0 + c + a + b / 2
                      = F(x0, y0) + a + b / 2
                    0 = a + (b / 2)
                    0 = 2a + b

some calculations:

m = dy / dx
dy = yl - y0
dx = xl - x0

y = dy / dx * x + B
so ydx - xdy+ Bdx = 0

comparing with ax+by+c = 0
a = dy, b = -dx, c = Bdx

initial midpoint M(x0 + 1, y0 + 1 / 2)

'''

pixels = []


def WritePixel(x, y, color):
    pixels.append((x, y, color))


def MidpointLine(x0, y0, xl, yl, color):
    dx = xl - x0
    dy = yl - y0

    # d_start = a + b / 2 = 0
    # a = dy, b = -dx, c = B.dx
    # dy - dx / 2 = 0
    # 2 * dy - dx = 0
    d = 2 * dy - dx

    # d_new = d_old + a
    dE = 2 * dy

    # d_new = d_old + a + b
    dNE = 2 * (dy - dx)

    x = x0
    y = y0

    WritePixel(x, y, color)

    while x < xl:
        # select E - 1 unit in x-axis
        if d <= 0:
            d += dE
            x += 1

        # select NE - select 1 unit in both the axis
        else:
            d += dNE
            x += 1
            y += 1

        WritePixel(x, y, color)


x0, y0, xl, yl = map(int, input().split())
MidpointLine(x0, y0, xl, yl, 255)

# (5, 8) to (9, 11)
