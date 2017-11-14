import numpy as np                  # for scientific computing
import matplotlib.pyplot as plt     # for generating plots
# %matplotlib notebook

X_train = np.array([[1,1], [2,2.5], [3,1.2], [5.5,6.3], [6, 9], [7,6]])
Y_train = ['red', 'red', 'red', 'blue', 'blue', 'blue']

print(X_train[5,0])
print(X_train[5,1])

print(X_train[:, 0])
print(X_train[:, 1])

X_test = np.array([3,4])

plt.figure()
plt.scatter(X_train[:,0], X_train[:,1], s = 170, color = Y_train[:], alpha = 0.5)
plt.scatter(X_test[0], X_test[1], s = 170, color = 'green')
plt.show()

def dist(x, y):
    return np.sqrt(np.sum((x-y)**2))

num = len(X_train) # Number of points in X_train
distance = np.zeros(num) # Numpy arrays of zeros
for i in range(num):
    distance[i] = dist(X_train[i], X_test)
print(distance)
