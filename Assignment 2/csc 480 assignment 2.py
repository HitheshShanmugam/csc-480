import random, copy, threading, time

lock = threading.Lock()

GAME_BOARD = []  ### Implemented a Multidimensional Array that stores the edges.
MOVE_TABLE = []  ### Implemented a Multidimensional and the table of moves are done.
MOVES_LEFT = []  ### The remaining number of edges in a game board.
USER_SCORE = 0
COMP_SCORE = 0
CHOSEN_DEPTH = 0
CHILD_KEY_COUNTER = 1
PLAYER_ONE = 1
PLAYER_TWO = 2
CURRENT_PLAYER = 1  ### Chages are done in order to find the player playing the MOVE_TABLE
PLAYED_MOVES = []


def get_Player():
    global CURRENT_PLAYER
    return CURRENT_PLAYER


### The MOVE is removed and also the MOVES_LEFT is updated

def add_Move(key):
    global GAME_BOARD
    global MOVES_LEFT
    for row in GAME_BOARD:
        for col in row:
            if col == key:
                if key in MOVES_LEFT:
                    row_Index = GAME_BOARD.index(row)
                    col_Index = row.index(col)
                    MOVES_LEFT.remove(key)
                    global MOVE_TABLE
                    MOVE_TABLE[row_Index][col_Index] = get_Player()
                    change_Player()


### The Boolean check for IF MOVE is returned

def isMove_Available(int):
    global MOVES_LEFT
    for x in MOVES_LEFT:
        if x == int:
            return True
    return False


### Checks to see if the Play has been done in IF MOVE_TABLE[x][y] or it returns Boolean

def isIndex_Available(x, y):
    global MOVE_TABLE
    if MOVE_TABLE[x][y] > 0:
        return True
    else:
        return False


### Every player is changed after a MOVE. So, MOVE_TABLE is implemented with the proper player indicator

def change_Player():
    global CURRENT_PLAYER
    global PLAYER_ONE
    global PLAYER_TWO
    if CURRENT_PLAYER == PLAYER_ONE:
        CURRENT_PLAYER = 2
    else:
        CURRENT_PLAYER = 1


### After determining the desired width the original gameboard is initialized

def create_Board(size):
    global lock
    lock.acquire(True)
    dotDash = 1
    NewLineOffSet = 1
    global GAME_BOARD
    global MOVE_TABLE
    GAME_BOARD = []
    MOVE_TABLE = []
    for x in range(2 * size + 1):
        GAME_BOARD.append([])
        MOVE_TABLE.append([])
        for y in range(2 * size + 1):
            if NewLineOffSet % 2 != 0:
                if x % 2 == 0:
                    print("+".center(5), end='')
                    GAME_BOARD[x].append(0)
                    MOVE_TABLE[x].append(0)
                else:
                    GAME_BOARD[x].append(random.randint(1, 5))
                    MOVE_TABLE[x].append(0)
                    print(str(GAME_BOARD[x][y]).center(5), end='')
            else:
                GAME_BOARD[x].append(dotDash)
                MOVE_TABLE[x].append(0)
                print('{:^5d}'.format(dotDash), end="")
                dotDash += 1
            NewLineOffSet += 1
        print("\n")
    global MOVES_LEFT
    MOVES_LEFT = list(range(1, int(GAME_BOARD[-1][-2]) + 1))
    lock.release()


### The updated moves are printed in  the game board which already exists

def print_GameBoard():
    global lock
    lock.acquire(True)
    global GAME_BOARD
    global MOVE_TABLE
    dot = "+"
    vertDash = "|"
    horDash = "--"
    xIndex = 0
    for x in GAME_BOARD:
        index = 0
        for y in x:
            if y == 0:  ### Prints the Dots
                print('{:^5s}'.format(dot), end="")
                continue
            if GAME_BOARD.index(x) % 2 != 0:
                if index % 2 != 0:
                    print('{:^5d}'.format(y), end="")  ### Prints the box points
                else:  ### Prints the vertical edge

                    ### global MOVES_LEFT

                    if y in MOVES_LEFT:
                        print('{:^5d}'.format(y), end="")  ### Prints the not used vertical edges
                    else:
                        print('{:^5s}'.format(vertDash), end="")  ### Prints the used vertical edges
            else:  ### Prints the horizontal edges

                ###global MOVES_LEFT

                if y in MOVES_LEFT:
                    print('{:^5d}'.format(y), end="")  ### Prints the not used horizontal edges
                else:
                    print('{:^5s}'.format(horDash), end="")  ### Prints the used vertical edges
            index += 1
        xIndex += 1
        print("\n")
    lock.release()


### Score Keeping, Checks for the IF MOVE to complete a square and then returns the points that are scored

def number_Of_Sides(edgeKey, moveTable):
    global GAME_BOARD
    global BOX_KEY_COUNTER
    points = 0
    boxBelow = []
    boxAbove = []
    boxRight = []
    boxLeft = []
    if isMove_Available(edgeKey):
        coordinates = get_Coordinates(edgeKey)
        row = coordinates[0]
        col = coordinates[1]

        if row == 0:  ### Below the Box
            if moveTable[row + 1][col - 1] > 0:
                coordinates = [row + 1, col - 1]
                boxBelow.append(coordinates)
            if moveTable[row + 1][col + 1] > 0:
                coordinates = [row + 1, col + 1]
                boxBelow.append(coordinates)
            if moveTable[row + 2][col] > 0:
                coordinates = [row + 2, col]
                boxBelow.append(coordinates)

        if col == 0:  ### Right of the Box
            if moveTable[row - 1][col + 1] > 0:
                coordinates = [row - 1, col + 1]
                boxRight.append(coordinates)
            if moveTable[row][col + 2] > 0:
                coordinates = [row, col + 2]
                boxRight.append(coordinates)
            if moveTable[row + 1][col + 1] > 0:
                coordinates = [row + 1, col + 1]
                boxRight.append(coordinates)

        if row == (len(GAME_BOARD) - 1):  ### Above the box
            if moveTable[row - 1][col - 1] > 0:
                coordinates = [row - 1, col - 1]
                boxAbove.append(coordinates)
            if moveTable[row - 1][col + 1] > 0:
                coordinates = [row - 1, col + 1]
                boxAbove.append(coordinates)
            if moveTable[row - 2][col] > 0:
                coordinates = [row - 2, col]
                boxAbove.append(coordinates)

        if col == (len(GAME_BOARD) - 1):  ### Left of the box
            if moveTable[row - 1][col - 1] > 0:
                coordinates = [row - 1, col - 1]
                boxLeft.append(coordinates)
            if moveTable[row][col - 2] > 0:
                coordinates = [row, col - 2]
                boxLeft.append(coordinates)
            if moveTable[row + 1][col - 1] > 0:
                coordinates = [row + 1, col - 1]
                boxLeft.append(coordinates)

        if row % 2 != 0 and col > 0 and col < (len(GAME_BOARD) - 1):  ### Checks for the Left and Right Boxes

            ### Left
            if moveTable[row - 1][col - 1] > 0:
                coordinates = [row - 1, col - 1]
                boxLeft.append(coordinates)
            if moveTable[row][col - 2] > 0:
                coordinates = [row, col - 2]
                boxLeft.append(coordinates)
            if moveTable[row + 1][col - 1] > 0:
                coordinates = [row + 1, col - 1]
                boxLeft.append(coordinates)

            ### Right
            if moveTable[row - 1][col + 1] > 0:
                coordinates = [row - 1, col + 1]
                boxRight.append(coordinates)
            if moveTable[row][col + 2] > 0:
                coordinates = [row, col + 2]
                boxRight.append(coordinates)
            if moveTable[row + 1][col + 1] > 0:
                coordinates = [row + 1, col + 1]
                boxRight.append(coordinates)

        if row % 2 == 0 and row > 0 and row < (len(GAME_BOARD) - 1):

            ### Below
            if moveTable[row + 1][col - 1] > 0:
                coordinates = [row + 1, col - 1]
                boxBelow.append(coordinates)
            if moveTable[row + 1][col + 1] > 0:
                coordinates = [row + 1, col + 1]
                boxBelow.append(coordinates)
            if moveTable[row + 2][col] > 0:
                coordinates = [row + 2, col]
                boxBelow.append(coordinates)

            ### Above
            if moveTable[row - 1][col - 1] > 0:
                coordinates = [row - 1, col - 1]
                boxAbove.append(coordinates)
            if moveTable[row - 1][col + 1] > 0:
                coordinates = [row - 1, col + 1]
                boxAbove.append(coordinates)
            if moveTable[row - 2][col] > 0:
                coordinates = [row - 2, col]
                boxAbove.append(coordinates)

        if len(boxBelow) == 3:
            points += GAME_BOARD[row + 1][col]
        if len(boxAbove) == 3:
            points += GAME_BOARD[row - 1][col]
        if len(boxRight) == 3:
            points += GAME_BOARD[row][col + 1]
        if len(boxLeft) == 3:
            points += GAME_BOARD[row][col - 1]
    return points


### The coordinates for the specific move key is returned

def get_Coordinates(edgeKey):
    global GAME_BOARD
    coordinates = []
    xindex = 0
    yindex = 0
    for x in GAME_BOARD:
        yindex = 0
        for y in x:
            if y == edgeKey:
                row = xindex
                col = yindex
                if row % 2 != 0 and col % 2 == 0:
                    coordinates.append(row)
                    coordinates.append(col)
                if row % 2 == 0 and col % 2 != 0:
                    coordinates.append(row)
                    coordinates.append(col)
            yindex += 1
        xindex += 1
    return coordinates


### Mini-Max Algorithm Begins

### The class for children game states

class Child:
    key = 0  ### basic identifier
    type = ""  ### max or min node
    moves_Left = []  ### moves left at the time of this move being played
    moves_Table = []  ### move locations, used for scoring
    parent_Key = 0  ### don't really need this.
    childKeys = []  ### don't really need this.
    childStates = []  ### don't really need this.
    move = 0  ### move key
    coordinates = []  ### used for scoring. location on move table
    points = 0  ### scoring method store
    depth = 0  ### ply number.
    total_Points = 0  ### where evaluation function is added to for terminal nodes

    def __init__(self, key, moves_Left, moves_Table, type, parent_Key, move, coordinates, points, total_Points, depth):
        self.key = key
        self.moves_Left = moves_Left
        self.moves_Table = moves_Table
        self.type = type  ### MAX/MIN
        self.parent_Key = parent_Key
        self.move = move  ###edge
        self.coordinates = coordinates
        self.points = points
        self.total_Points = total_Points
        self.depth = depth

    def get_Key(self):
        return self.key

    def get_Moves_Left(self):
        return self.moves_Left

    def get_Moves_Table(self):
        return self.moves_Table

    def getparent_Key(self):
        return self.parent_Key

    def get_Move(self):
        return self.move

    def get_Points(self):
        return self.points

    def gettotal_Points(self):
        return self.total_Points

    def get_Depth(self):
        return self.depth

    def get_Type(self):
        return self.type

    def get_Children(self):
        return self.childStates

    def set_total_Points(self, points):
        self.total_Points += points


def generate_Children(parentNode):
    global CHILD_KEY_COUNTER
    listOfChildren = []
    for move in parentNode.get_Moves_Left():  ### For each move
        childMoveTable = copy.deepcopy(parentNode.get_Moves_Table())  ### Creates a grid of moves
        coordinates = get_Coordinates(move)  ### The location of edge on board is returned
        if len(coordinates) > 0:
            if childMoveTable[coordinates[0]][coordinates[1]] == 0:  ### If the edge has not been played before
                points = number_Of_Sides(move,
                                         parentNode.get_Moves_Table())  ### Checks to see if this specific move results in any of the points
                childmoves_Left = copy.deepcopy(parentNode.get_Moves_Left())  ### Copies the left out list of moves
                childmoves_Left.remove(move)

                ### Scoring happens in nodes

                if parentNode.get_Type() == "MAX":  ### If this is considered to be a max node, then make the min nodes
                    childMoveTable[coordinates[0]][coordinates[1]] = 2
                    child = Child(CHILD_KEY_COUNTER, childmoves_Left, childMoveTable, "MIN",
                                  copy.deepcopy(parentNode.get_Key()), move, coordinates, points,
                                  (copy.deepcopy(parentNode.gettotal_Points()) + points),
                                  (copy.deepcopy(parentNode.get_Depth()) + 1))
                    listOfChildren.append(child)

                if parentNode.get_Type() == "MIN":  ### If this is considered to be a min node, then make the max nodes
                    childMoveTable[coordinates[0]][coordinates[1]] = 1
                    child = Child(CHILD_KEY_COUNTER, childmoves_Left, childMoveTable, "MAX", parentNode.get_Key(), move,
                                  coordinates, points, (copy.deepcopy(parentNode.gettotal_Points()) - points),
                                  (copy.deepcopy(parentNode.get_Depth()) + 1))
                    listOfChildren.append(child)
                CHILD_KEY_COUNTER += 1
    return listOfChildren  ### Parent Node list is implemented for children


def mini_max(node, alpha, beta):
    global CHOSEN_DEPTH
    if node.get_Depth() == ((2 * CHOSEN_DEPTH) - 1) or len(node.get_Moves_Left()) == 0:  ### Checks to see if there are more moves available (or) if the node is at the terminal depth
        return [node.get_Move(),
                node.gettotal_Points()]  ### Returns the f(n) of terminal node with the respective moves
    if node.get_Type() == "MAX":
        listofChildren = generate_Children(node)  ### Generates the children board states
        maxValue = -999999  ### The smallest value of code is set
        maxMove = 0  ### The move key is initialized

        ### Child States is iterated

        for child in listofChildren:
            valueNode = mini_max(child, alpha, beta)  ### The rest of the same depth nodesa re compared before the recursive goes down
            if valueNode[1] > maxValue:  ### The value of children with highest have been been updated
                maxMove = child.get_Move()  ### The max value node of the move has been updated
                maxValue = valueNode[1]
                node.set_total_Points(child.gettotal_Points())  ### The max value is passed to the parent
            alpha = max(alpha, valueNode[1])  ###  Alpha Beta Pruning is implemented
            if alpha >= beta:
                break  ### The leaf node below has been cut-off
        return [maxMove, maxValue]  ### Returns the max value with its move and value

    ### Same algorithm follows for the below problem instead with max it's min

    if node.get_Type() == "MIN":
        listofChildren = generate_Children(node)
        minValue = 999999
        minMove = 0
        for child in listofChildren:
            valueNode = mini_max(child, alpha, beta)
            if valueNode[1] < minValue:
                minMove = child.get_Move()
                minValue = valueNode[1]
                node.set_total_Points(child.gettotal_Points())
            beta = min(beta, valueNode[1])
            if alpha >= beta:
                break
        return [minMove, minValue]


### User Interface Algorithm Begins

BoardSize = input("Enter the Size of the Board\n")
size = int(BoardSize)
print("\n\nRepresentation of the GameBoard:")
print("-" * size * 15)
create_Board(size)
print("\n")
chosenDepth = input("Enter the number of plys for the computer to calculate?\n")
CHOSEN_DEPTH = int(chosenDepth)

while (len(MOVES_LEFT) > 0):
    CHILD_KEY_COUNTER = 1
    print("\n")
    print_GameBoard()
    print("-" * size * 15)
    print("Player Score: ", USER_SCORE, "\nComputer Score: ", COMP_SCORE)
    print("")
    print("Moves Played - ", PLAYED_MOVES)
    print("")
    userInput = 0
    while True:
        userInput = int(input("What move do you want to play?\n"))
        if userInput in MOVES_LEFT:
            break
    userPoints = number_Of_Sides(userInput, MOVE_TABLE)
    print("\nYou played move number {}, and scored {} points\n".format(userInput, userPoints))
    add_Move(userInput)
    USER_SCORE += userPoints
    PLAYED_MOVES.append(userInput)
    if (len(MOVES_LEFT) == 0):
        print_GameBoard()
        break
    else:
        print("Calculating Computer Move....\n")
        node = Child(0, MOVES_LEFT, MOVE_TABLE, "MAX", 0, 0, [], 0, 0, 0)
        start = time.time()
        results = mini_max(node, -999999, 999999)
        if results[0] not in MOVES_LEFT:
            results[0] = random.choice(MOVES_LEFT)
        pointsScored = number_Of_Sides(results[0], MOVE_TABLE)
        end = time.time()
        print("\nThe Computer played move number {}, and scored {} points \n(Calculation took {} seconds)".format(
            results[0], pointsScored, end - start))
        add_Move(results[0])
        COMP_SCORE += pointsScored
        PLAYED_MOVES.append(results[0])
    if len(MOVES_LEFT) == 0:
        break
print("")
print("")
print_GameBoard()
if COMP_SCORE > USER_SCORE:
    print("The Computer Won!\n\nThe final score was")
    print("Player Score: ", USER_SCORE, "\nComputer Score: ", COMP_SCORE)
else:
    print("You won the game against the Computer!\n\nThe final score was")
    print("Player Score: ", USER_SCORE, "\nComputer Score: ", COMP_SCORE)
