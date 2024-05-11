#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>



size_t write_header(char *buffer, size_t size, size_t nitems, void *userdata) {
        int total=size*nitems;
        char **header_data = (char **)userdata;
        *header_data=realloc(*header_data,strlen(*header_data)+total+1);
        strncat(*header_data,buffer,total);
        return total;
}

size_t write_body(char *buffer, size_t size, size_t nitems, void *userdata) {
        int total=size*nitems;
        char **loginform = (char **)userdata;
        *loginform=realloc(*loginform,strlen(*loginform)+total+1);
        char *result=strstr(buffer,"value");
        int start=result-buffer+7;
        while(buffer[start]!='"')
        {
                strncat(*loginform,&buffer[start],1);
                start++;
        }
        return total;
}

int main() {
        CURL *curl;
        CURLcode res;
        char *loginform=malloc(1);
        *loginform='\0';
        char *header=malloc(1);
        *header='\0';
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/");
                curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header);
                curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &loginform);
                res = curl_easy_perform(curl);
        }
        char *line=strtok(header,"=");
        line=strtok(0,"=");
        line=strtok(0,"=;");
        printf("%s\n",loginform);
        printf("%s\n",line);
        return 0;
}

