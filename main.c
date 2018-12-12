#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

/**********************************/
// Stack part

// Explanation : I have a stck in each row of queue and each stck has some nodes

//nodes of stck
typedef struct __node_t_stack {
    int value;
    struct __node_t_list *next;
} node_t_stack;

//stack
typedef struct __stack_t {
    node_t_stack *first;
    sem_t *mutex;
    sem_t *empty;
    sem_t *full;
} stack_t;

void init_items(stack_t *items) {

    items->first = malloc(sizeof(node_t_stack));
    items->first->next = NULL;

    items->mutex = malloc(sizeof(sem_t));
    items->empty = malloc(sizeof(sem_t));
    items->full = malloc(sizeof(sem_t));
    
    //initialize lock
    sem_init(items->mutex,0,1);
    //initialize semaphores
    sem_init(items->empty,0,10);
    sem_init(items->full,0,0); 
   
}

void insert_items(stack_t *items, int value) {

    node_t_stack *new = malloc(sizeof(node_t_stack));

    if (new == NULL) {
        printf("Error in malloc");
        return ;
    }

    new->value = value;
    new->next = NULL;
    sem_wait(items->empty);
        //lock
        sem_wait(items->mutex);
            //last in first out logic
            new->next = items->first;
            items->first = new;
        //unlock
        sem_post(items->mutex);
    sem_post(items->full);
}

int search_items(stack_t *items, int *value) {

    sem_wait(items->full);
        //lock
        sem_wait(items->mutex);
            int tmp = items->first->value;
            *value = tmp;
            items->first = items->first->next;
        //unlock
        sem_post(items->mutex);
    sem_post(items->empty);
    return -1;
}

/**************************************/
// Queue part
//nodes od queue
typedef struct __node_t {
    stack_t *items;
} node_t;

//queue
typedef struct __queue_t {
    node_t *nodes;
    int length;
} queue_t;

void *Pqueue_init(struct __queue_t *pqueue) {
    pqueue->length = 10;
    pqueue->nodes = malloc(pqueue->length * sizeof(node_t));

    for (int i = 0; i < pqueue->length; ++i) {
    // initialize each level's items(each level is like a stack)
        pqueue->nodes[i].items= malloc(sizeof(stack_t));
        init_items(pqueue->nodes[i].items);
    }
    return 0;
}

void *Pqueue_enqueue(struct __queue_t *pqueue, int level, int value) {
    insert_items(pqueue->nodes[level].items,value);
    return 0;
}

void *Pqueue_dequeue(struct __queue_t *pqueue, int level, int *value) {
    search_items(pqueue->nodes[level].items,value);
    return 0;
}
/****************************************************************/
// Producer/Consumer Part

queue_t *p_queue;

void *producer(void *args)
{
   
    int r ;
    while (1)
    {
        //creates random number
        r = (rand() % 1000) + 1;

        if(r>0 & r<=100)
            Pqueue_enqueue(p_queue, 0, r);
        else if (r>100 & r<=200)
            Pqueue_enqueue(p_queue, 1, r);
        else if (r>200 & r<=300)
            Pqueue_enqueue(p_queue, 2, r);
        else if (r>300 & r<=400)
            Pqueue_enqueue(p_queue, 3, r);
        else if (r>400 & r<=500)
            Pqueue_enqueue(p_queue, 4, r);
        else if (r>500 & r<=600)
            Pqueue_enqueue(p_queue, 5, r);
        else if (r>600 & r<=700)
            Pqueue_enqueue(p_queue, 6, r);
        else if (r>700 & r<=800)
            Pqueue_enqueue(p_queue, 7, r);
        else if (r>800 & r<=900)
            Pqueue_enqueue(p_queue, 8, r);
        else if (r>900 & r<=1000)
            Pqueue_enqueue(p_queue, 9, r);
        printf("Enqueued the number %d\n", r);
        sleep(rand()%5);
    }
}

void *consumer(void *args)
{
    //value to dequeue(better to malloc)

    int *value = malloc(sizeof(int));

    while (1)
    {
        for (int i = 0; i < 10; i++)
        {
                Pqueue_dequeue(p_queue, i, value);
                printf("Dequeued the number %d\n", *value);
                sleep(rand()%5);

        }
        sleep(rand()%5);
    }
}


/***********************************************/

int main(int argc, char *argv[])
{
    //initialize p_queue
    p_queue = malloc(sizeof(queue_t));
    Pqueue_init(p_queue);
    int CONST = 8;
    /***********************************/
    //TEST PART
    pthread_t pro[CONST], con[CONST];
    
    //Producer
    int rc;
    for (int i =0 ; i<CONST ;i++){
        rc = pthread_create(&pro[i], NULL, producer, NULL);
        // raise error
        if (rc<0){
            printf("Sth went wrong\n");
            return(0);
        }
  }

    //Consumer
    for (int i =0 ; i<CONST ;i++){
        rc = pthread_create(&con[i], NULL, consumer, NULL);
        // raise error
        if (rc<0){
            printf("Sth went wrong\n");
            return(0);
        }
  }

    //join in order to eait for works that are not done yet
    for (int i =0 ; i<CONST ;i++){
        pthread_join(pro[i], NULL);
        pthread_join(con[i], NULL);
      
  }
}
