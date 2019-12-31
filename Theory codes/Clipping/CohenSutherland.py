# from dust i have come, dust i will be

import sys

xMin, yMin, xMax, yMax = map(int, input().split())

inside = 0  # 0000
top = 8     # 1000
bottom = 4  # 0100
right = 2   # 0010
left = 1    # 0001


def getOutcode(x, y):
    ret = 0

    if x > xMax:
        ret |= right
    elif x < xMin:
        ret |= left

    if y > yMax:
        ret |= top
    elif y < yMin:
        ret |= bottom

    return ret


def CohenSutherland(x1, y1, x2, y2):
    accept = False

    while True:
        outcode1 = getOutcode(x1, y1)
        outcode2 = getOutcode(x2, y2)

        # trivially accepted
        if outcode1 == 0 and outcode2 == 0:
            accept = True
            break

        # trivially rejected
        elif outcode1 & outcode2:
            break

        else:
            if outcode1:
                out = outcode1
            else:
                out = outcode2

            slope = (y2 - y1) / (x2 - x1)

            # point is in top
            # so we find the intersecting point on top edge
            if out & top:
                x = x1 + (yMax - y1) / slope
                y = yMax

            # point is in bottom
            # so we find the intersecting point on bottom edge
            elif out & bottom:
                x = x1 + (yMin - y1) / slope
                y = yMin

            # point is in right
            # so we find the intersecting point on right edge
            elif out & right:
                x = xMax
                y = y1 + slope * (xMax - x1)

            # point is in left
            # so we find the intersecting point on left edge
            elif out & left:
                x = xMin
                y = y1 + slope * (xMin - x1)

            if out == outcode1:
                x1 = x
                y1 = y
            else:
                x2 = x
                y2 = y

    if accept:
        sys.stdout.write("(" + str(x1) + ", " + str(y1) + ") to (" + str(x2) + ", " + str(y2) + ")")
    else:
        sys.stdout.write("out of the view")

    print()


n = int(input())
for i in range(n):
    e1x, e1y, e2x, e2y = map(int, input().split())
    CohenSutherland(e1x, e1y, e2x, e2y)

'''
50 50 100 100
1
80 20 130 130


4 4 10 8
3
5 5 7 7
7 9 11 4
1 5 4 1


50 10 80 40
1
40 15 75 45


50 50 75 75
1
60 60 30 100
'''