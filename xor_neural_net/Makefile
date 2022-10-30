
CFLAGS = -g -Wall -Wextra
backprop: backprop.o layer.o neuron.o
	$(CC) $(LDFLAGS) -o net net.o layer.o neuron.o -lm

backprop.o: net.c
	$(CC) $(CFLAGS) -c net.c

layer.o: layer.c
	$(CC) $(CFLAGS) -c layer.c

neuron.o: neuron.c
	$(CC) $(CFLAGS) -c neuron.c


clean:
	rm *.o net
