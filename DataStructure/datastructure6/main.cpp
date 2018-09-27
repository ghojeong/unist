//20131674 JeongWan Gho
#include "hashmap.h"
#include <iostream>
#include <fstream>

using namespace std;

HashMap< MapElem<std::string, unsigned int> > hashmp; //hash map
//global variable

void spellcheck(std::string s)
{
	std::cout << "> " << s;
	// ToDo	

	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	//replace all the upper case letters to lower case letters
	MapElem<std::string, unsigned int>* temp=hashmp.find(s);
	MapElem<std::string, unsigned int>* temp2;
	std::string suggestion;
	//suggestion word has same length as s but one letter is dffierent
	char c;
	bool suggestionExist=false;
	bool first=true;

	if(temp!=NULL){
		printf(" is in the dictionary\n",s.c_str());
	}
	else{
		printf(" is NOT in the dictionary\n",s.c_str());
		printf("> %s : ",s.c_str());
		for(int i=0; i<s.length(); i++){
			for(int j=97; j<=127; j++){
				c=(char) j;
				for(int k=0; k<s.length(); k++){
					if( k==i ) suggestion.push_back(c);
					else suggestion.push_back(s[k]);
				}
				temp2=hashmp.find(suggestion);

				if(temp2!=NULL){
					suggestionExist=true;
					if(first){
						printf("%s", temp2->key.c_str());
						first=false;
					}
					else{
						printf(", %s", temp2->key.c_str());							}
				}
				suggestion.clear();
			}
		}
		if(suggestionExist) printf("\n");
		else printf("no suggestion\n");
	}
}


int main()
{
	// load dictionary
	char filename[] = "dictionary.txt";
	std::ifstream ifs(filename, std::ifstream::in);
	std::string s((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	std::transform(s.begin(), s.end(),
				   s.begin(), ::tolower);

	std::string token;	
	unsigned int len = s.length();
		
	for(int i=0; i<len; i++)
	{
		int ascii = s[i];
		
		if(ascii < 97 || ascii > 127) 
		{
			if(token.length() > 0) 
			{
				// ToDo
				// Insert token to hash map
				hashmp.insert(token, hashmp.hashfunction(token) );
				
				token.clear();
			}
			continue;
		}
		token.push_back(s[i]);
	}
	
//	hashmp.print();	
	//
	// infinite loop to accept user input word
	//

	while(1)
	{
		std::string s;
		std::cout << "> ";
		std::cin >> s;
		spellcheck( s );
		
		if(s == "q") break;
	}

	
	
	return 0;
}

