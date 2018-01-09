//
//  main.cpp
//  Option_Pricing
//
//  Created by Siddharth Bhaduri on 9/16/16.
//  Copyright © 2016 Siddharth Bhaduri. All rights reserved.
//

#include <iostream>
#include "card_game.h"

using namespace std;
 
int main (int argc, char * const argv[])
{
    int total_number_of_cards;
    
    
    sscanf (argv[1], "%d", &total_number_of_cards);
    
    Cards set; // create an object
    
    cout << "Total Number of Cards = " << total_number_of_cards << endl;
    set.initialize(total_number_of_cards/2);
    
    cout << "Value of the game = " << set.value(total_number_of_cards/2,total_number_of_cards/2) << endl;
    return 0;
}