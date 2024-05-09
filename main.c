#include <stdio.h>
#include <curl/curl.h>
#include <string.h>

struct info{
        int username;
        char password[100];
};



size_t write_chunk(char *data, size_t size, size_t nmemb, char *userdata){
        char *result=strstr(data,"value");
        int start=result-data+7;
        while(data[start]!='"')
        {
                printf("%c",data[start]);
                start++;
        }
        return 0;
}



int requestver1(){
        CURL *curl;
        CURLcode res;
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/");
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_chunk);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
        }
        return 0;
}

size_t header_callback(char *buffer, size_t size,size_t nitems, void *userdata){
        printf("%s",buffer);
        return size*nitems;
}

int requestcookie(){
        CURL *curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/");
                curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
                curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_callback);
                curl_easy_perform(curl);
        }
        return 0;
}











int main(){
        requestcookie();
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


