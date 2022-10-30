#include "backprop.h"
#include "layer.h"
#include "neuron.h"


layer *lay = NULL;
int num_layers = 3;
int *num_neurons;
float alpha;
float **input;
float **desired_outputs;
int num_training_ex = 4;
int n=1;
int epoch;

int main()
{

    num_neurons = (int*) malloc(num_layers * sizeof(int));
    memset(num_neurons,0,num_layers *sizeof(int));

    // Get number of neurons per layer
    for(int i=0;i<num_layers;i++)
    {
        if (i !=2)
        {
            num_neurons[i] = 2;
        }

        else
        {
            num_neurons[i] = 1;
        }   
    }

    printf("\n");

    // Initialize the neural network module

    create_architecture();
   
    printf("\n");


    input = (float**) malloc(num_training_ex * sizeof(float*));
    for(int i=0;i<num_training_ex;i++)
    {
        input[i] = (float*)malloc(num_neurons[0] * sizeof(float));
    }

    desired_outputs = (float**) malloc(num_training_ex* sizeof(float*));
    for(int i=0;i<num_training_ex;i++)
    {
        desired_outputs[i] = 
            (float*)malloc(num_neurons[num_layers-1] * sizeof(float));
    }

   

    //Initalization of the XOR table
    xor_table();
    printf("XOR table created \n");
    printf("\n");
    //Result of the XOR table
    xor_result();

    printf("Enter how many time you want the network to train \n");
    scanf("%d",&epoch);
    printf("\n");

    printf("Enter the learning rate \n");
    scanf("%f",&alpha);
    printf("\n");


    train_neural_net();
    test_nn();

    return 0;
}

void  xor_table()
{
    int i,j;

        for(i=0;i<num_training_ex;i++)
        {         
            for(j=0;j<num_neurons[0];j++)
            {
                if(i == 0)
                {
                    if(j == 0)
                    {
                            input[i][j] = 0;
                    }
                    if (j == 1)
                    {
                        input[i][j] = 0;
                    }
                }

                if(i == 1)                                                      
                {                                                               
                    if(j == 0)                                                  
                    {                                                           
                        input[i][j] = 0;                                    
                    }                                                           
               
                    if (j == 1)                                                 
                    {                                                           
                        input[i][j] = 1;                                        
                    }                                                           
                }                          
                
                if(i == 2)                                                      
                {                                                               
                    if(j == 0)                                                  
                    {                                                           
                        input[i][j] = 1;                                        
                    }                                                           
                    
                    if (j == 1)                                                     
                    {                                                           
                        input[i][j] = 0;                                        
                    }                                                           
                }  
            

                if(i == 3)
                {
                    if(j == 0)
                    {
                        input[i][j] = 1;
                    }
                
                    if (j == 1)
                    {
                        input[i][j] = 1;
                    }
                }
            }
            printf("\n");
        }
}


void xor_result()
{
    int i,j;
    
    for(i=0;i<num_training_ex;i++)
    {
        for(j=0;j<num_neurons[num_layers-1];j++)
        {
            if (i == 0 || i == 3)
            {
                desired_outputs[i][j] = 0;
            } 

            else 
            {
                desired_outputs[i][j] = 1;
            }
        }
    }
}

// Feed inputs to input layer
void feed_input(int i)
{
    int j;

    for(j=0;j<num_neurons[0];j++)
    {
        lay[0].neu[j].actv = input[i][j];

    }
}

// Create Neural Network 
int create_architecture()
{
    int i=0,j=0;
    lay = (layer*) malloc(num_layers * sizeof(layer));

    for(i=0;i<num_layers;i++)
    {
        lay[i] = create_layer(num_neurons[i]);      
        lay[i].num_neu = num_neurons[i];
        printf("Layer Number %d created\n", i+1);
        for(j=0;j<num_neurons[i];j++)
        {
            if(i < (num_layers-1)) 
            {
                lay[i].neu[j] = create_neuron(num_neurons[i+1]);
            }

            printf("Neuron number %d has been created\n",j+1);  
        }
        printf("\n");
    }

    printf("\n");

    
    initialize_weights();   
    return 0;   
}

int initialize_weights()
{
    int i,j,k;

    printf("Random initalisation of the weights : \n");

    for(i=0;i<num_layers-1;i++)
    {
        
        for(j=0;j<num_neurons[i];j++)
        {
            for(k=0;k<num_neurons[i+1];k++)
            {
                
                lay[i].neu[j].out_weights[k] = 
                    ((double)rand())/((double)RAND_MAX);
                printf("%d:w[%d][%d]: %f\n",k,i,j, 
                        lay[i].neu[j].out_weights[k]);
                lay[i].neu[j].dw[k] = 0.0;
            }

            if(i>0) 
            {
                lay[i].neu[j].bias = ((double)rand())/((double)RAND_MAX);
            }
        }
    }   
    printf("\n");
    
    for (j=0; j<num_neurons[num_layers-1]; j++)
    {
        lay[num_layers-1].neu[j].bias = ((double)rand())/((double)RAND_MAX);
    }

    return 0;
}

// Train Neural Network
void train_neural_net()
{
    int i;
    int it=0;

    // Gradient Descent
    for(it=0;it<epoch;it++)
    {
        for(i=0;i<num_training_ex;i++)
        {
            feed_input(i);
            forward_prop();           
            back_prop(i);
            update_weights();
        }
    }

printf("Training is done... \n ");
printf("\n");    
}



void update_weights()
{
    int i,j,k;

    for(i=0;i<num_layers-1;i++)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            for(k=0;k<num_neurons[i+1];k++)
            {
                // Update Weights
                lay[i].neu[j].out_weights[k] = (lay[i].neu[j].out_weights[k]) -
                    (alpha * lay[i].neu[j].dw[k]);
            }
            
            // Update Bias
            lay[i].neu[j].bias = lay[i].neu[j].bias - 
                (alpha * lay[i].neu[j].dbias);
        }
    }   
}

void forward_prop()
{
    int i,j,k;

    for(i=1;i<num_layers;i++)
    {   
        for(j=0;j<num_neurons[i];j++)
        {
            lay[i].neu[j].z = lay[i].neu[j].bias;

            for(k=0;k<num_neurons[i-1];k++)
            {
                lay[i].neu[j].z  = lay[i].neu[j].z + 
                    ((lay[i-1].neu[k].out_weights[j])* (lay[i-1].neu[k].actv));
            }

            // Relu Function
            if(i < num_layers-1)
            {
                if((lay[i].neu[j].z) < 0)
                {
                    lay[i].neu[j].actv = 0;
                }

                else
                {
                    lay[i].neu[j].actv = lay[i].neu[j].z;
                }
            }
            
            // Sigmoid function
            else
            {
                lay[i].neu[j].actv = 1/(1+exp(-lay[i].neu[j].z));
                
            }
        }
    }
}
//like previous Function but print the output
void forward_prop_()                                                         
{                                                                               
    int i,j,k;                                                                  
                                                                                
    for(i=1;i<num_layers;i++)                                                   
    {                                                                           
        for(j=0;j<num_neurons[i];j++)                                           
        {                                                                       
            lay[i].neu[j].z = lay[i].neu[j].bias;                               
                                                                                
            for(k=0;k<num_neurons[i-1];k++)                                     
            {                                                                      
                lay[i].neu[j].z  = lay[i].neu[j].z +
                    ((lay[i-1].neu[k].out_weights[j])* (lay[i-1].neu[k].actv));
            }                                                                                          
            if(i < num_layers-1)                                                
            {                                                                   
                if((lay[i].neu[j].z) < 0)                                       
                {                                                               
                    lay[i].neu[j].actv = 0;                                     
                }                                                               
                                                                                
                else                                                            
                {                                                               
                    lay[i].neu[j].actv = lay[i].neu[j].z;                       
                }                                                               
            }                                                                   
                                                                                
                                
            else                                                                
            {                                                                   
                lay[i].neu[j].actv = 1/(1+exp(-lay[i].neu[j].z));               
                printf("Output: %d\n", (int)round(lay[i].neu[j].actv));         
                printf("\n");                                                   
            }                                                                   
        }                                                                       
    }                                                                           
}                                


// Back Propagation algortihm
void back_prop(int p)
{
    int i,j,k;

    // Output Layer
    for(j=0;j<num_neurons[num_layers-1];j++)
    {           
        lay[num_layers-1].neu[j].dz = (lay[num_layers-1].neu[j].actv - 
                desired_outputs[p][j]) * (lay[num_layers-1].neu[j].actv) * 
            (1- lay[num_layers-1].neu[j].actv);

        for(k=0;k<num_neurons[num_layers-2];k++)
        {   
            lay[num_layers-2].neu[k].dw[j] = (lay[num_layers-1].neu[j].dz *
                    lay[num_layers-2].neu[k].actv);
            lay[num_layers-2].neu[k].dactv = 
                lay[num_layers-2].neu[k].out_weights[j] * 
                lay[num_layers-1].neu[j].dz;
        }
            
        lay[num_layers-1].neu[j].dbias = lay[num_layers-1].neu[j].dz;           
    }


    for(i=num_layers-2;i>0;i--)
    {
        for(j=0;j<num_neurons[i];j++)
        {
            if(lay[i].neu[j].z >= 0)
            {
                lay[i].neu[j].dz = lay[i].neu[j].dactv;
            }
            else
            {
                lay[i].neu[j].dz = 0;
            }

            for(k=0;k<num_neurons[i-1];k++)
            {
                lay[i-1].neu[k].dw[j] = lay[i].neu[j].dz * lay[i-1].neu[k].actv;    
                
                if(i>1)
                {
                    lay[i-1].neu[k].dactv = lay[i-1].neu[k].out_weights[j] *
                        lay[i].neu[j].dz;
                }
            }

            lay[i].neu[j].dbias = lay[i].neu[j].dz;
        }
    }
}

// Test the trained network
void test_nn() 
{
    int riska = 0;
    int i;
    while(riska < 4)
    {
        printf("Enter input to test:\n");

        for(i=0;i<num_neurons[0];i++)
        {
            scanf("%f",&lay[0].neu[i].actv);
        }
        forward_prop_();
        riska++;
    }
}
