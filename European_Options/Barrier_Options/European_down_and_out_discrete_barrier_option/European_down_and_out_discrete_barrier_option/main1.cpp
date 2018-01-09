//
//  main.cpp
//  European_down_and_out_discrete_barrier_option
//
//  Created by Siddharth Bhaduri on 12/3/16.
//  Copyright © 2016 Siddharth Bhaduri. All rights reserved.
//
//

// Down-and-Out Barrier Option Pricing Code written by Prof. Sreenivas
// I am using the method in Baron-Adesi, Fusari and Theal (paper on the
// Compass Website) to adjust the probabilities in a standard binomial
// Tree.  Written by Prof. Sreenivas for IE523: Financial Computing


#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include "normdist.h"
using namespace std;


double risk_free_rate, strike_price, initial_stock_price, expiration_time, volatility, barrier_price;

double put_option_price = 0.0;
double call_option_price = 0.0;
double brownian_call_price = 0.0;
double brownian_put_price = 0.0;
double maxS1 = 0.0;
double maxS2 = 0.0;
double maxS3 = 0.0;
double maxS4 = 0.0;
double minS1 = 0.0;
double minS2 = 0.0;
double minS3 = 0.0;
double minS4 = 0.0;

int no_of_trials, no_of_barrier;


double option_price_put_black_scholes(const double& S,      // spot price
                                      const double& K,      // Strike (exercise) price,
                                      const double& r,      // interest rate
                                      const double& sigma,  // volatility
                                      const double& time) {
    double time_sqrt = sqrt(time);
    double d1 = (log(S / K) + r*time) / (sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1 - (sigma*time_sqrt);
    return K*exp(-r*time)*N(-d2) - S*N(-d1);
};

double option_price_call_black_scholes(const double& S,       // spot (underlying) price
                                       const double& K,       // strike (exercise) price,
                                       const double& r,       // interest rate
                                       const double& sigma,   // volatility
                                       const double& time) {  // time to maturity
    double time_sqrt = sqrt(time);
    double d1 = (log(S / K) + r*time) / (sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double d2 = d1 - (sigma*time_sqrt);
    return S*N(d1) - K*exp(-r*time)*N(d2);
};

double N(const double& z) {
    if (z > 6.0) { return 1.0; }; // this guards against overflow
    if (z < -6.0) { return 0.0; };
    double b1 = 0.31938153;
    double b2 = -0.356563782;
    double b3 = 1.781477937;
    double b4 = -1.821255978;
    double b5 = 1.330274429;
    double p = 0.2316419;
    double c2 = 0.3989423;
    double a = fabs(z);
    double t = 1.0 / (1.0 + a*p);
    double b = c2*exp((-z)*(z / 2.0));
    double n = ((((b5*t + b4)*t + b3)*t + b2)*t + b1)*t;
    n = 1.0 - b*n;
    if (z < 0.0) n = 1.0 - n;
    return n;
};

double option_price_delta_call_black_scholes(const double& S,     // spot price
                                             const double& K,     // Strike (exercise) price,
                                             const double& r,     // interest rate
                                             const double& sigma, // volatility
                                             const double& time) {  // time to maturity
    double time_sqrt = sqrt(time);
    double d1 = (log(S / K) + r*time) / (sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double delta = N(d1);
    return delta;
};

double option_price_delta_put_black_scholes(const double& S, // spot price
                                            const double& K, // Strike (exercise) price,
                                            const double& r,  // interest rate
                                            const double& sigma,
                                            const double& time) {
    double time_sqrt = sqrt(time);
    double d1 = (log(S / K) + r*time) / (sigma*time_sqrt) + 0.5*sigma*time_sqrt;
    double delta = -N(-d1);
    return delta;
}

double closed_form_down_and_out_european_call_option()
{
    // I took this formula from Wilmott, Howison and Dewynne, "The Mathematics of Financial Derivatives"
    double K = (2 * risk_free_rate) / (volatility*volatility);
    double A = option_price_call_black_scholes(initial_stock_price, strike_price,
                                               risk_free_rate, volatility, expiration_time);
    double B = (barrier_price*barrier_price) / initial_stock_price;
    double C = pow(initial_stock_price / barrier_price, -(K - 1));
    double D = option_price_call_black_scholes(B, strike_price, risk_free_rate, volatility, expiration_time);
    return (A - D*C);
}


double closed_form_down_and_in_european_put_option()
{
    // just making it easier by renaming the global variables locally
    double S = initial_stock_price;
    double r = risk_free_rate;
    double T = expiration_time;
    double sigma = volatility;
    double H = barrier_price;
    double X = strike_price;
    
    // Took these formulae from some online reference
    double lambda = (r+((sigma*sigma)/2))/(sigma*sigma);
    double temp = 2*lambda - 2.0;
    double x1 = (log(S/H)/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
    double y = (log(H*H/(S*X))/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
    double y1 = (log(H/S)/(sigma*sqrt(T))) + (lambda*sigma*sqrt(T));
    return (-S*N(-x1) + X*exp(-r*T)*N(-x1 + sigma*sqrt(T)) +
            S*pow(H/S, 2*lambda)*(N(y)-N(y1)) -
            X*exp(-r*T)*pow(H/S, temp)*(N(y-sigma*sqrt(T))-N(y1-sigma*sqrt(T))));
}

double closed_form_down_and_out_european_put_option()
{
    float vanilla_put = option_price_put_black_scholes(initial_stock_price, strike_price,
                                                       risk_free_rate, volatility, expiration_time);
    float put_down_in = closed_form_down_and_in_european_put_option();
    return (vanilla_put - put_down_in);
}

double max(double a, double b) {
    return (b < a) ? a : b;
}

bool hit_barrier(double stock_price, bool breach) {
    if (breach == true) {
        return breach;
    }
    
    if (stock_price < barrier_price){
        return true;
    }
    else{
        return false;
    }
    
}

double sigma(double i) {
    return sqrt((( (i * expiration_time * (1 / no_of_barrier)) - 0)*(expiration_time - (i * expiration_time * (1 / no_of_barrier))) / (expiration_time - 0)));
    
}

double mu(double a, double b, double i) {

    return a + ((i * expiration_time * (1 / no_of_barrier)) - 0)*(b - a) / (expiration_time - 0);
}

double brownian_bridge(double a, double b) {
    double probabilitiy_bridge = 1;
    
    for (double i = 1; i < no_of_barrier; i++) {
        probabilitiy_bridge = probabilitiy_bridge * (1 - N((barrier_price - mu(a, b, i)) / sigma(i)));
    }
    
    return probabilitiy_bridge;
}

double get_uniform()
{
    return (((double) random())/(pow(2.0, 31.0)-1.0));
}


int main (int argc, char* argv[])
{
    
    sscanf (argv[1], "%lf", &expiration_time);
    sscanf (argv[2], "%lf", &risk_free_rate);
    sscanf (argv[3], "%lf", &volatility);
    sscanf (argv[4], "%lf", &initial_stock_price);
    sscanf (argv[5], "%lf", &strike_price);
    sscanf (argv[6], "%d", &no_of_trials);
    sscanf (argv[7], "%d", &no_of_barrier);
    sscanf (argv[8], "%lf", &barrier_price);
    
    cout << "European Down-and-Out Discrete Barrier Options Pricing via Monte Carlo Simulation"<< endl;
    cout << "Expiration Time (Years) = " << expiration_time << endl;
    cout << "Risk Free Interest Rate = " << risk_free_rate << endl;
    cout << "Volatility (%age of stock value) = " << volatility*100 << endl;
    cout << "Initial Stock Price = " << initial_stock_price << endl;
    cout << "Strike Price = " << strike_price << endl;
    cout << "Barrier Price = " << barrier_price << endl;
    cout << "Number of Trials = " << no_of_trials << endl;
    cout << "Number of Discrete Barriers = " << no_of_barrier << endl;
    cout << "--------------------------------" << endl;
    
    
    double R = (risk_free_rate - 0.5*pow(volatility, 2))*expiration_time / ((double)no_of_barrier);
    double SD = volatility*sqrt(expiration_time / ((double)no_of_barrier));
    
    bool S1_breach = false;
    bool S2_breach = false;
    bool S3_breach = false;
    bool S4_breach = false;
    
    for (int j = 0; j < no_of_trials; j++) {
        
        double S1 = initial_stock_price;
        double S2 = initial_stock_price;
        double S3 = initial_stock_price;
        double S4 = initial_stock_price;

        S1_breach = false;
        S2_breach = false;
        S3_breach = false;
        S4_breach = false;
        
        for (int i = 0; i < no_of_barrier; i++)
        {
            double x = get_uniform();
            double y = get_uniform();
            double a = sqrt(-2.0*log(x)) * cos(6.283185307999998*y);
            double b = sqrt(-2.0*log(x)) * sin(6.283185307999998*y);
           
            S1 = S1 * exp(R + SD*a);
            S1_breach = hit_barrier(S1, S1_breach);
            
            S2 = S2 * exp(R - SD*a);
            S2_breach = hit_barrier(S2, S2_breach);
            
            S3 = S3 * exp(R + SD*b);
            S3_breach = hit_barrier(S3, S3_breach);
            
            S4 = S4 * exp(R - SD*b);
            S4_breach = hit_barrier(S4, S4_breach);
            
        }
       
        S1 =  (S1 < barrier_price) ? 0 : S1;
        S2 =  (S2 < barrier_price) ? 0 : S2;
        S3 =  (S3 < barrier_price) ? 0 : S3;
        S4 =  (S4 < barrier_price) ? 0 : S4;
        
        if (S1 == 0){maxS1 = 0;}
        if (S2 == 0){maxS2 = 0;}
        if (S3 == 0){maxS3 = 0;}
        if (S4 == 0){maxS4 = 0;}
        
        if (S1 != 0){
            maxS1 = max(0.0, S1 - strike_price);
        }
        
        if (S2 != 0){
            maxS2 = max(0.0, S2 - strike_price);
        }
        if (S3 != 0){
            maxS3 = max(0.0, S3 - strike_price);
        }
        if (S4 != 0){
            maxS4 = max(0.0, S4 - strike_price);
        }
        
        brownian_call_price +=  (((brownian_bridge(initial_stock_price,S1))* maxS1) + ((brownian_bridge(initial_stock_price,S2))* maxS2) + ((brownian_bridge(initial_stock_price,S3))* maxS3) + (((brownian_bridge(initial_stock_price,S4)) * maxS4))) / 4.0;
        
        
        if (S1 == 0){minS1 = 0;}
        if (S2 == 0){minS2 = 0;}
        if (S3 == 0){minS3 = 0;}
        if (S4 == 0){minS4 = 0;}
        
        if (S1 != 0){
            minS1 = max(0.0, strike_price - S1);
        }
        
        if (S2 != 0){
            minS2 = max(0.0, strike_price - S2);
        }
        if (S3 != 0){
            minS3 = max(0.0, strike_price - S3);
        }
        if (S4 != 0){
            minS4 = max(0.0, strike_price  - S4);
        }

        
        brownian_put_price +=  (((brownian_bridge(initial_stock_price,S1))* minS1) + ((brownian_bridge(initial_stock_price,S2))* minS2) + ((brownian_bridge(initial_stock_price,S3))* minS3) + (((brownian_bridge(initial_stock_price,S4)) * minS4))) / 4.0;
        
        
        if (S1_breach == true) S1 = 0.0;
        if (S2_breach == true) S2 = 0.0;
        if (S3_breach == true) S3 = 0.0;
        if (S4_breach == true) S4 = 0.0;
        
        
        if (S1 == 0){maxS1 = 0;}
        if (S2 == 0){maxS2 = 0;}
        if (S3 == 0){maxS3 = 0;}
        if (S4 == 0){maxS4 = 0;}
        
        if (S1 != 0){
            maxS1 = max(0.0, S1 - strike_price);
        }
        
        if (S2 != 0){
            maxS2 = max(0.0, S2 - strike_price);
        }
        if (S3 != 0){
            maxS3 = max(0.0, S3 - strike_price);
        }
        if (S4 != 0){
            maxS4 = max(0.0, S4 - strike_price);
        }
        
        if (S1 == 0){minS1 = 0;}
        if (S2 == 0){minS2 = 0;}
        if (S3 == 0){minS3 = 0;}
        if (S4 == 0){minS4 = 0;}
        
        if (S1 != 0){
            minS1 = max(0.0, strike_price - S1);
        }
        
        if (S2 != 0){
            minS2 = max(0.0, strike_price - S2);
        }
        if (S3 != 0){
            minS3 = max(0.0, strike_price - S3);
        }
        if (S4 != 0){
            minS4 = max(0.0, strike_price  - S4);
        }
        
        call_option_price += (maxS1 + maxS2 + maxS3 +maxS4) / 4.0;
        put_option_price += (minS1 + minS2 + minS3 + minS4) / 4.0;
        
    }
    
    brownian_call_price = exp(-risk_free_rate*expiration_time)*(brownian_call_price / ((double)no_of_trials));
    brownian_put_price = exp(-risk_free_rate*expiration_time)*(brownian_put_price / ((double)no_of_trials));
    
    call_option_price = exp(-risk_free_rate*expiration_time)*(call_option_price / ((double)no_of_trials));
    put_option_price = exp(-risk_free_rate*expiration_time)*(put_option_price / ((double)no_of_trials));
    
    
    cout << "The average Call Price via explicit simulation of price paths             = " << call_option_price << endl;
    cout << "The average Call Price with Brownian-Bridge correction on the final price = "<< brownian_call_price << endl;
    //cout << "Theoretical Call Price = " << closed_form_down_and_out_european_call_option() << endl;
    cout << "The average Put Price via explicit simulation of price paths              = " << put_option_price << endl;
    cout << "The average Put Price with Brownian-Bridge correction on the final price  = "<< brownian_put_price << endl;
    //cout << "Theoretical Put Price = " << closed_form_down_and_out_european_put_option() << endl;
    cout << "--------------------------------" << endl;
    
}
