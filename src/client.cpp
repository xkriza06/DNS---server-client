#include "head.hpp"


#define PORT 53        

/* run client to get dns answer from resolver 
addr- adress of dns resolver
pcket- packet with dns query
pcksize- soze of given packet
// if success return answer msg form resolver
// else return empty DNS_PCK
*/
pair<DNS_PCK,int> client_run(char* addr, void* pcket, int pckSize)
{
    int sock;
    int sendLen;
    struct sockaddr_in resolver;
    socklen_t resolverLen;
    struct sockaddr_in fromResolver;
    socklen_t fromResolverLen = sizeof(fromResolverLen);
    
    DNS_PCK answer;
    int answerLen;

/////////////// create socket /////////////////////////////
    if ((sock = socket(AF_INET , SOCK_DGRAM , 0)) == -1)
    {
        cerr<<"failed socket create\n";
        // if fail return DNS_PCK with ID=0 
        return {{0},0};
    }
    resolver.sin_addr.s_addr = inet_addr(addr);
    resolver.sin_family = AF_INET;                   
    resolver.sin_port = htons(PORT);
    resolverLen = sizeof(resolver);


/////////////// send message to resolver /////////////////////////
    sendLen = sendto(sock,pcket,pckSize,0,(struct sockaddr *) &resolver, resolverLen);
    if (sendLen == -1)              
    {
        cerr<<"failed sendto in client\n";
        close(sock);
        return {{0},0};
    }
//////////////// set socket to receive msg /////////////////////////////////
    if (getsockname(sock,(struct sockaddr *) &fromResolver, &fromResolverLen) == -1) 
    {
        cerr<<"failed getsocketname in client\n";
        close(sock);
        return {{0},0};
    }  
////////////////////////// receive msg form resolver /////////////////////   
    if ((answerLen = recvfrom(sock,&answer, sizeof(answer),0,(struct sockaddr *) &fromResolver, &fromResolverLen)) == -1) 
    {  
        cerr<<"failed recvfrom in client\n";
        close(sock);
        return {{0},0};
    }
    else if (answerLen > 0)
    {
        close(sock);
    // if success return answer msg form resolver
        return {answer,answerLen};
    }
   
  close(sock);
  return {{0},0};
}
