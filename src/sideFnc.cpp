#include "head.hpp"


/* returns first domain name from dns packet and its type*/
pair<string,u_int16_t> dataToNameAndType(DNS_PCK pck)
{
    int offset=0;
   // int seqLen;
    u_int8_t seqLen;
    string name;
    u_int16_t size;
    while(1)
    {
        seqLen = pck.Data[offset];
        offset+=1;
        for(int i=0;i<seqLen;i++)
        {
            name+=pck.Data[offset++];
        }
        if(pck.Data[offset]==0)
        {
            break;
        }
        name+='.';
    }
    //type is located 2 bytes after null-byte
    size = pck.Data[offset+1] << 8 | pck.Data[offset+2];
    return {name, size};
}

/* checks if given name is located in filter */
bool findNameInFilter(string name, vector<string>filter)
{
    name = name.substr(0, name.find("/"));
    for(auto record : filter)
    {
        if(name==record)
        {
            return true;
        }
    }
    return false;
}

/* get ip from domain name */
char* getServerAddr(string server)
{
    auto name = gethostbyname(server.c_str());
    auto serverAddr = inet_ntoa(*((struct in_addr*) 
                           name->h_addr_list[0]));
    return serverAddr;

}