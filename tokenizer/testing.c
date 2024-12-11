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
    printf("\n\n");
}

void printlist_both(t_token *head)
{
    t_token *curr;
    t_token *last;

    if (!head)
        return;

    // Print table header
    printf("FORWARD PRINT\n");
    printf("%-20s | %-4s | %s\n", "WORD", "TYPE", "LEADING_SPACE");
    printf("----------------------|------|--------------\n");

    // Traverse and print each token in table format (forward direction)
    curr = head;
    while (curr)
    {
        printf("%-20s | %-4i | %i\n", curr->word, curr->type, curr->leading_space);
        if (!curr->next)
            last = curr; // Keep track of the last element for reverse traversal
        curr = curr->next;
    }

    // Print table header for reverse printing
    printf("\nBACKWARD PRINT\n");
    printf("%-20s | %-4s | %s\n", "WORD", "TYPE", "LEADING_SPACE");
    printf("----------------------|------|--------------\n");

    // Traverse and print each token in table format (backward direction)
    curr = last;
    while (curr)
    {
        printf("%-20s | %-4i | %i\n", curr->word, curr->type, curr->leading_space);
        curr = curr->prev;
    }
    printf("\n\n");
}


void    free_token_list(t_token *head)
{
	t_token	*tmp;

	while (head != NULL)
	{
		tmp = head;
		head = head->next;
        if (tmp->word)
		    free (tmp->word);
        free (tmp);
	}
}

void free_cmdtable(t_cmdtable **head)
{
    t_cmdtable *tmp;
    t_cmdtable *next;
    int i;

    tmp = *head;
    while (tmp != NULL)
    {
        next = tmp->next;

        if (tmp->args)
        {
            for (i = 0; tmp->args[i]; i++)
                free(tmp->args[i]);
            free(tmp->args);
        }

        if (tmp->redir)
            free(tmp->redir);

        free(tmp);
        tmp = next;
    }
    *head = NULL;
}


void print_cmdtable(t_cmdtable *cmd)
{
    int i;
    t_cmdtable *current = cmd;

    printf("-------------------------------------------------------------------------------------------\n");
    printf("| %-5s | %-20s | %-10s | %-15s | %-10s | %-10s |\n", 
           "Node", "Arguments", "Builtin", "Redir Type", "Redir", "Pipe After");
    printf("-------------------------------------------------------------------------------------------\n");

    int node_num = 1;
    while (current != NULL)
    {
        // Print node number
        printf("| %-5d | ", node_num);

        // Print arguments
        if (current->args)
        {
            printf("[");
            for (i = 0; current->args[i] != NULL; i++)
            {
                if (current->args[i] == NULL )  // Safety check for NULL values
                {
                    printf("NULL");
                    break;
                }
                printf("%s", current->args[i]);
                if (current->args[i + 1] != NULL)
                    printf(", ");
            }
            printf("] ");
        }
        else
        {
            printf("No Args           ");
        }

        // Print whether it is a built-in command
        printf("| %-10d | ", current->isbuiltin);

        // Print redir_type and redir values
        printf("| %-15d | %-10s | ", current->redir_type, current->redir ? current->redir : "No Redir");

        // Print whether it has a pipe after
        printf("%-10s |\n", current->has_pipe_after ? "Yes" : "No");

        // Print divider line
        printf("-------------------------------------------------------------------------------------------\n");

        // Safety check before moving to the next node
        if (current->next == current)  // Prevent self-referencing nodes
        {
            printf("Error: Self-referencing node detected. Aborting to prevent infinite loop.\n");
            break;
        }

        current = current->next;
        node_num++;
    }
}
