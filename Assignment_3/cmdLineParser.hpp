/*
*	File:	cmdLIneParser.hpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

#ifndef CMDLINEPARSER_HPP
#define CMDLINEPARSER_HPP

using namespace std;

class CmdLineParser{

	int argc;
	char **argv;

public:
	CmdLineParser(int _argc, char **_argv) : argc(_argc), argv(_argv){} 
	
	bool operator[](string param);
	string operator()(string param, string defvalue = "-1");
};

#endif