#ifndef HELPER_H
#define HELPER_H

//#include <iostream>
#include "piece.h"

/**
 * Test if following the moving rule of a rook but won't check the boarder nor the destination
 */
inline bool rookMove(int const RANK_D,int const FILE_D,int const RANK_S,int const FILE_S,
                     Piece*** const board)
{
  if(RANK_D==RANK_S && FILE_D!=FILE_S) // horizontally moving to a diffrent file
  {
    if(FILE_D > FILE_S) // moving right
    {
      int maxstep = FILE_D - FILE_S - 1; // num of squres between src and dest
      // Check if there is anther piece between the src and dest
      for(int i = 1; i <= maxstep; i++)
        if(board[RANK_S][FILE_S+i] != nullptr) return false;
    }
    
    if(FILE_D < FILE_S) // moving left
    {
      int maxstep = FILE_S - FILE_D - 1;
      for(int i = 1; i <= maxstep; i++)
        if(board[RANK_S][FILE_S-i] != nullptr) return false;
    }

    return true;
  }
  else if(FILE_D==FILE_S && RANK_D!=RANK_S)// vertically moving to a diffrent rank
  {
    if(RANK_D > RANK_S) // moving up
    {
      int maxstep = RANK_D - RANK_S - 1;
      for(int i = 1; i <= maxstep; i++)
        if(board[RANK_S+i][FILE_S] != nullptr) return false;
    }
    
    if(RANK_D < RANK_S) // moving down
    {
      int maxstep = FILE_S - FILE_D - 1;
      for(int i = 1; i <= maxstep; i++)
        if(board[RANK_S-i][FILE_S] != nullptr) return false;
    }

    return true;
  }
  else
    return false;
}




/**
 * Test if following the moving rule of a bishop but won't check the boarder nor the destination
 */
inline bool bishopMove(int const RANK_D,int const FILE_D,int const RANK_S,int const FILE_S,
                       Piece*** const board)
{
  if(RANK_D-RANK_S == FILE_D-FILE_S && RANK_D-RANK_S > 0) // up right
  {
    int maxstep = RANK_D - RANK_S - 1; // num of squres between src and dest
    // Check if there is anther piece between the src and dest
    for(int i = 1; i <= maxstep; i++)
      if(board[RANK_S+i][FILE_S+i] != nullptr) return false;

    return true;
  }
  
  if(RANK_D-RANK_S == FILE_S-FILE_D && RANK_D-RANK_S > 0) // up left
  {
    int maxstep = RANK_D - RANK_S - 1;
    for(int i = 1; i <= maxstep; i++)
      if(board[RANK_S+i][FILE_S-i] != nullptr) return false;

    return true;
  }
  
  if(RANK_S-RANK_D == FILE_S-FILE_D && RANK_S-RANK_D > 0) // down left
  {
    int maxstep = RANK_S - RANK_D - 1;
    for(int i = 1; i <= maxstep; i++)
      if(board[RANK_S-i][FILE_S-i] != nullptr) return false;

    return true;
  }
  
  if(RANK_S-RANK_D == FILE_D-FILE_S && RANK_S-RANK_D > 0) // down right
  {
    int maxstep = RANK_S - RANK_D - 1;
    for(int i = 1; i <= maxstep; i++)
      if(board[RANK_S-i][FILE_S+i] != nullptr) return false;

    return true;
  }

  return false;
}



/**
 * Overloading to print the colour and the type of a piece
 */
//std::ostream& operator<<(std::ostream& out, const Piece* & piece);


#endif
