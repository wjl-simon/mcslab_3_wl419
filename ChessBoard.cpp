#include "piece.h"
#include "ChessBoard.h"
#include <iostream>
//#include <cstring>
#include "helper.h"

using namespace std;

// The number of pieces at the beginning for each side
const int ChessBoard::NUM_P = 16;


ChessBoard::ChessBoard():board(nullptr),whitePieces(nullptr),blackPieces(nullptr),
                         moveTurn(WHITE),gameOver(false)
{
  setupBoard(); // set up a chess board
}



/**                                                                                                
 * Set up a chess board. Called by the constructor or the reset() only.
 * Make sure board, whitePieces and blackPieces are all nullptr when calling 
 */                                                                                                
void ChessBoard::setupBoard()
{
  // Make an empty board first
  board = new Piece** [BOARD_SIZE];
  for(int i = 0; i < BOARD_SIZE; i++)
    board[i] = new Piece* [BOARD_SIZE];
      
  for(int i = 0; i < BOARD_SIZE; i++)
    for(int j = 0; j < BOARD_SIZE; j++)
      board[i][j] = nullptr;

  // Generate the pieces and populate the chess board accordingly
  whitePieces = new Piece*[NUM_P], blackPieces = new Piece*[NUM_P];
  for(int i = 0; i < NUM_P; i++)
  {
    switch(i)
    {
      // rook: in rank 8 or rank 1
    case 0:
    case 7:
      whitePieces[i] = new Rook(WHITE,0,i);
      board[0][i] = whitePieces[i]; // the board[i][j] also have the ptr to a piece 
      blackPieces[i] = new Rook(BLACK,BOARD_SIZE-1,i);
      board[BOARD_SIZE-1][i] = blackPieces[i];
      break;
      
      // knight: in rank 8 or 1
    case 1:
    case 6:
      whitePieces[i] = new Knight(WHITE,0,i);
      board[0][i] = whitePieces[i];
      blackPieces[i] = new Knight(BLACK,BOARD_SIZE-1,i);
      board[BOARD_SIZE-1][i] = blackPieces[i];
      break;
      
      // bishop: in rank 8 or 1
    case 2:
    case 5:
      whitePieces[i] = new Bishop(WHITE,0,i);
      board[0][i] = whitePieces[i];
      blackPieces[i] = new Bishop(BLACK,BOARD_SIZE-1,i);
      board[BOARD_SIZE-1][i] = blackPieces[i];
      break;
      
      // queen: in rank 8 or 1
    case 3:
      whitePieces[i] = new Queen(WHITE,0,i);
      board[0][i] = whitePieces[i];
      blackPieces[i] = new Queen(BLACK,BOARD_SIZE-1,i);
      board[BOARD_SIZE-1][i] = blackPieces[i];
      break;
      
      // king: in rank 8 or 1
    case 4:
      whitePieces[i] = new King(WHITE,0,i);
      board[0][i] = whitePieces[i];
      blackPieces[i] = new King(BLACK,BOARD_SIZE-1,i);
      board[BOARD_SIZE-1][i] = blackPieces[i];
      break;
      
      // pawn: in rank 7 or 2
    default:
      whitePieces[i] = new Pawn(WHITE,1,i%BOARD_SIZE);
      board[1][i%BOARD_SIZE] = whitePieces[i];
      blackPieces[i] = new Pawn(BLACK,BOARD_SIZE-2,i%BOARD_SIZE);
      board[BOARD_SIZE-2][i%BOARD_SIZE] = blackPieces[i];
    }
  }

  cout << "A new chess game is started!" << endl;
}



/**
 * Clearing the chessboard and the pieces
 */
inline void ChessBoard::clearBoard()
{
  // Clearing the pieces
  if(whitePieces!=nullptr)
  {
    for(int i = 0; i < NUM_P ; i++)
      if(whitePieces[i]) delete whitePieces[i];
    
    delete [] whitePieces; whitePieces = nullptr;
  }

  if(blackPieces!=nullptr)
  {
    for(int i = 0; i < NUM_P ; i++)
      if(blackPieces[i]) delete blackPieces[i];
    
    delete [] blackPieces; blackPieces = nullptr;
  }

  // Clearing the board
  if(board)
  {
    for(int i = 0; i < BOARD_SIZE; i++)
      delete [] board[i];
    
    delete [] board; board = nullptr;
  }
}



/**
 * Clear the chessboard and reset the flags then generate a new game board
 */
void ChessBoard::resetBoard()
{
  // Get a new chess board
  clearBoard(); setupBoard();
  // Reset the flags
  moveTurn = WHITE;  gameOver = false;
}



/**
 * Return a ptr to a king
 */
Piece* ChessBoard::findKing(bool color) const
{
  Piece* pieceList[NUM_P]; // ptrs to all the pieces of "color" colour
  if(color == WHITE)
  {
    for(int i = 0; i < NUM_P; i++)
      pieceList[i] = whitePieces[i];
  }
  else
  {
    for(int i = 0; i < NUM_P; i++)
      pieceList[i] = blackPieces[i];
  }

  // Get the king
  for(int i = 0; i < NUM_P; i++)
  {
    if(pieceList[i] && pieceList[i]->getType() == KING) return pieceList[i];
  }

  cerr << "Cannot find the king!" << endl << endl;
  return nullptr;
}



/**
 * Check if a king is in check
 */
bool ChessBoard::isInCheck(bool color) const
{
  // Locate my king (own side's king of "color" colour) and get its coodinates
  Piece* myKing = findKing(color);
  std::string p = myKing->getPos();
  int fileKing = p[0] - 'A'; int rankKing = p[1] - '1';
  
  // Get the ptrs to all the pieces of the opponent
  Piece* oppoPieceList[NUM_P];
  if(color == WHITE)
  {
    for(int i = 0; i < NUM_P; i++)
      oppoPieceList[i] = blackPieces[i];
  }
  else
  {
    for(int i = 0; i < NUM_P; i++)
      oppoPieceList[i] = whitePieces[i];
  }

  // Traverse the opponent's pieces to see if my king would be attacked
  for(int i = 0; i < NUM_P; i++)
  {
    if(!oppoPieceList[i]) continue;
    
    if(oppoPieceList[i]->movePieceRuleTest(rankKing,fileKing,board)) return true;
  }
  
  return false;
}


/** 
 * Make a "fake move" on the board, it can handle scenarios where there is an opponent's piece
 * as well as simplly moveing
 * Piece* & hostPiece should be a nullptr
 * The oppent's "taken out" piece is stored in Piece* & hostPiece
 */
void ChessBoard::makeFakeMove(int const RANK_S, int const FILE_S, int const RANK_D,
                              int const FILE_D, Piece* & hostPiece)
{
  // Find my Piece
  Piece* myPiece = board[RANK_S][FILE_S];

  // Increment pawn/king/rook's moving counter
  if(myPiece->getType() == PAWN)
  {
    static_cast<Pawn*>(myPiece)->incCount();
  }
  else if(myPiece->getType() == KING)
  {
    static_cast<King*>(myPiece)->incCount();
  }
  else if(myPiece->getType() == ROOK)
  {
    static_cast<Rook*>(myPiece)->incCount(); 
  }

  if(hostPiece)
  {
    cerr << "Should Pass a null pointer in makeFakeMove()" << endl << endl;
    return;
  }

  // Test if the destination is hostile, if so: "take that piece"
  if(myPiece->isDestHostile(RANK_D,FILE_D,board))
  {
    hostPiece = board[RANK_D][FILE_D]; // get the ptr to that hostile piece
    const char hostPos[] = { char(FILE_D+'A'), char(RANK_D+'1'), '\0'};
    
    // "Taking" the hostile from the look-up on which isInCheck() and so forth depend
    std::string tempPos;
    if(hostPiece->getColor() == WHITE) // white hostile
    {
      for(int i = 0; i < NUM_P; i++) // locate the victim piece in whitePieces[]
      {
        if(!whitePieces[i]) continue;
        
        tempPos = whitePieces[i]->getPos();
        if(tempPos.compare(hostPos) == 0)
        {
          whitePieces[i] = nullptr; // temporarily whiping out from the look up, not deleting!
          break;
        }
      }
    }
    else// black hostile
    {
      for(int i = 0; i < NUM_P; i++)
      {
        if(!blackPieces[i]) continue;
        
        tempPos = blackPieces[i]->getPos();
        if(tempPos.compare(hostPos) == 0)
        {
          blackPieces[i] = nullptr; 
          break;
        }
      }
    }
  }
  
  // My piece makes a "fake" move to board[RANK_D][FILE_D]
  board[RANK_D][FILE_D] = myPiece;
  board[RANK_S][FILE_S] = nullptr;
  myPiece->setPos(RANK_D,FILE_D);
}


/**
 * Undo the fake move via makeFakeMove()
 */
void ChessBoard::undoMakeFakeMove(int const RANK_S, int const FILE_S, int const RANK_D,
                                  int const FILE_D, Piece* & hostPiece)
{
  // My piece
  Piece* myPiece = board[RANK_D][FILE_D];
  bool myColor = myPiece->getColor();

  // Decrement pawn/king/rook's moving counter
  if(myPiece->getType() == PAWN)
  {
    static_cast<Pawn*>(myPiece)->decCount();
  } 
  else if(myPiece->getType() == KING)
  {
    static_cast<King*>(myPiece)->decCount();
  }
  else if(myPiece->getType() == ROOK)
  {
    static_cast<Rook*>(myPiece)->decCount();
  }
  
  // Restore my piece
  board[RANK_S][FILE_S] = myPiece; myPiece->setPos(RANK_S,FILE_S);

  // Restore the taken piece
  if(hostPiece != nullptr)
  {
    board[RANK_D][FILE_D] = hostPiece;

    // Put back the taken-out piece into the white(black)Pieces
    if(hostPiece->getColor()==WHITE && hostPiece->getColor()!= myColor)
    {
      for(int i = 0; i < NUM_P; i++)
      {
        if(whitePieces[i] == nullptr)
        {
          whitePieces[i] = hostPiece; return;
        }
      }
    }
    else
    {
      cerr << "Cannot put back the taken piece!" << endl << endl;
      return;
    }
    
    // Put back into the blackPieces list
    if(hostPiece->getColor()==BLACK && hostPiece->getColor()!= myColor)
    {
      for(int i = 0; i < NUM_P; i++)
      {
        if(blackPieces[i] == nullptr)
        {
          blackPieces[i] = hostPiece; return; 
        }
      }
    }
    else
    {
      cerr << "Cannot put back the taken piece!" << endl << endl;
      return;
    }
  }
  else
    board[RANK_D][FILE_D] = nullptr;
}



/**
 * Simply telling if an action of moving a piece and/or take a piece would save the king from
 * being in check, effectively will not modify the data members 
 */
bool ChessBoard::doesThisMoveSaveKing(int const RANK_S, int const FILE_S,
                                      int const RANK_D, int const FILE_D)
{
  //--- 1. Find my Piece at the destination and get its position and color
  Piece* myPiece = board[RANK_S][FILE_S];
  bool myColor = myPiece->getColor();
 
  //--- 2. Test if legal
  if(myPiece->movePieceRuleTest(RANK_D,FILE_D,board) == false) return false;

  //--- 3.  Attempting some "fake moves" here:
  Piece* hostPiece = nullptr; // ptr to the hostile piece at destination (if there is such one)
  makeFakeMove(RANK_S,FILE_S,RANK_D,FILE_D,hostPiece); // make a "fake move"

  //--- 4.  Test if the king would be safe
  if(!isInCheck(myColor)) // no checkmate anymore: god save the king!
  {
    undoMakeFakeMove(RANK_S,FILE_S,RANK_D,FILE_D,hostPiece); // restore the pieces
    return true;
  }
  else // can't save the king: restore anyway
  {
    undoMakeFakeMove(RANK_S,FILE_S,RANK_D,FILE_D,hostPiece);
    return false; // can't save the king sorry
  }
}



/**
 * Given a set of a board plus the pieces (not necessary the member ones), Check if there
 * is no further valid move. Used to test for checkmate and stalemate, provided that a 
 * valid move has been ubmitted.
 */
bool ChessBoard::isNoFurtherValidMove(bool color)
{
  std::string myPos;
  int myRank, myFile;

  // Get the ptrs to the white/black pieces only
  Piece* pieceList[NUM_P];
  if(color == WHITE)
  {
    for(int i = 0; i < NUM_P; i++)
      pieceList[i] = whitePieces[i];
  }
  else
  {
    for(int i = 0; i < NUM_P; i++)
      pieceList[i] = blackPieces[i];
  }
  
  // Move my pieces 
  for(int k = 0; k < NUM_P; k++)
  {  
    if(!pieceList[k]) continue;

    myPos = pieceList[k]->getPos(); myFile = myPos[0] - 'A'; myRank = myPos[1] - '1';

    for(int i = 0; i < BOARD_SIZE; i++)
    {
      for(int j = 0; j < BOARD_SIZE; j++)
      {
        //See if any moves of the pieces of the own side could save the king
        if(doesThisMoveSaveKing(myRank,myFile,i,j)) return false;
      }
    }
  }

  return true;
}



/**
 * Castling, part of the submitMove()
 */
bool ChessBoard::castling(Piece* const myKing, int const RANK_D, int const FILE_D)
{
  //=== If myKing isn't really a king: reject his request
  if(myKing->getType() != KING) return false;

  //=== myKing mustn't have ever moved
  if(static_cast<King*>(myKing)->getCount() != 0) return false;

  //=== myKing should be safe now
  if(isInCheck(moveTurn)) return false;

  //=== Get myKing's position and color: which is useful
  string myPos = myKing->getPos();
  int const FILE_S = myPos[0] - 'A'; int const RANK_S = myPos[1] - '1';

  string rookPos; // the rook's position
  
  //=== The king should moves horizontally
  if(RANK_S != RANK_D) return false;
  
  //=== Castling!
  if(FILE_S - FILE_D == 2) // king moving left
  {
    // The left rook (board[RANK_S][0]) mustn't have ever moved as well
    if(!board[RANK_S][0]) return false;

    if(board[RANK_S][0]->getType()!=ROOK || board[RANK_S][0]->getColor()!= moveTurn) return false;

    if(static_cast<Rook*>(board[RANK_S][0])->getCount() != 0) return false;

    // Ensure that there is nothing between the king and the rook
    for(int i = 1; i < FILE_S ; i++)
    {
      if(board[RANK_S][FILE_S-i] != nullptr) return false;
    }

    // Get the rook's (origianl) position
    rookPos = board[RANK_S][0]->getPos();
    
    // His majesty starts to moves and cannot be attacked during his moving
    for(int i = 0; i < 2; i++)
    {
      if(doesThisMoveSaveKing(RANK_S, FILE_S - i, RANK_S, FILE_S - i - 1) == false)
        return false;
    }

    // Submit the king's move!
    board[RANK_S][FILE_D] = board[RANK_S][FILE_S];
    board[RANK_S][FILE_S] = nullptr;
    myKing->setPos(RANK_S,FILE_D);
    static_cast<King*>(myKing)->incCount(); // update count

    // The rook moves now
    board[RANK_S][0] = board[RANK_S][FILE_D+1]; // rook at the right to the king
    board[RANK_S][0] = nullptr;
    board[RANK_S][FILE_D+1]->setPos(RANK_S,FILE_D+1);
    static_cast<Rook*>(board[RANK_S][FILE_D+1])->incCount();
  }

  if(FILE_D - FILE_S == 2) // king moving right: the right rook moves
  {
    // The right rook (board[RANK_S][7]) mustn't have ever moved as well
    if(!board[RANK_S][BOARD_SIZE-1]) return false;

    if(board[RANK_S][BOARD_SIZE-1]->getType()!=ROOK ||
       board[RANK_S][BOARD_SIZE-1]->getColor()!= moveTurn)
      return false;

    if(static_cast<Rook*>(board[RANK_S][BOARD_SIZE-1])->getCount() != 0) return false;

    // Ensure that there is nothing between the king and the rook
    for(int i = 1; i < BOARD_SIZE-1-FILE_S ; i++)
    {
      if(board[RANK_S][FILE_S+i] != nullptr) return false;
    }

    // Get the rook's (origianl) position
    rookPos = board[RANK_S][BOARD_SIZE-1]->getPos();
    
    // His majesty starts to moves and cannot be attacked during his moving
    for(int i = 0; i < 2; i++)
    {
      if(doesThisMoveSaveKing(RANK_S, FILE_S + i, RANK_S, FILE_S + i + 1) == false)
        return false;
    }

    board[RANK_S][FILE_D] = board[RANK_S][FILE_S];
    board[RANK_S][FILE_S] = nullptr;
    myKing->setPos(RANK_S,FILE_D);
    static_cast<King*>(myKing)->incCount();

    board[RANK_S][BOARD_SIZE-1] = board[RANK_S][FILE_D-1]; // rook at the left to the king
    board[RANK_S][BOARD_SIZE-1] = nullptr;
    board[RANK_S][FILE_D-1]->setPos(RANK_S,FILE_D-1);
    static_cast<Rook*>(board[RANK_S][FILE_D-1])->incCount();
  }

  //=== Printing
  cout << *myKing << " commits castling and moves from " << myPos << " to "
       << myKing->getPos() << ", "
       << *board[RANK_S][FILE_D-1] << " moves from " << rookPos << " to "
       << board[RANK_S][FILE_D-1]->getPos() << endl;

  //=== Test if this leads to the opponent being in check or in checkmate or in stalemate
  bool oppoColor = (moveTurn == WHITE ? BLACK : WHITE);

  // Leading to opponent in check? 
  bool incheckFlag = isInCheck(oppoColor);
  // Leading to opponent has no legal move?
  bool noFurtherMove = isNoFurtherValidMove(oppoColor);

  if(incheckFlag && noFurtherMove) // opponent in checkmate
  {
    gameOver = true;
    cout << (moveTurn == WHITE ? "Black " : "White ") << "is in checkmate" << endl; 
  }
  else if(incheckFlag && !noFurtherMove) // opponent in check only
  {
    if(oppoColor == WHITE)
    {
      //whiteInCheck = true;
      cout << "White is in check" << endl;
    }
    else
    {
      //blackInCheck = true;
      cout << "Black is in check" << endl;
    }
  }
  else if(!incheckFlag && noFurtherMove) // opponnent in stalemate
  {
    gameOver = true;
    cout << "Stalemate. Game over." << endl;
  }
  // Otherwise: normal move and exit
  moveTurn = !moveTurn;// next trun: the opponent moves

  return true;
}



/**
 * Make one moving on the chessboard
 * srcPos: source position, desPos: destination position
 */
void ChessBoard::submitMove(char const * srcPos, char const * desPos)
{
  //=== Geting coordinates in int
  int const FILE_S = srcPos[0] - 'A'; int const RANK_S = srcPos[1] - '1';
  int const FILE_D = desPos[0] - 'A'; int const RANK_D = desPos[1] - '1';

  Piece* myPiece = nullptr; Piece* hostPiece = nullptr;

  //=== 0. Test if the game is over
  if(gameOver)
  {
    cerr << "The game was over, please start a new game!" << endl;
    return;  
  } 
  
  //=== 1.1 Test if the source position is empty
  if(!board[RANK_S][FILE_S])
  {
    cerr << "There is no piece at position " << srcPos << "!" << endl;
    return;
  }
  
  //=== 1.2. Test if the source position is of opponent's
  if(board[RANK_S][FILE_S]->getColor()!=moveTurn)
  {
     cerr << "It is not " << (moveTurn==WHITE ? "Black's ": "White's ")
         << " turn to move!" << endl;
     return;
  }
  else
    myPiece = board[RANK_S][FILE_S];

  //=== 1.3 Test for castling
  if(castling(myPiece,RANK_D,FILE_D))
    return;

  //=== 2. Test if the move is legal
  if(myPiece->movePieceRuleTest(RANK_D,FILE_D,board) == false)
  {
    cerr << *myPiece << " cannot move to " << desPos << "!" << endl;
    return;
  }

  //=== 3. My side make a fake move
  makeFakeMove(RANK_S,FILE_S,RANK_D,FILE_D,hostPiece);

  //=== 4. Get the info about the hostile piece (to print when a piece is taken out)
  std::string hostPieceInfo;
  if(hostPiece)
  {
    hostPieceInfo =
      (hostPiece->getColor()==WHITE ? "White's " : "Black's ");
    
    switch(hostPiece->getType())
      {
      case KING:
        hostPieceInfo += "King"; break;
      case QUEEN:
        hostPieceInfo += "Queen"; break;
      case ROOK:
        hostPieceInfo += "Rook"; break;
      case BISHOP:
        hostPieceInfo += "Bishop"; break;
      case KNIGHT:
        hostPieceInfo += "Knight"; break;
      case PAWN:
        hostPieceInfo += "Pawn"; break;
      default:
        hostPieceInfo += "Unknown Piece";
      }  
  }
  
  //=== 6. Test if this fake move leads to own sides' incheck or save the king
  if(isInCheck(moveTurn)) // my side is in check
  {
    // Restore
    undoMakeFakeMove(RANK_S,FILE_S,RANK_D,FILE_D,hostPiece);
    
    cerr << *myPiece << " cannot move to " << desPos << "!" << endl;
    return;
  }
  else // not in check
  {
    //if(moveTurn == WHITE) whiteInCheck = false;
    //else blackInCheck = false;
    
    delete hostPiece; // commit this move

    // print out this move
    cout << *myPiece << " moves from " << srcPos << " to " << myPiece->getPos();
    if(hostPiece)
      cout << " taking " << hostPieceInfo;
    
    cout << endl;
  }

  //=== 7. Test if this leads to the opponent being in check or in checkmate or in stalemate
  bool oppoColor = (moveTurn == WHITE ? BLACK : WHITE);

  // Leading to opponent in check? 
  bool incheckFlag = isInCheck(oppoColor);
  // Leading to opponent has no legal move?
  bool noFurtherMove = isNoFurtherValidMove(oppoColor);

  if(incheckFlag && noFurtherMove) // opponent in checkmate
  {
    gameOver = true;
    cout << (moveTurn == WHITE ? "Black " : "White ") << "is in checkmate" << endl; 
  }
  else if(incheckFlag && !noFurtherMove) // opponent in check only
  {
    if(oppoColor == WHITE)
    {
      //whiteInCheck = true;
      cout << "White is in check" << endl;
    }
    else
    {
      //blackInCheck = true;
      cout << "Black is in check" << endl;
    }
  }
  else if(!incheckFlag && noFurtherMove) // opponnent in stalemate
  {
    gameOver = true;
    cout << "Stalemate. Game over." << endl;
    return;
  }
  // Otherwise: normal move and exit
  moveTurn = !moveTurn;// next trun: the opponent moves
}



ChessBoard::~ChessBoard(){ clearBoard(); }



/**
 * Overloading to print the colour and the type of a piece e.g. White's Pawn
 */
std::ostream& operator<<(std::ostream& out, const Piece & piece)
{
  out << (piece.getColor() == BLACK ? "Black's ": "White's ");

  switch(piece.getType())
  {
  case KING:
    out << "King"; break;
  case QUEEN:
    out << "Queen"; break;
  case ROOK:
    out << "Rook"; break;
  case BISHOP:
    out << "Bishop"; break;
  case KNIGHT:
    out << "Knight"; break;
  case PAWN:
    out << "Pawn"; break;
  default:
    out << "Unknown Piece";
  }
  return out;
}
