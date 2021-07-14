#include "head.hpp"

int main(int argc, char *argv[])
{
    auto input = argsToVector(argc,argv);
    Params params(input);
    if(params.getF()=="none" || params.getS()=="none")
    {
        cerr<<"wrong args\n";
        return 1;
    }
    vector<string> adressFilter;

    if(loadFile(params.getF(),&adressFilter)==1)
    {
        cerr<<"failed loadFile \n";
        return 1;
    }
    //auto adressFilter=loadFile(params.getF());


    if(server_run(params.getS(), params.getP(), adressFilter)==1)
    {
        cerr<<"failed server_run \n";
        return 1;
    }
    return 0;
}

