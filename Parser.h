#include <iostream>
#include <vector>
#include <sstream>

using std::cout , std::vector, std::cin, std::string;


vector<string> Parse(const string& args)
{
    vector<string> parsed;
    std::stringstream ss(args);
    string word;
    
    while(ss >>word)
        parsed.push_back(word);

    return parsed;
}
