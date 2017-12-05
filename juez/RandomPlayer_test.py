from LogicalBoard import *
from random import randint, random
import os


class RandomPlayer_test:

    def __init__(self, color, oponent_color, columns, rows, c, p, probability):
        self.color = color
        self.oponent_color = oponent_color
        self.logicalBoard = LogicalBoard(columns, rows, c, p)
        self.probability = probability

        if not os.path.isdir('log'):
            os.mkdir('log')
        self.log_file = open('log/random_player_' + self.color + '.log', 'a')

    def move(self, last_move=None):
        if last_move is not None:
            self.log("He moved: " + str(last_move))
            self.logicalBoard.makeMove(self.oponent_color, last_move)

        move = None
        inteligente = False
        if random() < self.probability:
          inteligente = True
          
        if inteligente:
            move = self.get_win_move(self.color)
            if move is None:
                move = self.get_win_move(self.oponent_color)
        
        if move is None:
            aux = []
            while True:
                move = randint(0, self.logicalBoard.columns - 1)
                if(move in aux and len(aux)<self.logicalBoard.columns):
                    continue
                
                aux.append(move)

                if self.logicalBoard.isValidMove(move):

                    self.logicalBoard.makeMove(self.color, move)
                    puedeganar = self.get_win_move(self.oponent_color)
                    self.logicalBoard.undoMove(move)
                    
                    if puedeganar is None:
                      break
                    
                    if len(aux)>=self.logicalBoard.columns:
                      break
                    
                    if not inteligente:
                      break


        self.log("I moved: " + str(move))
        self.logicalBoard.makeMove(self.color, move)
        return move

    def log(self, msg):
        self.log_file.write(str(msg) + "\n")
        self.log_file.flush()

    def get_win_move(self, player_color):

        for i in range(self.logicalBoard.columns):
            if self.logicalBoard.isValidMove(i):
              self.logicalBoard.makeMove(player_color, i)
              if self.logicalBoard.isWinner(player_color):
                  self.logicalBoard.undoMove(i)
                  return i
              self.logicalBoard.undoMove(i)
        return None
