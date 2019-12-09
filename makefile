CXXFLAGS = -g -Wall -Wextra

OBJ = ChessBoard.o piece.o #helper.o errors.o

chess: ChessMain.cpp $(OBJ:.o=.h) $(OBJ:.o=.cpp) helper.h
	g++ $(CXXFLAGS) ChessMain.cpp $(OBJ:.o=.h) $(OBJ:.o=.cpp) helper.h -o $@
