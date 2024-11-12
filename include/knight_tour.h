/*******************************************************************************
*
* FILENAME : knight_tour.h
*
* DESCRIPTION : Knight's Tour is a classic puzzle in chess. It involves a knight
* piece placed on an empty chessboard and the challenge is to find a sequence of
* moves in which the knight visits every square on the board exactly once.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 20.06.2023
* 
*******************************************************************************/
#ifndef __NSRD_KNIGHT_TOUR_H__
#define __NSRD_KNIGHT_TOUR_H__

#include <stddef.h> /* size_t */

/*
DESCRIPTION
	Solves the Knight Tour problem by finding a sequence of moves for a knight
    on a chessboard. It starts from a given position and writes the resulting
    sequence into an array of 64 numbers. Each element in the array represents
    the position on the board at the corresponding step of the tour. It's
    important to note that the calculation may take some time. Please ensure
    that the starting position provided is valid (within the range 0-63) and
    that the array passed has a minimum size of 64 elements. Providing an
    invalid starting position or an array with fewer than 64 elements may lead
    to undefined behavior.
	There are two implementations available:
    - KnightsTourBasic uses the backtracking algorithm to solve the problem.
    - KnightsTourWarnsdorffs uses Warnsdorff's heuristic to solve the problem.
RETURN
	The function returns a pointer to the resulting array.
INPUT
	init_position: number of the square to start the tour from.
	path_arr: pointer of the resulting array of at least 64 elements.
*/
int KnightsTourBasic(int init_position, size_t *path_arr);
int KnightsTourWarnsdorffs(int init_position, size_t *path_arr);

#endif /* __NSRD_KNIGHT_TOUR_H__ */