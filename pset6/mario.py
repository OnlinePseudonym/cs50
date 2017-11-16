print("Enter desired height of the pyramid")
height = input()
while True:
    try:
        height = int(height)
    except ValueError:
        print("Please enter an intager greater than 0 and less than 23")
        continue
    if (height < 23 and
        height > 0):
        break
for row in range(height):
    for left in range(height):
        if (left < height - (row + 1)):
            print(' ', end='')
        else:
            print('#', end='')
    print('  ', end='')
    for right in range(height):
        if (right < row + 1):
            print('#', end='')
    print()
