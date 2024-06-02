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

        char *result=strstr(headers_data,"set-cookie: ASP.NET_SessionId=");
        int position=result-headers_data+strlen("set-cookie: ASP.NET_SessionId=");

        int y=0;
        while (((headers_data+position)[y])!=';'){
                y++;
        }

        char *tmp_session=malloc((y+1)*sizeof(char));
        *tmp_session='\0';
        strncat(tmp_session,headers_data+position,y);
        free(requestcookie);
        free(requestver1);
        free(headers_data);

        return tmp_session;
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
        char *result=strstr(headers_data,"set-cookie: .ASPXAUTH=");
        result+=strlen("set-cookie: .ASPXAUTH=");
        result=strstr(result,"set-cookie: .ASPXAUTH=");
        int position=result-headers_data+strlen("set-cookie: .ASPXAUTH=");

        int y=0;
        while (((headers_data+position)[y])!=';'){
                y++;
        }

        char *tmp_asp=malloc((y+1)*sizeof(char));
        *tmp_asp='\0';
        strncat(tmp_asp,headers_data+position,y);
        free(requestcookie);
        free(requestver1);
        free(headers_data);
        return tmp_asp;
}

int cookiev1(struct secret *s1, int username, char *password) {
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
        //parse tmp_header to get requestcookie
        char *result=strstr(tmp_header,"set-cookie: __RequestVerificationToken=");
        int position=result-tmp_header+strlen("set-cookie: __RequestVerificationToken=");

        int y=0;
        while (((tmp_header+position)[y])!=';'){
                y++;
        }
        s1->header=malloc((y+1)*sizeof(char));
        *s1->header='\0';
        strncat(s1->header,tmp_header+position,y);

        s1->asp=asp(s1->loginform,s1->header,username,password);
        s1->session=session(s1->loginform,s1->header);
        free(tmp_header);
        //
        return 0;
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
        free(requestcookie);
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
        free(requestcookie);
        free(url);
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

        size_t max_buffer_size=0;
        for (int i=0; i<count_str; i++){
                max_buffer_size+=(70+strlen(course_code[i])+strlen(course_code1[i])+strlen(course_code2[i])+strlen(course_code3[i]));
        }
        char *attend_json=malloc(max_buffer_size*sizeof(char));
        attend_json[0]='[';
        attend_json[1]='\0';
        for (int i=0; i<count_str; i++){
                size_t buffer_size=(70+strlen(course_code[i])+strlen(course_code1[i])+strlen(course_code2[i])+strlen(course_code3[i]));
                char *buffer=malloc(buffer_size*sizeof(char));
                snprintf(buffer,buffer_size,"{\"code\":\"%s\", \"title\":\"%s\", \"completion_percentage\":\"%s %s\"}", course_code[i],course_code2[i], course_code1[i],course_code3[i]);
                strncat(attend_json, buffer, max_buffer_size-strlen(attend_json)-1); 
                if (i<count_str-1){
                        strncat(attend_json, ",", max_buffer_size-strlen(attend_json)-1); 
                }
                free(buffer);
        }
        strncat(attend_json, "]", max_buffer_size - strlen(attend_json) - 1);
        printf("%s\n",attend_json);

        free(attend_json);
        free(buff);
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
        free(requestcookie);
        return 0;
}

int parse_examschedule(char *buff){
        int count=0;
        int locate[50];
        char *result=buff;
        char searchbuff[][50]={"<td data-title=\"Course Code\">","<td data-title=\"Course Title\">","<td data-title=\"Exam Date\">","<td data-title=\"Time\">","<td data-title=\"Paper Type\">"};

        while((strstr(result,searchbuff[4]))!=NULL){

                result=strstr(result,searchbuff[0]);
                locate[count]=result-buff+strlen(searchbuff[0]);
                result+=strlen(searchbuff[0]);
                count++;

                result=strstr(result,searchbuff[1]);
                locate[count]=result-buff+strlen(searchbuff[1]);
                result+=strlen(searchbuff[1]);
                count++;

                result=strstr(result,searchbuff[2]);
                locate[count]=result-buff+strlen(searchbuff[2]);
                result+=strlen(searchbuff[2]);
                count++;

                result=strstr(result,searchbuff[3]);
                locate[count]=result-buff+strlen(searchbuff[3]);
                result+=strlen(searchbuff[3]);
                count++;

                result=strstr(result,searchbuff[4]);
                locate[count]=result-buff+strlen(searchbuff[4])+strlen("<b>Exam Mode : ");;
                result+=strlen(searchbuff[4]);
                count++;

        }

        char extracted_data[count][100];
        for (int i=0; i<(count); i++){
                int y=0;
                while (((buff+locate[i]))[y]!='<'){
                        extracted_data[i][y]=((buff+locate[i])[y]);
                        y++;
                }
                extracted_data[i][y]='\0';
        }

        // to json

        size_t max_buffer_size=0;  // default for json key
        for (int i=0; i<count; i++){
                max_buffer_size+=(70+strlen(extracted_data[i]));
        }
        char *examsched_json=malloc(max_buffer_size*sizeof(char));
        examsched_json[0]='[';
        examsched_json[1]='\0';
        for (int i=0; i<count; i+=5){
                size_t buffer_size=(80+strlen(extracted_data[i])+strlen(extracted_data[i+1])+strlen(extracted_data[i+2])+strlen(extracted_data[i+3])+strlen(extracted_data[i+4]));
                char *buffer=malloc(buffer_size*sizeof(char));
                snprintf(buffer,buffer_size,"{\"Course_Code\":\"%s\", \"Course_Title\":\"%s\", \"Date\":\"%s\", \"Time\":\"%s\", \"Exam_Type\":\"%s\"}",extracted_data[i],extracted_data[i+1],extracted_data[i+2],extracted_data[i+3],extracted_data[i+4]);
                strncat(examsched_json, buffer, max_buffer_size-strlen(examsched_json)-1); 

                if (i+5<count){
                        strncat(examsched_json, ",", max_buffer_size-strlen(examsched_json)-1); 
                }
                free(buffer);
        }
        strncat(examsched_json, "]", max_buffer_size - strlen(examsched_json) - 1);
        printf("%s\n",examsched_json);

        free(examsched_json);
        free(buff);
        return 0;

}

int exam_schedule(char *cook, char *asp, char *session){
        char *requestcookie = malloc((strlen(cook)+strlen(asp)+100) * sizeof(char));
        snprintf(requestcookie, (strlen(cook)+strlen(asp)+100) * sizeof(char), "Cookie: __RequestVerificationToken=%s; .ASPXAUTH=%s; asp.net_sessionid=%s", cook, asp, session);
        CURL *curl;
        CURLcode res;
        char *test=malloc(1);
        *test='\0';
        struct curl_slist *headers = NULL;
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/Examination/ExamSchedule?X-Requested-With=XMLHttpRequest");
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
        parse_examschedule(test);
        free(requestcookie);
        return 0;
}

int parse_result(char *buff){
        int count=0;
        int locate[150];
        char *result=buff;
        char searchbuff[][50]={"<td data-title=\"Sno\">","<td data-title=\"Course Code\">","<td data-title=\"Course Title\">","<td data-title=\"Max Total\">","<td data-title=\"ACU\">","<td data-title=\"Go\">","<td data-title=\"GP\">","<td data-title=\"CP\">","<td data-title=\"ECU\">","<td data-title=\"PublishDate\">","<td data-title=\"Semester\">","<td data-title=\"SGPA\">","<td data-title=\"CGPA\">","<td data-title=\"Back Papers\">"};

        while((strstr(result,searchbuff[9]))!=NULL){

                result=strstr(result,searchbuff[0]);
                locate[count]=result-buff+strlen(searchbuff[0]);
                result+=strlen(searchbuff[0]);
                count++;

                result=strstr(result,searchbuff[1]);
                locate[count]=result-buff+strlen(searchbuff[1]);
                result+=strlen(searchbuff[1]);
                count++;

                result=strstr(result,searchbuff[2]);
                locate[count]=result-buff+strlen(searchbuff[2]);
                result+=strlen(searchbuff[2]);
                count++;

                result=strstr(result,searchbuff[3]);
                locate[count]=result-buff+strlen(searchbuff[3]);
                result+=strlen(searchbuff[3]);
                count++;

                result=strstr(result,searchbuff[4]);
                locate[count]=result-buff+strlen(searchbuff[4]);
                result+=strlen(searchbuff[4]);
                count++;

                result=strstr(result,searchbuff[5]);
                locate[count]=result-buff+strlen(searchbuff[5]);
                result+=strlen(searchbuff[5]);
                count++;

                result=strstr(result,searchbuff[6]);
                locate[count]=result-buff+strlen(searchbuff[6]);
                result+=strlen(searchbuff[6]);
                count++;

                result=strstr(result,searchbuff[7]);
                locate[count]=result-buff+strlen(searchbuff[7]);
                result+=strlen(searchbuff[7]);
                count++;

                result=strstr(result,searchbuff[8]);
                locate[count]=result-buff+strlen(searchbuff[8]);
                result+=strlen(searchbuff[8]);
                count++;

                result=strstr(result,searchbuff[9]);
                locate[count]=result-buff+strlen(searchbuff[9]);
                result+=strlen(searchbuff[9]);
                count++;

        }

        // left semester, scgpa, cgpa, back papers

        int locate_ex[4];
        result=strstr(result,searchbuff[10]);
        result=strstr(result," ");
        locate_ex[0]=result-buff+strlen(searchbuff[10]);
        result+=strlen(searchbuff[10]);

        result=strstr(result,searchbuff[11]);
        result=strstr(result," ");
        locate_ex[1]=result-buff+strlen(searchbuff[11]);
        result+=strlen(searchbuff[11]);

        result=strstr(result,searchbuff[12]);
        result=strstr(result," ");
        locate_ex[2]=result-buff+strlen(searchbuff[12]);
        result+=strlen(searchbuff[12]);

        result=strstr(result,searchbuff[13]);
        result=strstr(result," ");
        locate_ex[3]=result-buff+strlen(searchbuff[13]);
        result+=strlen(searchbuff[13]);

        //done


        char extracted_data[count][100];
        for (int i=0; i<count; i++){
                int y=0;
                while (((buff+locate[i]))[y]!='<'){
                        extracted_data[i][y]=((buff+locate[i])[y]);
                        y++;
                }
                extracted_data[i][y]='\0';
        }

        char extracted_data_ex[4][100];
        for (int i=0; i<4; i++){
                int y=0;
                int k=0;
                while (((buff+locate_ex[i]))[k]==' '){
                        k++;
                }
                while (((buff+locate_ex[i]+k))[y]!='&'){
                        extracted_data_ex[i][y]=((buff+locate_ex[i]+k)[y]);
                        y++;
                }
                extracted_data_ex[i][y]='\0';
        }

        size_t max_buffer_size=0;  // default for json key
        max_buffer_size+=(70+strlen(extracted_data_ex[0])+strlen(extracted_data_ex[1])+strlen(extracted_data_ex[2])+strlen(extracted_data_ex[3]));

        for (int i=0; i<count; i++){
                max_buffer_size+=(70+strlen(extracted_data[i]));
        }
        char *examresult_json=malloc(max_buffer_size*sizeof(char));
        examresult_json[0]='[';
        examresult_json[1]='\0';
        size_t buffer_size_ex=(70+strlen(extracted_data_ex[0])+strlen(extracted_data_ex[1])+strlen(extracted_data_ex[2])+strlen(extracted_data_ex[3]));
        char *buffer_ex=malloc(buffer_size_ex*sizeof(char));
        snprintf(buffer_ex,buffer_size_ex,"{\"Semester\":\"%s\", \"SGPA\":\"%s\", \"CGPA\":\"%s\", \"Back Papers\":\"%s\"}",extracted_data_ex[0],extracted_data_ex[1],extracted_data_ex[2],extracted_data_ex[3]);
        strncat(examresult_json, buffer_ex, max_buffer_size-strlen(examresult_json)-1);
        strncat(examresult_json, ",", max_buffer_size-strlen(examresult_json)-1); 

        for (int i=0; i<count; i+=10){
                size_t buffer_size=(140+strlen(extracted_data[i])+strlen(extracted_data[i+1])+strlen(extracted_data[i+2])+strlen(extracted_data[i+3])+strlen(extracted_data[i+4])+strlen(extracted_data[i+5])+strlen(extracted_data[i+6])+strlen(extracted_data[i+7])+strlen(extracted_data[i+8])+strlen(extracted_data[i+9]));
                char *buffer=malloc(buffer_size*sizeof(char));
                snprintf(buffer,buffer_size,"{\"Sno\":\"%s\", \"Course_Code\":\"%s\", \"Course_Title\":\"%s\", \"Max_Total\":\"%s\", \"ACU\":\"%s\", \"Go\":\"%s\", \"GP\":\"%s\", \"CP\":\"%s\", \"ECU\":\"%s\", \"PublishDate\":\"%s\"}",extracted_data[i],extracted_data[i+1],extracted_data[i+2],extracted_data[i+3],extracted_data[i+4],extracted_data[i+5],extracted_data[i+6],extracted_data[i+7],extracted_data[i+8],extracted_data[i+9]);
                strncat(examresult_json, buffer, max_buffer_size-strlen(examresult_json)-1); 

                if (i+10<count){
                        strncat(examresult_json, ",", max_buffer_size-strlen(examresult_json)-1); 
                }
                free(buffer);
        }
        strncat(examresult_json, "]", max_buffer_size - strlen(examresult_json) - 1);
        printf("%s\n",examresult_json);
        free(examresult_json);
        free(buffer_ex);
        free(buff);



        return 0;
}


int exam_result(char *cook, char *asp, char *session, int sem){
        char *requestcookie = malloc((strlen(cook)+strlen(asp)+strlen(session)+100) * sizeof(char));
        snprintf(requestcookie, (strlen(cook)+strlen(asp)+strlen(session)+100) * sizeof(char), "Cookie: __RequestVerificationToken=%s; ASP.NET_SessionId=%s; .ASPXAUTH=%s", cook, session, asp);
        char semester[20];
        snprintf(semester, sizeof(semester), "sem=%d", sem);
        CURL *curl;
        CURLcode res;
        char *test=malloc(1);
        *test='\0';
        struct curl_slist *headers = NULL;
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/Examination/Examination?X-Requested-With=XMLHttpRequest");
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, semester);
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
        parse_result(test); 
        return 0;
}

int parse_course(char *buff){
        int count_comp=0;
        int count_domain=0;
        int locate_comp[150];
        int locate_domain[150];
        char *result=buff;
        char searchbuff[][50]={"<td data-title=\"Course Code\">","<td data-title=\"Course Name\">","<td data-title=\"Type\">","<td data-title=\"Course Syllabus\">","<td data-title=\"Attendance\">","<td data-title=\"Internal Asses.\">"};
        int comp=strstr(buff,"</tbody")-buff;

        while((strstr(result,searchbuff[5]))!=NULL){
                if (comp>=(strstr(result,searchbuff[0])-buff)){
                        result=strstr(result,searchbuff[0]);
                        locate_comp[count_comp]=result-buff+strlen(searchbuff[0]);
                        result+=strlen(searchbuff[0]);
                        count_comp++;

                        result=strstr(result,searchbuff[1]);
                        locate_comp[count_comp]=result-buff+strlen(searchbuff[1]);
                        result+=strlen(searchbuff[1]);
                        count_comp++;

                        result=strstr(result,searchbuff[2]);
                        locate_comp[count_comp]=result-buff+strlen(searchbuff[2]);
                        result+=strlen(searchbuff[2]);
                        count_comp++;

                        result=strstr(result,searchbuff[3]);
                        result=strstr(result,"href=");
                        locate_comp[count_comp]=result-buff+strlen("href=");
                        result+=strlen(searchbuff[3]);
                        count_comp++;

                        result=strstr(result,searchbuff[4]);
                        result=strstr(result,"class");
                        result=strstr(result+strlen("class"),"class");
                        result=strstr(result+strlen("class"),">");
                        locate_comp[count_comp]=result-buff+strlen(">");
                        result+=strlen(searchbuff[4]);
                        count_comp++;

                        result=strstr(result,searchbuff[5]);
                        locate_comp[count_comp]=result-buff+strlen(searchbuff[5]);
                        result+=strlen(searchbuff[5]);
                        count_comp++;
                }else{
                        result=strstr(result,searchbuff[0]);
                        result=strstr(result," ");
                        locate_domain[count_domain]=result-buff+strlen(searchbuff[0]);
                        result+=strlen(searchbuff[0]);
                        count_domain++;

                        result=strstr(result,searchbuff[1]);
                        result=strstr(result," ");
                        locate_domain[count_domain]=result-buff+strlen(searchbuff[1]);
                        result+=strlen(searchbuff[1]);
                        count_domain++;

                        result=strstr(result,searchbuff[2]);
                        result=strstr(result," ");
                        locate_domain[count_domain]=result-buff+strlen(searchbuff[2]);
                        result+=strlen(searchbuff[2]);
                        count_domain++;

                        result=strstr(result,searchbuff[3]);
                        result=strstr(result,"href=");
                        locate_domain[count_domain]=result-buff+strlen("href=");
                        result+=strlen(searchbuff[3]);
                        count_domain++;

                        result=strstr(result,searchbuff[4]);
                        result=strstr(result," ");
                        locate_domain[count_domain]=result-buff+strlen(searchbuff[4]);
                        result+=strlen(searchbuff[4]);
                        count_domain++;

                        result=strstr(result,searchbuff[5]);
                        result=strstr(result," ");
                        locate_domain[count_domain]=result-buff+strlen(searchbuff[5]);
                        result+=strlen(searchbuff[5]);
                        count_domain++;
                }
        }

        // for compulsory course
        char extracted_data_comp[count_comp][200];
        int link=3;
        for (int i=0; i<count_comp; i++){
                int y=0;
                if (link==i){
                        while ((buff+locate_comp[i])[y]!=' '){
                                extracted_data_comp[i][y]=((buff+locate_comp[i])[y]);
                                y++;
                        }
                        link+=6;
                }else {
                        while ((buff+locate_comp[i])[y]!='<'){
                                extracted_data_comp[i][y]=((buff+locate_comp[i])[y]);
                                y++;
                        }
                }
                extracted_data_comp[i][y]='\0';
        }

        //for domain course
        
        char extracted_data_domain[count_domain][200];
        link=3;
        int attend=4;
        for (int i=0; i<count_domain; i++){
                int y=0;
                int k=0;
                while (((buff+locate_domain[i]))[k]==' '){
                        k++;
                }
                if (link==i){
                        int nptel_link=0;
                        if (((buff+locate_domain[i])+k)[y]=='"'){
                                nptel_link++;
                        }
                        while (((buff+locate_domain[i])+k+nptel_link)[y]!=' '){
                                if (((buff+locate_domain[i])+k+nptel_link)[y]=='"'){
                                        break;
                                }
                                extracted_data_domain[i][y]=((buff+locate_domain[i]+k+nptel_link)[y]);
                                y++;
                        }
                        link+=6;
                }else if (attend==i){
                        if ((strstr(buff+locate_domain[i],"class")-buff)<locate_domain[i+1]){
                                // ugly fix because nptel i dont like it.
                                char *temp=strstr(buff+locate_domain[i],"");
                                temp=strstr(temp,"class");
                                temp=strstr(temp+strlen("class"),"class");
                                temp=strstr(temp+strlen("class"),">")+strlen(">");
                                while ((temp)[y]!='<'){
                                        extracted_data_domain[i][y]=((temp)[y]);
                                        y++;
                                }
                        }else{
                                while (((buff+locate_domain[i])+k)[y]!='\r'){
                                        extracted_data_domain[i][y]=((buff+locate_domain[i]+k)[y]);
                                        y++;
                                }
                        }
                        attend+=6;
                }else {
                        while (((buff+locate_domain[i]+k))[y]!='\r'){
                                extracted_data_domain[i][y]=((buff+locate_domain[i]+k)[y]);
                                y++;
                        }
                }
                extracted_data_domain[i][y]='\0';
        }

        size_t max_buffer_size=0;  // default for json key
        for (int i=0; i<count_comp; i++){
                max_buffer_size+=(70+strlen(extracted_data_comp[i]));
        }
        for (int i=0; i<count_domain; i++){
                max_buffer_size+=(70+strlen(extracted_data_domain[i]));
        }
        char *course_json=malloc(max_buffer_size*sizeof(char));
        course_json[0]='[';
        course_json[1]='\0';
        for (int i=0; i<count_comp; i+=6){
                size_t buffer_size=(140+strlen(extracted_data_comp[i])+strlen(extracted_data_comp[i+1])+strlen(extracted_data_comp[i+2])+strlen(extracted_data_comp[i+3])+strlen(extracted_data_comp[i+4])+strlen(extracted_data_comp[i+5])+strlen(extracted_data_comp[i+6])+strlen(extracted_data_comp[i+7])+strlen(extracted_data_comp[i+8])+strlen(extracted_data_comp[i+9]));
                char *buffer=malloc(buffer_size*sizeof(char));
                snprintf(buffer,buffer_size,"{\"Course_Code\":\"%s\", \"Course_Name\":\"%s\", \"Type\":\"%s\", \"link\":\"%s\", \"Attendance\":\"%s\", \"Internal Asses.\":\"%s\"}",extracted_data_comp[i],extracted_data_comp[i+1],extracted_data_comp[i+2],extracted_data_comp[i+3],extracted_data_comp[i+4],extracted_data_comp[i+5]);
                strncat(course_json, buffer, max_buffer_size-strlen(course_json)-1); 
                strncat(course_json, ",", max_buffer_size-strlen(course_json)-1); 
                free(buffer);
        }
        for (int i=0; i<count_domain; i+=6){
                size_t buffer_size=(140+strlen(extracted_data_domain[i])+strlen(extracted_data_domain[i+1])+strlen(extracted_data_domain[i+2])+strlen(extracted_data_domain[i+3])+strlen(extracted_data_domain[i+4])+strlen(extracted_data_domain[i+5])+strlen(extracted_data_domain[i+6])+strlen(extracted_data_domain[i+7])+strlen(extracted_data_domain[i+8])+strlen(extracted_data_domain[i+9]));
                char *buffer=malloc(buffer_size*sizeof(char));
                snprintf(buffer,buffer_size,"{\"Course_Code\":\"%s\", \"Course_Name\":\"%s\", \"Type\":\"%s\", \"link\":\"%s\", \"Attendance\":\"%s\", \"Internal Asses.\":\"%s\"}",extracted_data_domain[i],extracted_data_domain[i+1],extracted_data_domain[i+2],extracted_data_domain[i+3],extracted_data_domain[i+4],extracted_data_domain[i+5]);
                strncat(course_json, buffer, max_buffer_size-strlen(course_json)-1); 

                if (i+6<count_domain){
                        strncat(course_json, ",", max_buffer_size-strlen(course_json)-1); 
                }
                free(buffer);
        }
        strncat(course_json, "]", max_buffer_size - strlen(course_json) - 1);
        printf("%s\n",course_json);
        free(course_json);
        free(buff);
        return 0;

}

int course_list(char *cook, char *asp, char *session, int sem){
        char *requestcookie = malloc((strlen(cook)+strlen(asp)+strlen(session)+100) * sizeof(char));
        snprintf(requestcookie, (strlen(cook)+strlen(asp)+strlen(session)+100) * sizeof(char), "Cookie: __RequestVerificationToken=%s; ASP.NET_SessionId=%s; .ASPXAUTH=%s", cook, session, asp);
        char semester[20];
        snprintf(semester, sizeof(semester), "sem=%d", sem);
        CURL *curl;
        CURLcode res;
        char *test=malloc(1);
        *test='\0';
        struct curl_slist *headers = NULL;
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://s.amizone.net/Academics/MyCourses/CourseListSemWise");
                curl_easy_setopt(curl, CURLOPT_POST, 1L);
                curl_easy_setopt(curl, CURLOPT_POSTFIELDS, semester);
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
        parse_course(test);
        return 0;
}

int main(){
        struct secret test;
        scanf("%d", &test.username);
        scanf("%s",test.passwd);
        cookiev1(&test,test.username,test.passwd);
        course_list(test.header, test.asp, test.session, 2);
        /*exam_result(test.header, test.asp, test.session, 1);*/
        /*exam_schedule(test.header, test.asp, test.session);*/
        /*attendence(test.header, test.asp);*/
        /*calender_schedule(test.header, test.asp,0);*/
        /*class_schedule(test.header, test.asp,1);*/
        /*fee(test.header, test.session, test.asp);*/
}
