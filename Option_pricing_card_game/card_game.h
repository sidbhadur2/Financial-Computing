//
//  card_game.h
//  Option_Pricing
//
//  Created by Siddharth Bhaduri on 9/16/16.
//  Copyright © 2016 Siddharth Bhaduri. All rights reserved.
//

#ifndef card_game_h
#define card_game_h
#include <algorithm>
using namespace std;

class Cards{
    
    double ** card_array;
    
    int deck_size;

public:
    
    void initialize(int n)
    {
        deck_size = n;
        card_array = new double*[deck_size];
        
        for (int k = 0; k < deck_size; k++){
            card_array[k] = new double[deck_size];
        }
        for (int i = 0; i < deck_size; i++){
            for (int j = 0; j < deck_size; j++){
                card_array[i][j] = -1; // empty deck
            }
        }
    }
    

    double value(int r, int b)
    {
        if (0 == r)
            return ((double) b);
        if (0 == b)
            return (0);
        
        double term1;
        double term2;
        
        if(-1 == card_array[r-1][b-1]){
            term1 = ((double) r/(r+b)) * value(r-1, b);
            
            term2 = ((double) b/(r+b)) * value(r, b-1);
            
            card_array[r-1][b-1] = max((term1 + term2), (double) (b - r));
            
            //return max((term1 + term2), (double) (b - r));
        }
        
        else{
           // return card_array[r-1][b-1];
        }
        return card_array[r-1][b-1];
    }
};
#endif /* card_game_h */