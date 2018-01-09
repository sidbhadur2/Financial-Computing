//
//  main.cpp
//  N_queens
//
//  Created by Siddharth Bhaduri on 8/28/16.
//  Copyright © 2016 Siddharth Bhaduri. All rights reserved.
//


// N Queens Problem via (Backtracking, which is implemented by) Recursion
// Written by Prof. Sreenivas for IE523: Financial Computing

#include <iostream>
#include "n_queens_siddharth_bhaduri.h"
#include "n_queens_all_solutions_siddharth_bhaduri.h"

int main (int argc, char * const argv[])
{
    // --------------------------------To Run Part 1, simply comment out line 31 - 36
    Board x;
    int board_size;
    sscanf (argv[1], "%d", &board_size);
    //x.initialize(board_size);
    //x.print_board(); 
    x.nQueens(board_size);
    
    
    
    // ---------------------------------------  Part 2 for All Solutions Code is below this line
    // To run this part = Part 2 , comment out lines 20 - 25
    Board1 x1;
    int board_size1;
    sscanf (argv[1], "%d", &board_size1);
    //x.initialize(board_size);
    //x.print_board();
    x1.nQueens_all(board_size1);
    
    return 0;
}