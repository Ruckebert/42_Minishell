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
    printf("%-20s | %-4s | %s\n", "WORD", "TYPE", "LEADING_SPACE");
    printf("----------------------|------|--------------\n");

    // Traverse and print each token in table format
    curr = head;
    while (curr)
    {
        // %-20s ensures the word is left-aligned with 20 characters space
        // %-4d ensures the type is left-aligned with 4 characters space
        // %d prints the leading_space value
        printf("%-20s | %-4i | %i\n", curr->word, curr->type, curr->leading_space);
        curr = curr->next;
    }
}

void	free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head != NULL)
	{
		tmp = head;        // Store current node
		head = head->next; // Move to next node
		// Free the fields
		if (tmp->word != NULL)
			free(tmp->word);
		// Free the node itself
		free(tmp);
	}
}
