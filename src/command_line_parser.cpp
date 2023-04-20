#include "command_line_parser.hpp"
#include <iostream>


using namespace std;

CommandLineParser:: CommandLineParser(){};

void CommandLineParser::addAuthorInformation(string info){
    this->author_info = info;
}

void CommandLineParser::addArgument(char name, std::string description, std::string default_val){
    checkUnique(name);
    Argument argument;
    argument.description = description;
    argument.value = default_val;
    this->arguments[name] = argument; 
    
    
}
void CommandLineParser::checkUnique(char name){
    if(this->arguments.find(name) != this->arguments.end()){
        string arg(1, name);
        throw runtime_error("Argument " + arg + " already used");
    }
}

void CommandLineParser::parse(int argc, char* argv[]){
    //parse command line
    for (int i = 1; i < argc; i++){
        char* arg = argv[i];
        if (arg[0] == '-'){
            char name = arg[1];
            if (this->arguments.find(name) != this->arguments.end()){
                if(i + 1 < argc && argv[i + 1][0] != '-'){
                    this->arguments[name].value = argv[i+1];
                    i++;
                }
                else{
                    string s(1, name);
                    throw runtime_error("No value for argument " + s  +  " found");
                }
            }
            else{
                info();
                string s(1, name);
                throw runtime_error("Argument -" + s + " unkown");
            }
        }

    }
}
string CommandLineParser::getArgument(char name){
    if (this->arguments.find(name) != this->arguments.end()){
        return this->arguments.at(name).value;
    }
    string s(1, name);
    throw runtime_error("Argument -" + s + " unkown");
}

void CommandLineParser::info(){
    cerr << "\n" << this->author_info << "\n" << endl;
    cerr << "Required arguments" << endl;
    for (auto arg : this->arguments){
        cerr << "-" << arg.first << ":\t" << arg.second.description << endl;
    }
}


