#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(){
        FILE *file;
        char ex[543];
        file=fopen("test.txt","r");
        fgets(ex,543,file);
        char *result=strstr(ex,"value");
        int start=result-ex+7;
        while(ex[start]!='"')
        {
                printf("%c",ex[start]);
                start++;
        }

        return 0;

}

