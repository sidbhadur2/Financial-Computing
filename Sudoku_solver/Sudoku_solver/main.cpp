//
//  main.cpp
//  Sudoku_solver
//
//  Created by Siddharth Bhaduri on 9/4/16.
//  Copyright © 2016 Siddharth Bhaduri. All rights reserved.
// Soduku Solver using Brute-Force Search implemted using
// recursion.
// Written for IE523: Financial Computation by Prof. Sreenivas
// and GE523: Discrete Event Systems
//

#include <iostream>
#include "sudoku.h"
#include "sudoku_all_solutions.h"

int main (int argc, char * const argv[])
{
    Sudoku x;
    x.read_puzzle(argc, argv);
    x.print_puzzle();
    x.Solve(0,0);
    cout <<endl;
    cout <<"Final " ;
    x.print_puzzle();
    
    // ---------------- Use code below this line for part 2 - all solutions
    
//    Sudoku1 x1;
//    x1.read_puzzle_all(argc, argv);
//    x1.print_puzzle_all();
//    x1.Solve_all(0,0);
    
    return 0;
}