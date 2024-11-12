/*******************************************************************************
*
* FILENAME : knight_tour.c
*
* DESCRIPTION : The Knight's Tour implementation.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 20.06.2023
* 
*******************************************************************************/

#include <assert.h> /* assert */
#include <string.h> /* memcpy */
#include <sys/time.h> /* settimer() */

#include "bitarray.h"
#include "knight_tour.h"

#define FIVE_MINUTES (300)

#define INVALID_POSITION (-1)
#define WRONG_MOVE (-1)
#define MAX_POSSIBLE_MOVES (8)
#define SIZE_OF_MOVES (sizeof(move_t) * MAX_POSSIBLE_MOVES)
#define NO_AVAILABLE_MOVES (-1)

#define BOARD_MAX_SIDE_IDX (7)
#define BOARD_SIDE_SIZE (8)
#define BOARD_MAX_IDX (63)
#define BOARD_SIZE (BOARD_SIDE_SIZE * BOARD_SIDE_SIZE)

typedef char move_t;
typedef move_t board_moves_lut_t[BOARD_SIZE][MAX_POSSIBLE_MOVES];

static void FillUpMovesLut(board_moves_lut_t moves_lut);
static int IsBoardFull(bitsarr_t *board);
static int IsMoveAvailable(bitsarr_t *board, int position);

static int SolveTheProblem(board_moves_lut_t moves, bitsarr_t *board,
                            size_t *path_arr, int position,int initial_position);
static size_t CheckDegree(board_moves_lut_t moves, move_t move,bitsarr_t *board);
static move_t FindMinDegree(board_moves_lut_t moves, move_t square_moves[8],
                                                            bitsarr_t *board);
static move_t SolveTheProblemWarnsdorffs(board_moves_lut_t moves,
                                            bitsarr_t *board, size_t *path_arr,
                                            int position, int initial_position);


int KnightsTourBasic(int init_position, size_t *path_arr)
{
    struct itimerval timer = {0};

    bitsarr_t board = 0;

    board_moves_lut_t moves = {0};

    assert(NULL != path_arr);

    FillUpMovesLut(moves);

    timer.it_value.tv_sec = FIVE_MINUTES;
	setitimer(ITIMER_VIRTUAL, &timer, 0);

    return (SolveTheProblem(moves, &board,path_arr,init_position,init_position));
}

int KnightsTourWarnsdorffs(int init_position, size_t *path_arr)
{
    struct itimerval timer = {0};

    bitsarr_t board = 0;

    board_moves_lut_t moves = {0};

    assert(NULL != path_arr);

    FillUpMovesLut(moves);

    timer.it_value.tv_sec = FIVE_MINUTES;
	setitimer(ITIMER_VIRTUAL, &timer, 0);

    return (SolveTheProblemWarnsdorffs(moves, &board, path_arr, init_position,
                                                                init_position));
}

static int SolveTheProblem(board_moves_lut_t moves, bitsarr_t *board,
                            size_t *path_arr, int position,int initial_position)
{
    move_t possible_moves[MAX_POSSIBLE_MOVES] = {0};
    int res = WRONG_MOVE;
    int i = 0;

    assert(NULL != moves);
    assert(NULL != board);

    *board = BitArraySetOn(*board, position);

    if (IsBoardFull(board))
    {
        path_arr[position] = initial_position;
        return (position);
    }

    memcpy(possible_moves, moves[position], SIZE_OF_MOVES);

    for (;0 > res && i < MAX_POSSIBLE_MOVES; ++i)
    {
        if (IsMoveAvailable(board, possible_moves[i]))
        {
            res = SolveTheProblem(moves, board, path_arr, possible_moves[i],
                                                            initial_position);
        }
    }

    if (0 <= res)
    {
        path_arr[position] = res;
        return (position);
    }
    else
    {
        *board = BitArraySetOff(*board, position);
        return (WRONG_MOVE);
    }
}

static move_t SolveTheProblemWarnsdorffs(board_moves_lut_t moves,
                                            bitsarr_t *board, size_t *path_arr,
                                            int position, int initial_position)
{
    move_t possible_moves[MAX_POSSIBLE_MOVES] = {0};
    int res = NO_AVAILABLE_MOVES;
    int i = 0;
    move_t next = 0;

    assert(NULL != moves);
    assert(NULL != board);

    *board = BitArraySetOn(*board, position);

    if (IsBoardFull(board))
    {
        path_arr[position] = initial_position;
        return (position);
    }

    memcpy(possible_moves, moves[position], SIZE_OF_MOVES);

    for (;0 > res && i < MAX_POSSIBLE_MOVES; ++i)
    {

        next = FindMinDegree(moves, possible_moves, board);

        if (NO_AVAILABLE_MOVES == next)
        {
            path_arr[position] = initial_position;
            return (position);
        }
        
        res = SolveTheProblemWarnsdorffs(moves, board, path_arr, next,
                                                    initial_position);
    }

    if (0 <= res)
    {
        path_arr[position] = res;
        return (position);
    }
    else
    {
        *board = BitArraySetOff(*board, position);
        return (WRONG_MOVE);
    }
}

static move_t FindMinDegree(board_moves_lut_t moves,
                    move_t square_moves[MAX_POSSIBLE_MOVES], bitsarr_t *board)
{
    size_t i = 0;
    move_t min = NO_AVAILABLE_MOVES;
    int min_degree = MAX_POSSIBLE_MOVES + 1;
    int degree = 0;

    assert(NULL != moves);
    assert(NULL != square_moves);
    assert(NULL != board);

    for (; i < MAX_POSSIBLE_MOVES; ++i)
    {   

        if (IsMoveAvailable(board, square_moves[i]))
        {
            degree = CheckDegree(moves, square_moves[i], board);

            if (min_degree > degree)
            {
                min = square_moves[i];
                min_degree = degree;
            }
        }
    }

    return (min);
}

static int IsBoardFull(bitsarr_t *board)
{
    assert(NULL != board);

    return (0 == BitArrayCountOff(*board));
}

static int IsMoveAvailable(bitsarr_t *board, int position)
{
    assert(NULL != board);

    return (INVALID_POSITION!=position && 0==BitArrayGetVal(*board, position));
}

static size_t CheckDegree(board_moves_lut_t moves, move_t move,bitsarr_t *board)
{
    size_t i = 0;
    size_t counter = 0;

    assert(NULL != moves);
    assert(NULL != board);

    for (i = 0; i < MAX_POSSIBLE_MOVES; ++i)
    {
    
       if (IsMoveAvailable(board, moves[(unsigned char) move][i]))
       {
            ++counter;
       }
    }

    return (counter);
}

static void FillUpMovesLut(board_moves_lut_t moves_lut)
{
    int xMove[MAX_POSSIBLE_MOVES] = {2, 1, -1, -2, -2, -1, 1, 2};
    int yMove[MAX_POSSIBLE_MOVES] = {1, 2, 2, 1, -1, -2, -2, -1};

    int i = 0;
    int x = 0;
    int y = 0;

    assert(NULL != moves_lut);

    for (; y < BOARD_SIDE_SIZE; ++y)
    {
        for (x = 0; x < BOARD_SIDE_SIZE; ++x)
        {
            for (i = 0; i < BOARD_SIDE_SIZE; ++i)
            {
                if (BOARD_MAX_SIDE_IDX < x + xMove[i] || 0 > x + xMove[i])
                {
                    moves_lut[BOARD_SIDE_SIZE * y + x][i] = INVALID_POSITION;
                    continue;
                }
                if (BOARD_MAX_IDX < (y * BOARD_SIDE_SIZE) + 
                    BOARD_SIDE_SIZE * yMove[i] ||
                    0 > (y * BOARD_SIDE_SIZE) +
                    BOARD_SIDE_SIZE * yMove[i])
                {
                    moves_lut[BOARD_SIDE_SIZE * y + x][i] = INVALID_POSITION;
                    continue;
                }

                moves_lut[BOARD_SIDE_SIZE * y + x][i] =
                (x + xMove[i])+(y * BOARD_SIDE_SIZE)+(BOARD_SIDE_SIZE *yMove[i]);
            }
        }
    }
}