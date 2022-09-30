#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_LINE 80 /* 80 chars per line, per command */

struct node
{
    char *input;
    struct node *next;
    struct node *prev;
};

struct node *create_node(char *input)
{
    struct node *new_node = (struct node *)malloc(sizeof(struct node));

    // new_node->input = malloc(sizeof(char) * MAX_LINE);
    new_node->input = input; // SET THE VALUE

    return new_node ;
}

struct node *insert_node(struct node *current, char *input)
{

    // WHEN THE LIST IS EMPTY
    if (current == NULL)
    {
        current = create_node(input);      // POINT TO NEW NODE
        current->next = current; // MAKE IT CIRCULAR
        current->prev = current ; // NEW

        return current;
    }

    struct node *new_node = create_node(input) ;
    new_node->next = current->next ;
    new_node->prev = current; // NEW
    current->next = new_node ;

    current = new_node ;

    current->next->prev = current ; // NEW

    return current ;

}

void display_list(struct node* current) {
    struct node *temp = current ;

    if(current == NULL) {
        printf("%s\n", "No Commands in history") ;
        return ;
    }

    while (temp->prev != current)
    {
        // printf("%s\n", "check-1") ;
        printf(" %s ->", temp->input) ;
        temp = temp->prev ;
    }
    // printf("%s\n", "check-2") ;
    printf(" %s", temp->input) ;
    printf("\n") ;
}


int list_length(struct node* current) {

    if(current == NULL) {
        return 0 ;
    }

    struct node *temp = current->next ;
    int count = 0;


    if(temp == current) {
        return 1 ;
    }

    while (temp != current)
    {
        // printf("%s ", temp->input) ;
        count++ ;
        temp = temp->next ;
    }

    return count+1 ;
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


char* command_at_index(struct node* current, int n) {

    struct node *temp = current ;
    for (int i = 1; i < n; i++)
    {
        temp = temp->prev ;
    }

    return temp->input ;
    
}

int hist_found(char *input) {
    char *temp = strstr(input, "hist") ;

    return (temp ? 1 : 0) ;

}

char * get_command_history(char *input, struct node* current) {

    int length = len(input) ;

    // hist with no args
    if(length <=5) {
        display_list(current) ;
        return input ;
    }
    else { 
        //TODO: Fix the code for 2 digit args----------
        // char *num = input[6] ;
        // int n = atoi(num) ;
        int n = (int) input[6] ;
        n = n-48 ;
        printf("Number: %d\n", n) ;

        if(list_length(current) < n || n==0) {
            printf("%s\n", "No command found") ;
        }
        else {
            input = command_at_index(current, n) ;
            printf("Command at %d is %s\n", n, input) ;
            return input ;
        }
    }

}


int main(void)
{
    struct node *current = NULL;


    while(1) {

        printf("osh>");
        fflush(stdout);

        char *user_input = read_input();

        if(hist_found(user_input)) {
            user_input = get_command_history(user_input, current) ;
            printf("%s\n", user_input) ;

        }
        else {
            current = insert_node(current, user_input);
        }
        // free(user_input);
    }

    // char *user_input = read_input();
    // current = insert_node(current, user_input);

    // char *cmd2 = read_input();
    // current = insert_node(current, cmd2);

    // char *cmd3 = read_input();
    // current = insert_node(current, cmd3);

    // // printf("%s %s\n", "Output:", current->input);
    // // printf("%s %s\n", "Output:", current->next->input);
    // // printf("%s %s\n", "Output:", current->next->next->input);
    // // display_list(current) ;

    // char *cmd4 = "hist -3" ;
    // get_command_history(cmd4, current) ;
    // printf("Length: %d\n", list_length(current)) ;
}