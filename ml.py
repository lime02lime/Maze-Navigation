# %%
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from sklearn import ensemble
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from sklearn import tree

from keras import Sequential
from keras.layers import Dense


data = pd.read_csv("mldata.csv")
data.head()
data.info()

inputs = data.to_numpy()[:, :4]
labels = data.to_numpy()[:, 4]

treeClassifier = tree.DecisionTreeClassifier()
treeClassifier.fit(inputs, labels)

test = treeClassifier.predict(inputs)
print(test == labels)

# %%


# %%
# Let's

svm = SVC(C=1000, kernel='rbf', gamma='scale')
svm.fit(x, y)

xline1 = np.linspace(0, 40, 401)
xline2 = np.linspace(0, 360, 361)
xx, yy = np.meshgrid(xline1, xline2)
xx = xx.flatten()
yy = yy.flatten()
print(xx, yy)
coords = np.array([xx, yy]).transpose()
c = svm.predict(coords)
c = c.reshape((len(xline2), len(xline1)))

plt.text(30, 300, "Black: no slip\n White: slip")
plt.xlabel("f2 (N)")
plt.ylabel("f2ang (degrees)")
plt.contourf(xline1, xline2, c, cmap="bone", dpi=1000)
# plt.scatter(x[:, 0], x[:, 1], c=y)

# %%

model = Sequential()
model.add(Dense(units=4, activation="tanh"))
model.add(Dense(units=2, activation="softmax"))
model.compile(optimizer="adam", loss="mse")

x = df1[["f2", "f2ang"]].to_numpy()
y = df1["slips"]

scaler = StandardScaler()
scaler.fit(x)
x = scaler.transform(x)
print(x)
y2d = np.array([abs(y-1), y]).transpose()

model.fit(x, y2d, epochs=1200)

print(scaler.transform(coords))
c = model.predict(scaler.transform(coords))
preds = np.array([row[1] > row[0] for row in c])
preds = preds.astype(int)

preds = preds.reshape((len(xline2), len(xline1)))

# %%

rf = ensemble.RandomForestClassifier(n_estimators=10, max_depth=5)

prob = rf.predict_proba(Xgrid)
classes = []
for row in prob:
    classes.append(row.argmax())
classes = np.array(classes)

prob_plot = classes.reshape(200, 200)

fig, ax = plt.subplots()

# (let's avoid using a random forest classifier, would probably be a pain to transfer all of the decision rules)
