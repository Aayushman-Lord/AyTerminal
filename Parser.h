#include <iostream>
#include <vector>
#include <cctype> // for std::isspace

using std::cout, std::vector, std::cin, std::string;

vector<string> Parse(const string& args)
{
    vector<string> parsed; // final parsed tokens
    string current;         // current token being built

    bool inQuotes = false; // tracks if we're inside "quotes"
    bool escaped = false;  // tracks if previous char was '\'

    // loop through every character in input
    for (char c : args)
    {
        // if previous character was '\'
        // then add current char directly
        if (escaped)
        {
            current += c;
            escaped = false;
            continue;
        }

        // escape character detected
        // next character will be treated literally
        if (c == '\\')
        {
            escaped = true;
            continue;
        }

        // toggle quote mode
        // text inside quotes becomes one token
        if (c == '"')
        {
            inQuotes = !inQuotes;
            continue;
        }

        // if we hit whitespace outside quotes,
        // current token is complete
        if (std::isspace(c) && !inQuotes)
        {
            // avoid pushing empty tokens
            if (!current.empty())
            {
                parsed.push_back(current);
                current.clear();
            }

            continue;
        }

        // normal character
        current += c;
    }

    // push last token if it exists
    if (!current.empty())
        parsed.push_back(current);

    return parsed;
}
