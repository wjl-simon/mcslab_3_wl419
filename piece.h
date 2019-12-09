#ifndef PIECE_H
#define PIECE_H

#include <string>
#include <iostream>
//#include "helper.h"

#define WHITE false // colour of the pieces
#define BLACK true
#define BOARD_SIZE 8 // num of rows/columns in a chess board

/*===== ENUMERATE PIECES NAME =====*/
enum PieceType {KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN};


/*===== BASE CLASS=====*/
class Piece
{
 protected:

  PieceType const TYPE; // the type of a piece, i.e. KING/QUEEN/ROOK,etc
  bool const COLOR; // the colour of this piece: false means white, true means black
  int currentRank; // current position of this piece
  int currentFile;
  
 public:

  Piece(PieceType type, bool color, int rank, int file);

  /**
   * Test if the destination of a move is in of the board, true if so
   */
  bool isInside(int rank, int file) const;

  /**
   * Test if the destination of a move has opponent's pieces (hostile piece), returns true if so
   */
  bool isDestHostile(int const RANK_D, int const FILE_D, Piece*** const board) const;

  /**
   * Return the type of this piece 
   */
  PieceType getType() const;
  
  /**
   * Return the colour of this piece 
   */
  bool getColor() const;

  /**
   * Return the position of this piece (in char*, e.g. A1, C3)
   */
  std::string getPos() const;
  
  /**
   * Test if a move of this piece follows the corresponding rule of its type and returns true
   * if succeed.
   * It checks if there this move overleaps, if the destination is inside the board as well as 
   * if there is another piece at the destination (ok if the destination has a hostile one, but
   * not ok if the destination has an own side's piece)
   */
  virtual bool movePieceRuleTest(int const RANK_D, int const FILE_D,
                                 Piece*** const board) const = 0;

  /**
   * Directly set the position of a piece to the target position, regardless of the rules
   */
  void setPos(int const targetRank, int const targetFile);
    
  virtual ~Piece();

};


/*===== King =====*/

class King: public Piece::Piece
{
 public:
  
  King(bool color, int rank, int file);

  bool movePieceRuleTest(int const RANK_D, int const FILE_D,
                         Piece*** const board) const override;

  ~King() override;
};

/*===== Queen =====*/

class Queen: public Piece::Piece
{
 public:

  Queen(bool color, int rank, int file);

  bool movePieceRuleTest(int const RANK_D, int const FILE_D,
                         Piece*** const board) const override;

  ~Queen() override;
};

/*===== Rook =====*/

class Rook: public Piece::Piece
{
 public:

  Rook(bool color, int rank, int file);

  bool movePieceRuleTest(int const RANK_D, int const FILE_D,
                         Piece*** const board) const override;

  ~Rook() override;
};

/*===== Bishop =====*/

class Bishop: public Piece::Piece
{
 public:

  Bishop(bool color, int rank, int file);

  bool movePieceRuleTest(int const RANK_D, int const FILE_D,
                         Piece*** const board) const override;

  ~Bishop() override;
};

/*===== Knight =====*/

class Knight: public Piece::Piece
{
 public:

  Knight(bool color, int rank, int file);

  bool movePieceRuleTest(int const RANK_D, int const FILE_D,
                         Piece*** const board) const override;

  ~Knight() override;
};


/*===== Pawn =====*/

class Pawn : public Piece::Piece
{
  //bool commitFirstMove; // flag for if the first move of this Pawn piece has been commited
  int moveTimes; // a counter indicating how many times this pawn moves
  
 public:

  Pawn(bool color, int rank, int file);

  bool movePieceRuleTest(int const RANK_D, int const FILE_D,
                         Piece*** const board) const override;
  void incCount();

  void decCount();
  
  ~Pawn() override;
};



#endif
