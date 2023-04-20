#ifndef COMMAND_LINE_PARSER_HPP
#define COMMAND_LINE_PARSER_HPP

#include <string>
#include <map>


class CommandLineParser{
struct Argument {
    std::string description;
    std::string value;
};

public:
    CommandLineParser();
    void addArgument(char name, std::string description, std::string default_value);
    void parse(int argc, char* argv[]);
    void info();
    void addAuthorInformation(std::string info);
    std::string getArgument(char c);
    //void securityMessage(std::string errmsg, char* argv[], int pos);



private:
    std::map<char, Argument> arguments;
    std::string author_info;
    void checkUnique(char name);
};









#endif // COMMAND_LINE_PARSER_HPP