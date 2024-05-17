import pandas as pd
import numpy as np
from tslearn.metrics import dtw
import matplotlib.pyplot as plt

import random
import colorsys

couleurs_aleatoires = [(random.random(), random.random(), random.random()) for _ in range(10)]


def insert_repeating_pattern(data, pattern, positions):
    for pos in positions:
        data[pos:pos+len(pattern)] = pattern
    return data

# Generate some random data
data = np.random.rand(100)

# Define a repeating pattern
pattern = [0.5, 0.6, 0.6, 0.7, 0.7]

# Define positions to insert the pattern
positions = [10, 30, 50, 70, 90]

# Insert the repeating pattern into the data
data = insert_repeating_pattern(data, pattern, positions)
# Load data
#s = pd.read_csv('data_1714062162.csv', header=0)["angle"]
s=pd.Series(data)
min_angle = abs(s.min())
#print(s)
n = s.shape[0]
s=s+min_angle

# Define the window size and the step size
window_size = round(n/20)
step_size = 1

# Create a list to store the windows
windows = []

# Slide the window across the signal
for i in range(0, n - window_size, step_size):
    window = s[i:i+window_size].values  # Convert to numpy array
    windows.append(window)

# Calculate the pairwise DTW distances between the windows
distances = np.ones((len(windows), len(windows)))*n
distance_array = []
for i in range(len(windows)):
    for j in range(i+1, len(windows)):
        distance = dtw(windows[i], windows[j])/n
        
        distances[i, j] = distance
        distance_array.append(distance)
        #distances[j, i] = distance

# Define the threshold as the mean plus two standard deviations 
def est_contenu_recursivement(valeurT, tuple_a_verifier):
  for i, element in enumerate(tuple_a_verifier):
      for j in tuple_a_verifier[i]:
          if j  == valeurT:
              return True
  return False
for i in range(0,3):
    mean_value = np.mean(distance_array)
    std_dev = np.std(distance_array)
    distance_array =  [c for c in distance_array if c <= mean_value-std_dev]
threshold=np.mean(distance_array)

# Find pairs of windows where the distance is below the threshold
similar_windows = np.where(distances <= threshold)
similar_array = {}




for i, element in enumerate(similar_windows):
    #print(i)
    if i not in similar_array:
        similar_array[i]=[]
        #print(i
        for j, valeur in enumerate(similar_windows[i]):
            if not est_contenu_recursivement(j,similar_array):
                similar_array[i].append(j)  
print(similar_array)
ax0 = plt.subplot(211)
# Plot the original signal

ax0.plot(s-min_angle, label='Original Signal')
ax0.set_xlim(0, n-window_size)
# Plot the similar windows

#for i in range(len(similar_windows[0])):
#    start = similar_windows[0][i] * step_size
#    stop = start + window_size
#    plt.plot(range(start, stop), s[start:stop], label=f'Window {i+1}')

for i, element in enumerate(similar_array):
    if len(similar_array[i]) > 3:   
        color_actuel=couleurs_aleatoires[i]
        for j in similar_array[i]:
            start = similar_windows[0][j] * step_size
            stop = start + window_size
            ax0.plot(range(start, stop), s[start:stop]-min_angle, color=color_actuel)
ax1 = plt.subplot(212)


ax1.plot(distance_array, label='distances')
# Tracez une ligne horizontale à la valeur moyenne
ax1.axhline(mean_value, color='r', linestyle='--')
ax1.axhline(mean_value-std_dev, color='g', linestyle='--')
ax1.fill_between(range(len(distance_array)), mean_value - std_dev, mean_value + std_dev, color='b', alpha=0.2)
plt.show()
