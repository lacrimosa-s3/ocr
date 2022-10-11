#include "solver.h"

// Taille de la matrice
#define  array_size = 9;
// Case vide associé à 0
#define empty = 0

// Fonction qui return:
// 0 si la le nombre à la place (i,j) de la matrice est valide
// 1 sinon

def int check_case(int n,int i, int j, int[][] board)
{
    // check line
    for (int j0 = 0; j0 < array_size; j0++)
    {
        if (board[i][j0] != n && board[i][j0] != 0){
            return 0;
        }
    }

    // check column
    for (int i0 = 0; i0 < array_size; i0++)
    {
        if (board[i0][j] != n && board[i0][j] != 0){
            return 0;
        }
    }

    // determine the start of the square
    int i0 = 6;
    int j0 = 6;

    if (i <= 3){
        i0 = 0;
        j0 = 0;
    }

    else if (i <= 6){
        i0 = 3;
        j0 = 3;
    }

    // check square
    for (int i1 = i0; i1 < i0 + 3 ; i1++)
    {
        for (int j1 = j0; j1 < j0 + 3; j1++)
        {
            if board[i1][j1] != n && board[i1][j1] != 0{
                return 0;
            }
        }
    }

    return 1;
}

//regarde si le sudoku est possible
def int check_sudoku (int[][] board){

    for (int i = 0; i < board_size; i++){

        for (int j = 0; j < board_size; j++){

            int check = check_case(board[i][j],i,j,board)
            if (check == 0){
                return 0;
            }

        }
    }

    return 1;
}

// return 1 si on a trouver une case vide (coordonné enregistrer dans i et j)
// return 0 si on a rien trouver
def int next_empty (int[][] board, int i, int j ){

    for (int i0 = i, i0 < board_size, i0++){

        for (int j0 = j, j0 < board_size, j0++){

            if (board[i0][j0] == 0)
            {
                i = i0;
                j = j0;
                return 1;
            }
        }
    }
    return 0;
}

// résout le sudoku (si possible) retourn 0 si il y n'a pas de solution 1 sinon
def bool solver (int[][] board)
{
    int i = 0;
    int j = 0;

    if (next_empty(board,i,j) == 0)
    {
        if (check_sudoku != 0)
        {
            return 1;
        }
    }

    for (int nb = 1; nb <= board_size; nb++)
    {
        if (check_case(nb,i,j,board) != 0)
        {
            board[i][j] = nb;
            if (solver(board))
                return 1;

            board[i][j] == empty;
        }
    }
    return 0;
}
