#include "balancing_scale_processing.h"

int main(int argc, char* argv[]) 
{
    if ( argc != 2 ) 
    {
        cout<<"Wrong number of arguments passed,Input file name needed"<<endl; 
        return 1;
    }
    string inputFile(argv[1]);
    
    BalancerProcessor ofp;
    if (ofp.ParseInputFileToTree(inputFile)) //Read Balancer File
    {
        ofp.PerformTreeBalancing(); //Perform balancer mapping
        ofp.PrintBalancerOutput();  //Print output to console 
        return 0;
    }
    else
    {
        return 1;
    }
}


