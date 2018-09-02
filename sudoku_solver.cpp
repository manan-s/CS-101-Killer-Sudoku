#include <iostream>
using namespace std;

/* each cage will be assigned a unique id.
 * the cells array for each cage will contain the id of the cells belonging to the cage.
 * capacity of the cage = no of cells
 * cell id = row*10 + col (where row is the row number and col is the column number of the cell)
 * each cell will have a unique cell id.
 * each cage will have a unique id which equals the index of the cage in the array of cages.
 */
struct cage
{
	int sum, capacity;
	int cells[81];
	int possible_num[9] ;
};

/* If a cell is not assigned any number, then value = 0.
 * each cell will have cage_id, i.e. the id of the cage to which the cell belongs
 */
struct cell {
	int value;
	int cage_id;
};


bool solve_sudoku(cage[],cell[9][9], int);
void print_board(cell[9][9]);
bool isCageFilled(cage, cell[9][9]);
bool isCageSumSatisfied(cage, cell[9][9]);
bool isSafe(cell[9][9], cage[], int, int, int);
bool findEmptyCell(cage[], cell[9][9], int&, int&, int);
void processCages(cage[], int);

int main() {

	cell board[9][9];

	//initialize board
	for (int i=0; i<9; i++)
		for (int j=0; j<9; j++)
			board[i][j].value = 0;

	//read input
	int no_of_cages;
	cin>>no_of_cages;
	cage Cage[no_of_cages];

	for (int cage_id = 0; cage_id<no_of_cages; cage_id++) {
        
		int cage_sum, no_of_cells;
		cin >> no_of_cells >> cage_sum;
        
		Cage[cage_id].sum = cage_sum;
		Cage[cage_id].capacity = no_of_cells;

		int row, col;
		for (int j=0; j<no_of_cells; j++) {			
			cin >> row >> col;
			Cage[cage_id].cells[j] = row*10 + col;
			board[row][col].cage_id = cage_id;
		}
		
		//if a cage has only one cell, then assign cage_sum to that cell
		if(no_of_cells == 1) {
			board[row][col].value = cage_sum; 
		}
	}
	
	processCages(Cage,no_of_cages);
	
	if(solve_sudoku(Cage, board, no_of_cages))
        print_board(board);
    else
        cout<<"Cant solve the given sudoku puzzle :( \n";

	return 0;
}


bool solve_sudoku(cage Cage[], cell board[9][9], int no_of_cages) {
 
    int row=0, col=0;
    if(!findEmptyCell(Cage, board, row ,col, no_of_cages)) 
	{
		return true;
    }
	
	cage currentCage = Cage[board[row][col].cage_id];
    for (int num=1; num<=9; num++) {
    	if(currentCage.possible_num[num-1] == 0 ) continue ;
        if (isSafe(board, Cage, row, col, num)) {
            board[row][col].value = num;

            if (isCageSumSatisfied(currentCage, board)) {
	
                if(solve_sudoku(Cage, board, no_of_cages) )
                	return true;
                	
            }
            board[row][col].value = 0;
        }
    }
    return false;
}

void print_board(cell board[][9])
{
    for(int row=0 ; row<9 ; row++)
    {
        for(int col=0 ; col<9 ; col++)
            cout<<board[row][col].value <<" " ;
        cout<<"\n" ;
    }
}


/* Checks whether all the cells in a particular cage are filled.
 * returns true if they are filled
 * else returns false
 */
bool isCageFilled(cage Cage, cell board[9][9]) {
	for (int cell=0; cell<Cage.capacity; cell++) {
	    int row, col;
	    row = Cage.cells[cell]/10;
	    col = Cage.cells[cell]%10;
		if (board[row][col].value == 0)
			return false;
	}
	return true;
}

/* checks whether the sum of all the cells in a cage
 * equals the cage sum and returns true if it is.
 * Returns true if the cage is not yet filled.
 * else returns false
 */
bool isCageSumSatisfied(cage Cage, cell board[9][9]) {
	if (!isCageFilled(Cage, board)) {return true;}
	int sum = 0, row, col;
	for (int cell=0; cell<Cage.capacity; cell++) {
	    row = Cage.cells[cell]/10;
	    col = Cage.cells[cell]%10;
		sum += board[row][col].value;
	}
	if (sum == Cage.sum)
		{ return true;}
	else
		{return false;}
}

/* Returns true if there is no conflict in the row while assigning a number
 * returns false if the number is already used.
 */
bool UsedInRow(cell board[9][9], int row, int num)
{
    for (int col = 0; col < 9; col++)
        if (board[row][col].value == num)
            return true;
    return false;
}


/* Returns true if there is no conflict in the column while assigning a number
 * returns false if the number is already used.
 */
bool UsedInCol(cell board[9][9], int col, int num)
{
    for (int row = 0; row < 9; row++)
        if (board[row][col].value == num)
            return true;
    return false;
}


/* Returns a boolean which indicates whether any assigned entry
 * within the specified 3x3 box matches the given number.
 */
bool UsedInBox(cell board[9][9], int boxStartRow, int boxStartCol, int num)
{
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (board[row+boxStartRow][col+boxStartCol].value == num)
                return true;
    return false;
}


/*Returns a true if the number is already present in the cage
   Else returns false
*/
bool UsedInCage(cell board[9][9], cage Cage, int num)
{
    int no_of_cells = Cage.capacity  ;
    for(int cell=0; cell < no_of_cells; cell++)
    {
         int row = Cage.cells[cell]/10 ;
         int col = Cage.cells[cell]%10 ;
            if(board[row][col].value == num )
                return true ;
    }
    return false ;
}


/* Returns a boolean which indicates whether it will be legal to assign
 * num to the given row,col location.
 */
bool isSafe(cell board[9][9], cage Cage[], int row, int col, int num)
{
    /* Check if 'num' is not already placed in current row,
       current column, current 3x3 box and the current cage */
    int cage_id = board[row][col].cage_id;
    return !UsedInRow(board, row, num) &&
           !UsedInCol(board, col, num) &&
           !UsedInBox(board, row - row%3 , col - col%3, num) &&
           !UsedInCage(board, Cage[cage_id], num) ;
}

/* Searches the grid to find a cell that is still unassigned.
 * Sets the reference parameters row and col to the row and column of this cell
 * returns false if no such cell is found.
 */
bool findEmptyCell(cage Cages[], cell board[9][9], int &row, int &col, int no_of_cages) {
	int id;
	for (id=0; id<no_of_cages; id++) {
		int n = Cages[id].capacity;
		
		for (int j=0; j<n; j++) {
			row = Cages[id].cells[j]/10;
			col = Cages[id].cells[j]%10;
			
			if(board[row][col].value == 0) return true;
		}
	}
	return false;
}

/*
 * This function is used to fill the possible_num array of the cages
 * It is used to check whether a particular number is possible to assign in the cage.
 * if a number x can be present in the cage, then possible_num[x] = 1
 * else possible_num[x] = 0
 */
void processCages(cage Cage[], int no_of_cages){
	
	for(int cage_id=0; cage_id<no_of_cages; cage_id++) {
	
		for(int x=0; x<9; x++)
    		Cage[cage_id].possible_num[x] = 1 ;
    
	    int cage_sum = Cage[cage_id].sum;
		int no_of_cells = Cage[cage_id].capacity ;

		switch(no_of_cells)
		{      
			case 2:
            	switch(cage_sum)
            	{
	                case 3:
	                    for(int i=2;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
	                    break ;
	                case 4:
	                    for(int i=3;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;  Cage[cage_id].possible_num[1] = 0 ;
	                    break ;
	                case 5:
	                    for(int i=4;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
	                    break ;
	                case 6:
	                    for(int i=5;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;  Cage[cage_id].possible_num[2] = 0 ;
	                    break ;
	                case 7:
	                    for(int i=6;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
	                    break ;
	                case 8:
	                    for(int i=7;i<9;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[3] = 0 ;
	                    break ;
	                case 9:
	                    Cage[cage_id].possible_num[8] = 0 ;  
	                    break ;
	                case 10:
	                    Cage[cage_id].possible_num[4] = 0 ;
	                    break ;
	                case 11:
	                    Cage[cage_id].possible_num[0] = 0 ;
	                    break ;
	                case 12:
	                    Cage[cage_id].possible_num[0] = 0 ;  Cage[cage_id].possible_num[1] = 0 ; Cage[cage_id].possible_num[5] = 0 ;
	                    break ;
	                case 13:
	                    Cage[cage_id].possible_num[0] = 0 ; Cage[cage_id].possible_num[1] = 0 ; Cage[cage_id].possible_num[2] = 0 ;
	                    break ;
	                case 14:
	                    Cage[cage_id].possible_num[0] = 0 ; Cage[cage_id].possible_num[1] = 0 ; Cage[cage_id].possible_num[3] = 0 ;
	                    Cage[cage_id].possible_num[6] = 0 ; Cage[cage_id].possible_num[2] = 0 ;
	                    break ;
	                case 15:
	                    Cage[cage_id].possible_num[0] = 0 ; Cage[cage_id].possible_num[1] = 0 ; Cage[cage_id].possible_num[3] = 0 ;
	                    Cage[cage_id].possible_num[4] = 0 ; Cage[cage_id].possible_num[2] = 0 ;
	                    break ;
	                case 16:
	                    for(int i=0;i<6;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[7] = 0 ;
	                    break ;
	                case 17:
	                    for(int i=0;i<7;i++) Cage[cage_id].possible_num[i] = 0 ;
	                    break ;
	                default :
	
	                    break;

            	};
            break ;
            
            case 3:
            switch(cage_sum)
            {
                case 19:
                    Cage[cage_id].possible_num[0] = 0 ;
                    break ;
                case 20:
                    Cage[cage_id].possible_num[0] = 0 ; Cage[cage_id].possible_num[1] = 0 ;
                    break ;
                case 6:
                    for(int i=3;i<9;i++) Cage[cage_id].possible_num[i] = 0 ; 
                    break ;
                case 7:
                    for(int i=4;i<9;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[2] = 0 ;
                    break ;
                case 8:
                    for(int i=5;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 9:
                    for(int i=6;i<9;i++) Cage[cage_id].possible_num[i]= 0 ;  
                    break ;
                case 10:
                    Cage[cage_id].possible_num[7] = 0 ; Cage[cage_id].possible_num[8] = 0 ;
                    break ;
                case 11:
                    Cage[cage_id].possible_num[8] = 0 ;
                    break ;
                case 21:
                    for(int i=0;i<3;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 22:
                    for(int i=0;i<4;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 23:
                    for(int i=0;i<5;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[6] = 0 ;
                    break ;
                case 24:
                    for(int i=0;i<6;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                default :
                    break;

            };
            break ;
            
            case 4:
            switch(cage_sum)
            {
                case 10:
                    for(int i=4;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 11:
                    for(int i=5;i<9;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[3] = 0 ;
                    break ;
                case 12:
                    for(int i=6;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 14:
                    Cage[cage_id].possible_num[8] = 0 ; 
                    break ;
               
                case 13:
                    for(int i=7;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 26:
                    Cage[cage_id].possible_num[0] = 0 ;
                    break ;
                case 27:
                    Cage[cage_id].possible_num[0] = 0 ; Cage[cage_id].possible_num[1] = 0 ;
                    break ;
                case 28:
                    for(int i=0;i<3;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 29:
                    for(int i=0;i<4;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[5] = 0 ;
                    break ;
                case 30:
                    for(int i=0;i<5;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                default :
                    break;
            };
            break ;
            
            case 5:
            switch(cage_sum)
            {
                case 15:
                    for(int i=5;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 16:
                    for(int i=6;i<9;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[4] = 0 ;
                    break ;
                case 17:
                    for(int i=7;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 18:
                    Cage[cage_id].possible_num[8] = 0 ; 
                    break ;               
                case 32:
                    Cage[cage_id].possible_num[0] = 0 ;
                    break ;
                case 33:
                    Cage[cage_id].possible_num[0] = 0 ; Cage[cage_id].possible_num[1] = 0 ;
                    break ;
                case 34:
                    for(int i=0;i<3;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[4] = 0 ;
                    break ;
                case 35:
                    for(int i=0;i<4;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                default :
                    break;
            };
            break ;
            
            case 6:
           	switch(cage_sum)
            {
                case 21:
                    for(int i=6;i<9;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                case 22:
                    for(int i=7;i<9;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[5] = 0 ;
                    break ;
                case 23:
                    Cage[cage_id].possible_num[8] = 0 ;
                    break ;
                case 37:
                    Cage[cage_id].possible_num[0] = 0 ; 
                    break ;
                case 38:
                    for(int i=0;i<2;i++) Cage[cage_id].possible_num[i] = 0 ; Cage[cage_id].possible_num[3] = 0 ;
                    break ;
                case 39:
                    for(int i=0;i<3;i++) Cage[cage_id].possible_num[i] = 0 ;
                    break ;
                default :
                   break;
            };
            break ;
            
            case 7:
            switch(cage_sum)
            {
            	case 28:
                    Cage[cage_id].possible_num[7] = 0 ; Cage[cage_id].possible_num[8] = 0 ;
                    break ;
                case 29:
                    Cage[cage_id].possible_num[6] = 0 ; Cage[cage_id].possible_num[8] = 0 ;
                    break ;
                case 41:
                    Cage[cage_id].possible_num[0] = 0 ; Cage[cage_id].possible_num[2] = 0 ;
                    break ;
                case 42:
                    Cage[cage_id].possible_num[0] = 0 ; Cage[cage_id].possible_num[1] = 0 ;
                    break ;            		
			}
           
            case 8:
            switch(cage_sum)
            {
                case 36:
                    Cage[cage_id].possible_num[8] = 0 ;
                    break ;
                case 37:
                    Cage[cage_id].possible_num[7] = 0 ;
                    break ;
                case 38:
                    Cage[cage_id].possible_num[6] = 0 ;
                    break ;
                case 39:
                    Cage[cage_id].possible_num[5] = 0 ;
                    break ;
                case 40:
                    Cage[cage_id].possible_num[4] = 0 ;
                    break ;
                case 41:
                    Cage[cage_id].possible_num[3] = 0 ;
                    break ;
                case 42:
                    Cage[cage_id].possible_num[2] = 0 ;
                    break ;
                case 43:
                    Cage[cage_id].possible_num[1] = 0 ;
                    break ;
                case 44:
                    Cage[cage_id].possible_num[0] = 0 ;
                    break ;
                default :
                    break;
            }
            break;
    	}
	}
}

