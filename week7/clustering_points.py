import numpy as np
import matplotlib.pyplot as plt

X = np.array([[1,1], [2,2.5], [3,1.2], [5.5, 6.3], [6,9], [7,6], [8,8]])

plt.figure()
plt.scatter(X[:,0], X[:,1], s = 170, color = 'black', alpha = 0.5)
plt.show()

from sklearn.cluster import KMeans

k = 3
kmeans = KMeans(n_clusters = k)
kmeans.fit(X);
centroids = kmeans.cluster_centers_ # Get centroid's coordinates
labels = kmeans.labels_ # Get label assignment

colors = ['r.', 'g.', 'y.'] # Define two colors for the plot below
plt.figure()
for i in range(len(X)):
    plt.plot(X[i,0], X[i,1], colors[labels[i]], markersize = 30)
plt.scatter(centroids[:,0], centroids[:,1], marker = "x", s = 300, linewidths = 5)
plt.show()

corpus = ['I love CS50. Staff is awesome, awesome, awesome!',
          'I have a dog and a cat',
          'Best of CS50? Staff. And cakes. Ok, CS50 staff.',
          'My dog keeps chasing my cat. Dogs!']

from sklearn.feature_extraction.text import CountVectorizer
count_vect = CountVectorizer(stop_words = 'english')
Z = count_vect.fit_transform(corpus)
Z.todense()

vocab = count_vect.get_feature_names()
print(vocab)

from sklearn.feature_extraction.text import TfidfVectorizer
vectorizer = TfidfVectorizer(stop_words = 'english')
X = vectorizer.fit_transform(corpus)
X.todense()

k = 2
# Define the proper notion of distance to deal with documents
from sklearn.metrics.pairwise import cosine_similarity
dist = 1 - cosine_similarity(X)
model = KMeans(n_clusters = k)
model.fit(X);

print("Top terms per cluster:\n")
order_centroids = model.cluster_centers_.argsort()[:, ::-1]
terms = vectorizer.get_feature_names()
for i in range(k):
    print("Cluster %i:" %i, end ="")
    for ind in order_centroids[i, :3]:
        print(' %s,' %terms[ind], end = '')
    print("")

import pandas as pd # Pandas provides easy-to-use data structures
from io import StringIO
import requests

act = requests.get('https://docs.google.com/spreadsheets/d/1udJ4nd9EKlX_awB90JCbKaStuYh6aVjh1X6j8iBUXIU/export?format=csv')
dataact = act.content.decode('utf-8') # To convert to string for StringIO
frame = pd.read_csv(StringIO(dataact))

print(frame)

corpus = []
for i in range(0, frame["Synopsis"].size):
    corpus.append(frame["Synopsis"][i])

from sklearn.feature_extraction.text import TfidfVectorizer
vectorizer = TfidfVectorizer(stop_words = 'english', min_df = 0.2)
X = vectorizer.fit_transform(corpus)

k = 2
from sklearn.metrics.pairwise import cosine_similarity
dist = 1 - cosine_similarity(X)
model = KMeans(n_clusters = k)
model.fit(X);

no_words = 3 # Number of words to prit per cluster
order_centroids = model.cluster_centers_.argsort()[:, ::-1]
terms = vectorizer.get_feature_names()
labels = model.labels_

print("Top terms per cluster:\n")
for i in range(k):

    print("Cluster %d movies:" % i, end='')
    for title in frame["Title"][labels == i]:
        print(' %s,' % title, end='')
    print()

    print("Cluster %d words:" % i, end='')
    for ind in order_centroids[i, :no_words]:
        print (' %s' % terms[ind], end=','),
    print()
    print()
