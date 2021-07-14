#include <iostream> 
#include <map>
#include <vector> 
#include <sstream> 
#include <fstream> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <err.h>
#include <netdb.h>
#include <unistd.h>


using namespace std;


class Params
{
    private:
        string s;
        string f;
        int p;
    public:
        Params(vector<string> args);
        ~Params()=default;
        void printParams() const;
        string getS() const;
        string getF() const;
        int getP() const;       
};


struct DNS_PCK 
  {
  u_int16_t ID; 	
  u_int8_t Flags; 
  u_int8_t Rcode; 	
  u_int16_t Qcount; 	
  u_int16_t Acount; 
  u_int16_t NScount; 
  u_int16_t ARcount; 	
  unsigned char Data[500]; 
  };
typedef struct DNS_PCK DNS_PCK;

#define ADRESS_TYPE_A 1

/*** Flags for DNS header.  OR these together. ***/
#define FLAG_REPLY 0x80     /* is this a query or reply?
			       0=query, 1=reply */
#define FLAG_OPCODE_MASK 0x30 	/* query mask */
#define FLAG_OPCODE_QUERY 0x00 	/* standard query */
#define FLAG_OPCODE_IQUERY 0x10 /* inverse query */
#define FLAG_OPCODE_STATUS 0x20 /* server status request */
/* other opcode values bits reserved */
#define FLAG_AA 0x04 	/* authoritative answer */
#define FLAG_TC 0x02 	/* message truncated */
#define FLAG_RD 0x01 	/* recursion denied */

/* Flags added to the rcode byte */
#define FLAG_RA 0x80 	/* recursion available */
#define FLAG_AAA 0x20 	/* answer authenticated */
#define RCODE_MASK 0x0f
enum RCODE
  {
  RCODE_NO_ERROR=0, 	/* no error condition */
  RCODE_FORMAT_ERROR, 	/* format error */
  RCODE_SERVER_ERROR, 	/* server error */
  RCODE_NAME_ERROR, 	/* name error */
  RCODE_NOT_IMP, 		/* not implemented (not available) */
  RCODE_REFUSED, 	/* refused */
  };
typedef enum RCODE RCODE;


vector<string> argsToVector(int argNum, char *arg[]);
int server_run(string DNSserver, int port, vector<string>filter);
int loadFile(string file, vector<string> *lines);
pair<string,u_int16_t> dataToNameAndType(DNS_PCK pck);
bool findNameInFilter(string name, vector<string>filter);
char* getServerAddr(string server);
pair<DNS_PCK,int> client_run(char* addr, void* pcket, int pckSize);