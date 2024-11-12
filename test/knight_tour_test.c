/*******************************************************************************
*
* FILENAME : knight_tour_test.c
*
* DESCRIPTION : The Knight's Tour unit tests.
* 
* AUTHOR : Nick Shenderov
*
* DATE : 20.06.2023
* 
*******************************************************************************/

#include <string.h> /* printf */

#include "knight_tour.h"
#include "testing.h"


typedef char move_t;
typedef move_t board_moves_lut_t[64][8];

static void FillUpMovesLut(board_moves_lut_t moves_lut);
static int PathIsValid(size_t path[64], board_moves_lut_t moves_lut);
static void PrintPath(size_t path[64]);


static void TestBacktracking(void);
static void TestWarnsdorffs(void);

int main()
{
	TH_TEST_T tests[] = {
		{"Backtracking from 0", TestBacktracking},
		{"Warnsdorff's from 8", TestWarnsdorffs},
		TH_TESTS_ARRAY_END
	};

	TH_RUN_TESTS(tests);

	return (0);
}

static void TestBacktracking(void)
{
    board_moves_lut_t moves = {0};
    size_t path[64] = {0};

    FillUpMovesLut(moves);

    TH_ASSERT(0 == KnightsTourBasic(0, path));

    printf("\n");

    PrintPath(path);

    TH_ASSERT(1 == PathIsValid(path, moves));


     printf("\n");
}

static void TestWarnsdorffs(void)
{
    board_moves_lut_t moves = {0};
    size_t path[64] = {0};

    FillUpMovesLut(moves);
    
    TH_ASSERT(8 == KnightsTourWarnsdorffs(8, path));

    printf("\n");

    PrintPath(path);

    TH_ASSERT(1 == PathIsValid(path, moves));

    
     printf("\n");
}

static void FillUpMovesLut(board_moves_lut_t moves_lut)
{
    int xMove[8] = {2, 1, -1, -2, -2, -1, 1, 2};
    int yMove[8] = {1, 2, 2, 1, -1, -2, -2, -1};

    size_t i = 0;
    size_t x = 0;
    size_t y = 0;

    for (; y < 8; ++y)
    {
        for (x = 0; x < 8; ++x)
        {
            for (i = 0; i < 8; ++i)
            {
                /* if (0 > x + xMove[i] || 7 < x + xMove[i]) */
                if (0 > (int) x + xMove[i] || 7 < (int) x + xMove[i])
                {
                    moves_lut[8 * y + x][i] = -1;
                    continue;
                }
                /* if (0 > (y * 8) + 8 * yMove[i] || 63 < (y * 8) + 8 * yMove[i]) */
                if (0 > ((int) y * 8) + 8 * yMove[i] || 63 < ((int) y * 8) + 8 * yMove[i])
                {
                    moves_lut[8 * y + x][i] = -1;
                    continue;
                }

                moves_lut[8 * y + x][i] = (x + xMove[i])+(y * 8)+(8 * yMove[i]);
            }
        }
    }
}

static int PathIsValid(size_t path[64], board_moves_lut_t moves_lut)
{
    size_t lut[64] = {0};
    size_t ok_counter = 0;
    size_t i = 0;
    size_t q = 0;

    for (; i < 64; ++i)
    {
        if (0 != lut[path[i]])
        {
            return (-1);
        }
        for (q = 0; q < 8; ++q)
        {
            if ((int) path[i] == moves_lut[i][q])
            {
                lut[path[i]] = 1;
                ++ok_counter;
                break;
            }
        }
    }

    return (ok_counter == 63);
    
}

static void PrintPath(size_t path[64])
{
    int y = 0;
    int x = 0;

    for (; y < 8; ++y)
    {
        for (x = 0; x < 8; ++x)
        {
            printf("%ld ", path[y*8 + x]);
        }
        
        printf("\n");
    }
}