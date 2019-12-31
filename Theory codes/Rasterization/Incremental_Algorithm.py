# from dust i have come, dust i will be

x0, y0, xn, yn = map(int, input().split())

x = x0
y = y0

m = (yn - y0) / (xn - x0)

pixels = []
while x < xn and y < yn:
    # if x is incremented 1 unit, increment of y would be fractional
    # y1 = mx1 + c
    # y1 = m(x0 + 1) + c
    # y1 = mx0 + c + m
    # y1 = y0 + m
    if m < 1.0:
        x += 1
        y += m

        pixels.append((x, round(y)))

    # if y is incremented 1 unit, increment of x would be fractional
    # x1 = (y1 - c) / m
    # x1 = (y0 + 1 - c) / m
    # x1 = (y0 - c) / m + (1 / m)
    # x1 = x0 + 1 / m
    else:
        x += 1 / m
        y += 1

        pixels.append((round(x), y))
