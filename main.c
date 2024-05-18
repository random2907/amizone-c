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
        char *session;
};

size_t write_header(char *buffer, size_t size, size_t nitems, char *userdata) {
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



char *session(char *v1, char *cook){
        char *requestver1 = malloc((strlen(v1)+100) * sizeof(char));
        snprintf(requestver1,(strlen(v1)+100) * sizeof(char), "__RequestVerificationToken=%s", v1);
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
        line=strtok(0,"=;");
        return line;
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
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/");
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
        s1->session=session(s1->loginform,s1->header);
}

// real good

int fee(char *cook, char *session, char *asp){
        char *requestcookie = malloc((strlen(cook)+strlen(asp)+100) * sizeof(char));
        snprintf(requestcookie,(strlen(cook)+strlen(asp)+100) * sizeof(char), "Cookie: __RequestVerificationToken=%s ASP.NET_SessionId=%s; .ASPXAUTH=%s", cook, session, asp);
        CURL *curl;
        CURLcode res;
        struct curl_slist *headers = NULL;
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/FeeStructure/FeeStructure/AllFeeReceipt");
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "ifeetype=1");
                headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded; charset=UTF-8");
                headers = curl_slist_append(headers, "Referer: https://s.amizone.net/Home");
                headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
                headers = curl_slist_append(headers, "Connection: keep-alive");
                headers = curl_slist_append(headers, requestcookie);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
        }
        return 0;
}



int class_schedule(char *cook, char *asp, int current){
        char start[11], end[11];
        if (current==0){
                scanf("%s",start);
                scanf("%s",end);
        }else{
                time_t current_time;
                struct tm *timeinfo;
                time(&current_time);
                timeinfo = localtime(&current_time);
                strftime(start, sizeof(start), "%Y-%m-%d", timeinfo);
                strftime(end, sizeof(end), "%Y-%m-%d", timeinfo);
        }

        char *requestcookie = malloc((strlen(cook)+strlen(asp)+100) * sizeof(char));
        snprintf(requestcookie, (strlen(cook)+strlen(asp)+100) * sizeof(char), "Cookie: __RequestVerificationToken=%s; .ASPXAUTH=%s", cook, asp);
        char *url = malloc((strlen(start)+strlen(end)+100) * sizeof(char));
        snprintf(url, (strlen(cook)+strlen(asp)+100) * sizeof(char), "https://s.amizone.net/Calendar/home/GetDiaryEvents?start=%s&end=%s&_=1707456987909", start, end);
        CURL *curl;
        CURLcode res;
        struct curl_slist *headers = NULL;
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, url);
                headers = curl_slist_append(headers, "Referer: https://s.amizone.net/Home");
                headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
                headers = curl_slist_append(headers, "Connection: keep-alive");
                headers = curl_slist_append(headers, requestcookie);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
        }
        return 0;
}


size_t write_attend(char *buffer, size_t size, size_t nitems, char **userdata) {
        int total=size*nitems;
        *userdata=realloc(*userdata,strlen(*userdata)+total+1);
        strncat(*userdata,buffer,total);
        return 1;
}

int parse_attend(char *buff){
        // course_code
        int position_str[50];
        int count_str=0;
        char *result=buff;
        while ((result=strstr(result,"sub-code"))!=NULL){
                position_str[count_str]=result-buff+strlen("sub-code")+2;
                result += strlen("sub-code");
                count_str++;
        }
        char course_code[count_str][100];
        for (int i=0; i<count_str; i++){
                int y=0;
                while (((buff+position_str[i])[y])!=' '){
                        course_code[i][y]=((buff+position_str[i])[y]);
                        y++;
                }
                course_code[i][y]='\0';
        }

        // percentage_attend

        int position_str1[50];

        int count_str1=0;
        char *result1=buff;
        while ((result1=strstr(result1,"class-count"))!=NULL){
                position_str1[count_str1]=result1-buff+strlen("class-count")+2;
                result1 += strlen("class-count");
                count_str1++;
        }

        char course_code1[count_str1][100];

        for (int i=0; i<count_str1; i++){
                int y=0;
                while (((buff+position_str1[i])[y])!=' '){
                        course_code1[i][y]=((buff+position_str1[i])[y]);
                        y++;
                }
                course_code1[i][y]='\0';
        }

        // course_name


        char course_code2[count_str][100];

        for (int i=0; i<count_str; i++){
                int y=0;
                while (((buff+position_str[i]+strlen(course_code[i])+strlen(" </span>       "))[y])!='<'){
                        course_code2[i][y]=((buff+position_str[i]+strlen(course_code[i])+strlen(" </span>       "))[y]);
                        y++;
                }
                course_code2[i][y]='\0';
        }

        // number_attend


        char course_code3[count_str][100];

        for (int i=0; i<count_str; i++){
                int y=0;
                while (((buff+position_str1[i]+strlen(course_code1[i])+strlen(" <span>"))[y])!='<'){
                        course_code3[i][y]=((buff+position_str1[i]+strlen(course_code1[i])+strlen(" <span>"))[y]);
                        y++;
                }
                course_code3[i][y]='\0';
        }

        // to json

        char attend_json[5000];
        attend_json[0]='[';
        for (int i=0; i<count_str; i++){
                char buffer[200];
                snprintf(buffer,sizeof(buffer),"{\"code\":\"%s\", \"title\":\"%s\", \"completion_percentage\":\"%s %s\"}", course_code[i],course_code2[i], course_code1[i],course_code3[i]);
                strncat(attend_json, buffer, sizeof(attend_json)-strlen(attend_json)-1); 
                if (i<count_str-1){
                        strncat(attend_json, ",", sizeof(attend_json)-strlen(attend_json)-1); 
                }
        }
        strncat(attend_json, "]", sizeof(attend_json) - strlen(attend_json) - 1);
        printf("%s",attend_json);

        return 0;
}

int attendence(char *cook, char *asp){
        char *requestcookie = malloc((strlen(cook)+strlen(asp)+100) * sizeof(char));
        snprintf(requestcookie, (strlen(cook)+strlen(asp)+100) * sizeof(char), "Cookie: __RequestVerificationToken=%s; .ASPXAUTH=%s", cook, asp);
        CURL *curl;
        CURLcode res;
        char *test=malloc(1);
        *test='\0';
        struct curl_slist *headers = NULL;
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/Home/_Home?X-Requested-With=XMLHttpRequest");
                headers = curl_slist_append(headers, "Referer: https://s.amizone.net/Home");
                headers = curl_slist_append(headers, "X-Requested-With: XMLHttpRequest");
                headers = curl_slist_append(headers, "Connection: keep-alive");
                headers = curl_slist_append(headers, requestcookie);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_attend);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &test);
                res = curl_easy_perform(curl);
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
        }

        parse_attend(test);

        return 0;
}


int main(){
        struct secret test;
        scanf("%d", &test.username);
        scanf("%s",test.passwd);
        cookiev1(&test,test.username,test.passwd);
        attendence(test.header, test.asp);
        //calender_schedule(test.header, test.asp,0);
        //class_schedule(test.header, test.asp,1);
        //fee(test.header, test.session, test.asp);
}
