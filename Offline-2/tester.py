# from dust i have come, dust i will be

import os


# builds the input file that will be read by c++ code from test case folder
def build_input_file(path):
    inp = open(path, "r")
    scene = open("scene.txt", "w")

    scene.writelines(inp.readlines())


# give actual path not relative path
def check_output(actual_ans_path, your_ans_path):
    aa = open(actual_ans_path)
    ya = open(your_ans_path)

    a = aa.readlines()
    b = ya.readlines()

    if len(a) != len(b):
        return False

    for i in range(len(a)):
        if a[i] != b[i]:
            return False

    return True


if os.system("g++ -std=c++11 1505107.cpp") == 0:
    print("code compiled")
else:
    print("code compilation error")

for i in range(3):
    build_input_file(os.getcwd() + "/Assignment 2/test cases/" + str(i + 1) + "/scene.txt")

    if os.system("./a.out") == 0:
        print("successfully ran the code")
    else:
        print("error running the code")

    base_path_actual = os.getcwd() + "/Assignment 2/test cases/" + str(i + 1)
    s1 = check_output(base_path_actual + "/stage1.txt",
                      os.getcwd() + "/stage1.txt")

    s2 = check_output(base_path_actual + "/stage2.txt",
                      os.getcwd() + "/stage2.txt")

    s3 = check_output(base_path_actual + "/stage3.txt",
                      os.getcwd() + "/stage3.txt")

    print("test case no.", (i + 1))
    print("stage1", s1)
    print("stage2", s2)
    print("stage3", s3)
