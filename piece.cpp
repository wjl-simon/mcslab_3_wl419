#include "piece.h"
#include "helper.h"
#include <cmath>

/*===== Base Class =====*/

Piece::Piece(PieceType type, bool color, int rank, int file):
  TYPE(type), COLOR(color), currentRank(rank), currentFile(file){}



/**
 * Test if the destination of a move is in of the board, true if so
 */
bool Piece::isInside(int rank, int file) const
{
  if(rank < 0|| rank > BOARD_SIZE-1 || file < 0 || file > BOARD_SIZE-1) return false;
  else return true;
}



/**
 * Test if the destination of a move has opponent's pieces (hostile piece), returns true if so
 */
bool Piece::isDestHostile(int const RANK_D, int const FILE_D, Piece*** const board) const
{
  if(!board[RANK_D][FILE_D]) return false;
  else if(board[RANK_D][FILE_D]->getColor() != COLOR) return true;
  else return false;
}



/*
 * Return the type of this piece 
 */
PieceType Piece::getType() const { return TYPE; }



/*
 * Return the colour of this piece 
 */
bool Piece::getColor() const { return COLOR; }



/**
 * Return the position of this piece (in char*, e.g. A1, C3)
 */
std::string Piece::getPos() const
{
  std::string c = std::to_string(currentFile+'A') + std::to_string(currentRank+'1');
  return c;
}



/**
 * Directly set the position of a piece to the target position, regardless of the rules
 */
void Piece::setPos(int const targetRank, int const targetFile)
{ currentRank =  targetRank; currentFile = targetFile; }



Piece::~Piece(){}


/*===== King =====*/

King::King(bool color, int rank, int file): Piece(KING,color,rank,file){}

bool King::movePieceRuleTest(int const RANK_D, int const FILE_D, Piece*** const board) const
{
  // The current coordinate
  int const RANK_S = currentRank; int const FILE_S = currentFile;
  
  // Test if the destination of a move is in of the board
  if(isInside(RANK_D,FILE_D) == false) return false;

  // Test if the destination has own side's piece
  if(!isDestHostile(RANK_D,FILE_D,board)) return false;

  // Test if the destination follows the rule:
  // a king could only make one quare of move
  if(RANK_D <= RANK_S+1 && RANK_D >= RANK_S-1)
  {
    if(FILE_D <= FILE_S+1 && FILE_D >= FILE_S+1)
    {
      if(RANK_D != RANK_S && FILE_D != FILE_S) // mustn't remain where it was
        return true;
    }
  }
  return false;
}


King::~King(){}


/*===== Queen =====*/

Queen::Queen(bool color, int rank, int file): Piece(QUEEN,color,rank,file){}


bool Queen::movePieceRuleTest(int const RANK_D, int const FILE_D, Piece*** const board) const
{
  // The current coordinate
  int const RANK_S = currentRank; int const FILE_S = currentFile;
  
  // Test if the destination of a move is in of the board
  if(isInside(RANK_D,FILE_D) == false) return false;
  
  // Test if the desttination has own side's piece
  if(!isDestHostile(RANK_D,FILE_D,board)) return false;
  
  // Test if the destination follows the rule:
  // a queen combines the power of rook and bishop
  return (bishopMove(RANK_D,FILE_D,RANK_S,FILE_S,board) ||
          rookMove(RANK_D,FILE_D,RANK_S,FILE_S,board));

}


Queen::~Queen(){}



/*===== Rook =====*/

Rook::Rook(bool color, int rank, int file): Piece(ROOK,color,rank, file){}


bool Rook::movePieceRuleTest(int const RANK_D, int const FILE_D, Piece*** const board) const
{
  // The current coordinate
  int const RANK_S = currentRank; int const FILE_S = currentFile;
  
  // Test if the destination of a move is in of the board
  if(isInside(RANK_D,FILE_D) == false) return false;

  // Test if the destination has own side's piece
  if(!isDestHostile(RANK_D,FILE_D,board)) return false;

  // Test if the destination follows the rule of moving a rook
  return rookMove(RANK_D,FILE_D,RANK_S,FILE_S,board);
}


Rook::~Rook(){}


/*===== Bishop =====*/

Bishop::Bishop(bool color, int rank, int file): Piece(BISHOP,color,rank,file){}

bool Bishop::movePieceRuleTest(int const RANK_D, int const FILE_D, Piece*** const board) const
{
  int const RANK_S = currentRank; int const FILE_S = currentFile;

  // Test if the destination of a move is in of the board
  if(isInside(RANK_D,FILE_D) == false) return false;
  
  // Test if the destination has own side's piece
  if(!isDestHostile(RANK_D,FILE_D,board)) return false;
  
  // Test if the destination follows the rule of moving a bishop
  return bishopMove(RANK_D,FILE_D,RANK_S,FILE_S,board);
}


Bishop::~Bishop(){}



/*===== Knight =====*/

Knight::Knight(bool color, int rank, int file): Piece(KNIGHT,color,rank,file){}


bool Knight::movePieceRuleTest(int const RANK_D, int const FILE_D, Piece*** const board) const
{
  // The current coordinate
  int const RANK_S = currentRank; int const FILE_S = currentFile;

  // Test if the destination of a move is in of the board
  if(isInside(RANK_D,FILE_D) == false) return false;

  // Test if the destination has own side's piece
  if(!isDestHostile(RANK_D,FILE_D,board)) return false;
  
  // Test if the destination follows the rule of moving a knight
  if(abs(RANK_D-RANK_S)==1 && abs(FILE_D-FILE_S)==2) return true; // 1 vertically and 2 horizontally

  if(abs(RANK_D-RANK_S)==2 && abs(FILE_D-FILE_S)==1) return true; // 1 horizontally and 2 vertically
  
  return false; // illegal move!
}


Knight::~Knight(){}



/*===== Pawn =====*/

Pawn::Pawn(bool color, int rank, int file): Piece(PAWN,color,rank,file), moveTimes(0){}

/**
 * This function could only test but cannot capture a piece nor make a move
 */
bool Pawn::movePieceRuleTest(int const RANK_D, int const FILE_D, Piece*** const board) const
{
  // The current coordinate
  int const RANK_S = currentRank; int const FILE_S = currentFile;
  
  // Test if the destination of a move is in of the board
  if(isInside(RANK_D,FILE_D) == false) return false;

  // NB:1st move of this piece: can move 1 or 2 squares upwards, or capture pieces
  //--- WHITE PAWN MOVING
  if(COLOR == WHITE)
  {
    // Test if moving 2 squres forwards
    if(RANK_D-RANK_S == 2 && FILE_D == FILE_S)
    {
      if(moveTimes == 0) // it's the first move
      {
        for(int i = 1; i <= RANK_D-RANK_S; i++) // test if the squares forward are empty
          if(board[RANK_S+i][FILE_S] != nullptr) return false;

        //commitFirstMove = true; // first move of this pawn has been made
        return true;
      }
      else return false; // non-first move can't move 2 squres forwards
    }

    // Test if moving 1 squre forwards
    if(RANK_D-RANK_S == 1 && FILE_D == FILE_S)
    {
      if(board[RANK_D][FILE_D] == nullptr) // test if the square forward is empty
      {
        //if(!commitFirstMove) commitFirstMove = true; // it's the first move

        return true;
      }
      else return false;
    }

    // Test if capturing the opponent's pieces
    if(RANK_D-RANK_S == 1 && abs(FILE_D-FILE_S) == 1)
    {
      if(isDestHostile(RANK_D,FILE_D,board)) // test if the detination has opponent's pieces
      {
        //if(!commitFirstMove) commitFirstMove = true; // it's the first move

        return true;
      }
      else return false;
    }
   }

  //--- BLACK PAWN MOVING
  if(COLOR == BLACK)
  {
    if(RANK_S-RANK_D == 2 && FILE_D == FILE_S) // test if Moving 2 squres forwards
    {
      if(moveTimes == 0) // it's the first move
      {
        for(int i = 1; i <= RANK_S-RANK_D; i++) // test if the square forward is empty
          if(board[RANK_S-i][FILE_S] != nullptr) return false;

        //commitFirstMove = true; // first move of this pawn has been made
        return true;
      }
      else return false; // non-first move can't move for 2 squres
    }

    // Test if moving 1 squre forwards
    if(RANK_S-RANK_D == 1 && FILE_D == FILE_S)
    {
      if(board[RANK_D][FILE_D] == nullptr) // test if the square forward is empty
      {
        //if(!commitFirstMove) commitFirstMove = true; // it's the first move

        return true;
      }
      else return false;
    }

    // Test if Capturing the opponent's pieces
    if(RANK_S-RANK_D == 1 && abs(FILE_D-FILE_S) == 1) 
    {
      if(isDestHostile(RANK_D,FILE_D,board))  // test if the detination has opponent's pieces
      {
        //if(!commitFirstMove) commitFirstMove = true; // it's the first move

        return true;
      }
      else return false;
    }
  }
  
  // Illgal move
  return false;
}




/**
 * Increment the Pawn's moving times
 */
void Pawn::incCount(){ ++moveTimes; }



/**
 * Increment the Pawn's moving times
 */
void Pawn::decCount() { --moveTimes; }


Pawn::~Pawn(){}
