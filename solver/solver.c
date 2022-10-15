#include "solver.h"
#include <stdio.h>

// Fonction qui return:
// 1 si la le nombre à la place (i,j) de la matrice est valide
// 0 sinon

int check_case(int n,int i, int j, int board[9][9])
{
    // check line
    for (int j0 = 0; j0 < 9; j0++)
    {
    if (board[i][j0] == n && j0 != j)
        {
            return 0;
        }
    }

    // check column
    for (int i0 = 0; i0 < 9; i0++)
    {
        if (board[i0][j] == n && i0 != i){
        return 0;
        }
    }

    // determine the start of the square
    int i0 = 6;
    int j0 = 6;

     if (i < 6)
        i0 = 3;

    if (i < 3)
        i0 = 0;

    if (j < 6)
        j0 = 3;

    if(j < 3)
        j0 = 0;

   
    // check square
    for (int i1 = i0; i1 < i0 + 3 ; i1++)
    {
        for (int j1 = j0; j1 < j0 + 3; j1++)
        {
            if (board[i1][j1] == n)
            {
                if (i1 != i && j1 != j)
                    return 0;
            }
        }
    }

    return 1;
}

//regarde si le sudoku est possible
int check_sudoku (int board[9][9]){

    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (board[i][j] != 0)
            {
                int check = check_case(board[i][j],i,j,board);
                if (check == 0)
                {
                    return 0;
                }
            }
        }
    }

    return 1;
}

// return une liste contenant les 2 indexes de la prochaine case vide
// si il n'y en a pas , i,j = -1,-1
int *next_empty (int board[9][9])
{
    int res0[2] = {-1,-1};
    int* res = res0;
    for (int i0 = 0; i0 < 9; i0++)
    {
        for (int j0 = 0; j0 < 9; j0++)
        {
            if (board[i0][j0] == 0)
            {    
                res[0] = i0;
                res[1] = j0;
                return res;
            }
        }
    }
    return res;
}

// résout le sudoku (si possible) retourn 0 si il y n'a pas de solution 1 sinon
int solver (int board[9][9])
{
    int *index;
    index = next_empty(board);

    if (index[0] == -1)
    {
        return 1;    
    }

    int i = index[0];
    int j = index[1];

    for (int nb = 1; nb <= 9; nb++)
    {
        if (check_case(nb,i,j,board) != 0 )
        {
            board[i][j] = nb;
            if (solver(board))
                return 1;
        }
        board[i][j] = 0;
    }
    return 0;
}

void print (int board[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("%i ",board[i][j]);
        }
        printf("%c",'\n');
    }
}

int mat[9][9] = {
{0,0,0,0,0,4,5,8,0},
{0,0,0,7,2,1,0,0,3},
{4,0,3,0,0,0,0,0,0},
{2,1,0,0,6,7,0,0,4},
{0,7,0,0,0,0,2,0,0},
{6,3,0,0,4,9,0,0,1},
{3,0,6,0,0,0,0,0,0},
{0,0,0,1,5,8,0,0,6},
{0,0,0,0,0,6,9,5,0}
};

