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

#define MAX_LINE 80   /* 80 chars per line, per command */
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

void initialize_with_NULL(char *args[])
{
    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
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
        if (command_str[i] == '\n')
        {
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

    while (string[i] != '\0')
    {
        count++;
        i++;
    }

    return count;
}

// in order to have children run many arguments concurrently, creating a new function that treats each argument separately by each child
void run_child(char *child_arg)
{
    char *args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    initialize_with_NULL(args);   // Initializing every element of args with NULL pointer
    tokenize_populate_args(args, child_arg);

    execvp(args[0], args);
}

char **tokenize_user_input_with_ampersand(char *user_input)
{
    // TODO: !!! FREE IN PARENT PROCESS !!!
    char **ampersand_tokenized_user_input;                                                                 // Array of strings i.e. Array of char *.
    ampersand_tokenized_user_input = malloc(sizeof(*ampersand_tokenized_user_input) * (MAX_LINE / 2 + 1)); // allocating space for 40 ampersand separated arguments
    initialize_with_NULL(ampersand_tokenized_user_input);                                                  // Important to get length of total ampersand separated tokens

    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
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

    while (argument[i] != NULL)
    {
        count++;
        i++;
    }

    return count;
}

//=========== IMPLEMENTATION OF DOUBLY CIRCULAR LINKED LIST ===========
struct Node
{
    char *input;
    struct Node *next;
    struct Node *prev;
};

struct Node *create_node(char *input)
{
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    new_node->input = malloc(sizeof(char) * MAX_LINE);
    new_node->input = input; // SET THE VALUE

    return new_node;
}

struct Node *insert_node(struct Node *current, char *input)
{

    // WHEN THE LIST IS EMPTY
    if (current == NULL)
    {
        current = create_node(input); // POINT TO NEW NODE
        current->next = current;      // MAKE IT CIRCULAR
        current->prev = current;      // NEW

        return current;
    }

    struct Node *new_node = create_node(input);
    new_node->next = current->next;
    new_node->prev = current; // NEW
    current->next = new_node;

    current = new_node;

    current->next->prev = current; // NEW
    // printf("Node to be inserted: %s\n", current->input);

    return current;
}

void display_list(struct Node *current)
{
    struct Node *temp = current;

    if (current == NULL)
    {
        printf("%s\n", "No Commands in history");
        return;
    }

    while (temp->prev != current)
    {
        // printf("%s\n", "check-1") ;
        printf(" %s ->", temp->input);
        temp = temp->prev;
    }
    // printf("%s\n", "check-2") ;
    printf(" %s", temp->input);
    printf("\n");
}

int list_length(struct Node *current)
{

    if (current == NULL)
    {
        return 0;
    }

    struct Node *temp = current->next;
    int count = 0;

    if (temp == current)
    {
        return 1;
    }

    while (temp != current)
    {
        // printf("%s ", temp->input) ;
        count++;
        temp = temp->next;
    }

    return count + 1;
}

char *command_at_index(struct Node *current, int n)
{

    struct Node *temp = current;
    for (int i = 1; i < n; i++)
    {
        temp = temp->prev;
    }

    return temp->input;
}

int hist_found(char *input)
{
    char *temp = strstr(input, "hist");

    return (temp ? 1 : 0);
}

char *get_command_history(char *input, struct Node *current)
{

    int length = len(input);

    // hist with no args
    if (length <= 5)
    {
        display_list(current);
        return input;
    }
    else
    {
        // TODO: Fix the code for 2 digit args----------
        //  char *num = input[6] ;
        //  int n = atoi(num) ;
        int n = (int)input[6];
        n = n - 48;
        printf("Number: %d\n", n);

        if (list_length(current) < n || n == 0)
        {
            printf("%s\n", "No command found");
        }
        else
        {
            input = command_at_index(current, n);
            // printf("Command at %d is %s\n", n, input);
            printf("%s\n", input) ;
            return input;
        }
    }
}

//===============================================================================

//===============  IMPLEMENTATION FOR HANDLING PIPES=======================================
int pipe_found(char *input)
{
    char *temp = strstr(input, "||");
    return (temp ? 1 : 0);
}

char **tokenize_user_input_with_pipe(char *user_input)
{
    // TODO: !!! FREE IN PARENT PROCESS !!!
    char **pipe_tokenized_user_input;                                                            // Array of strings i.e. Array of char *.
    pipe_tokenized_user_input = malloc(sizeof(*pipe_tokenized_user_input) * (MAX_LINE / 2 + 1)); // allocating space for 40 ampersand separated arguments
    initialize_with_NULL(pipe_tokenized_user_input);                                             // Important to get length of total ampersand separated tokens

    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
        pipe_tokenized_user_input[i] = malloc(sizeof(**pipe_tokenized_user_input) * (MAX_LINE / 2 + 1)); // allocating space for each 40 space separated arguments i.e. one whole input of one child processs
    }

    int i = 0;
    char *token = strtok(user_input, "|");
    pipe_tokenized_user_input[i] = token;
    // printf("Testing: %s\n", token);

    while (token != NULL)
    {
        i++;
        token = strtok(NULL, "|");
        pipe_tokenized_user_input[i] = token;
        // printf("Testing: %s\n", token);
    }

    return pipe_tokenized_user_input;
}

void run_piped_child(char *child_arg)
{
    char *args[MAX_LINE / 2 + 1]; /* command line (of 80) has max of 40 arguments */
    initialize_with_NULL(args);   // Initializing every element of args with NULL pointer
    tokenize_populate_args(args, child_arg);

    execvp(args[0], args);
}

void close_all_pipes(int fd[][2], int num_pipes)
{
    for (int i = 0; i < num_pipes - 1; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            close(fd[i][j]);
        }
    }
}

void handle_pipe_commands(char *user_input)
{
    char **pipe_tokenized_user_input;
    pipe_tokenized_user_input = tokenize_user_input_with_pipe(user_input);
    int num_forks = len_via_NULL(pipe_tokenized_user_input);

    int num_pipes = num_forks - 1;
    int fd[num_pipes][2];

    // create pipes
    for (int i = 0; i < num_pipes; i++)
    {
        if (pipe(fd[i]) < 0)
        {
            printf("An Error Ocurred in creating a pipe \n");
            return 1;
        }
    }

    // create Forks
    int pipe_id = 0;
    for (int i = 0; i < num_forks; i++)
    {
        int p_id = fork();

        if (p_id < 0)
        {
            printf("An Error Ocurred in creating a fork \n");
            return 1;
        }

        // Child process
        if (p_id == 0)
        {
            // First Command
            if (i == 0)
            {
                dup2(fd[i][1], STDOUT_FILENO);
                close_all_pipes(fd, num_pipes);
                run_piped_child(pipe_tokenized_user_input[i]);
            }

            // LAST Command
            else if (i == num_forks - 1)
            {
                dup2(fd[i-1][0], STDIN_FILENO);
                close_all_pipes(fd, num_pipes);
                run_piped_child(pipe_tokenized_user_input[i]);
            }
            else
            {
                dup2(fd[i - 1][0], STDIN_FILENO);
                dup2(fd[i][1], STDOUT_FILENO);
                close_all_pipes(fd, num_pipes);
                run_piped_child(pipe_tokenized_user_input[i]);
            }
        }
    }

    // Parent closes all pipes
    close_all_pipes(fd, num_pipes);
    while (wait(NULL) > 0);
}

// IMPLEMENTATION FOR Part 4(b) //

char **tokenize_user_input_with_space(char *user_input)
{
    // TODO: !!! FREE IN PARENT PROCESS !!!
    char **space_tokenized_user_input;                                                                 // Array of strings i.e. Array of char *.
    space_tokenized_user_input = malloc(sizeof(*space_tokenized_user_input) * (MAX_LINE / 2 + 1)); // allocating space for 40 space separated arguments
    initialize_with_NULL(space_tokenized_user_input);                                                  // Important to get length of total space separated tokens

    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
        space_tokenized_user_input[i] = malloc(sizeof(**space_tokenized_user_input) * (MAX_LINE / 2 + 1)); // allocating space for each 40 space separated arguments i.e. one whole input of one child processs
    }

    int i = 0;
    char *token = strtok(user_input, " ");
    space_tokenized_user_input[i] = token;
    // printf("Testing: %s\n", token);

    while (token != NULL)
    {
        i++;
        token = strtok(NULL, " ");
        space_tokenized_user_input[i] = token;
        // printf("Testing: %s\n", token);
    }

    return space_tokenized_user_input;
}

// Returns 1 if true, 0 if false

int check_for_Tree(char *user_input) 
{
    char **space_tokenized_user_input = tokenize_user_input_with_space(user_input);
    
    if (strcmp(space_tokenized_user_input[0], "Tree") == 0)
    {
        return 1;    
    } 
    else
    {
        return 0;
    }
}

char **parse_tree_args(char *user_input)
{
    // TODO: !!! FREE IN PARENT PROCESS !!!
    char **parsed_tree_input;                                                                 // Array of strings i.e. Array of char *.
    parsed_tree_input = malloc(sizeof(*parsed_tree_input) * (MAX_LINE / 2 + 1)); // allocating space for 40 space separated arguments
    initialize_with_NULL(parsed_tree_input);                                                  // Important to get length of total space separated tokens

    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
        parsed_tree_input[i] = malloc(sizeof(**parsed_tree_input) * (MAX_LINE / 2 + 1)); // allocating space for each 40 space separated arguments i.e. one whole input of one child processs
    }

    int i = 0;
    char *token = strtok(user_input, " ");
    parsed_tree_input[i] = token;
    // printf("Testing: %s\n", token);

    while (i < 2)
    {
        i++;
        token = strtok(NULL, "");
        parsed_tree_input[i] = token;
        // printf("Testing: %s\n", token);
    }

    return parsed_tree_input;
}

int in_order_traversal(struct node* tree_node, char *tree_user_input, int *i)
{
    if (tree_node == NULL)
    {
        return 0;
    }
    
    in_order_traversal(tree_node->left_child, tree_user_input, i);
    
    if (tree_node->left_child != NULL)
    {
        tree_user_input[*i] = ' ';
        tree_user_input[*i + 1] = '|';
        tree_user_input[*i + 2] = ' ';
        *i += 3;
    }

    int k;
    for (k = 0; k < strlen(tree_node->data); k++)
    {
        tree_user_input[*i + k] = tree_node->data[k];
    }

    *i += k; 

    if (tree_node->right_child != NULL)
    {
        tree_user_input[*i] = ' ';
        tree_user_input[*i + 1] = '|';
        tree_user_input[*i + 2] = ' ';
        *i += 3;   
    }

    in_order_traversal(tree_node->right_child, tree_user_input, i);

    return *i;
}

void initialize_tree_user_input_with_NULL(char *input)
{
    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
        input[i] = '\0';
    }
}

// ==================================================

int main(void)
{
    int should_run = 1;
    char **child_process_args = malloc(sizeof(*child_process_args) * (MAX_PID + 1)); // So we can reference each child's arg by that child's process id
    initialize_with_NULL(child_process_args);
    int rc;
    struct Node *current = NULL;

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);

        char *user_input = read_input(); // TODO: Add condition for 40 character limit
        char *temp = strdup(user_input);

        // EXIT CONDITION
        if(strstr(user_input, "exit")) {
            should_run = 0 ;
            break ;
        }


        if (hist_found(user_input))
        {
            // this will change the user_input to the specified command from the history
            user_input = get_command_history(user_input, current);
            // printf("check: %s\n", user_input);
        }
        else
        {
            current = insert_node(current, temp);
            // printf("Node to be inserted: %s\n", current->input);
        }

        if (pipe_found(user_input))
        {
            handle_pipe_commands(user_input) ;
            continue;
        }

        if (check_for_Tree(strdup(user_input)) == 1)
        {
            char *tree_user_input = malloc(sizeof(char)*999);
            initialize_tree_user_input_with_NULL(tree_user_input);
            
            char **parsed_tree_args = parse_tree_args(strdup(user_input));

            if (parsed_tree_args[1] != NULL)
            {
                struct node *tree_root = tree_maker(parsed_tree_args[1]);
                int i = 0;
                in_order_traversal(tree_root, tree_user_input, &i);
                
                user_input = tree_user_input;
            }
            
            // continue;
        }

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
                child_process_args[(int)getpid()] = ampersand_tokenized_user_input[i];
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
            run_child(child_process_args[(int)getpid()]);
            child_process_args[(int)getpid()] = NULL; // depopulate child args when done
        }
        else
        {
            while (wait(NULL) > 0); // wait for all child processes
        }

        // free(user_input); // history buffer doesn't work by Turning it on
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