#include "../minishell.h"

void printCharPointerArray(char **arr) 
{
	int i = 0;

    while (arr[i] != NULL) 
	{
        printf("%s\n", arr[i]);
		i++;
    }
}

void print_words(t_token *head)
{
    t_token *current = head;

    while (current != NULL)
    {
        if (current->word != NULL)
            printf("%s\n", current->word);
        current = current->next;
    }
}
/*
void printlist(t_token *head)
{
    t_token *curr;

    if (!head)
        return;
    curr = head;
    while(curr)
    {
        printf("\nWORD:%s\n", curr->word);
        curr = curr->next;
    }
}
*/

void printlist_type(t_token *head)
{
    t_token *curr;

    if (!head)
        return;
    curr = head;
    while(curr)
    {
        printf("%i\n", curr->type);
        curr = curr->next;
    }
}


void printlist(t_token *head)
{
    t_token *curr;

    if (!head)
        return;

    // Print table header
    printf("%-20s | %s\n", "WORD", "TYPE");
    printf("----------------------|------\n");

    // Traverse and print each token in table format
    curr = head;
    while (curr)
    {
        // %-20s ensures the word is left-aligned with 20 characters space
        printf("%-20s | %i\n", curr->word, curr->type);
        curr = curr->next;
    }
}
