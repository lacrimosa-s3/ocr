#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include "solver.h"


void exit_help()
{
    char help[] = "format: solver grid_** ";
    errx(EXIT_FAILURE, "%s", help);
}

void test_solver(int argc, char** argv)
{
    if (argc != 2)
        exit_help();

    FILE* grid_file;
    char line[12];

    grid_file = fopen(argv[1], "r");
    if (grid_file == NULL)
        errx(EXIT_FAILURE, "%s", "Error file");

    int grid[9][9];
    int i = 0;

    //cree une grille a partir d'un fichier
    while(fgets(line,12,grid_file))
    {
        //skip les lignes vide
        if (line[0] == '\n')
            continue;

        int i0 = 0;
        //transforme chaque char de la ligne en int pui ajout dans une grille
        int j = 0;
        while (line[i0] != 0)
        {
            //skip les espaces
            if (line[i0] == ' ' || line[i0] == '\n')
                i0++;
            
            //remplace les points par 0 dans la grille
            else if (line[i0] == '.')
            {
                grid[i][j] = 0;
                j++;
                i0++;
            }

            //remplace le char par son int dans la grille
            else if (line[i0] >= '1' && line[i0] <= '9'){
                grid[i][j] = (int)line[i0] - '0';
                j++;
                i0++;
            }

            //sinon c'est que il y q un char autre qu'un chiffre ou un espace 
            //donc c'est une erreur !
            else
                errx(EXIT_FAILURE, "%s", "wrong grid format");
        }
        i++;
    }
    int ok = solver(grid);
    if (ok == 0)
        errx(EXIT_FAILURE, "%s", "Sudoku unsolvable !");
    else
    {
        char* name = argv[1];
        strcat(name,".result");

        FILE* grid_solved_file = fopen(name,"w");
        char data[100] = {};
        int i0 = 0;
        for (int i1 = 0; i1 < 3; i1++)
        {
            char c;
            for(int j1 = 0; j1 < 9 ; j1++)
            {
                if (j1 % 3 == 0 && j1 != 0)
                {
                    c = ' ';
                    data[i0] = c;
                    i0++;
                }
                c  = (char)grid[i1][j1]+'0';
                data[i0] = c;
                i0++;
            }

            c = '\n';
            data[i0] = c;
            i0++;

            if (i1 % 3 == 0 && i1 != 0)
            {
                data[i0] = c;
                i0++;
            }
        }
        data[i0] = 0;
        fputs(data,grid_solved_file);
        fclose(grid_solved_file);
        fclose(grid_file);
    }
}

void prints (char str[])
{
    printf("%s",str);
}

int main (int argc, char** argv)
{
    test_solver(argc,argv);
    return EXIT_SUCCESS;
}
