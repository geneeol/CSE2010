import random

n = 5000
elements = list(range(1, n+1))
random.shuffle(elements)

with open("rand_input.txt", "w") as f:
    f.write(str(n) + "\n")
    for element in elements:
        f.write("i " + str(element) + "\n")

    f.write("f " + str(n // 2) + "\n")
    f.write("f " + str(n + 1) + "\n")
    f.write("p\n")
