/*
   Implementation of Sudoku-Solver by DEEPAK KUMAR
*/
/*
This program implements the sudoku solving algorithms with some heuristics that make the solving process to take less than a microsecond.

Firstly, rule based solution is implied(via easySquares()). This is exactly how humans solve the Sudoku(but much faster). now if still
some cells are left, we need to guess(via backTrack()). It is not brute force approach, its recursive backtracking as it is doing it
only for the possible ones(the a[][][] condition). Also it starts with the cell having least no. of possibilities. This plays a huge
role in reducing the solving time. Moreover, after each guess, the algorithm again looks for the rule based solution. As a result, my
implementation requires only 5-6 guesses max for a hard Sudoku problem.
The program also tells if the no solutions are possible for a given problem and if multiple solutions are possible. If multiple solutions,
it can print all solutions(after 1-2 changes), the number of solutions given by the variable 'answers'(Deepak.answers here).
*/

#include <iostream>
#include <ctime>
using namespace std;

class SudokuSolver{
    int grid[9][9];
    int a[9][9][9];

    clock_t start;
    int min;
    void updatePossibilityMat(int,int,int,int);

    public:
        bool valid;
        int answers;
        SudokuSolver(int arr[][9])
        {
            for(int i = 0;i < 9;i++)
                for(int j = 0;j < 9;j++)
                    grid[i][j] = arr[i][j];
            start = clock();
        }

        void getPuzzle();
        void printGrid();
        void fillPossibilityMat();
        bool easySquares();
        bool isFeasible(int [][9],int, int, int);
        bool backTrack(int);   // keeps filling in grid[][]
        int goldenCell();

} ;

void SudokuSolver :: getPuzzle(){
    for(int i = 0;i < 9;i++)
        for(int j = 0;j < 9;j++)
            cin >> grid[i][j];

    start = clock();
}

void SudokuSolver :: fillPossibilityMat()
{
    answers = 0;
    int *t = (int *) a;
    for(int i = 0;i < 9*9*9;i++)
        t[i] = 0;
    for(int i = 0;i < 9;i++)
    {
        for(int j = 0;j < 9;j++)
        {
            if(grid[i][j] == 0){
                for(int n = 1;n < 10;n++)
                {
                    if(isFeasible(grid,i,j,n)){
                        a[i][j][n-1] = 1;
                        //cout << i << " " << j << " " << n << endl;
                    }
                }
            }
        }
    }
}

bool SudokuSolver :: easySquares()
{
    bool repeat;
    repeat = false;
    valid = true;

    //cout <<  "entered" << endl;
    for(int i = 0;i < 9;i++)
    {
        for(int j = 0;j < 9;j++)
        {
             int temp,count = 0;   // count gives no. of possible values for each cell
             if(grid[i][j] == 0){
                 for(int n = 1;n < 10;n++)
                 {
                     //cout << a[i][j][n-1] << endl;
                     if(a[i][j][n-1]){
                         count++;
                         temp = n;
                     }
                 }
                 if(count == 0){
                     //cout << "8461" << endl;
                     valid = false;    // checks for validity of input
                     return false;
                 }
                 // if any block contains only 1 possibility, fill
                 if(count == 1) {
                     //cout << i << " " << j << " " << temp << endl;
                     grid[i][j] = temp;
                     updatePossibilityMat(i,j,temp,0);
                     repeat = true;
                 }
             }
        }
    }
    // if a no. not possible in any other column of a row
    for(int i = 0;i < 9;i++)
        for(int n = 1;n < 10;n++)
        {
            int count = 0, temp;
            for(int j = 0;j < 9;j++)
            {
                if(a[i][j][n-1]){
                    count++;
                    temp = j;
                }
            }
            if(count == 1){
                //cout << "entered row one" << i << temp << n << endl;
                grid[i][temp] = n;
                repeat = true;
                updatePossibilityMat(i,temp,n,0);
            }
            /* Note that count = 0 here can either mean that its invalid or that
               the number is already filled in that row  */
        }
    // if a no. not possible in any other row of a column
    for(int i = 0;i < 9;i++)
        for(int n = 1;n < 10;n++)
        {
            int count = 0, temp;
            for(int j = 0;j < 9;j++)
            {
                if(a[j][i][n-1]){
                    count++;
                    temp = j;
                }
            }
            if(count == 1){
                //cout << "entered column one" << temp << i << n << endl;
                grid[temp][i] = n;
                repeat = true;
                updatePossibilityMat(temp,i,n,0);
            }
        }

    // if a  no. not possible in any other cell of a block
    for(int block_row = 0;block_row < 3;block_row++)
        for(int block_col = 0;block_col < 3;block_col++)
            for(int n = 1;n < 10;n++){
                int temp1,temp2,count = 0;
                for(int i = block_row*3;i < block_row*3 + 3;i++)
                    for(int j = block_col*3 ;j < block_col*3 + 3;j++)
                    {
                        if(a[i][j][n-1]){
                            count++;
                            temp1 = i;
                            temp2 = j;
                        }
                    }
                 if(count == 1){
                     //cout << "entered block one" << temp1 << temp2 << n << endl;
                     grid[temp1][temp2] = n;
                     repeat = true;
                     updatePossibilityMat(temp1,temp2,n,0);
                 }
            }
    return repeat;
}

void SudokuSolver :: updatePossibilityMat(int r,int c,int n,int set_to)
{
    for(int x = 0;x < 9;x++)
    {
        a[r][x][n-1] = set_to;
        a[x][c][n-1] = set_to;
        a[r][c][x] = set_to;
    }
    int row = r/3 *3;
    int col = c/3 *3;
    for(int x = row;x < row+3;x++)
        for(int y = col;y < col+3;y++)
            a[x][y][n-1] = set_to;
}

void SudokuSolver :: printGrid() {
    cout << (clock() - start)/(double)CLOCKS_PER_SEC << endl;
	cout << "\n" << endl;
	for (int i = 0; i < 9; i++)
	{

	    if (i%3 == 0){
	        cout << "     ";
	        for(int k = 0;k < 33;k++)
	        cout << "-" ;
	        cout << endl;
	    }
		cout << "     ";
		for (int j = 0; j < 9; j++)
		{
		    if(j%3 == 0)  cout << "  ";
			cout << grid[i][j] ;
			cout << "  ";
		}

		cout << "\n" << endl;
	}
	cout << "     ";
	for(int k = 0;k < 33;k++)
	cout << "-" ;
	cout << endl;
	//cout << (clock() - start)/(double)CLOCKS_PER_SEC << endl;
}

bool SudokuSolver :: isFeasible(int arr[][9], int row, int column, int val)
{
    for (int i = 0; i < 9; i++)       // for row
         if (arr[row][i] == val )
              return false;

	for (int i = 0; i < 9; i++)       // for column
	     if (arr[i][column] == val )
	    	  return false;

    int row_of_block = row/3;
	int col_of_block = column/3;

    for (int i = row_of_block * 3; i < (row_of_block*3 + 3); i++){      // for block
		for (int j = col_of_block * 3; j < (col_of_block*3 + 3); j++){
				if (arr[i][j] == val)
				     return false;
		}
	}

    return true;
}

bool SudokuSolver :: backTrack (int rc)
{
    if(rc == -1){
        answers++;
	    printGrid();
        if(answers > 1)
            return true;
        return false;
    }
    int row = rc / 9;
    int column = rc % 9;

    for(int n = 1;n < 10;n++)
    {
        if(a[row][column][n-1]){
            int temp_grid[9][9];
            int temp_pos[9][9][9];

            for(int i = 0;i < 9;i++)
                for(int j = 0;j < 9;j++)
                    temp_grid[i][j] = grid[i][j];

            for(int i = 0;i < 9;i++)
                for(int j = 0;j < 9;j++)
                    for(int k = 0;k < 9;k++)
                        temp_pos[i][j][k] = a[i][j][k];

            grid[row][column] = n;
            updatePossibilityMat(row,column,n,0);
            while(easySquares());   // must be undone after the call

            if(valid){
            /* the next 2 are awesome lines. using them u can return control from a recurring fuction
            to the main function even if the recurring function is deep inside. Once returned true,
            it will continuously return true until main is reached		*/
                if(backTrack(goldenCell()))
                    return true;
            }
            //cout << row << " " << column << " " << n << endl;

            for(int i = 0;i < 9;i++)
                for(int j = 0;j < 9;j++)
                    grid[i][j] = temp_grid[i][j];

            for(int i = 0;i < 9;i++)
                for(int j = 0;j < 9;j++)
                    for(int k = 0;k < 9;k++)
                        a[i][j][k] = temp_pos[i][j][k];

            a[row][column][n-1] = 0;
        }
    }
    grid[row][column] = 0;
	return false;
}

int SudokuSolver :: goldenCell()
{
    int rc = -1;
    min = 10;
    int count;
    for(int i = 0;i < 9;i++){
        for(int j = 0;j < 9;j++){
            count = 0;
            for(int k = 0;k < 9;k++)
            {
                if(a[i][j][k])
                   count++ ;
            }
            //cout << count << "  ";
            if((count > 0) && (count < min)){
                min = count;
                rc = i*9 + j;
            }
        }
        //cout << endl;
    }
    return rc;
}

int main()
{
    int temp_grid[9][9] = {0,8,0,0,0,9,7,4,3,
                           0,5,0,0,0,8,0,1,0,
                           0,1,0,0,0,0,0,0,0,
                           8,0,0,0,0,5,0,0,0,
                           0,0,0,8,0,4,0,0,0,
                           0,0,0,3,0,0,0,0,6,
                           0,0,0,0,0,0,0,7,0,
                           0,3,0,5,0,0,0,8,0,
                           9,7,2,4,0,0,0,5,0 };

    SudokuSolver Deepak(temp_grid);
    //Deepak.getPuzzle();       // if you want to input the problem
    //Deepak.printGrid();

    Deepak.fillPossibilityMat();
    while(Deepak.easySquares());
    //Deepak.printGrid();

    if(Deepak.valid){
        //cout << Deepak.goldenCell() << endl;
        if (Deepak.backTrack(Deepak.goldenCell()))
             cout << "multiple solutions" << endl;
	    else if(Deepak.answers == 0)
	         cout << "No solution possible !!!" << endl;
	    else
	         cout << "unique solution" << endl;
	}
    else
        cout << "No solution possible" << endl;


	system("pause");
	return 0;
}

/*{0,3,0,0,4,1,5,0,0,
                           0,1,0,7,0,0,0,0,0,
                           0,0,7,0,3,0,0,0,4,
                           2,0,6,0,0,0,9,0,0,
                           0,0,0,6,0,4,0,0,0,
                           0,0,5,0,0,0,6,0,3,
                           5,0,0,0,9,0,7,0,0,
                           0,0,0,0,0,5,0,2,0,
                           0,0,4,3,7,0,0,8,0 } ; */

/*{0,8,0,0,0,9,7,4,3,
                           0,5,0,0,0,8,0,1,0,
                           0,1,0,0,0,0,0,0,0,
                           8,0,0,0,0,5,0,0,0,
                           0,0,0,8,0,4,0,0,0,
                           0,0,0,3,0,0,0,0,6,
                           0,0,0,0,0,0,0,7,0,
                           0,3,0,5,0,0,0,8,0,
                           9,7,2,4,0,0,0,5,0 }; */
