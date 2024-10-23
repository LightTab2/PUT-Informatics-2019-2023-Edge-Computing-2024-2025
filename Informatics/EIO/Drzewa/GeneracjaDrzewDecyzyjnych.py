import pandas as pd
import numpy as np

FILE = 'titanic-homework.csv'
BINARY_TREE = False
TEST_DATA_PROBABLITY = 0.0
DECISIONS = 'Survived'
IGNORED_COLUMNS = ['PassengerId', 'Name']

if BINARY_TREE == False:
	from treelib import Node, Tree
	tree = Tree()
	tree.create_node("Root", 0)
treeIndex = 0

def probability(data, classifier, value):
	valueCounts = data[classifier].value_counts()
	return valueCounts[value]/len(data)

def entropy(data):
	ent = 0
	for decision in data[DECISIONS].unique():
		prob = probability(data, DECISIONS, decision)
		if prob != 0.0:
			ent = ent - prob*np.log2(prob)
	return ent

def entropyRelative(data, classifier):
	ent = 0
	for value in data[classifier].unique():
		#print(value, probability(data, classifier, value), entropyPart(data, classifier, value))
		ent = ent + probability(data, classifier, value) * entropyPart(data, classifier, value)
	return ent

def entropyPart(data, classifier, value):
	#print(data[data[classifier] == value])
	return entropy(data[data[classifier] == value])

def gain(data, classifier):
	return entropy(data) - entropyRelative(data, classifier)

class Node(object):
	def __init__(self):
		self.value = None
		self.condition = None
		if BINARY_TREE:
			self.left = None # Typ: Node, wierzchołek znajdujący się po lewej stornie
			self.right = None # Typ: Node, wierzchołek znajdujący się po prawej stornie
		else:
			self.nodes = []

	def perform_split(self, data, parentIndex = 0):
		global treeIndex
		maxGain = 0
		for col in data.columns:
			if col != DECISIONS:
				gained = gain(data, col)
				#print(col, gained)
				if gained > maxGain:
					maxGain = gained
					bestClassifier = col
		# if uzyskano poprawę funkcji celu (bądź inny, zaproponowany przez Ciebie warunek):
		if maxGain != 0:
			#print(bestClassifier)
			if BINARY_TREE:
				leastEntropy = -np.log2(1/len(data))
				for value in data[bestClassifier].unique():
					ent = entropyPart(data, bestClassifier, value)
					if ent < leastEntropy:
						leastEntropy = ent
						bestVal = value

				self.condition = [ bestClassifier, bestVal ]
				self.left = Node()
				self.right = Node()
				self.left.perform_split(data[data[bestClassifier] != bestVal])
				self.right.perform_split(data[data[bestClassifier] == bestVal])
			else:
				self.condition = [bestClassifier]
				for value in data[bestClassifier].unique():
					self.condition.append(value)
					self.nodes.append(Node())
					treeIndex = treeIndex + 1
					tree.create_node(bestClassifier + "==" + str(value), treeIndex, parent=parentIndex)
					self.nodes[-1].perform_split(data[data[bestClassifier] == value], treeIndex)
		else:
			bestProb = 0
			maxSize = 0
			for decision in data[DECISIONS].unique():
				prob = probability(data, DECISIONS, decision)
				size = len(data[data[DECISIONS] == decision])
				if prob == bestProb:
					if (size > maxSize):
						maxSize = size
						bestVal = decision
				elif prob > bestProb:
					bestProb = prob
					bestVal = decision
			self.value = bestVal
			if BINARY_TREE == False:
				treeIndex = treeIndex + 1
				tree.create_node(str(bestVal), treeIndex, parent=parentIndex)

	def predict(self, example):
		if (BINARY_TREE):
			if self.value is None:
				if example[self.condition[0]] == self.condition[1]:
					return self.right.predict(example)
				else:
					return self.left.predict(example)
		elif self.value is None:
			nodeIndex = 0
			for value in self.condition[1:]:
				if example[self.condition[0]] == value:
					return self.nodes[nodeIndex].predict(example)
				nodeIndex = nodeIndex + 1

		return self.value

	#I got it from there: https://stackoverflow.com/questions/34012886/print-binary-tree-level-by-level-in-python
	def display(self):
		lines, *_ = self._display_aux()
		for line in lines:
			print(line)

	def _display_aux(self):
		"""Returns list of strings, width, height, and horizontal coordinate of the root."""
		# No child.
		if self.right is None and self.left is None:
			if self.value is None:
				line = '%s' % str(self.condition[0]) + '==' + str(self.condition[1])
			else:
				line = '%s' % str(self.value)
			width = len(line)
			height = 1
			middle = width // 2
			return [line], width, height, middle

		# Only left child.
		if self.right is None:
			lines, n, p, x = self.left._display_aux()
			if self.value is None:
				s = '%s' % str(self.condition[0]) + '==' +  str(self.condition[1])
			else:
				s = '%s' % str(self.value)
			u = len(s)
			first_line = (x + 1) * ' ' + (n - x - 1) * '_' + s
			second_line = x * ' ' + '/' + (n - x - 1 + u) * ' '
			shifted_lines = [line + u * ' ' for line in lines]
			return [first_line, second_line] + shifted_lines, n + u, p + 2, n + u // 2

		# Only right child.
		if self.left is None:
			lines, n, p, x = self.right._display_aux()
			if self.value is None:
				s = '%s' % str(self.condition[0]) + '==' +  str(self.condition[1])
			else:
				s = '%s' % str(self.value)
			u = len(s)
			first_line = s + x * '_' + (n - x) * ' '
			second_line = (u + x) * ' ' + '\\' + (n - x - 1) * ' '
			shifted_lines = [u * ' ' + line for line in lines]
			return [first_line, second_line] + shifted_lines, n + u, p + 2, u // 2

		# Two children.
		left, n, p, x = self.left._display_aux()
		right, m, q, y = self.right._display_aux()
		if self.value is None:
			s = '%s' % str(self.condition[0]) + '==' +  str(self.condition[1])
		else:
			s = '%s' % str(self.value)
		u = len(s)
		first_line = (x + 1) * ' ' + (n - x - 1) * '_' + s + y * '_' + (m - y) * ' '
		second_line = x * ' ' + '/' + (n - x - 1 + u + y) * ' ' + '\\' + (m - y - 1) * ' '
		if p < q:
			left += [n * ' '] * (q - p)
		elif q < p:
			right += [m * ' '] * (p - q)
		zipped_lines = zip(left, right)
		lines = [first_line, second_line] + [a + u * ' ' + b for a, b in zipped_lines]
		return lines, n + m + u, max(p, q) + 2, n + u // 2

if __name__ == "__main__":
	### Implementacja wczytywania danych i losowy podział na dane uczące i testowe
	data = pd.read_csv(FILE)
	data = data.drop(labels = IGNORED_COLUMNS, axis = 1)
	data['Age'] = pd.cut(data['Age'], bins=[0, 20, 40, 100], include_lowest=True, labels=['young','middle','old'])
	randMask = np.random.rand(len(data)) < TEST_DATA_PROBABLITY
	test_data = data[randMask]
	data = data[~randMask]
	for col in data.columns:
		gained = gain(data, col)
		print(col, gained)
	tree_root = Node()
	tree_root.perform_split(data)
	if (BINARY_TREE):
		tree_root.display()
	else:
		print(tree)
	print('Training complete!')

	### Implementacja zmierzenia trafności klasyfikacji (!) na danych testowych i uczących np.
	fail = 0
	for i, element in data.iterrows():
		y = tree_root.predict(element)
		if y != element[DECISIONS]:
			print('Bad prediciton in training dataset!', y, 'expected:', element[DECISIONS])
			fail = fail + 1
	print('Training dataset size:', len(data))
	print('Accuracy:', 1 - fail / len(data))

	fail = 0
	for i, element in test_data.iterrows():
		y = tree_root.predict(element)
		if y != element[DECISIONS]:
			fail = fail + 1

	print('Test dataset size:', len(test_data))
	if (len(test_data) != 0):
		print('Accuracy:', 1 - fail/len(test_data))

#Example output (BINARY_TREE = True):
#Data loading complete!
#                                                                         ____Sex==male__________________________________________________________________________________________________
#                                                                        /                                                                                                               \
#                                                               _____SibSp==0                                                                                                    ____Parch==5
#                                                              /             \                                                                                                  /            \
#                                                      ____Pclass==1         1                                                             _________________________________Parch==3         0
#                                                     /             \                                                                     /                                         \
#                                            _____Parch==5          1                                                             ____Parch==0________________________              0
#                                           /             \                                                                      /                                    \
#                                   ____Pclass==2         1                                                              ____SibSp==3                        _____Pclass==3
#                                  /             \                                                                      /            \                      /              \
#                 _____________SibSp==5          1                                                             _____SibSp==4         0              ____Pclass==2          0
#                /                     \                                                                      /             \                      /             \
#       _____SibSp==1_____             0                                                              ____Pclass==2         0                  SibSp==0          0
#      /                  \                                                                          /             \                          /        \
# Age==young          Parch==1                                                               ____Age==old          1                          0        1
#/          \        /        \                                                             /            \
#1          0        0        1                                                         Parch==2         0
#                                                                                      /        \
#                                                                                      1        0
#Training complete!
#Training dataset size: 65
#Accuracy: 1.0
#Test dataset size: 35
#Accuracy: 0.8571428571428572




#Example output (BINARY_TREE = False):
#Data loading complete!
#Root
#├── Sex==female
#│   ├── SibSp==0
#│   │   └── 1
#│   ├── SibSp==1
#│   │   ├── Pclass==1
#│   │   │   └── 1
#│   │   ├── Pclass==2
#│   │   │   └── 1
#│   │   └── Pclass==3
#│   │       ├── Parch==0
#│   │       │   └── 0
#│   │       ├── Parch==1
#│   │       │   └── 1
#│   │       └── Parch==5
#│   │           └── 1
#│   ├── SibSp==2
#│   │   └── 0
#│   ├── SibSp==3
#│   │   └── 1
#│   ├── SibSp==4
#│   │   └── 1
#│   └── SibSp==5
#│       └── 0
#└── Sex==male
#    ├── Pclass==1
#    │   ├── Age==middle
#    │   │   ├── SibSp==0
#    │   │   │   └── 1
#    │   │   └── SibSp==1
#    │   │       └── 0
#    │   ├── Age==old
#    │   │   └── 0
#    │   └── Age==young
#    │       ├── SibSp==0
#    │       │   └── 1
#    │       └── SibSp==3
#    │           └── 0
#    ├── Pclass==2
#    │   ├── Age==middle
#    │   │   └── 0
#    │   └── Age==young
#    │       └── 1
#    └── Pclass==3
#        ├── Parch==0
#        │   └── 0
#        ├── Parch==1
#        │   ├── Age==middle
#        │   │   └── 1
#        │   └── Age==young
#        │       └── 0
#        ├── Parch==2
#        │   └── 0
#        ├── Parch==3
#        │   └── 0
#        └── Parch==5
#            └── 0
#
#Training complete!
#Training dataset size: 67
#Accuracy: 1.0
#Test dataset size: 33
#Accuracy: 0.9393939393939394
