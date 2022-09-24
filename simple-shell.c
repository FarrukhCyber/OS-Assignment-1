/**
 * Simple shell interface program.
 *
 * Operating System Concepts - Tenth Edition
 * Copyright John Wiley & Sons - 2018
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* 80 chars per line, per command */
#define MAX_PID 32768 /* max pid value in Linux */

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

void initialize_with_NULL(char* args[])
{
    for (int i = 0; i < MAX_LINE / 2 + 1; i++) {
        args[i] = NULL;
    }
}

char *read_input()
{
    char *command_str = malloc(sizeof(char) * MAX_LINE); // !!!FREE IN CHILD PROCESS!!!
    fgets(command_str, MAX_LINE, stdin);
    
    // Remove newline character
    for (int i = 0; i < MAX_LINE; i++)
    {
        if (command_str[i] == '\n') {
            command_str[i] = '\0';
        }
    }

    return command_str;
}

void tokenize_populate_args(char *args[], char *user_input)
{
    int i = 0;
    char *token = strtok(user_input, " ");
    args[i] = token;

    while (token != NULL)
    {
        i++;
        token = strtok(NULL, " ");
        args[i] = token;
    }
}

int len(char *string)
{
    int count = 0;
    int i = 0;

    while (string[i] != '\0') {
        count++;
        i++;
    }

    return count;
}

// in order to have children run many arguments concurrently, creating a new function that treats each argument separately by each child
void run_child(char *child_arg)
{
    char *args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    initialize_with_NULL(args); // Initializing every element of args with NULL pointer
    tokenize_populate_args(args, child_arg);

    execvp(args[0], args);
}

char **tokenize_user_input_with_ampersand(char *user_input)
{
    // TODO: !!! FREE IN PARENT PROCESS !!!
    char **ampersand_tokenized_user_input; // Array of strings i.e. Array of char *.
    ampersand_tokenized_user_input = malloc(sizeof(*ampersand_tokenized_user_input) * (MAX_LINE / 2 + 1)); // allocating space for 40 ampersand separated arguments
    initialize_with_NULL(ampersand_tokenized_user_input); // Important to get length of total ampersand separated tokens
    
    for (int i = 0; i < MAX_LINE / 2 + 1; i++) {
        ampersand_tokenized_user_input[i] = malloc(sizeof(**ampersand_tokenized_user_input) * (MAX_LINE / 2 + 1)); // allocating space for each 40 space separated arguments i.e. one whole input of one child processs
    }

    int i = 0;
    char *token = strtok(user_input, "&");
    ampersand_tokenized_user_input[i] = token;
    // printf("Testing: %s\n", token);

    while (token != NULL)
    {
        i++;
        token = strtok(NULL, "&");
        ampersand_tokenized_user_input[i] = token;
        // printf("Testing: %s\n", token);
    }

    return ampersand_tokenized_user_input;
}

int len_via_NULL(char **argument)
{
    int count = 0;
    int i = 0;

    while (argument[i] != NULL) {
        count++;
        i++;
    }

    return count;
}

int main(void)
{
    int should_run = 1;
    char **child_process_args = malloc(sizeof(*child_process_args) * (MAX_PID + 1)); // So we can reference each child's arg by that child's process id
    initialize_with_NULL(child_process_args);
    int rc;

    while (should_run)
    {        
        printf("osh>");
        fflush(stdout);

        char *user_input = read_input(); // TODO: Add condition for 40 character limit

        // TODO: !!! ADD EXIT CONDITON !!!

        char **ampersand_tokenized_user_input = tokenize_user_input_with_ampersand(user_input);
        
        int num_forks = len_via_NULL(ampersand_tokenized_user_input);
        
        // parent spawning forks in a for loop with iter < num_forks
        // How to have run_child

        for (int i = 0; i < num_forks; i++)
        {
            rc = fork();

            if (rc == 0) // if child, exit loop, if parent, continue spawning children
            {
                child_process_args[(int) getpid()] = ampersand_tokenized_user_input[i];
                break;
            }
        }

        if (rc < 0)
        {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        else if (rc == 0) // child
        {
            run_child(child_process_args[(int) getpid()]);
            child_process_args[(int) getpid()] = NULL; // depopulate child args when done
        } 
        else
        {
            while (wait(NULL) > 0); // wait for all child processes
        }

        free(user_input);
    }

    return 0;
}

/**
 * After reading user input, the steps are:
 * (1) fork a child process
 * (2) the child process will invoke execvp()
 * (3) if command includes &, parent and child will run concurrently
 */

/**
 * 1) read user input and count how many forks to create
 * 2) create and populate args for each child separately. 
 * 3) create a fork for each child and for each && delimited string
 */