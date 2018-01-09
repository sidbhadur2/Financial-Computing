//
//  main.cpp
//  American_option_trinomial_memoization
//
//  Created by Siddharth Bhaduri on 11/14/16.
//  Copyright © 2016 Siddharth Bhaduri. All rights reserved.
//


#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
using namespace std;

double up_factor, uptick_prob, risk_free_rate, strike_price;
double initial_stock_price, expiration_time, volatility, R;
double downtick_prob;
int no_of_divisions;
double ** option_array;
int option_size;

double ** option_array_two;
int option_size_two;

double max(float a, float b) {
    return (b < a )? a:b;
}

void initialize(int n)
{
    option_size = n;
    option_array = new double*[option_size];
    
    for (int k = 0; k < (option_size); k++){
        option_array[k] = new double[(2*option_size) + 1];
    }
    
    for (int i = 0; i < (option_size); i++){
        for (int j = 0; j < (2*option_size) + 1; j++){
            option_array[i][j] = -1; // empty array
        }
    }
}

void initialize_put(int n)
{
    option_size_two = n;
    option_array_two = new double*[option_size_two];
    
    for (int k = 0; k < (option_size_two); k++){
        option_array_two[k] = new double[(2*option_size_two) + 1];
    }
    
    for (int i = 0; i < (option_size_two); i++){
        for (int j = 0; j < (2*option_size) + 1; j++){
            option_array_two[i][j] = -1; // empty array
        }
    }
}

double american_call_option(int k, int i, float current_stock_price) {
    if (k == no_of_divisions){
        return max(0.0, (current_stock_price - strike_price));
    }
    else{
        if (-1 == option_array[k][i+no_of_divisions]){
            
            option_array[k][i+no_of_divisions] = max((current_stock_price - strike_price),
                                     (uptick_prob*american_call_option(k+1, i+1, current_stock_price*up_factor) +
                                      (1-uptick_prob-downtick_prob)*american_call_option(k+1, i, current_stock_price) +
                                      (downtick_prob)*american_call_option(k+1, i-1, current_stock_price/up_factor))/R);
            
        }
        else{
            
        }
    }
    
    return option_array[k][i+no_of_divisions];
}

double american_put_option(int k, int i, float current_stock_price) {
    if (k == no_of_divisions){
        return max(0.0, (strike_price - current_stock_price));
    }
    else{
        if(-1 == option_array_two[k][i+no_of_divisions]){
            option_array_two[k][i+no_of_divisions] = max((strike_price - current_stock_price),
                                     (uptick_prob*american_put_option(k+1, i+1, current_stock_price*up_factor) + (1-uptick_prob - downtick_prob)*american_put_option(k+1, i, current_stock_price) +
                                      (downtick_prob)*american_put_option(k+1, i-1, current_stock_price/up_factor))/R);
        }
        else{
        }
    }
    
    return option_array_two[k][i+no_of_divisions];
}

int main (int argc, char* argv[])
{
    
    sscanf (argv[1], "%lf", &expiration_time);
    sscanf (argv[2], "%d", &no_of_divisions);
    sscanf (argv[3], "%lf", &risk_free_rate);
    sscanf (argv[4], "%lf", &volatility);
    sscanf (argv[5], "%lf", &initial_stock_price);
    sscanf (argv[6], "%lf", &strike_price);
    
    
    up_factor = exp(volatility*sqrt(2*expiration_time/((float) no_of_divisions)));
    
    R = exp(risk_free_rate*expiration_time/((float) no_of_divisions));
    
    uptick_prob = pow( ((sqrt(R) - (1/sqrt(up_factor)))/(sqrt(up_factor)-(1/sqrt(up_factor)))) ,2);
    
    downtick_prob = pow( ((sqrt(up_factor) - (sqrt(R)))/(sqrt(up_factor)-(1/sqrt(up_factor)))) ,2);
    
    
    
    cout << "Recursive Trinomial American Option Pricing" << endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Number of Divisions = " << no_of_divisions << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility*100 << endl;
    initialize(no_of_divisions);
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "--------------------------------------" << endl;
    //cout << "R = " << R <<endl;
    cout << "Up Factor = " << up_factor << endl;
    cout << "Uptick Probability = " << uptick_prob << endl;
    cout << "Downtick Probability = " << downtick_prob << endl;
    cout << "Notick Probability = " << 1 - uptick_prob - downtick_prob << endl;
    cout << "--------------------------------------" << endl;
    double call_price = american_call_option(0, 0,initial_stock_price);
    cout << "Trinomial Price of an American Call Option = " << call_price << endl;
    initialize_put(no_of_divisions);
    double put_price = american_put_option(0, 0, initial_stock_price);
    cout << "Trinomial Price of an American Put Option = " << put_price << endl;
    cout << "--------------------------------------" << endl;
    cout << "Let us verify the Put-Call Parity: S+P-C = Kexp(-r*T) for American Options" << endl;
    cout <<  initial_stock_price << " + " << put_price << " - " << call_price;
    cout << " = " << strike_price << "exp(-" << risk_free_rate << " * " << expiration_time << ")" << endl;
    cout << initial_stock_price + put_price - call_price << " ?=? " << strike_price*exp(-risk_free_rate*expiration_time) << endl;
    if (abs(initial_stock_price + put_price - call_price - strike_price*exp(-risk_free_rate*expiration_time)) <= 1e-3)
        cout << "Looks like Put-Call Parity holds within three decimal places" << endl;
    else
        cout << "Looks like Put-Call Parity does NOT hold" << endl;
    cout << "--------------------------------------" << endl;

}
