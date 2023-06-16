# Producer_Consumer_Pthreads

## What it does
This simple program written in C simulates the classic Producer-Consumer synchronization problem in operating systems using the Pthreads API, mutex semaphore, and counting semaphore.

The program takes 3 arguments from the user: The number of seconds the program should run, the number of producer threads, and the number of consumer threads. For example, Executing ./prod_cons_pthreads 30 5 2, will make the program run for 30 seconds, spawn 5 producer threads, and 2 consumer threads. The producer and consumer threads will use a shared buffer that holds 5 items.

When the main program has spawned all producer and consumer threads, it will go to sleep for a number of seconds equal to the number of seconds the program should run. The producer and consumer threads will sleep for 1 through 4 seconds choosen at at random. It will either produce an item then put it in the buffer, or consume an item from the buffer, respectively, then repeat the process. The item produced will be an integer with a value between 1 through 50.

Whenever an item is inserted into the buffer it will print the value being insert and the index of the buffer that it is being inserted into, then print the contents of the buffer. It will also print the a value for in and out. In indicates the index where the next item will be inserted into. Out indicates the index where the next item will be taken out from.

When the specified number of seconds is used up, all threads will be terminated. The program will output the number of items produced and the number of items consumed.

## How to compile the program
Type the command, "make" in the terminal to compile the program. The "make" command will access the file, makefile, and execute the instructions within. Type in terminal "make clean" to remove the program executable.

## Sample run of the program
![](https://github.com/liuyang3141/Producer_Consumer_Pthreads/blob/main/img/prod_cons_pthreads.gif)

## Disclaimer
I wrote this program as part of my computer science homework assignments at California State University East Bay. I've uploaded this particular assignment to Github as part of my portfolio. To students, you may use this as a study guide, but do not copy my work and plagiarize.