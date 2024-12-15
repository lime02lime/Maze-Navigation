# %%
import tensorflow as tf
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

from sklearn import ensemble
from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from sklearn import tree
from sklearn.tree import _tree

from keras import Sequential
from keras.layers import Dense

# %%
# Importing data taken at AH flat and fitting a decision tree model to it
dataAH = pd.read_csv("mldata.csv")
dataAH.head()
dataAH.info()


inputsAH = dataAH.to_numpy()[:, :4]
labelsAH = dataAH.to_numpy()[:, 4]


treeClassifier = tree.DecisionTreeClassifier(min_samples_split=3)
treeClassifier.fit(inputsAH, labelsAH)

# For now, testing fit only according to training data (just to see if it works to a certain depth)
test = treeClassifier.predict(inputsAH)
print(test == labelsAH)
# %%


def tree_to_code(tree, feature_names):
    tree_ = tree.tree_
    feature_name = [
        feature_names[i] if i != _tree.TREE_UNDEFINED else "undefined!"
        for i in tree_.feature
    ]
    print("def tree({}):".format(", ".join(feature_names)))

    def recurse(node, depth):
        indent = "  " * depth
        if tree_.feature[node] != _tree.TREE_UNDEFINED:
            name = feature_name[node]
            threshold = tree_.threshold[node]
            print("{}if {} <= {}:".format(indent, name, threshold))
            recurse(tree_.children_left[node], depth + 1)
            print("{}else:  # if {} > {}".format(indent, name, threshold))
            recurse(tree_.children_right[node], depth + 1)
        else:
            print("{}return {}".format(indent, tree_.value[node]))

    recurse(0, 1)


tree_to_code(treeClassifier, ['C', 'Rn', 'Gn', 'Bn'])

# %%


datalab = pd.read_csv("mllab.csv")
datalab.head()
datalab.info()

inputslab = datalab.to_numpy()[:, :4]
labelslab = datalab.to_numpy()[:, 4]

treeClassifier = tree.DecisionTreeClassifier(min_samples_split=3)
treeClassifier.fit(inputslab, labelslab)

test = treeClassifier.predict(inputslab)
print(test == labelslab)

# %%
combined_data = pd.concat([datalab, dataAH])
# Shuffling data
combined_data = combined_data.sample(frac=1)

val_index = int(len(combined_data) * 3 / 4)
train_data = combined_data.iloc[:val_index]
val_data = combined_data[val_index:]

inputs_combined = train_data.to_numpy()[:, :4]
labels_combined = train_data.to_numpy()[:, 4]

treeClassifier = tree.DecisionTreeClassifier(min_samples_split=3)
treeClassifier.fit(inputs_combined, labels_combined)

test_features = val_data.to_numpy()[:, :4]
test_labels = val_data.to_numpy()[:, 4]

test = treeClassifier.predict(test_features)
print(test == test_labels)

# %%

# Reshaping the inputs so that 'similar' ones are closer to each other numerically
# new order::
# 0: white
# 1: pink
# 2: red
# 3: orange
# 4: yellow
# 5: green
# 6: light blue
# 7: blue
# 8: black


def new_index(idx):
    if idx == 0:
        return 2
    if idx == 1:
        return 5
    if idx == 2:
        return 7
    if idx == 3:
        return 4
    if idx == 4:
        return 1
    if idx == 5:
        return 3
    if idx == 6:
        return 6
    if idx == 7:
        return 0
    if idx == 8:
        return 8


combined_data['Label'] = combined_data['Label'].apply(new_index)

val_index = int(len(combined_data) * 3 / 4)
train_data = combined_data.iloc[:val_index]
val_data = combined_data[val_index:]

inputs_combined = train_data.to_numpy()[:, :4]
labels_combined = train_data.to_numpy()[:, 4]

test_features = val_data.to_numpy()[:, :4]
test_labels = val_data.to_numpy()[:, 4]

# transform_combined_output_data = []
# for label in labels_combined:
#     outvec = np.zeros(9)
#     for i in range(9):
#         if i == label:
#             outvec[i] = 1
#     transform_combined_output_data.append(outvec)
# transform_combined_output_data = np.stack(transform_combined_output_data)


# transform_test_output_data = []
# for label in test_labels:
#     outvec = np.zeros(9)
#     for i in range(9):
#         if i == label:
#             outvec[i] = 1
#     transform_test_output_data.append(outvec)
# transform_test_output_data = np.stack(transform_test_output_data)


model = Sequential()
model.add(tf.keras.Input(shape=(4,)))
model.add(Dense(units=4, activation="relu"))
model.add(Dense(units=4, activation="relu"))
model.add(Dense(units=1, activation="linear"))
model.compile(optimizer="adam", loss="mse")

history = model.fit(
    x=inputs_combined, y=labels_combined, epochs=500, batch_size=20)
# %%
test = model.predict(inputs_combined)
print(test.astype(int) == labels_combined)


test = model.predict(test_features)
print(test.astype(int) == test_labels)

# scaler = StandardScaler()
# scaler.fit(x)
# x = scaler.transform(x)
# print(x)
# y2d = np.array([abs(y-1), y]).transpose()

# model.fit( y2d, epochs=1200)

# print(scaler.transform(coords))
# c = model.predict(scaler.transform(coords))
# preds = np.array([row[1] > row[0] for row in c])
# preds = preds.astype(int)

# preds = preds.reshape((len(xline2), len(xline1)))

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
