//
//  n_queens_all_solutions_siddharth_bhaduri.h
//  N_queens
//
//  Created by Siddharth Bhaduri on 8/29/16.
//  Copyright © 2016 Siddharth Bhaduri. All rights reserved.
//

#ifndef n_queens_all_solutions_siddharth_bhaduri_h
#define n_queens_all_solutions_siddharth_bhaduri_h

using namespace std;
class Board1
{
    
    // private data member: size of the board
    int size;
    
    // pointer-to-pointer initialization of the board
    int **chess_board;
    
    int count;
    // count for number of solutions
    
    bool check;
    // check for if recursion is done
    
    // private member function:  returns 'false' if
    // the (row, col) position is not safe.
    bool is_this_position_safe(int row, int col)
    {
        // write the appropriate code on your own that returns
        // "true" if the (row,col) position is safe.  If it is
        // unsafe (i.e. some other queen can threaten this position)
        // return "false"
        for (int i = 0; i < size; i++){
            if (chess_board[i][col]){ // check all rows in the same column for a queen
                return false;
            }
        }
        
        for (int j = 0; j < size; j++){
            if (chess_board[row][j]){ // check all columns in the same row for a queen
                return false;
            }
        }
        
        // check diagonals
        // left to right diagonal
        int l = 0;
        int m = 0;
        
        for (l = row, m = col; l < size && m < size; l++, m++){
            if(chess_board[l][m]){
                return false;
            }
        }
        
        for (l = row, m = col; l >= 0 && m >=0; l--, m--){
            if(chess_board[l][m]){
                return false;
            }
        }
        
        //right to left diagonal
        
        for (l = row, m = col; l < size && m >=0; l++, m--){
            if(chess_board[l][m]){
                return false;
            }
        }
        
        for (l = row, m = col; l >= 0 && m < size; l--, m++){
            if(chess_board[l][m]){
                return false;
            }
        }
        return true;
    }
    
    
    void initialize(int n)
    {
        size = n;
        chess_board = new int*[size];
        
        for (int k = 0; k < size; k++){
            chess_board[k] = new int[size];
        }
        for (int i = 0; i < size; i++){
            for (int j = 0; j <= size; j++){
                chess_board[i][j] = 0; // empty board
            }
        }
        // write the appropriate code that uses the pointer-to-pointer
        // method to initialize the (n x n) chessboard.  Once initialized,
        // put zeros in all entries.  Later on, if you placed a queen in
        // the (i,j)-th position, then chessboard[i][j] will be 1.
    }
    
    
    // private member function: prints the board position
    void print_board_all()
    {
        count ++;
        
        // to print out the ---- in the terminal
        std::cout << size << "x" << size << " Solution #: " << count << std::endl;
        for (int k = 1; k<= 2*size; k++){
            std::cout << "-";
        }
        std::cout << std::endl;
        
        
        // write the appropriate code here to print out the solved
        // board as shown in the assignment description
        for (int i = 0; i < size; i++){
            for (int j = 0; j < size; j++){
                if (chess_board[i][j]){
                    std::cout << "Q";
                }
                else{
                    std::cout << "-";
                }
                std:: cout << ' ';
                // prints the boards state
            }
            std::cout << endl;
        }
        
        // to print out the ----
        for (int l = 1; l<= 2*size; l++){
            std::cout << "-";
        }
        std::cout << std::endl;
        
        
        check = true;
        return;
        
    }
    
    
    // private member function: recursive backtracking
    bool solve_all(int col)
    {
      
        // implement the recursive backtracking procedure described in
        // pseudocode format in figure 1 of the description of the first
        // programming assignment
        
      //  int l = 0;
        
        if (col >= size){
            //print_board_all();
            //std::cout << "Total solutions are " << count << " in number." << std::endl;
            return true;
        }
        
        else {
            for (int l = 0; l < size; l++){
                if (is_this_position_safe(l,col)){
                    chess_board[l][col] = 1;
                    
                    if (solve_all(col +1)){
                        print_board_all();
                     }
                } //end if safe position
                chess_board[l][col] = 0;
                
            } // end for
        
        } // end else
        
        return false;
    }
    
public:
    
    // Solves the n-Queens problem by (recursive) backtracking
    void nQueens_all(int n)
    {
        initialize(n);
        check = false;
        bool test;
        test = (solve_all(0));
        
        if (check){
            std::cout << "There are " << count << " solutions to the " << n << "-Queens Problem" << std::endl;
        }
        
        else{
            std::cout << "There is no solution to the " << n << "-Queens Problem" << std::endl;
        }
    }
};


#endif /* n_queens_all_solutions_siddharth_bhaduri_h */
