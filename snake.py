#snake
from __future__ import print_function
import random, keyboard, time, copy, colorama

colorama.init()
cursor_up = lambda lines: '\x1b[{0}A'.format(lines)
cursor_down = lambda lines: '\x1b[{0}B'.format(lines)

board = []
snakebody = '■'
apple = 'O'
nullcords = []
boardcords = []
for i in range(27):
    row = []
    for j in range(36):
        row.append('.')
        boardcords.append([i, j])
    board.append(row)
applecords = []
bodycords = [[7, 7]]
movedir = 'd'
points = 0
timeperiod = 0.05
board[7][7] = snakebody

def display():
    global board
    bstring = ''
    for row in board:
        rstring = ' '.join(row)
        bstring += rstring + '\n'
    print(bstring)
    print(cursor_up(31))

def spawn():
    global nullcords, applecords, board, boardcords, bodycords
    nullcords = []
    for i in boardcords:
        if i not in bodycords and i not in applecords:
            nullcords.append(i)
            
    newspawn = random.choice(nullcords)
    nullcords.remove(newspawn)
    applecords.append(newspawn)
    board[newspawn[0]][newspawn[1]] = apple

def move():
    global movedir, board, nullcords, applecords, bodycords, points, timeperiod
    cell = copy.deepcopy(bodycords[0])
    row = cell[0]
    col = cell[1]
    board[row][col] = '.'

    if movedir == 'd':
        bodycords[0][1] += 1
        
    elif movedir == 'a':
        bodycords[0][1] -= 1
        if bodycords[0][1] < 0:
            return True
        
    elif movedir == 's':
        bodycords[0][0] += 1

    elif movedir == 'w':
        bodycords[0][0] -= 1
        if bodycords[0][0] < 0:
            return True
        
    if bodycords.count(bodycords[0]) > 1:
        return True
    
    if cell in applecords:
        applecords.remove(cell)
        nullcords.append(cell)
        points += 1
        bodycords.insert(1, cell)
        spawn()
        board[row][col] = snakebody
              
    else:
        for part in range(1, len(bodycords)):
            bodycords[part], cell = cell, bodycords[part]
            board[bodycords[part][0]][bodycords[part][1]] = snakebody
        board[cell[0]][cell[1]] = '.'
    try:
        board[bodycords[0][0]][bodycords[0][1]] = snakebody
    except IndexError:
        return True
  
    display()

def pause():
    time.sleep(1)
    while True:
        try:
            if keyboard.is_pressed('space'):
                break
        except:
            break
        
spawn()
spawn()
spawn()
while True:
    if move():
        break
    time.sleep(timeperiod)
    try:  
        if keyboard.is_pressed('w') and movedir != 's':
            movedir = 'w'
        elif keyboard.is_pressed('a') and movedir != 'd': 
            movedir = 'a'
        elif keyboard.is_pressed('s') and movedir != 'w': 
            movedir = 's'
        elif keyboard.is_pressed('d') and movedir != 'a': 
            movedir = 'd'
        elif keyboard.is_pressed('space'):
            pause()
            
    except:
        break
print(cursor_down(31))
print('GAME OVER')
print('POINTS :', points)
