import threading
import time

threadLock = threading.Lock()

### repeats to add key for nodes

KEY_COUNTER = 1

GOAL = [1, 2, 3, 8, 0, 4, 7, 6, 5]

### Initial States

EASY = [1, 3, 4, 8, 6, 2, 7, 0, 5]
MEDIUM = [2, 8, 1, 0, 4, 3, 7, 6, 5]
HARD = [5, 6, 7, 4, 0, 8, 3, 2, 1]


### Prints the state into square

def print_Children(child):
    print(child[0], "  ", child[1], "  ", child[2])
    print(child[3], "  ", child[4], "  ", child[5])
    print(child[6], "  ", child[7], "  ", child[8])
    print()
    print("-----------------------------------------")


### Traceback the program for finding the path from the goal node

def print_FinalPath(NodeList, node):
    traceBack = Stack()
    traceBack.push(node)
    totalPathCost = 0
    while node.get_Key() > 1:
        parentKey = node.get_ParentKey()
        node = NodeList.get(parentKey)
        traceBack.push(node)
    while traceBack.size() > 0:
        node = traceBack.pop()
        totalPathCost = totalPathCost + node.get_Cost()
        print("-----------------------------------------")
        print()
        print("Node Key:", node.get_Key())
        print("Node State:", node.get_State())
        print("Action:", node.get_Action())
        print("Depth:", node.get_Depth())
        print("Edge Cost:", node.get_Cost())
        print("Total Cost:", totalPathCost)
        print("Heuristic Cost:", node.getHcost())
        print("f(n):", node.getHcost() + totalPathCost)
        print()
        print_Children(node.get_State())


### Classes for nodes which are given in the graph

class Node:
    key = 0
    state = []
    children = []
    childrenKeys = []
    visited = False
    parentKey = 0
    parentState = []
    action = ""
    depth = 0
    cost = 0
    totalPathCost = 0
    h_Cost = 0

    ### Implementation of the Constructor

    def __init__(self, given_Key, given_State, given_ParentKey, given_ParentState, given_Action, given_ChildKeys,
                 given_Depth, given_Cost):
        self.key = given_Key
        self.state = given_State
        self.parentKey = given_ParentKey
        self.parentState = given_ParentState
        self.childrenKeys = given_ChildKeys
        self.action = given_Action
        self.depth = given_Depth
        self.cost = given_Cost
        self.totalPathCost = given_Cost

    ### Setters and Getters are given

    def add_Children(self, givenChildren):
        self.children = givenChildren

    def add_ChildKeys(self, key):
        self.childrenKeys.append(key)

    def get_ChildKeys(self):
        return self.childrenKeys

    def set_Visited(self):
        self.visited = True

    def get_Children(self):
        return self.children

    def get_Visited(self):
        return self.visited

    def get_State(self):
        return self.state

    def get_Key(self):
        return self.key

    def get_ParentKey(self):
        return self.parentKey

    def get_Cost(self):
        return self.cost

    def get_TotalPathCost(self):
        return self.totalPathCost

    def add_TotalCost(self, x, y):
        self.totalPathCost = x + y

    def get_ParentState(self):
        return self.parentState

    def get_Action(self):
        return self.action

    def set_Depth(self, given_Depth):
        self.depth = given_Depth

    def get_Depth(self):
        return self.depth

    def getHcost(self):
        return self.h_Cost

    def set_Hcost(self, cost):
        self.h_Cost = cost

    ### Heurstic Functions

    ### Excludes number of tiles that are out of place

    def h1(self):
        h = 0
        state = list(self.get_State())
        if state[0] != 1:
            h += 1
        if state[1] != 2:
            h += 1
        if state[2] != 3:
            h += 1
        if state[3] != 8:
            h += 1
        if state[4] != 0:
            h += 1
        if state[5] != 4:
            h += 1
        if state[6] != 7:
            h += 1
        if state[7] != 6:
            h += 1
        if state[8] != 5:
            h += 1
        return h

    ### Counts the number of positions each tile is from the correct position

    def h2(self):
        total = 0
        for x in self.get_State():
            pos1 = self.get_State().index(x)
            pos2 = GOAL.index(x)
            h = abs(pos1 - pos2)
            total = total + h
        return total

    ### Counts the number of positions each tile is from their current position and multiplies them with it's value/cost

    def h3(self):
        total = 0
        for x in self.get_State():
            pos1 = self.get_State().index(x)
            pos2 = GOAL.index(x)
            h = abs(pos1 - pos2)
            total = (total + (h * x))
        return total

    ### Successor method is used for successive child states of the current state

    def generateChildren(self):

        ### Depending on the current state, the potential moves/children are split into 4
        ### Making copies of the original state

        child_1 = list(self.state)
        child_2 = list(self.state)
        child_3 = list(self.state)
        child_4 = list(self.state)
        action_1 = ""
        action_2 = ""
        action_3 = ""
        action_4 = ""
        cost_1 = 0
        cost_2 = 0
        cost_3 = 0
        cost_4 = 0
        temp_ChildList = []

        ### Move the set based on which of the index 0 is located on.

        if self.state[0] == 0:
            cost_1 = self.state[1]
            child_1[0] = child_1[1]
            child_1[1] = 0
            action_1 = "Right"
            cost_2 = self.state[3]
            child_2[0] = child_2[3]
            child_2[3] = 0
            action_2 = "Down"
        elif self.state[1] == 0:
            cost_1 = self.state[0]
            child_1[1] = child_1[0]
            child_1[0] = 0
            action_1 = "Left"
            cost_2 = self.state[2]
            child_2[1] = child_2[2]
            child_2[2] = 0
            action_2 = "Right"
            cost_3 = self.state[4]
            child_3[1] = child_3[4]
            child_3[4] = 0
            action_3 = "Down"
        elif self.state[2] == 0:
            cost_1 = self.state[1]
            child_1[2] = child_1[1]
            child_1[1] = 0
            action_1 = "Left"
            cost_2 = self.state[5]
            child_2[2] = child_2[5]
            child_2[5] = 0
            action_2 = "Down"
        elif self.state[3] == 0:
            cost_1 = self.state[0]
            child_1[3] = child_1[0]
            child_1[0] = 0
            action_1 = "Up"
            cost_2 = self.state[4]
            child_2[3] = child_2[4]
            child_2[4] = 0
            action_2 = "Right"
            cost_3 = self.state[6]
            child_3[3] = child_3[6]
            child_3[6] = 0
            action_3 = "Down"
        elif self.state[4] == 0:
            cost_1 = self.state[1]
            child_1[4] = child_1[1]
            child_1[1] = 0
            action_1 = "Up"
            cost_2 = self.state[3]
            child_2[4] = child_2[3]
            child_2[3] = 0
            action_2 = "Left"
            cost_3 = self.state[5]
            child_3[4] = child_3[5]
            child_3[5] = 0
            action_3 = "Right"
            cost_4 = self.state[7]
            child_4[4] = child_4[7]
            child_4[7] = 0
            action_4 = "Down"
        elif self.state[5] == 0:
            cost_1 = self.state[2]
            child_1[5] = child_1[2]
            child_1[2] = 0
            action_1 = "Up"
            cost_2 = self.state[4]
            child_2[5] = child_2[4]
            child_2[4] = 0
            action_2 = "Left"
            cost_3 = self.state[8]
            child_3[5] = child_3[8]
            child_3[8] = 0
            action_3 = "Down"
        elif self.state[6] == 0:
            cost_1 = self.state[3]
            child_1[6] = child_1[3]
            child_1[3] = 0
            action_1 = "Up"
            cost_2 = self.state[7]
            child_2[6] = child_2[7]
            child_2[7] = 0
            action_2 = "Right"
        elif self.state[7] == 0:
            cost_1 = self.state[4]
            child_1[7] = child_1[4]
            child_1[4] = 0
            action_1 = "Up"
            cost_2 = self.state[6]
            child_2[7] = child_2[6]
            child_2[6] = 0
            action_2 = "Right"
            cost_3 = self.state[8]
            child_3[7] = child_3[8]
            child_3[8] = 0
            action_3 = "Left"
        elif self.state[8] == 0:
            cost_1 = self.state[5]
            child_1[8] = child_1[5]
            child_1[5] = 0
            action_1 = "Up"
            cost_2 = self.state[7]
            child_2[8] = child_2[7]
            child_2[7] = 0
            action_2 = "Left"

        ### Looks at the state of 4 potential moves to see of they have changed from their original state and
        ### on the off chance that state is equal to 'Parent State', on the off chance if it is not equal to 'Original State' (or) 'Parent State'
        ### The 'Child State' is placed into a temp child list of nodes that's returned to search algorithm.

        parentDepth = self.get_Depth()
        if child_1 not in [self.state, self.parentState]:
            #### Locks the threads, so GLOBAL KEY_COUNTER doesn't seems to be a troubleshooter
            with threadLock:
                global KEY_COUNTER
                KEY_COUNTER += 1
            childNode1 = Node(KEY_COUNTER, child_1, self.key, self.state, action_1, [], parentDepth + 1, cost_1)
            temp_ChildList.append(childNode1)
        if child_2 not in [self.state, self.parentState]:
            with threadLock:
                KEY_COUNTER += 1
            childNode2 = Node(KEY_COUNTER, child_2, self.key, self.state, action_2, [], parentDepth + 1, cost_2)
            temp_ChildList.append(childNode2)
        if child_3 not in [self.state, self.parentState]:
            with threadLock:
                KEY_COUNTER += 1
            childNode3 = Node(KEY_COUNTER, child_3, self.key, self.state, action_3, [], parentDepth + 1, cost_3)
            temp_ChildList.append(childNode3)
        if child_4 not in [self.state, self.parentState]:
            with threadLock:
                KEY_COUNTER += 1
            childNode4 = Node(KEY_COUNTER, child_4, self.key, self.state, action_4, [], parentDepth + 1, cost_4)
            temp_ChildList.append(childNode4)

        ### Returns the list of child nodes
        return temp_ChildList


def sortedQueue(q):
    ### q determines the list of lists
    ### selection sort is used for the multidimensinonal list
    for i in range(len(q)):
        minimum = i
        for j in range(i + 1, len(q)):

            ### The smallest h1 value has been selected from the tuple.
            if q[j][0] < q[minimum][0]:
                minimum = j

        ### It is then added at the front.
        q[minimum], q[i] = q[i], q[minimum]
    return q


### Queue class is implemented for the FIFO utilities.
class Queue:
    def __init__(self):
        self.queue = list()

    ### adds item into the Queue.
    def push(self, data):
        self.queue.insert(0, data)

    ### returns item from the Queue
    def pop(self):
        if len(self.queue) > 0:
            return self.queue.pop()

    ### The size of the queue is checked
    def size(self):
        return len(self.queue)

    ### prints the size of queue
    def print_Queue(self):
        return self.queue


### Stack Data Structure Class for LIFO utilities.
class Stack:
    def __init__(self):
        self.items = []

    ### checks if the stack is empty (or) not
    def isEmpty(self):
        return self.items == []

    ### adds item into the stack
    def push(self, item):
        self.items.append(item)

    ### returns an item
    def pop(self):
        return self.items.pop()

    ### returns the size of stack
    def size(self):
        return len(self.items)

    def emptyStack(self):
        self.items.clear()


### Dictionary Class is used here in order to save all the searched nodes.
class Graph:
    def __init__(self):
        self.dict = {}

    def add(self, key, value):
        self.dict.update({key: value})

    def get(self, key):
        return self.dict.get(key)

    def print(self):
        print(self.dict.items())

    def emptyGraph(self):
        self.dict.clear()

    def size(self):
        return len(self.dict)


### A* use.
### Checks if the state has been noticed and then compares them with their F(n) value.
def checkReplace__VisitedNodes(nodeList, node):
    for element in nodeList:
        if element.get_State() == node.get_State():
            if (element.getHcost() + element.get_TotalPathCost()) > (node.getHcost() + node.get_TotalPathCost()):
                loc = nodeList.index(element)
                nodeList[loc] = node
            return nodeList
    return nodeList


### A* use.
### The same step is followed but it doesn't replace and only returns a boolean.
def check__VisitedNodes(nodeList, node):
    for element in nodeList:
        if element.get_State() == node.get_State():
            if (element.getHcost() + element.get_TotalPathCost()) > (node.getHcost() + node.get_TotalPathCost()):
                return True
    return False


### Now for 'Uniform Cost Search'
### Checks if the state has been noticed and then compares with the total path cost.
def replace__NodeCost(nodeList, node):
    for element in nodeList:
        if element.get_State() == node.get_State():
            if (element.getHcost() + element.get_TotalPathCost()) > (node.getHcost() + node.get_TotalPathCost()):
                loc = nodeList.index(element)
                nodeList[loc] = node
            return nodeList
    return nodeList


def st(nodeList, node):
    for element in nodeList:
        if element.get_State() == node.get_State():
            if (element.getHcost() + element.get_TotalPathCost()) > (node.getHcost() + node.get_TotalPathCost()):
                return True
    return False


### Search Algorithms

### Breadth - First Search

def bfs(startNode):
    start = time.time()
    nodesPopped = 0
    searchQueue = Queue()  # The nodes that are holded by the FIFO queue are to be searched next.
    NodeList = Graph()  # dictionary is used for all searched nodes.
    checkedStates = []  # state checking to reduce redundancy.

    ### adding the initial node to queue
    searchQueue.push(startNode)

    ### observing the current node state
    while searchQueue.size() > 0:
        node = searchQueue.pop()
        nodesPopped += 1
        print("Key:", node.get_Key(), " :  Nodes Popped:", nodesPopped, " :  State:", node.get_State(), ":  Depth:",
              node.get_Depth(), ":  Queue Size:", searchQueue.size())

        ### adds the node for checking the states and to node list
        checkedStates.append(node.get_State())
        NodeList.add(node.get_Key(), node)

        ### if node is a goal state
        if node.get_State() == GOAL:
            print("you've solved the puzzle")

            ### prints the solution path
            print_FinalPath(NodeList, node)

            ### prints and calculates total runtime.
            end = time.time()
            print("\nTotal Runtime:", end - start, "seconds")
            print("Total Nodes Popped:", nodesPopped, "\n")
            return

        ### loop for adding child states to the queue
        else:
            node.set_Visited()
            children = node.generateChildren()
            for element in children:

                ### for each child created, check if we've seen state before
                ### if the state hasn't been seen, it is then added to search queue
                if element.get_State() not in checkedStates:
                    searchQueue.push(element)


### Depth - First Search

def dfs(startNode):
    start = time.time()
    nodesPopped = 0
    s = Stack()
    visitedStates = []
    NodeList = Graph()
    s.push(startNode)

    while s.size() > 0:

        ### pops the element of LIFO
        node = s.pop()
        nodesPopped += 1
        if node.get_State() in visitedStates:
            continue
        NodeList.add(node.get_Key(), node)
        print("Key:", node.get_Key(), " :  Nodes Popped:", nodesPopped, " :  State:", node.get_State(), ":  Depth:",
              node.get_Depth(), ":  Stack Size:", s.size())

        ### the goal state is found
        if node.get_State() == GOAL:
            print("you have solved the puzzle")

            ### prints the solution path
            print_FinalPath(NodeList, node)

            ### calculates the total runtime
            end = time.time()
            print("\nTotal Runtime:", end - start, "seconds")
            print("Total Nodes Popped:", nodesPopped, "\n")
            return
        ### loop for adding child nodes to stack
        if node.get_Visited() == False:

            ### if unseen state

            if node.get_State() not in visitedStates:
                visitedStates.append(node.get_State())
                node.set_Visited()

                ### generate child with successor funtion
                children = node.generateChildren()
                node.add_Children(children)

                ### Reversed the child list as it was found on the wrong side of the tree, so this solution improved the result.
                for child in reversed(children):
                    key = child.get_Key()

                    ### The provided result seems to be more difficult
                    if key not in node.get_ChildKeys():
                        node.add_ChildKeys(key)
                        s.push(child)


### Iterative Deepening

def Iterative_Deepening(startNode):
    start = time.time()
    nodesPopped = 0
    currentDepth = 0
    maxDepth = 1  ##  sets the max dep that search is allowed to search. dfs with depth limit
    s = Stack()
    NodeList = Graph()
    while currentDepth < 1000:  ### nested while loop is implemented..
        maxDepth = maxDepth + 1
        NodeList.emptyGraph()
        s.emptyStack()
        s.push(startNode)

        while currentDepth <= maxDepth:
            if s.size() > 0:
                node = s.pop()  ### pops the last element on a list
                nodesPopped += 1
            else:
                break
            print("Key:", node.get_Key(),
                  " :  Nodes Popped:", nodesPopped,
                  " :  State:", node.get_State(),
                  ":  Depth:", node.get_Depth(),
                  ":  Stack Size:", s.size())
            NodeList.add(node.get_Key(), node)

            ### checks the goal node
            if node.get_State() == GOAL:
                print("you have solved the puzzle")

                ### prints the solution path
                print_FinalPath(NodeList, node)

                ### calculates and prints the total run time of the puzzle
                end = time.time()
                print("\nTotal Runtime:", end - start, "seconds")
                print("Total Nodes Popped:", nodesPopped, "\n")
                return


            if node.get_Visited() == True:
                continue
            NodeList.add(node.get_Key(), node)

            ### only the child within the depth limit is added.
            if node.get_Depth() < maxDepth:

                ### should be same as the dfs from here
                children = node.generateChildren()
                node.add_Children(children)
                for child in children:
                    if child.get_State() != node.get_State():

                        ### another time it is checked in order to notice the duplicating states which are redundant.
                        if child.get_State() != node.get_ParentState():
                            key = child.get_Key()
                            if key not in node.get_ChildKeys():
                                node.add_ChildKeys(key)
                                s.push(child)


### Uniform - Cost Search

### Last three algos are the same except, the Uniform Cost uses weight as cost, BFS uses only the Heuristic and A* uses both of it.
def UniformCost(startNode):
    start = time.time()
    nodesPopped = 0  ### utilized for record-keeping and for review on number of things popped of arranged line
    NodeList = Graph()  ### keeps track of the nodes that have been visited and seen
    visitedStates = []  # keeps track of states
    visitedStatesWithCost = []  # keeps track of states with their lowest cost
    costList = []  # Sorted queue based on total path cost
    costList.append([startNode.get_TotalPathCost(), startNode])

    while len(costList) > 0:
        sortedQueue(costList)
        min = costList.pop(0)  # pops first element off sorted list.
        nodesPopped += 1
        node = min[1]
        NodeList.add(node.get_Key(), node)
        print("Key:", node.get_Key(), " :  Nodes Popped:", nodesPopped, " :  State:", node.get_State(), ":  Cost:",
              node.get_TotalPathCost(), ":  Queue Size:", len(costList))

        ### Goal state is found
        if node.get_State() == GOAL:
            print("you found the goal!!")

            ### print solution path
            print_FinalPath(NodeList, node)

            ### calculates and prints runtime
            end = time.time()
            print("\nTotal Runtime:", end - start, "seconds")

            ### print("Total Nodes visited:", NodeList.size())
            print("Total Nodes Popped:", nodesPopped, "\n")
            return

        ### expansion of the node
        children = node.generateChildren()

        ### once expanded, add node to visited list
        if node.get_State() not in visitedStates:
            visitedStates.append(node.get_State())
            visitedStatesWithCost.append([node.get_TotalPathCost(), node.get_State()])
        else:
            ### compares total cost of current node and previous node of same state
            for element in visitedStatesWithCost:
                if node.get_State() == element[1]:
                    if node.get_TotalPathCost() < element[0]:
                        loc = visitedStatesWithCost.index(element)
                        visitedStatesWithCost[loc] = [node.get_TotalPathCost(), node.get_State()]

        ### add edge cost to total path cost for each child node
        for child in children:
            child.add_TotalCost(child.get_Cost(), node.get_TotalPathCost())

            ### simple add state and cost to to queue if haven't been seen before
            if child.get_State() not in visitedStates:
                costList.append([child.get_TotalPathCost(), child])
            else:

                ### compares total costs of states before adding child on queue
                for element in visitedStatesWithCost:
                    if child.get_State() == element[1]:
                        if child.get_TotalPathCost() < element[0]:
                            costList.append([child.get_TotalPathCost(), child])


### (GREEDY) Best - First Search

def Best(startNode):
    start = time.time()
    nodesPopped = 0  ### counts the number of items popped off sorted queue
    NodeList = Graph()  ### stores each node visited and seen
    bestFirst = []  ### sorted queue storing
    visitedStates = []  ### keeps track of states
    hCostList = []  ### keeps track of states with their lowest h1()
    bestFirst.append([startNode.h1(), startNode])
    while len(bestFirst) > 0:
        sortedQueue(bestFirst)
        min_h1 = bestFirst.pop(0)
        nodesPopped += 1
        node = min_h1[1]
        print("Key:", node.get_Key(), " :  Nodes Popped:", nodesPopped, " :  State:", node.get_State(), ":  Depth:",
              node.get_Depth(), ":  Queue Size:", len(bestFirst))
        NodeList.add(node.get_Key(), node)

        ### the goal node is found
        if node.h1() == 0:
            print("\nyou solved the puzzle!!\n")

            ### Prints the solution path
            print_FinalPath(NodeList, node)

            ### It calculates and prints the total runtime.
            end = time.time()
            print("\nTotal Runtime:", end - start, "seconds")
            print("Total Nodes Popped:", nodesPopped, "\n")
            return

        children = node.generateChildren()
        if node.get_State() not in visitedStates:
            visitedStates.append(node.get_State())
            hCostList.append([node.h1(), node.get_State()])
        else:
            for element in hCostList:
                if node.get_State() == element[1]:
                    if node.h1() < element[0]:
                        loc = hCostList.index(element)
                        hCostList[loc] = [node.h1(), node.get_State()]
        for child in children:
            if child.get_State() not in visitedStates:
                element = [child.h1(), child]
                child.set_Hcost(child.h1())
                bestFirst.append(element)
            else:
                ### compares the total costs of states before adding child on queue
                for element in hCostList:
                    if child.get_State() == element[1]:
                        if child.get_TotalPathCost() < element[0]:
                            bestFirst.append([child.h1(), child])


### A* SEARCH

def A_STAR(startNode, h):
    nodesPopped = 0  ### counter for items popped off front of sorted list.
    start = time.time()  ### start calculating execution time
    NodeList = Graph()
    pathScores = []
    visitedNodes = []  # [node.get_State()] - used to compare f(n) of nodes with same state
    visitedStates = []  # [node.get_State()] - allows for quicker checking to see if node has been seen or not

    ### depending on which heuristic you pick in user interface
    if h == 1:
        ascore = startNode.get_TotalPathCost() + startNode.h1()
    if h == 2:
        ascore = startNode.get_TotalPathCost() + startNode.h2()
    if h == 3:
        ascore = startNode.get_TotalPathCost() + startNode.h3()

    pathScores.append([ascore, startNode])  ### f(n) = total path cost + heuristic cost of that node

    while len(pathScores) > 0:
        sortedQueue(pathScores)
        min = pathScores.pop(0)
        nodesPopped += 1
        node = min[1]
        NodeList.add(node.get_Key(), node)
        print("Key:", node.get_Key(), " :  Nodes Popped:", nodesPopped, " :  State:", node.get_State(), ":  Depth:",
              node.get_Depth(), ":  Queue Size:", len(pathScores))

        ### The goal node is found
        if node.get_State() == GOAL:
            print("\nyou solved the puzzle!!\n")

            ### prints the solution path
            print_FinalPath(NodeList, node)

            ### calculates and prints the total runtime
            end = time.time()
            print("\nTotal Runtime:", end - start, "seconds")
            print("Total Nodes Popped:", nodesPopped, "\n")
            return

        #### checks if the state is in visited states and replaces it with if f(n) is lower
        visitedNodes = checkReplace__VisitedNodes(visitedNodes, node)
        visitedStates.append(node.get_State())
        children = node.generateChildren()

        ### changes with flag inputed via user interface
        for child in children:
            if h == 1:
                h_cost = child.h1()
            if h == 2:
                h_cost = child.h2()
            if h == 3:
                h_cost = child.h3()
            child.set_Hcost(h_cost)
            if child.get_State() not in visitedStates:
                child.add_TotalCost(child.get_Cost(), node.get_TotalPathCost())
                pathScores.append([child.get_TotalPathCost() + h_cost, child])
            else:
                if check__VisitedNodes(visitedNodes, child):
                    child.add_TotalCost(child.get_Cost())
                    pathScores.append([child.get_TotalPathCost() + h_cost, child])


### User Interface Node

while True:
    difficulty = input("Choose a difficulty (Enter the corresponding number):\n1). EASY\n2). MEDIUM\n3). HARD\n")
    if difficulty == "1":
        startNode = Node(1, EASY, 0, [], "Start", [], 1, 0)
        break
    if difficulty == "2":
        startNode = Node(1, MEDIUM, 0, [], "Start", [], 1, 0)
        break
    if difficulty == "3":
        startNode = Node(1, HARD, 0, [], "Start", [], 1, 0)
        break

while True:
    userInput = input(
        "What search algorithm do you wish to run?\n1). BREATH-FIRST SEARCH\n2). DEPTH-FIRST SEARCH\n3). ITERATIVE DEEPENING\n4). UNIFORM COST SEARCH\n5). (GREEDY) BEST-FIRST SEARCH\n6). A* SEARCH\n")
    if userInput == "1":
        bfs(startNode)
        break
    if userInput == "2":
        dfs(startNode)
        break
    if userInput == "3":
        Iterative_Deepening(startNode)
        break
    if userInput == "4":
        UniformCost(startNode)
        break
    if userInput == "5":
        Best(startNode)
        break
    if userInput == "6":
        while True:
            hValue = input(
                "Which Heuristic do you want to run with A* Search?\n1). HEURISTIC #1 \n2). HEURISTIC #2\n3). HEURISTIC #3\n")
            if hValue == "1":
                A_STAR(startNode, 1)
                break
            if hValue == "2":
                A_STAR(startNode, 2)
                break
            if hValue == "3":
                A_STAR(startNode, 3)
                break
    if hValue in ["1", "2", "3"]:
        break
