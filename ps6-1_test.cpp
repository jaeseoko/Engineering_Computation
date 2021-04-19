#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;


vector<string> ReadFile(string path) 
{
    ifstream myfile (path);
    vector<string> lines;
    int len = 0;
    if (myfile){
        string line;
        while (getline(myfile, line)){
            lines.push_back(line);
        }
    }
    return lines;
}

std::vector <std::string> Parse(std::string input)
{
	std::vector <std::string> args;
    
	int state=0;
	for(auto c : input)
	{
		if(0==state)
		{
			if(' '==c || '\t'==c || '\n'==c || '\r'==c || '.' ==c || ',' ==c )
			{
			}
			else
			{
				state=1;
				args.push_back("");
				args.back().push_back(c);
			}
		}
		else // if(1==state)
		{
			if(' '==c || '\t'==c || '\n'==c || '\r'==c || '.' ==c || ',' ==c)
			{
				state=0;
			}
			else
			{
				args.back().push_back(c);
			}
		}
	}
	return args;
}

int main(void)
{
    int count = 0;
    int wordcount = 0;
    std::string filename;
    std::string keyword;
    cout << "Enter Input File Name> ";
    cin >> filename;
    cout << "Enter Keyword> ";
    cin >> keyword;
    

    auto mylines = ReadFile(filename);
    for (auto i: mylines)
    {
        auto words = Parse(i);
        for (int i=0; i < words.size(); ++i)
        {
            // cout << words[i] << "\n";
            if(words[i].size() == keyword.size())
            {
                
                for (int j=0; j<words[i].size(); ++j)
                {
                    
                    if(words[i][j]==keyword[j])
                    {
                        count++;
                    }
                    
                }
            }
            if(count == keyword.size())
            {
                wordcount++;
            }
            count = 0;
        }
    }
    // cout << keyword.size() <<"\n";
    cout<< "The keyword appears " << wordcount << " times." << endl;
    return 0;
}