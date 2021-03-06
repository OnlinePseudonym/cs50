from sklearn import datasets
import numpy as np
import matplotlib.pyplot as plt
digits = datasets.load_digits()

print(digits.images[0])

plt.figure()
plt.imshow(digits.images[0], cmap = plt.cm.gray_r, interpolation = 'nearest')
plt.show()

print(digits.target[0])

X_train = digits.data[0:10]
Y_train = digits.target[0:10]

X_test = digits.data[345]

plt.figure()
plt.imshow(digits.images[345], cmap = plt.cm.gray_r, interpolation = 'nearest')
plt.show()

def dist(x, y):
    return np.sqrt(np.sum((x-y)**2))

num = len(X_train)
distance = np.zeros(num)
for i in range(num):
    distance[i] = dist(X_train[i], X_test)
min_index = np.argmin(distance)
print(Y_train[min_index])

print(digits.target[min_index])

num = len(X_train) # Number of points in X_train
no_errors = 0
distance = np.zeros(num) # Numpy arrays of zeros
for j in range(1697, 1797):
    X_test = digits.data[j]
    for i in range(num):
        distance[i] = dist(X_train[i], X_test)
    min_index = np.argmin(distance)
    if Y_train[min_index] != digits.target[j]:
        no_errors += 1
print(no_errors)

X_train = digits.data[0:1000]
Y_train = digits.target[0:1000]

num = len(X_train) # Number of points in X_train
no_errors = 0
distance = np.zeros(num) # Numpy arrays of zeros
for j in range(1697, 1797):
    X_test = digits.data[j]
    for i in range(num):
        distance[i] = dist(X_train[i], X_test)
    min_index = np.argmin(distance)
    if Y_train[min_index] != digits.target[j]:
        no_errors += 1
print(no_errors)
