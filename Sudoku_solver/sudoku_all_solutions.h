//
//  sudoku_all_solutions.h
//  Sudoku_solver
//
//  Created by Siddharth Bhaduri on 9/8/16.
//  Copyright © 2016 Siddharth Bhaduri. All rights reserved.
//

#ifndef sudoku_all_solutions_h
#define sudoku_all_solutions_h

#include <iostream>
#include <map>
#include <vector>
#include <fstream>

using std::vector;
using namespace std;
class Sudoku1
{
    // Private
    int puzzle[9][9];
    
    int count; // number of solutions
    
    //private vector
    vector<int> sudoku_numbers;
    
    // Private member function that checks if the named row is valid
    bool row_valid(int row)
    {
        std::map<int, int> counts;
        
        for (int i = 0; i < 9; i++) {
            counts[puzzle[row][i]]++; // increment the counter for the current value
        }
        
        for (int j = 1; j < 10; j++){
            if(counts[j] >1){
                return false;
            }
        }
        
        // write code that checks if "row" is valid
        return true;
    }
    
    // Private member function that checks if the named column is valid
    bool col_valid(int col)
    {
        std::map<int, int> counts_col;
        
        for (int j = 0; j < 9; j++) {
            counts_col[puzzle[j][col]]++; // increment the counter for the current value
        }
        
        for (int k = 1; k < 10; k++){
            if(counts_col[k] >1){
                return false;
            }
        }
        // check validity of "col"
        return true;
    }
    
    // Private member function that checks if the named 3x3 block is valid
    bool block_valid(int row, int col)
    {
        
        std::map<int, int> counts_block;
        
        int i = row - row%3;
        int j = col - col%3;
        
        int l = i + 2;
        int m = j + 2;
        
        for (int row1 = i; row1 <= l; row1++){
            for (int col1 = j; col1 <= m; col1++) {
                counts_block[puzzle[row1][col1]]++; // increment the counter for the current value
            }
        }
        
        for (int k = 1; k < 10; k++){
            if(counts_block[k] >1 ){
                return false;
            }
        }
        
        // check 3 x 3 block validity
        return true;
    }
    
public:
    // Public member function that reads the incomplete puzzle
    // we are not doing any checks on the input puzzle -- that is,
    // we are assuming they are indeed valid
    void read_puzzle_all(int argc, char * const argv[])
    {
        // write code that reads the input puzzle using the
        // guidelines of figure 23 of the bootcamp material
        
        ifstream input_file(argv[1]);
        
        int last;
        //  flag = false;
        
        if (input_file.is_open()){ // if exists in local directory
            while(input_file >> last){ // if not at end of file
                // std::cout << last << endl;
                sudoku_numbers.push_back(last);
            }
        }
        
        else{
            std::cout << "File does not exist!!" << endl;
        }
        
        int count = 0;
        for (int l = 0; l < 9; l++){
            for (int m = 0; m < 9; m++, count ++){
                puzzle[l][m] = sudoku_numbers[count];
            }
        }
    }
    
    // Public member function that prints the puzzle when called
    void print_puzzle_all()
    {
        
        std::cout << std::endl << "Board Position" << std::endl;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                // check if we have a legitimate integer between 1 and 9
                if ((puzzle[i][j] >= 1) && (puzzle[i][j] <= 9))
                {
                    // printing initial value of the puzzle with some formatting
                    std::cout << puzzle[i][j] << " ";
                }
                else {
                    // printing initial value of the puzzle with some formatting
                    std::cout << "X ";
                }
            }
            std::cout << std::endl;
        }
    }
    
    bool flag_all(int & row, int & col){
        
        for (row = 0; row < 9; row++ ){
            for (col = 0; col < 9; col++)
            {
                if (puzzle[row][col] == 0)
                {
                    // cout << "EMPTY SPOT" <<endl;
                    return false;
                }
            }
            
        }
        return true; // not found - done with recursion
    }
    
    // Public member function that (recursively) implements the brute-force
    // search for possible solutions to the incomplete Sudoku puzzle
    bool Solve_all(int row, int col)
    {
        // this part of the code identifies the row and col number of the
        // first incomplete (i.e. 0) entry in the puzzle.  If the puzzle has
        // no zeros, the variable row will be 9 => the puzzle is done, as
        // each entry is row-, col- and block-valid...
        // use the pseudo code of figure 3 of the description
        
        int i, j = 0;
        
        if (flag_all(row, col)){
            return true;
        }
        
        for (i = row; i < 9; i ++){
            for (j = col; j < 9; j++){
                for (int k = 1; k <= 9; k++){
                    puzzle[i][j] = k;
                    if( (row_valid(i)) && (col_valid(j)) && (block_valid(i,j)) ){
                        if (Solve_all(i,j)){
                            count ++;
                            std:: cout << endl;
                            std::cout << "Solution #: " << count;
                            
                            //cout << " Placed SOMETHING" <<endl;
                            print_puzzle_all();
                        } // end if
                    } // end if with all constraints
                } // end for loop trying all k values
                puzzle[i][j] = 0;
                return false;
            }
        }
        return false;
    }
};


#endif /* sudoku_all_solutions_h */
