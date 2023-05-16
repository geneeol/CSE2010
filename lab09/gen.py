with open('big_input.txt', 'w') as f:
    f.write('5\n')
    for i in range(1, 5001):
        f.write(f'i {i}\n')
    f.write('f 2500\n')
    f.write('f 5001\n')
    f.write('p\n')
