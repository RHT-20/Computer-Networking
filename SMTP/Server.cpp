#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <netdb.h>
#include<arpa/inet.h>
using namespace std;

#define MX 1000005
struct sockaddr_in server,client;
struct hostent *client_ip;
socklen_t sz = sizeof(struct sockaddr_in);
int server_socket_fd,client_socket_fd;
string mail_from,rcpt_to,subject,client_name,data,main_str;
char msge[MX];

void print_error(string str)
{
    cout<<str<<endl;
    //exit(0);
    return;
}

void init(int port)
{
    server_socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_socket_fd==-1)    print_error("Socket creation error.");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    bzero(&server.sin_zero,8);

    int bind_res = bind(server_socket_fd,(struct sockaddr *)&server,sz);
    if(bind_res==-1)    print_error("Binding error.");

    int listen_res = listen(server_socket_fd,5);
    if(listen_res==-1)  print_error("Listening error.");

    return;
}

bool HELO_request()
{
    int ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    printf("C: %s\n",msge);
    data = "";
    for(int i=0; msge[i]; i++)
    {
        if(msge[i]==' ')    break;
        data += msge[i];
    }
    if(data=="HELO")
    {
        data = "250 Service ready.";
        strcpy(msge,data.c_str());
        printf("S: %s\n",msge);
        ln = send(client_socket_fd,msge,strlen(msge),0);
        return  true;
    }
    else if(data=="MAIL FROM")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="RCPT TO")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="DATA")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="QUIT")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else
    {
        strcpy(msge,"500 Improper command.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 500 Improper command.");
    }
    return  false;
}

bool check_email(string email)
{
    regex regx("(^[a-z0-9._-]+@[a-z0-9.-]+\.[a-z]{2,}$)");

    return  regex_match(email, regx);
}

bool MAIL_FROM_request()
{
    bool f = false;
    int ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    printf("C: %s\n",msge);

    data = "";
    for(int i=0; msge[i]; i++)
    {
        if(msge[i]==':')    break;
        data += msge[i];
    }
    if(data=="MAIL FROM")
    {
        mail_from = "";
        for(int i=0; msge[i]; i++)
        {
            if(f)   mail_from += msge[i];
            if(msge[i]==':')    f = true;
        }

        if(!check_email(mail_from))
        {
            strcpy(msge,"Invalid email ID.");
            ln = send(client_socket_fd,msge,strlen(msge),0);
            print_error("S: Invalid email ID.");

            return  false;
        }

        data = "250 OK.";
        strcpy(msge,data.c_str());
        printf("S: %s\n",msge);
        ln = send(client_socket_fd,msge,strlen(msge),0);

        return  true;
    }
    else if(data=="HELO")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="RCPT TO")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="DATA")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="QUIT")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else
    {
        strcpy(msge,"500 Improper command.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 500 Improper command.");
    }
    return  false;
}

bool RCPT_TO_request()
{
    bool f = false;
    int ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    printf("C: %s\n",msge);
    data = "";
    for(int i=0; msge[i]; i++)
    {
        if(msge[i]==':')    break;
        data += msge[i];
    }
    if(data=="RCPT TO")
    {
        rcpt_to = "";
        for(int i=0; msge[i]; i++)
        {
            if(f)   rcpt_to += msge[i];
            if(msge[i]==':')    f = true;
        }

        if(!check_email(rcpt_to))
        {
            strcpy(msge,"Invalid email ID.");
            ln = send(client_socket_fd,msge,strlen(msge),0);
            print_error("S: Invalid email ID.");

            return  false;
        }

        data = rcpt_to+".txt";
        FILE * File = fopen(data.c_str(),"r");
        if(File==NULL)
        {
            strcpy(msge,"Mail box unavailable");
            ln = send(client_socket_fd,msge,strlen(msge),0);
            print_error("S: Mail box unavailable");
        }
        fclose(File);

        data = "250 OK.";
        strcpy(msge,data.c_str());
        printf("S: %s\n",msge);
        ln = send(client_socket_fd,msge,strlen(msge),0);

        return  true;
    }
    else if(data=="HELO")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="MAIL FROM")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="DATA")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="QUIT")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else
    {
        strcpy(msge,"500 Improper command.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 500 Improper command.");
    }
    return  false;
}

bool DATA_transfer()
{
    int ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    printf("C: %s\n",msge);
    if(strcmp(msge,"DATA")==0)
    {
        data = "354 OK";
        strcpy(msge,data.c_str());
        printf("S: %s\n",msge);
        ln = send(client_socket_fd,msge,strlen(msge),0);
        ln = recv(client_socket_fd,msge,MX,0);
        msge[ln] = '\0';

        for(int i=0; msge[i]; i++)
        {
            if(i==0)    printf("C: ");
            ln = 0;
            if(i<(strlen(msge)-1))  ln = count(msge+i,msge+i+1,'\n');
            if(ln)
            {
                printf("\nC: ");
                continue;
            }
            printf("%c",msge[i]);
        }
        puts("");

        /// write in file
        data = rcpt_to+".txt";
        FILE *File = fopen(data.c_str(),"a+");
        fprintf(File,"%s\n",msge);
        fprintf(File,"****************************************************\n\n");
        fclose(File);

        data = "250 OK";
        strcpy(msge,data.c_str());
        printf("S: %s\n",msge);
        ln = send(client_socket_fd,msge,strlen(msge),0);

        return  true;
    }
    else if(data=="HELO")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="MAIL FROM")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="RCPT TO")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="QUIT")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else
    {
        strcpy(msge,"500 Improper command.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 500 Improper command.");
    }
    return  false;
}

bool QUIT_request()
{
    int ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    printf("C: %s\n",msge);
    if(strcmp(msge,"QUIT")==0)
    {
        data = "221 BYE";
        strcpy(msge,data.c_str());
        printf("S: %s\n",msge);
        ln = send(client_socket_fd,msge,strlen(msge),0);

        close(client_socket_fd);

        return  true;
    }
    else if(data=="HELO")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="MAIL FROM")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="RCPT TO")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else if(data=="DATA")
    {
        strcpy(msge,"503 Bad sequence of commands.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 503 Bad sequence of commands.");
    }
    else
    {
        strcpy(msge,"500 Improper command.");
        ln = send(client_socket_fd,msge,strlen(msge),0);
        print_error("S: 500 Improper command.");
    }
    return  false;
}

int main(int argc,char *args[])
{
    bool f;
    int ln;

    if(argc<2)  print_error("Number of arguments less than 2.");

    init(atoi(args[1]));

    while(1)
    {
        client_socket_fd = accept(server_socket_fd,(struct sockaddr *)&client,&sz);
        if(client_socket_fd==-1)    print_error("Client connection error.");
        //getnameinfo((struct sockaddr *)&client,sz,msge,sizeof(msge),NULL,0,NI_NAMEREQD);
        //client_name = (string)msge;
        //printf("%s\n",msge);

        data = "220 OK.";
        //data += client_name;
        strcpy(msge,data.c_str());
        printf("S: %s\n",msge);
        ln = send(client_socket_fd,msge,strlen(msge),0);
        //printf("S: Client connected.Port = %d.IP address = %s\n",ntohs(client.sin_port),inet_ntoa(client.sin_addr));


        if(!HELO_request())         continue;
        if(!MAIL_FROM_request())    continue;
        if(!RCPT_TO_request())      continue;
        if(!DATA_transfer())        continue;
        if(!QUIT_request())         continue;
    }

    close(server_socket_fd);

    return  0;
}

