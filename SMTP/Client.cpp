#include<bits/stdc++.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctime>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <netdb.h>
#include<arpa/inet.h>
#include<errno.h>
using namespace std;

#define MX 1000005
struct sockaddr_in client;
struct hostent *host_ip;
int client_socket_fd,connect_res,ln;
socklen_t sz = sizeof(struct sockaddr);
string sender,receiver,host,subject,file_name,data,main_str;
char msge[MX];
int port;

void print_error(string str)
{
    cout<<str<<endl;
    exit(0);
}

void process_args(int cnt,char *str[])
{
    int flag=0;
    bool f=false;
    receiver = "";
    host = "";
    subject = "";
    file_name = "";
    port = 0;
    for(int j=0; str[1][j]; j++)
    {
        if(str[1][j]=='@')
        {
            flag++;
            continue;
        }
        else if(str[1][j]==':')
        {
            flag++;
            continue;
        }
        else if(flag==0)    receiver += str[1][j];
        else if(flag==1)    host += str[1][j];
        else if(flag==2)    port = (port*10)+(str[1][j]-'0');
    }
    file_name = (string)str[cnt-1];
    for(int k=2; k<(cnt-1); k++)    subject += (string)str[k];

    host_ip = gethostbyname(host.c_str());
    //if(host_ip==NULL)   print_error("No such host found");

    return;
}

void init()
{
    client_socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(client_socket_fd==-1)    print_error("Socket creating error.");

    client.sin_family = AF_INET;
    bcopy((char *)host_ip->h_addr,(char *)&client.sin_addr.s_addr,host_ip->h_length);
    client.sin_port = htons(port);
    bzero(&client.sin_zero,8);

    //user_name = getenv("USER");

    return;
}

void HELO_request()
{
    printf("C: ");
    getline(cin,data);
    strcpy(msge,data.c_str());
    ln = send(client_socket_fd,msge,strlen(msge),0);
    ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    if(msge[0]!='2')
    {
        data = "S: ";
        data += (string)msge;
        print_error(data);
    }
    printf("S: %s\n",msge);

    return;
}

void MAIL_FROM_request()
{
    printf("C: ");
    getline(cin,data);

    bool f=false;
    sender = "";
    for(int i=0; data[i]; i++)
    {
        if(data[i]==':')
        {
            f = true;
            continue;
        }
        else if(f)  sender += data[i];
    }

    strcpy(msge,data.c_str());
    ln = send(client_socket_fd,msge,strlen(msge),0);
    ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    if(msge[0]!='2')
    {
        data = "S: ";
        data += (string)msge;
        print_error(data);
    }
    printf("S: %s\n",msge);

    return;
}

void RCPT_TO_request()
{
    printf("C: ");
    getline(cin,data);

    bool f=false;
    receiver = "";
    for(int i=0; data[i]; i++)
    {
        if(data[i]==':')
        {
            f = true;
            continue;
        }
        else if(f)  receiver += data[i];
    }

    strcpy(msge,data.c_str());
    ln = send(client_socket_fd,msge,strlen(msge),0);
    ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    if(msge[0]!='2')
    {
        data = "S: ";
        data += (string)msge;
        print_error(data);
    }
    printf("S: %s\n",msge);

    return;
}

string get_date()
{
    time_t now = time(0);

    string date = ctime(&now);
    //cout<<date<<endl;

    string res[10],ans="";
    int sp=0;


    res[0] = "";
    for(int i=0; date[i]; i++)
    {
        if(date[i]==' ')
        {
            sp++;
            res[sp] = "";
            continue;
        }
        res[sp] += date[i];
    }

    //for(int i=0; i<=5; i++) cout<<res[i]<<endl;

    ans = res[4] + ", " + res[0] + ", " + res[3] + "-" + res[1] + "-" + res[5];

    return  ans;
}

void DATA_transfer()
{
    printf("C: ");
    getline(cin,data);
    strcpy(msge,data.c_str());
    ln = send(client_socket_fd,msge,strlen(msge),0);
    ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    if(!(msge[0]=='3' && msge[1]=='5' && msge[2]=='4'))
    {
        data = "S: ";
        data += (string)msge;
        print_error(data);
    }
    printf("S: %s\n",msge);

    main_str = "MAIL FROM: ";
    main_str += sender;
    main_str += "\n";
    printf("C: MAIL FROM: %s\n",sender.c_str());

    main_str += "RCPT TO: ";
    main_str += receiver;
    main_str += "\n";
    printf("C: RCPT TO: %s\n",receiver.c_str());

    main_str += "SUBJECT: ";
    main_str += subject;
    main_str += "\n";
    printf("C: SUBJECT: %s\n",subject.c_str());

    main_str += "DATE: ";
    string date = get_date();
    main_str += date;
    main_str += "\n";
    printf("C: DATE: %s\n",date.c_str());

    /// Read from file
    FILE *File = fopen(file_name.c_str(),"r");
    data = "";
    while(fgets(msge,MX,File))
    {
        printf("C: %s",msge);
        data = (string)msge;
        main_str += data;
    }
    fclose(File);

    strcpy(msge,main_str.c_str());
    ln = send(client_socket_fd,msge,strlen(msge),0);
    ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    if(msge[0]!='2')
    {
        data = "S: ";
        data += (string)msge;
        print_error(data);
    }
    printf("S: %s\n",msge);

    return;
}

void QUIT_request()
{
    printf("C: ");
    getline(cin,data);
    strcpy(msge,data.c_str());
    ln = send(client_socket_fd,msge,strlen(msge),0);
    ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    if(msge[0]!='2')
    {
        data = "S: ";
        data += (string)msge;
        print_error(data);
    }
    printf("S: %s\n",msge);

    close(client_socket_fd);

    return;
}

int main(int argc,char *args[])
{
    if(argc<4)  print_error("Number of arguments less than 4.");

    process_args(argc,args);
    init();

    connect_res = connect(client_socket_fd,(struct sockaddr *)&client,sz);
    if(connect_res==-1) print_error("Server connection error.");
    ln = recv(client_socket_fd,msge,MX,0);
    msge[ln] = '\0';
    if(msge[0]!='2')    print_error("S: 404");
    printf("S: %s\n",msge);

    HELO_request();
    MAIL_FROM_request();
    RCPT_TO_request();
    DATA_transfer();
    QUIT_request();

    return  0;
}

