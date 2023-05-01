#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUMBER_OF_THREADS       15
#define PUZZLE_SIZE             9
#define true                    1
#define false                   0

void *column_worker(void *param);
void *row_worker(void *param);
void *subfield_worker(void *param);


int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
        {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,5,3,4,6,7,8,9,1,2},
        {-1,6,7,2,1,9,5,3,4,8},
        {-1,1,9,8,3,4,2,5,6,7},
        {-1,8,5,9,7,6,1,4,2,3},
        {-1,4,2,6,8,5,3,7,9,1},
        {-1,7,1,3,9,2,4,8,5,6},
        {-1,9,6,1,5,3,7,2,8,4},
        {-1,2,8,7,4,1,9,6,3,5},
        {-1,3,4,5,2,8,6,1,7,9}
};

int status_map[NUMBER_OF_THREADS] = {0};

typedef struct
{
    int thread_id;
    int x;
    int y;
} parameters;

int main(int argc, char *argv[])
{
    char con[1];
    char filename[100];
    printf("Please enter your filename: ");
    scanf("%s",filename);
    
    FILE *myFile;
    myFile = filename;
    int i,j;
    int counter = 0;
    int truth = true;

    if (myFile == NULL)
    {
        printf("Error Reading File\n");
        exit (0);
    }
    else
    {
    for (i = 1; i < PUZZLE_SIZE + 1; i++)
    {
        for (j =1; j < PUZZLE_SIZE + 1; j++)
        {
            fscanf(myFile, "%d,", &puzzle[i][j] );
        }

    }
    }

    pthread_t workers[NUMBER_OF_THREADS];
    parameters * data;


    for (i=1; i<PUZZLE_SIZE; i+=3) {

        data=(parameters *)malloc(sizeof(parameters));
        data->thread_id=counter;
        data->x=1;
        data->y=i;

        pthread_create(&workers[data->thread_id], 0, row_worker, data);

        counter++;
    }


    for (i=1; i<PUZZLE_SIZE; i+=3) {

        data=(parameters *)malloc(sizeof(parameters));

        data->thread_id=counter;
        data->x=i;
        data->y=1;

        pthread_create(&workers[data->thread_id], 0, column_worker, data);

        counter++;
    }


    for (i=1; i<PUZZLE_SIZE; i+=3) {
        for (j=1; j<PUZZLE_SIZE; j+=3) {

            data=(parameters *)malloc(sizeof(parameters));

            data->thread_id=counter;
            data->x=i;
            data->y=j;

            pthread_create(&workers[data->thread_id], 0, subfield_worker, data);

            ++counter;
        }
    }


    for (i=0; i<NUMBER_OF_THREADS; i++){
        pthread_join(workers[i], NULL);
    }


    for (i=0; i<NUMBER_OF_THREADS; i++) {
        if (status_map[i] == 0) {
            truth=false;
        }
    }


    if (truth==true) {
        printf("File: %s is a Sudoku puzzle!\n", filename);
    }
    else {
        printf("File: %s is not in Sudoku format.\n", filename);
    }


    return 0;
}


void *row_worker(void *params){
    parameters* p=(parameters *)params;


    int i, j;

    int digits[PUZZLE_SIZE + 1]={0};

    int flag=1;


    for ((i=p->y); (i<p->y+3); i++) {
        for (j=1; j<PUZZLE_SIZE+1; j++) {

            digits[puzzle[j][i]]=digits[puzzle[j][i]]+1;
        }
    }

    for (i=1; i<=PUZZLE_SIZE; i++) {
        if (digits[i]!=3) {
            flag=0;
            break;
        }
    }

    if (flag) {
        status_map[p->thread_id]=1;
    }

    pthread_exit(0);
}


void *column_worker(void *params){
    parameters* p = (parameters *)params;


    int i, j;

    int digits[PUZZLE_SIZE+1]={0};

    int flag=1;


    for (i=p->x; (i<p->x+3); i++) {
        for (j=1; j<PUZZLE_SIZE+1; j++) {
            //increment digits array
            digits[puzzle[i][j]] = digits[puzzle[i][j]] + 1;
        }
    }


    for (i = 1; i <= PUZZLE_SIZE; i++) {
        if (digits[i] != 3) {
            flag = 0;
            break;
        }
    }

    if (flag) {
        status_map[p->thread_id] = 1;
    }

    pthread_exit(0);
}


void *subfield_worker(void *params){
    parameters* p = (parameters *)params;


    int i, j;
    int digits[PUZZLE_SIZE + 1] = {0};

    int flag =1;


    for (i=p->x; (i<p->x+3); i++) {
        for (j=p->y; (j<p->y+3); j++) {
            digits[puzzle[i][j]] = 1;
        }
    }

    for (i=1; i<PUZZLE_SIZE+1; i++) {
        if (digits[i] == 0) {
            flag = 0;
            break;
        }
    }

    if (flag) {
        status_map[p->thread_id] = 1;
    }

    pthread_exit(0);
}