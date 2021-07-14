#include "head.hpp"

/* copy args to vector<string> */
vector<string>argsToVector(int argNum, char *arg[])
{
    vector<string> args;
    for(int i=1; i<argNum; ++i)
    {
        string str=arg[i];
        args.push_back(str);
    }
    return args;
}

/* parse vector of args to struct */
Params::Params(vector<string> args)
{
    this->s="none";
    this->p=53;
    this->f="none";

    int nextArg=0;
    for(auto arg:args)
    {
        switch(nextArg)
        {
            case 0:
                if(arg=="-s" || arg=="-S")
                {
                    nextArg=1;
                }
                else if(arg=="-f" || arg=="-F")
                {
                    nextArg=2;
                }
                else if(arg=="-p" || arg=="-P")
                {
                    nextArg=3;
                }
                else
                {
                    exit(1);
                }
                break;
            case 1:
                this->s=arg;
                nextArg=0;
                break;
            case 2:
                this->f=arg;
                nextArg=0;
                break;
            case 3:
                stringstream num(arg); 
                num >> this->p;
                nextArg=0;
                break;                       
        }
    }
}

void Params::printParams() const
{
    cout<<"-s je "<<getS()<<endl;
    cout<<"-p je "<<getP()<<endl;
    cout<<"-f je "<<getF()<<endl;
}
string Params::getS() const
{
    return this->s;
}
int Params::getP() const
{
    return this->p;
}
string Params::getF() const
{
    return this->f;
}




