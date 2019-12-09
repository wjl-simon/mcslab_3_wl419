#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "piece.h"

class ChessBoard
{
  static const int NUM_P; // the number of pieces at the beginning for each side, which is 16
    
  Piece*** board; // the chess game board
  Piece** whitePieces; // fast look-up for the white piece ptrs
  Piece** blackPieces; // fast look-up for the black piece ptrs

  bool moveTurn; // if = WHITE: white's turn to move; =BLACK: black's turn to move
  bool whiteInCheck;
  bool blackInCheck;
  bool gameOver; // true if a board game ends i.e. a king being checkmated or stalemate
  
  /**
   * Set up a chess board. Called by the constructor or the reset() only.
   * Make sure board, whitePieces and blackPieces are all nullptr when calling
   */
  void setupBoard();

  /**
   * Clear the board as well as the two piece lists
   */
  void clearBoard();

  /**
   * Return a ptr to a king
   * Especially usefully when needing to make a fake move to test for e.g. incheck 
   */
  Piece* findKing(bool color) const;

  /**** 
   * Make a "fake move" on the board, it can handle scenarios where there is an opponent's piece
   * as well as simplly moveing
   * Piece* & hostPiece should be a nullptr
   * The oppent's "taken out" piece is stored in Piece* & hostPiece
   */
  void makeFakeMove(int const RANK_S, int const FILE_S,
                    int const RANK_D, int const FILE_D, Piece*& hostPiece);

  /**
   * Undo the fake move via makeFakeMove()
   */
  void undoMakeFakeMove(int const RANK_S, int const FILE_S, int const RANK_D,
                        int const FILE_D, Piece*& hostPiece);
  
  /**
   * Check if a king is in check (used to test if a submitted move would lead to this)
   * Especially usefully when needing to make a fake move to test for e.g. incheck
   */
  bool isInCheck(bool color) const;

  /**
   * Simply telling if an action of moving a piece and/or take a piece would save the king from
   * being in check, effectively will not modify the data members 
   */
  bool doesThisMoveSaveKing(int const RANK_S, int const FILE_S,
                            int const RANK_D, int const FILE_D);
  
  /**
   * Check if a king is checkmated (used to test if a submitted move would lead to this)
   * Effectively will not change the data members
   */
  bool isCheckmate(bool color);

  /**
   * Check if there is no further leagl move.
   * Used to test for stalemate, provided that a valid move has been submitted.
   */
  bool isNoFurtherLegalMove(bool color) const;

  
 public:

  ChessBoard();
  
  /**
   * Make one moving on the chessboard
   * srcPos: source position, desPos: destination position
   */
  void submitMove(char const * srcPos, char const * desPos);

  /**
   * Reset the chessboard
   */
  void resetBoard();

  virtual ~ChessBoard();
  
};




/**
 * Overloading to print the colour and the type of a piece
 */
std::ostream& operator<<(std::ostream& out, const Piece & piece);

#endif
