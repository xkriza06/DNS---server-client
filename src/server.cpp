#include "head.hpp"

/* create server to listen to on given port and receive dns packet
DNSserver- adress/name of resolver to resolve query
port- portnumber for listen to on
filter- vector of blocked adresses
return 1 if fail
*/
int server_run(string DNSserver, int port, vector<string>filter)
{
    int udp_sck;                           
    struct sockaddr_in server;                          
    struct sockaddr client;        
    socklen_t clientLen;

    DNS_PCK request;
    int requestLen;
    
/////////////////// create socket ///////////////////////
    server.sin_family = AF_INET;                     
    server.sin_addr.s_addr = htonl(INADDR_ANY);     
    server.sin_port = htons(port);                   
    
    if ((udp_sck = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        cerr<<"failed socket create in server\n";
        return 1;
    }

    if (bind(udp_sck, (struct sockaddr *)&server, sizeof(server)) == -1)
    { 
        cerr<<"failed bind in server\n";
        close(udp_sck);
        return 1;
    }

//////////////////////// listen to /////////////////////////////////
    while(1)
    {
        clientLen=sizeof(client);
        requestLen = recvfrom(udp_sck,&request,sizeof(request),0,&client,&clientLen);

        if(requestLen != -1)
        {
            // get name from query
            auto parsedData=dataToNameAndType(request);
            string queryName= parsedData.first;
            u_int16_t queryType= parsedData.second;

            //if wrong type
            if(queryType!=ADRESS_TYPE_A)
            {
                /* send back an error msg */
                request.Flags = FLAG_REPLY;
                request.Rcode =  RCODE_NOT_IMP;
                int sentToClient;
                sentToClient= sendto(udp_sck, &request, requestLen, 0, &client, clientLen);
                if (sentToClient == -1)
                {
                    cerr<<"failed sendto in server\n";
                }
            }
            // if is blocked
            else if(findNameInFilter(queryName,filter))
            {
                /* send back an error msg */
                request.Flags = FLAG_REPLY;
                request.Rcode =  RCODE_REFUSED;
                int sentToClient;
                sentToClient= sendto(udp_sck, &request, requestLen, 0, &client, clientLen);
                if (sentToClient == -1)
                {
                    cerr<<"failed sendto in server\n";
                }
            }
            // if isnt blocked
            else
            {
                /*send along to dns server in parameters*/
                auto resolverAddr = getServerAddr(DNSserver);
                auto answer = client_run(resolverAddr,&request,requestLen);

                //if fail return DNS_PCK with ID=0 
                if(answer.second==0)
                {
                    cerr<<"failed client_run in server\n";
                }
                // send answer back to client
                else
                {
                    int sentToClient;
                    sentToClient= sendto(udp_sck, &(answer.first), answer.second, 0, &client, clientLen);
                    if (sentToClient == -1)
                    {
                        cerr<<"failed sendto in server\n";
                    }
                }   
            }
        }
        else
        {
            cerr<<"failed recvfrom in server\n";
        }
        
    }

    close(udp_sck);
    return 0;
}