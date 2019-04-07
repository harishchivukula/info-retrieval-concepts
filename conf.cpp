/*
*
*@file- conf.cpp
*@authors- Sameer Mungole | Harish Chivukula
*@date- March 2019
*
*Implementation of -
*http://snowball.tartarus.org/algorithms/porter/stemmer.html
*https://tartarus.org/martin/PorterStemmer/def.txt
*
*/
#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <cstdio>
#include <cstring>

using namespace std;
string erasePunc(string word) {
	int k = word.find('.');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find(',');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find('!');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find('?');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find(';');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find('-');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find('/');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find(':');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find('\'');
	if (k != string::npos)
		word.erase(k, 1);
	k = word.find('\'');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find('\"');
	if (k != string::npos)
		word.erase(k, 1);
	k = word.find('\"');
	if (k != string::npos)
		word.erase(k, 1);

	k = word.find('&');
	if (k != string::npos)
		word.erase(k, 1);
	
	return word;
}

string lowercase(string word) {
	char lcase[20];
	
	lcase[word.size() + 1];
	copy(word.begin(), word.end(), lcase);
	lcase[word.size()] = '\0';
	for(int j=0;j<=strlen(lcase);j++)
		if(lcase[j]>=65 && lcase[j]<=92)
			lcase[j]=lcase[j]+32;
	
	return lcase;
}



void filePrep(int* wordcount) {
	string word;
	ifstream txtptr;
	ofstream wrtptr;
	txtptr.open("inptext.txt");
	wrtptr.open("prepinp.txt", ios::trunc);
	while (txtptr >> word)
	{
		word=erasePunc(word);
						
		wrtptr << lowercase(word);
		wrtptr << endl;
	}
	wrtptr.close();
	txtptr.close();
}

void removeFluff(int* tokencount) {

	char inStr[30], fluffStr[30];
	int flag;
	FILE *inPtr = NULL,*outPtr = NULL,*fluffPtr = NULL;
	inPtr = fopen("prepinp.txt","r");
	if (inPtr == NULL)
		cout<<"\nfluff.txt cannot be opened !";
	outPtr = fopen("indexterms.txt","w");
	fluffPtr = fopen("fluff.txt","r");

	while(fscanf(inPtr,"%s",inStr) != EOF)
	{	
		flag = 0;
		while(fscanf(fluffPtr,"%s",fluffStr) != EOF)
		{
			if(strcmp(inStr,fluffStr) == 0)
			{
				flag = 1;
				break;
			}
	
		}
		if(flag == 0)
		{
			fprintf(outPtr, "%s\n", inStr);
			*tokencount = *tokencount + 1;
		}
		fseek(fluffPtr, 01, 0);
	}
	fclose(outPtr);
	fclose(inPtr);
	fclose(fluffPtr);
}

void VCmake(const char in[20], const char vc[20], const char VC[20], const char m[20]) {

	string VCword, indexStr;
	char vcword[20];

	ifstream indexPtr;
	ofstream vcPtr, VCptr, measurePtr;

	indexPtr.open(in);
	vcPtr.open(vc, ios::trunc);
	VCptr.open(VC, ios::trunc);
	measurePtr.open(m, ios::trunc);

	int x , k;

	while(indexPtr >> indexStr) {
		memset(vcword, 0, 20);
		VCword.clear();
		vcword[indexStr.size() + 1];
		copy(indexStr.begin(), indexStr.end(), vcword);

		for(int j = 0; j < strlen(vcword); j++)
			if(vcword[j]=='a' ||vcword[j]=='e' ||vcword[j]=='i' ||vcword[j]=='o' ||vcword[j]=='u'|| (vcword[j]=='y' && vcword[j-1]=='c' && j!=0))
				vcword[j]='v';
			else
				vcword[j]='c';
		vcPtr << vcword << endl;

		x = 0; k = 0;
		while(k<=strlen(vcword)) {
			x = k + 1;
			while(vcword[x] == vcword[k] && x <= strlen(vcword))
				x++;
			if(vcword[k] == 'v')
				VCword = VCword + 'V';
			else if(vcword[k] == 'c')
 				VCword = VCword + 'C';
			k = x;
		}
		VCptr << VCword << endl;

		x = 0; k = 0;
		while(x <= VCword.size()) {
			if(VCword[x] == 'V')
				if((x + 1) != VCword.size()) {
					x++; 
					k++;
				}
			x++;
		}
		measurePtr << k << endl;
	}

	measurePtr.close();
	VCptr.close();
	vcPtr.close();
	indexPtr.close();
}

void reverseStr(string& str) { 
	int n = str.length(); 
 
	char temp;
	for (int i = 0; i < n / 2; i++) {
		temp = str[i];
		str[i] = str[n - i - 1];
		str[n - i - 1] = temp;
	}
} 

void replaceStem(string indexStr, char stem[20], const char app[10], int len) {
	reverseStr(indexStr);
	if(indexStr.length() - len > 2) {
		copy(indexStr.begin(), indexStr.end() - len, stem);
		strcat(stem, app);	
	}
	else 
		copy(indexStr.begin(), indexStr.end(), stem);
}

int find(string indexStr, const char suffix[10]) {
	return indexStr.find(suffix);
}

int find2(string indexStr, char suffix[20]) {
	return indexStr.find(suffix);
}
void step1A() {
	string indexStr;
	char stem[20];

	ifstream indexPtr;
	ofstream suffPtr;
	indexPtr.open("indexterms.txt");
	suffPtr.open("suffixstrip.txt", ios::trunc);

	while(indexPtr >> indexStr)	{
		memset(stem, 0, 20);
		reverseStr(indexStr);
		if(find(indexStr, "sess") == 0) {
			replaceStem(indexStr, stem, "ss", 4);
			suffPtr << stem << endl;
		}
		else if((find(indexStr, "sei") == 0 && find(indexStr, "seiceps") && find(indexStr, "samtsirhc") && find(indexStr, "seibar") && find(indexStr, "seiriarp")) || find(indexStr, "dei") == 0 || find(indexStr, "rei") == 0 ) {
			replaceStem(indexStr, stem, "i", 3);
			suffPtr << stem <<endl;
		}
		else if((find(indexStr, "s") == 0 && find(indexStr, "seiceps") && find(indexStr, "suoirav") && find(indexStr, "saib") && find(indexStr, "salta") && find(indexStr, "somsoc") && find(indexStr, "siad")) && find(indexStr, "ss") != 0) {
			replaceStem(indexStr, stem, "", 1);
			suffPtr << stem <<endl;
		}
		else {
			reverseStr(indexStr);
			suffPtr << indexStr << endl;
		}
	}

	suffPtr.close();
	indexPtr.close();

	VCmake("suffixstrip.txt", "vcterms.txt", "VCterm.txt", "measure.txt");
}

void step1B() {
			
	string indexStr, vcStr;
	char stem[20];
	int measure, flag, val;

	ifstream indexPtr,measurePtr,vcPtr;
	ofstream suffPtr;
	indexPtr.open("suffixstrip.txt");
	measurePtr.open("measure.txt");
	vcPtr.open("vcterms.txt");
	suffPtr.open("suffixstep3.txt", ios::trunc);

	while(indexPtr >> indexStr && measurePtr >> measure && vcPtr >> vcStr) {
		flag = 0;
		val = 0;
		memset(stem, 0, 20);
		reverseStr(indexStr);
		if(find(indexStr, "dee") == 0 && measure > 0) {
			replaceStem(indexStr, stem, "ee", 3);
			suffPtr << stem << endl;
		}
		else if(find(vcStr,"v") != string::npos) {
			reverseStr(vcStr);
			if((find(indexStr, "de") == 0) || (!(find(indexStr, "debme") || find(indexStr, "derhs")) && (find(indexStr, "debme") || !find(indexStr, "derhs")))) {
				replaceStem(indexStr, stem, "", 2); 
				val = 2;
				reverseStr(vcStr);
				if(find(vcStr,"vcv")==1 && (find(indexStr, "g") == 1 || find(indexStr, "v") == 1 || find(indexStr , "c") == 1)) {
					strcat(stem, "e");
					flag = 1;
				}
				flag = 1;
			}
			else if(find(indexStr, "gni") == 0 ) {					
				replaceStem(indexStr, stem, "", 3);
				val = 3;
				flag = 1;

			}

			if(flag == 1) {
				
				if(find(indexStr, "at") == val) {
					memset(stem, 0, 20);
					replaceStem(indexStr, stem, "eta", 2 + val);
				}
				else if(find(indexStr, "ut") == val) {
					memset(stem, 0, 20);
					replaceStem(indexStr, stem, "etu", 2 + val);
				}
				else if(find(indexStr, "uc") == val) {
					memset(stem, 0, 20);
					replaceStem(indexStr, stem, "ecu", 2 + val);
				}
				else if (find(indexStr, "bl") == val) {
					memset(stem, 0, 20);
					replaceStem(indexStr, stem, "elb", 2 + val);
				}
				else if(find(indexStr, "iz") == val) {
					memset(stem, 0, 20);
					replaceStem(indexStr, stem, "ezi", 2 + val);
				}
				else if((find(vcStr,"cc") == 0) && (stem[indexStr.length() - val - 1] == stem[indexStr.length() - val - 2]) && (stem[indexStr.length() - val] != 'l' || stem[indexStr.length() - val] != 's' || stem[indexStr.length() - val] != 'z') ) {
					memset(stem, 0, 20);
					replaceStem(indexStr, stem, "", 1 + val);
				}
				else if(measure == 1 && (find(vcStr,"cvc") == 0) && (stem[indexStr.length() - val] != 'w' || stem[indexStr.length() - val] != 'y' || stem[indexStr.length() - val] != 'x')) {
					memset(stem, 0, 20);
					replaceStem(indexStr, stem, "e", val);
				}
				suffPtr << stem << endl;
			} else {
				reverseStr(indexStr);
				suffPtr << indexStr << endl;
			}
		} else {
			reverseStr(indexStr);
			suffPtr << indexStr << endl;
		}
	}

	suffPtr.close();
	vcPtr.close();
	measurePtr.close();
	indexPtr.close();	

	VCmake("suffixstep3.txt", "vcterms.txt", "VCterm.txt", "measure.txt");
}

void step1C() {
	string indexStr, vcStr;
	char stem[20];

	ifstream indexPtr,measurePtr,vcPtr;
	ofstream suffPtr;
	indexPtr.open("suffixstep3.txt");
	vcPtr.open("vcterms.txt");
	suffPtr.open("suffixstrip.txt", ios::trunc);

	while(indexPtr >> indexStr && vcPtr >> vcStr) {

		memset(stem, 0, 20);
		reverseStr(indexStr);

		if(find(indexStr, "y") == 0) {
			if(find(vcStr,"v") != string::npos && indexStr.length() > 3)
				replaceStem(indexStr, stem, "i", 1);
			else 
				replaceStem(indexStr, stem, "", 0);
			suffPtr << stem << endl;
		} else {
			reverseStr(indexStr);
			suffPtr << indexStr << endl;
		}
	}

	suffPtr.close();
	vcPtr.close();
	indexPtr.close();	
	VCmake("suffixstrip.txt", "vcterms.txt", "VCterm.txt", "measure.txt");
}

void step2() {
	string indexStr;
	char stem[20];
	int measure, flag;
	ifstream indexPtr, measurePtr;
	ofstream suffPtr;
	indexPtr.open("suffixstrip.txt");
	measurePtr.open("measure.txt");
	suffPtr.open("suffixstep3.txt", ios::trunc);

	while(indexPtr >> indexStr && measurePtr >> measure)  {
		memset(stem, 0, 20);
		reverseStr(indexStr);
		if (measure > 0) {
			flag = 0;
			if (find(indexStr,"lanoita") == 0) {
				replaceStem(indexStr, stem, "ate", 7);
				flag = 1;
			}
			else if (find(indexStr,"lanoit") == 0) {
				replaceStem(indexStr, stem, "tion", 6);
				flag = 1;
			}
			else if (find(indexStr,"icne") == 0) {
				replaceStem(indexStr, stem, "ence", 4);
				flag = 1;
			}
			else if (find(indexStr,"icna") == 0) {
				replaceStem(indexStr, stem, "ance", 4);
				flag = 1;
			}
			else if (find(indexStr,"rezi") == 0) {
				replaceStem(indexStr, stem, "ize", 4);
				flag = 1;
			}
			else if (find(indexStr,"ilba") == 0) {
				replaceStem(indexStr, stem, "able", 4);
				flag = 1;
			}
			else if (find(indexStr,"illa") == 0) {
				replaceStem(indexStr, stem, "al", 4);
				flag = 1;
			}
			else if (find(indexStr,"ilten") == 0) {
				replaceStem(indexStr, stem, "ent", 5);
				flag = 1;
			}
			else if (find(indexStr,"ile") == 0) {
				replaceStem(indexStr, stem, "e", 3);
				flag = 1;
			}
			else if (find(indexStr,"ilsuo") == 0) {
				replaceStem(indexStr, stem, "ous", 5);
				flag = 1;
			}
			else if (find(indexStr,"noitazi") == 0) {
				replaceStem(indexStr, stem, "ize", 7);
				flag = 1;
			}
			else if (find(indexStr,"noita") == 0) {
				replaceStem(indexStr, stem, "ate", 5);
				flag = 1;
			}
			else if (find(indexStr,"rota") == 0) {
				replaceStem(indexStr, stem, "ate", 4);
				flag = 1;
			}
			else if (find(indexStr,"msila") == 0) {
				replaceStem(indexStr, stem, "al", 5);
				flag = 1;
			}
			else if (find(indexStr,"ssenevi") == 0) {
				replaceStem(indexStr, stem, "ive", 7);
				flag = 1;
			}
			else if (find(indexStr,"ssenluf") == 0) {
				replaceStem(indexStr, stem, "ful", 7);
				flag = 1;
			}
			else if (find(indexStr,"ssensuo") == 0) {
				replaceStem(indexStr, stem, "ous", 7);
				flag = 1;
			}
			else if (find(indexStr,"itila") == 0) {
				replaceStem(indexStr, stem, "al", 5);
				flag = 1;
			}
			else if (find(indexStr,"itivi") == 0) {
				replaceStem(indexStr, stem, "ive", 5);
				flag = 1;
			}
			else if (find(indexStr,"itilib") == 0) {
				replaceStem(indexStr, stem, "ble", 6);
				flag = 1;
			}
			if (flag == 1) {
				suffPtr << stem << endl;
			} else {
				reverseStr(indexStr);
				suffPtr << indexStr << endl;
			}
		} else {
			reverseStr(indexStr);
			suffPtr << indexStr << endl;
		}
	}

	suffPtr.close();
	measurePtr.close();
	indexPtr.close();	

	VCmake("suffixstep3.txt", "vcterms.txt", "VCterm.txt", "measure.txt");
} 

void step3() {
	string indexStr;
	char stem[20];
	int measure;
	ifstream indexPtr, measurePtr;
	ofstream suffPtr;
	indexPtr.open("suffixstep3.txt");
	measurePtr.open("measure.txt");
	suffPtr.open("suffixstrip.txt", ios::trunc);

	while(indexPtr >> indexStr && measurePtr >> measure)  {
		memset(stem, 0, 20);
		reverseStr(indexStr);
		if (measure > 0) {
			if (find(indexStr,"etaci") == 0) {
				replaceStem(indexStr, stem, "ic", 5);
			}
			else if (find(indexStr,"evita") == 0) {
				replaceStem(indexStr, stem, "", 5);
			}
			else if (find(indexStr,"ezila") == 0) {
				replaceStem(indexStr, stem, "al", 5);
			}
			else if (find(indexStr,"itici") == 0) {
				replaceStem(indexStr, stem, "ic", 5);

			}
			else if (find(indexStr,"laci") == 0) {
				replaceStem(indexStr, stem, "ic", 4);
		
			}
			else if (find(indexStr,"luf") == 0) {
				replaceStem(indexStr, stem, "", 3);
		
			}
			else if (find(indexStr,"ssen") == 0) {
				replaceStem(indexStr, stem, "", 4);
			}
			else {
				replaceStem(indexStr, stem, "", 0);
			}
				suffPtr << stem << endl;
		} else {
			reverseStr(indexStr);
			suffPtr << indexStr << endl;
		}
	}

	suffPtr.close();
	measurePtr.close();
	indexPtr.close();

	VCmake("suffixstrip.txt", "vcterms.txt", "VCterm.txt", "measure.txt");
}

void step4() {
	string indexStr, vcStr;
	char stem[20];
	int measure, flag;
	ifstream indexPtr, measurePtr, vcPtr;
	ofstream suffPtr;
	indexPtr.open("suffixstrip.txt");
	vcPtr.open("vcterms.txt");
	measurePtr.open("measure.txt");
	suffPtr.open("suffixstep3.txt", ios::trunc);

	while(indexPtr >> indexStr && measurePtr >> measure && vcPtr >> vcStr)  {
		memset(stem, 0, 20);
		reverseStr(indexStr);
		if (measure > 1) {
			flag = 0;
			if (find(indexStr,"la") == 0) {
				replaceStem(indexStr, stem, "", 2);
				flag = 1;
			}
		/*	else if (find(indexStr,"ecna") == 0) {
				replaceStem(indexStr, stem, "", 4);
				flag = 1;
			}
			else if (find(indexStr,"ecne") == 0) {
				replaceStem(indexStr, stem, "", 4);
				flag = 1;
			}*/
			else if (find(indexStr,"re") == 0) {
				replaceStem(indexStr, stem, "", 2);
		/*		reverseStr(vcStr);
				if(find(vcStr,"vcv")==1 && (find(indexStr, "g") == 2 || find(indexStr, "v") == 2 || find(indexStr, "") == 2) && (find(indexStr, "rehtie") != 0 || find(indexStr, "rehtien") != 0 || find(indexStr, "regae") != 0)) {
					replaceStem(indexStr, stem, "", 2);
					flag = 1;	
				}
				else {
					strcat(stem, "er");
					flag = 1;
				}*/
				flag = 1;
			}
		/*	else if (find(indexStr,"ci") == 0) {
				replaceStem(indexStr, stem, "", 2);
				flag = 1;
			}*/
			else if (find(indexStr,"elba") == 0) {
				replaceStem(indexStr, stem, "", 4);
				flag = 1;
			}
			else if (find(indexStr,"elbi") == 0) {
				replaceStem(indexStr, stem, "", 4);
				flag = 1;
			}
			else if (find(indexStr,"tna") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}
			else if (find(indexStr,"tneme") == 0) {
				replaceStem(indexStr, stem, "", 5);
				flag = 1;
			}
			else if (find(indexStr,"tnem") == 0) {
				replaceStem(indexStr, stem, "", 4);
				flag = 1;
			}
		/*	else if (find(indexStr,"tne") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}*/
			else if (find(indexStr,"nois") == 0 || find(indexStr,"noit") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}
			else if (find(indexStr,"uo") == 0) {
				replaceStem(indexStr, stem, "", 2);
				flag = 1;
			}
		/*	else if (find(indexStr,"msi") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}
		/*	else if (find(indexStr,"eta") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}*/
			else if (find(indexStr,"iti") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}
			else if (find(indexStr,"suo") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}
			else if (find(indexStr,"evi") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}
			else if (find(indexStr,"ezi") == 0) {
				replaceStem(indexStr, stem, "", 3);
				flag = 1;
			}
			else if (find(indexStr, "ili") == 0) {
				replaceStem(indexStr, stem, "", 2);
				flag = 1;
			}
			if (flag == 1) {
				suffPtr << stem << endl;
			} else {
				reverseStr(indexStr);
				suffPtr << indexStr << endl;
			}
		} else {
			reverseStr(indexStr);
			suffPtr << indexStr << endl;
		}
	}

	suffPtr.close();
	measurePtr.close();
	indexPtr.close();
	vcPtr.close();

	VCmake("suffixstep3.txt", "vcterms.txt", "VCterm.txt", "measure.txt");
}

void step5A() {
	string indexStr,vcStr;
	char stem[20];
	int measure, flag;
	ifstream indexPtr, measurePtr, vcPtr;
	ofstream suffPtr;
	indexPtr.open("suffixstep3.txt");
	vcPtr.open("vcterms.txt");
	measurePtr.open("measure.txt");
	suffPtr.open("suffixstrip.txt", ios::trunc);

	while( indexPtr >> indexStr && measurePtr >> measure && vcPtr >> vcStr) {
		memset(stem, 0, 20);
		reverseStr(indexStr);
		reverseStr(vcStr);
		if (measure > 1) {
			if (find(indexStr, "e") == 0) {
				replaceStem(indexStr, stem, "", 1);
				suffPtr << stem << endl;
			}
		} else if (measure == 1 && (find(vcStr,"cvc") == 0)) {
			if (find(indexStr, "e") == 0) {
				replaceStem(indexStr, stem, "", 1);
				suffPtr << stem << endl;
			}
		} else {
			reverseStr(indexStr);
			suffPtr << indexStr << endl;
		}
	}

	suffPtr.close();
	vcPtr.close();
	measurePtr.close();
	indexPtr.close();

	VCmake("suffixstep3.txt", "vcterms.txt", "VCterm.txt", "measure.txt");
}

void step5B() {
	string indexStr,vcStr;
	char stem[20];
	int measure, flag;
	ifstream indexPtr, measurePtr, vcPtr;
	ofstream suffPtr;
	indexPtr.open("suffixstep3.txt");
	vcPtr.open("vcterms.txt");
	measurePtr.open("measure.txt");
	suffPtr.open("suffixstrip.txt", ios::trunc);

	while ( indexPtr >> indexStr && measurePtr >> measure && vcPtr >> vcStr) {
		memset(stem, 0, 20);
		reverseStr(indexStr);
		reverseStr(vcStr);
		if(find(indexStr, "i") == 0) {
					reverseStr(indexStr);
			copy(indexStr.begin(), indexStr.end() - 1, stem);
			strcat(stem, "y");
			suffPtr << stem << endl;
		}

		else if (measure > 1 && (find(vcStr,"cc") == 0) && find(indexStr,"l") == 0) {
			replaceStem(indexStr, stem, "", 1);
			suffPtr << stem << endl;
		} else {
			reverseStr(indexStr);
			suffPtr << indexStr << endl;
		}
	}

	suffPtr.close();
	vcPtr.close();
	measurePtr.close();
	indexPtr.close();

	VCmake("suffixstrip.txt", "vcterms.txt", "VCterm.txt", "measure.txt");
}

int finalTerms() {

	char buffer[200][20] = {""};
	char inStr[20];
	int flag, count = 0;

	ifstream inPtr;
	ofstream outPtr;

	inPtr.open("suffixstrip.txt");
	outPtr.open("STEMS.txt");

	while(inPtr >> inStr) {
		flag = 0;
		for(int i = 0; i < count; i++) {
			if(strcmp(inStr, buffer[i]) == 0) {

				flag = 1;

			}
		}
		if (flag != 1) {
			strcat(buffer[count], inStr);
			count++;
			outPtr << inStr << endl;
		}
		else
			continue;
	}

	outPtr.close();
	inPtr.close();
	return count;
}

void finalOutput(string stem, ofstream& outPtr) {
	ifstream indexPtr, suffixPtr;
	string indexStr, suffixStr;
	int count = 0, flag = 0;

	indexPtr.open("indexterms.txt");
	suffixPtr.open("suffixstrip.txt");

	
	while(indexPtr >> indexStr && suffixPtr >> suffixStr) {

		if(suffixStr == stem && (suffixStr != indexStr)) {
			flag++;
			if(flag == 1)
				outPtr << stem << " <-- ";
			outPtr << indexStr << " ";
			count++;
		}
	}
	if (flag > 0)
		outPtr << "(" << count << ")" << endl;
	indexPtr.close();
	suffixPtr.close();
}

void displayFinal() {
	string stemStr, outStr;

	ifstream stemPtr;
	ofstream outPtr;

	stemPtr.open("STEMS.txt");
	outPtr.open("output.txt");
	outPtr << "ACTUAL STEM <-- ORIGINAL WORDS (NUMBER OF OCCURRENCES IN DOCUMENT)" << endl << endl;
	
	while(stemPtr >> stemStr) {
		finalOutput(stemStr, outPtr); 
	}
	outPtr.close();
	stemPtr.close();
}

int main () {
	int wordcount=0, tokencount=0;

	filePrep(&wordcount);
	removeFluff(&tokencount);
	VCmake("indexterms.txt", "vcterms.txt", "VCterm.txt", "measure.txt");

	step1A();
	step1B();
	step1C();

	step2();
	step3();

	step4();
	step5A();
	step5B();

	finalTerms();

	displayFinal();
	return 0;
}