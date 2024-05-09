#include <regex.h>
#include <stdio.h>

int main()
{
	regex_t reegex;
        regmatch_t match;
	int value;
	value = regcomp( &reegex, "[1-9]", 0);
        value = regexec( &reegex, "GeeksforGeeks1",1, &match, 0);

        printf("%d",match.rm_so);
	return 0;
}

