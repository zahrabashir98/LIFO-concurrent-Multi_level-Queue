# LIFO-concurrent-Multi_level-Queue
A Last In First Out concurrent multi_level queue  that uses 10 priority levels and each level can contain 10 items only.

## Details:
This Folder consists of one file only:
` main.c `

## Getting started
Open terminal and cd to the directory which contains the above files.
Run the following commands:
``` 
gcc main.c -pthread -o main
./main
```
## Logic
At the exam session I thought about using a simple thread lock (mutex) and that actually works (and there isn't any differrnce between using that or a semaphore with first value of 1 that here I used it) but it was  slow and was not the best implemntation, it does works but not the best way.
At the exam session I could implement all parts of question and just some small parts remained which I put comments and they were about logic almost.
After thinking of better methods i decided to use a semaphore with the follwoing values:
* sem_init(items->empty,0,10);
* sem_init(items->full,0,0);
* sem_init(items->mutex,0,1);

The empty semaphore has the first value of 10 that it decreases whenever i want to put a value and if it was full oi just waits inorder to recieve a signal from the other one.
and the mutex works as simple lock.

I tried to use memory allocation (malloc) in my methods for better performance and not loosing data.

about the toatal logic i can say that each my work starts in main and it creats a queue and runs producers and consumers :
producers create random number and calls Pqueue_enqueue() according to the condition of question
and the consumer calls Pqueue_dequeue()

## Dependencies
In Linux environment all you need is a terminal to run the commands and a gcc compiler.
