#include <stdio.h>
#include <stdlib.h>
#include "solver.h"
#include <string.h>
int canBeDone(int*, size_t, size_t,int);
int solver_func(int*);
void init_grid(char*,int*);
int empty_cell(int*,size_t*,size_t*);
char* name_file_out(char*);


int solvMain(char* filename)
{
	printf("yo");
	FILE *fileRead;

	/*if(argc !=2)
	{
		printf("Please enter a file.");
		return 0;

	}*/
	 
	//Open your_file in read-only mode
	fileRead = fopen(filename, "r");

 	if(fileRead == NULL)
    	{
		printf("Your file must be filled");
        	perror("fopen");
        	exit(EXIT_FAILURE);
    	}  

	//Create a buffer with the space needed
	char buffer[110];
   	//Seek to the beginning of the file
   	fseek(fileRead, 0, SEEK_SET);

   	//Read Data and put it on the buffer
   	size_t useless = fread(buffer, 110, 1, fileRead);
	useless ++;
	char* buffer_pt = buffer;
	fclose(fileRead);
			
	//create an int array of 81 element 
	int grid2[81];
	int* grid_pt = grid2;
	init_grid(buffer_pt,grid_pt);
	int good = solver_func(grid_pt);
	if(!good)
	{
		printf("No solution found.");
		return 0;
	}
			
	//Writing the file
	FILE *fileWrite;
	fileWrite = fopen(name_file_out(filename),"w");

	for(size_t index = 0; index<82; index++)
	{
		fprintf(fileWrite,"%i",*(grid_pt+index));
		if (index%3 == 2)
		{
			if(index%9 == 8)
			{
				if(index%27 == 26)
				{
					fprintf(fileWrite,"\n");
				}
				fprintf(fileWrite,"\n");
			}
			else
			{
				fprintf(fileWrite," ");
			}
		}
	}
	fclose(fileWrite);
	return 1;
}

char* name_file_out(char* name)
{
	size_t len_filename = 0;
	while(*(name+len_filename) != 0)
	{
		len_filename++;
	}

	char* str = malloc((len_filename+7)*sizeof(char));
	if (str == NULL)
	{
		printf("Not enought memory!");
		return "error";
	}
	for(size_t i = 0; i<len_filename; i++)
	{
		*(str+i) = *(name+i);
	}
	*(str+len_filename) = '.';
	*(str+len_filename+1) = 'r';
	*(str+len_filename+2) = 'e';
	*(str+len_filename+3) = 's';
	*(str+len_filename+4) = 'u';
	*(str+len_filename+5) = 'l';
	*(str+len_filename+6) = 't';
	*(str+len_filename+7) = 0;

	return str;
}

void init_grid(char *buffer_pt,int* grid)
{
	size_t index_grid = 0;
	char ch;

	for(size_t index_buffer = 0; index_buffer<110; index_buffer++)
	{
		ch = *(buffer_pt + index_buffer);
		if(ch == 46)
		{
			*(grid + index_grid) = 0;
			index_grid++;
		}
		else
		{
			if(ch>48 && ch<58)
			{
				*(grid + index_grid) = (int)ch - 48;
				index_grid++;
			}
		}
	}
}

int canBeDone(int*grid, size_t row, size_t col, int n)
{
	//Set the position of the top left cell in the square 
	int col_square = (col/3)*3;
	int row_square = (row/3)*3;

	//Searching
	for(size_t index = 0; index < 9; index++)
	{
		if (*(grid + row*9 + index) == n) return 0; //search n in the column
		if (*(grid + index*9 +col) == n) return 0;  //search n in the row
		if (*(grid + (row_square+(index%3))*9 + (col_square+(index/3))) == n) return 0; //search n in the square
	}

	return 1;
}

int emptyCell(int* grid, size_t *row, size_t *col) 
{
	for (size_t row_index = 0; row_index < 9; row_index++) 
	{
    		for (size_t col_index = 0; col_index < 9; col_index++) 
		{
      		if (*(grid+row_index*9 + col_index)==0)  
			{
        		*row = row_index;
        		*col = col_index;
				return 1; //positive research
      		}
    	}
  	}
  	return 0;//negatif research
}

int solver_func(int *grid)
{
	size_t row;
	size_t col;
	if(!emptyCell(grid,&row,&col)) //positive exit case (all cell are filled)
	{
		return 1;
	}
	for(int value = 1; value<10; value++)
	{
		if (canBeDone(grid,row,col,value))
		{
			*(grid + row*9 + col) = value;
			if(solver_func(grid))
			{
				return 1; 
			}
			else
			{
				*(grid + row*9 + col) = 0;
			}
		}
			
	}
	return 0;
}
