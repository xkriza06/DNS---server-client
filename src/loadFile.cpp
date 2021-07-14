#include "head.hpp"

/* copy file by valid lines to vector<string> */ 
int loadFile(string file, vector<string> *lines)
{
    ifstream iFile(file);
    if (iFile.is_open()) 
    {
        string line;
        while (getline(iFile, line)) 
        {   
            if(!line.empty())
            {
                if(line.at(0)!='#')
                {
                    lines->push_back(line);
                } 
            }
        }
        iFile.close();
    }
    else
    {
        cerr<<"failed is_open \n";
        return 1;
    }
    
    return 0;
}
