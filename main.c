#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>


struct secret{
        int username;
        char passwd[100];
        char *loginform;
        char *header;
        char *asp;
};

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

size_t body_consume(char *buffer, size_t size, size_t nitems, void *userdata) {
        int total=size*nitems;
        return total;
}

char *asp(char *v1, char *cook, int username, char *password){
        char *requestver1 = malloc((strlen(v1)+100) * sizeof(char));
        snprintf(requestver1,(strlen(v1)+100) * sizeof(char), "__RequestVerificationToken=%s&_UserName=%d&_QString=&_Password=%s", v1, username, password);
        char *requestcookie = malloc((strlen(cook)+100) * sizeof(char));
        snprintf(requestcookie,(strlen(cook)+100) * sizeof(char), "Cookie: __RequestVerificationToken=%s", cook);
        CURL *curl;
        CURLcode res;
        char *headers_data=malloc(1);
        *headers_data='\0';
        struct curl_slist *headers = NULL;
        char *url = "https://s.amizone.net/";
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url);
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestver1);
                headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
                headers = curl_slist_append(headers, "Referer: https://s.amizone.net/");
                headers = curl_slist_append(headers, "Connection: keep-alive");
                headers = curl_slist_append(headers, requestcookie);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, body_consume);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
                curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header);
                curl_easy_setopt(curl, CURLOPT_HEADERDATA, &headers_data);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
        }
        char *line=strtok(headers_data,"=");
        line=strtok(0,"=");
        line=strtok(0,"=");
        line=strtok(0,"=");
        line=strtok(0,"=");
        line=strtok(0,"=");
        line=strtok(0,"=;");
        return line;
}

void cookiev1(struct secret *s1, int username, char *password) {
        CURL *curl;
        CURLcode res;
        s1->loginform=malloc(1);
        *s1->loginform='\0';
        char *tmp_header=malloc(1);
        *tmp_header='\0';
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/");
                curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_header);
                curl_easy_setopt(curl, CURLOPT_HEADERDATA, &tmp_header);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_body);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s1->loginform);
                res = curl_easy_perform(curl);
        }
        s1->header=strtok(tmp_header,"=");
        s1->header=strtok(0,"=");
        s1->header=strtok(0,"=;");
        s1->asp=asp(s1->loginform,s1->header,username,password);
}









int main(){
        struct secret test;
        printf("Enter User ID: ");
        scanf("%d", &test.username);
        printf("Enter User ID: ");
        scanf("%s", test.passwd);
        cookiev1(&test,test.username,test.passwd);
        int count=1;
        while (count==0){
                printf("\n1. Exam Result\n2. Exam Schedule\n3. Fee Structure\n4. Calender Schedule\n5. Course\n6. Attendance\n7. Class Schedule\n8. Exit\nEnter your choice: ");
                int choice;
                while (scanf("%d", &choice)!= 1) {
                        while (getchar() != '\n');
                        printf("Invalid Choice\n");
                        break;
                }               
                switch(choice){
                        case 1:
                                printf("1");
                                break;
                        case 2:
                                printf("2");
                                break;
                        case 3:
                                printf("3");
                                break;
                        case 4:
                                printf("4");
                                break;
                        case 5:
                                printf("5");
                                break;
                        case 6:
                                printf("6");
                                break;
                        case 7:
                                printf("7");
                                break;
                        case 8:
                                printf("8");
                                count++;
                                break;
                        default:
                                printf("Invalid Choice\n");
                                break;
                }
        }
}


