#include "../minishell.h"

int main (void)
{
	char str[] = "echo \"hello\" |cat '-e >' myfile | test";

	char *wordable;
	char *quotes;
}