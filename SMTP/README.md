* First run Server.cpp:  
g++ Server.cpp -o Server  
./Server 20000  
[here, "20000" is port number]  
  
* Then run Client.cpp:  
g++ Client.cpp -o Client  
./Client bob@student11-H97M-D3H:20000 "First conversation with Bob via mail" mail.txt  
[here, "bob" is sender name, "student11-H97M-D3H" is host name of your PC, "20000" is port number, then subject of your mail and then message file name]  
  
* After that conversation will be something like this:  
S: 220 OK.  
C: HELO  
S: 250 Service ready.  
C: MAIL FROM:bob@gmail.com  
S: 250 OK.  
C: RCPT TO:john@gmail.com  
S: 250 OK.  
C: DATA  
S: 354 OK  
C: MAIL FROM: bob@gmail.com  
C: RCPT TO: john@gmail.com  
C: SUBJECT: First conversation with Bob via mail  
C: DATE: 00:10:41, Sun, 3-Feb-2019  
C: Hello Bob.  
C: How are you?  
S: 250 OK  
C: QUIT  
S: 221 BYE  
[here, client's command sequence HELO, MAIL FROM:bob@gmail.com[sender's mail ID], RCPT TO:john@gmail.com[recipient's mail ID],DATA, QUIT]  
