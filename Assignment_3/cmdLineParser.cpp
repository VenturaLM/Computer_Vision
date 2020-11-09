/*
*	File:	cmdLIneParser.cpp
*	Author:	Ventura Lucena Martínez
*	Email:	i72lumav@uco.es
*/

#include <iostream>

#include "cmdLineParser.hpp"

using namespace std;

bool CmdLineParser::operator[](string param)
{
		int idx=-1;  
				for ( int i=0; i<argc && idx==-1; i++ ) 
						if ( string ( argv[i] ) == param )
								idx=i;    
				return ( idx!=-1 ) ;    
}
	 
string CmdLineParser::operator()(string param, string defvalue)
{
				int idx=-1;
				for ( int i=0; i<argc && idx==-1; i++ ) 
						if ( string ( argv[i] ) == param ) 
								idx=i; 
				if ( idx==-1 ) 
						return defvalue;   
				else  
						return ( argv[idx+1] );
}
/*
//Example of use
int main(int argc,char **argv){
	
	 CmdLineParser cml(argc,argv);
	 //check if a command is present
	 if (cml["-p"]){
			 cout<<"-p option is in the command line" << endl;
			 float f= stof(cml("-p")); //convert to float
			 int i= stoi(cml("-p")); //convert to int
			 cout << "You have introduced the value " << cml("-p") << endl;
	 }

	 cout << "You have introduced the value " << cml("-p") << endl;
	 //used with default values
	 float f= stof(cml("-p","1")); //if -p is not, then, return 1
}
*/