#define BACKPROP_H
#include "layer.h"

int init(void);
int dinit(void);

int create_architecture(void);
int initialize_weights(void);
void feed_input(int i);
void train_neural_net(void);
void forward_prop(void);
void compute_cost(int i);
void back_prop(int p);
void update_weights(void);
void xor_table(void);
void xor_result(void);
void test_nn(void);

int initialize_dummy_weights(void);
