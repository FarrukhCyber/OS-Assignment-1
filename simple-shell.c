/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

struct node
{
    char *data;
    struct node *left_child;
    struct node *right_child;
};

struct tree
{
    struct node *root_pointer;
};

int find_length(char *arr)
{
    int counter = 0;
    int i = 0;
    while (arr[i] != '\0')
    {
        counter = counter + 1;
        i = i + 1;
    }
    // printf("length is %d", counter);
    return counter;
}

char *parser(char *arr, int starting_index)
{
    char *return_array = (char *)malloc(30 * sizeof(char));
    int i = 0;
    for (i = 0; i < 30; i = i + 1)
    {
        return_array[i] = '\0';
    }
    return_array[0] = '(';

    i = 1;
    int j = starting_index + 1;
    int count = 1;
    while (count != 0)
    {
        return_array[i] = arr[j];
        if (arr[j] == '(')
        {
            count = count + 1;
        }
        if (arr[j] == ')')
        {
            count = count - 1;
        }
        i = i + 1;
        j = j + 1;
    }

    printf("\n %s \n", return_array);

    return return_array;
}

struct node *tree_maker(char *args)
{
    // printf("\nargs are %s\n", args);
    struct node *root = malloc(sizeof(struct node));
    /////////////////////////////////////////
    //////////////////////////////////////
    ///////////////////////////////////////
    int start = 1;
    for (start = 1; args[start] != ','; start = start + 1)
    {
    }
    int number_chars = start - 1;
    ////now allocate space
    root->data = (char *)malloc((number_chars + 1) * sizeof(char));

    for (int dd = 0; dd < number_chars; dd = dd + 1)
    {
        root->data[dd] = args[1 + dd];
    }
    root->data[number_chars] = '\0';
    ///////////////////////////////////////////
    //////////////////////////////////////
    ////////////////////////////////////////////
    // printf("the number of characters are %d", number_chars);
    start = start + 1;
    // printf("start is %d", start);
    char *right_args = parser(args, start);
    int p_two = find_length(right_args) + start + 1;
    char *left_args = parser(args, p_two);

    int right_length = find_length(right_args);
    int left_length = find_length(left_args);
    // printf("right args are %s\n", right_args);
    // printf("left args are %s\n", left_args);

    if (right_length == 2)
    {
        root->right_child = NULL;
        // return root.right_child;
    }
    else
    {

        root->right_child = tree_maker(right_args);
        // return root.right_child;
    }

    if (left_length == 2)
    {

        root->left_child = NULL;
        // return root.left_child;
    }
    else
    {

        root->left_child = tree_maker(left_args);
        // return root.left_child;
    }

    struct node *left_leaf = (root->left_child);
    struct node *right_leaf = root->right_child;

    /*
    if (root->left_child)
    {
        printf("the data in roots' sleft child is %s", left_leaf->data);
    }
    if (root->right_child)
    {
        printf("the data in roots' right child is %s", right_leaf->data);
    }
    */

    return root;
}

int getLen(char *temp)
{
    int i = 0;
    while (temp[i] != '\0')
    {
        // printf("%c ", temp[i] ) ;
        i++;
    }

    return i + 1;
}


void process(char *temp)
{
    int i = 0;
    while (temp[i] != '\n')
    {
        i++;
    }

    temp[i] = '\0';
}

void readInput(char *args[])
{
    int count = 0;
    char cmd[41];
    char item[41];

    // read input
    fgets(cmd, 41, stdin);
    process(cmd);

    if (strlen(cmd) == 0 ){
        args[0] = "noArgs" ;
        return ;
    }

    // printf("check-1: %s ", cmd);


    // printf("check-2: %s ", cmd);

    // Create Tokens-----------------
    int size = strlen(cmd);
    char delim[] = " ";

    char *ptr = strtok(cmd, delim);

    while (ptr != NULL)
    {
        args[count] = ptr;
        count++;
        ptr = strtok(NULL, delim);
    }
    args[count] = '\0';
    // printf("%s\n", args[count]);

    // Remove '\n' from the last token
    // char *temp = args[count - 1];
    // temp[strlen(temp) - 1] = '\0';
    // args[count - 1] = temp;
    // printf("%s", args[count-1]) ;
    // printf("Count: %d\n", count);

    // printf("%s\n", args[0]);
    // printf("%s\n", args[1]);
    // printf("%s\n", args[2]);
    // printf("%s\n", args[3]);
}

int main(void)
{
    char *args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    int should_run = 1;

    int x = 0;

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        readInput(args);
        

        // Check Parsing is done correctly
        //===============
        int i = 0;
        for (i = 0; args[i] != NULL; i++)
        {
            printf("%s ", args[i]);
        }
        printf("%s\n", args[i]);
        // printf("%s %s %s\n", args[0], args[1], args[2]);


        // EXIT CONDITION
        //===============
        if (strcmp(args[0], "exit") == 0)
        {
            printf("\nBye\n");
            should_run = 0;
        }

        // int rc = fork();

        // if (rc < 0)
        // { // fork failed; exit
        //     fprintf(stderr, "fork failed\n");
        //     exit(1);
        // }
        // else if (rc == 0)
        // {   // child (new process)
        //     // printf("hello, I am child (pid:%d)\n", (int)getpid());
        //     execvp(args[0], args);
        // }
        // else
        // {   // parent goes down this path (main)
        //     int wc = wait(NULL);
        //     // printf("hello, I am parent of %d (wc:%d) (pid:%d)\n",
        //     //        rc, wc, (int)getpid());
        // }

        /**
         * After reading user input, the steps are:
         * (1) fork a child process
         * (2) the child process will invoke execvp()
         * (3) if command includes &, parent and child will run concurrently
         */
    }

    return 0;
}
