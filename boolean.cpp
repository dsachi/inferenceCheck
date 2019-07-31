/*/////////////////////////////////////////////////

Name: Christopher Thomas Phelan
Student ID: R00524502
Project: Boolean Inference Check
Class: Theory of Programming Languages

/////////////////////////////////////////////////*/

/*/////////////////////////////////////////////////
Notes:
This program will take an input stream from the command line.


The following are valid tokens:
#, :=, T, F, ->, v, ^, ~, (, ), ., ;, var
var is any lowercase letter except v

/////////////////////////////////////////////////*/


#include <iostream>
#include <string>
#include <vector>
using namespace std;

//GLOBAL VARIABLES 
int undefFlag = 0;


// Write out classes
class stack{
	//Custom Stack

	
	public:
		//Declare push for stack
		void push(bool b) {
			top++;
			stk[top] = b;
		}
		
		//Declare pop for stack
		bool pop(){
			if (stkEmpty()) {
				cout << "Stack is empty" << endl;
			} else {
				top--;
				return stk[top+1];
			}
			
		}
		

	protected:
		bool stk[100];
		int top = -1; // This starts at -1 so that the first push is located in array stk[0]
		
		//Error message if stack is empty
		bool stkEmpty(){
			if (stk[top] < 0){
				return true;
			} else {
				return false;
			}
		}


};

//Variable Table is a custom class to store variables and boolean values.
class varTable{
	public:
		//Add or modify variable table
		void modify(char var, char boolVal){
			bool inTable = false;
			
			for (int i = 0; i < 28; i++){
				if (var == arrTable[i][0]) {
					arrTable[i][1] = boolVal;
					inTable = true;
					break; //Exit the loop when the variable is found in the table
				}
			}
			
			if (inTable == false){
				assign(var, boolVal);
			}
			
		}
		
		//This function searches the table for variable. 
		bool isInTable(char var){
			for (int i = 0; i < 28; i++){
				if (var == arrTable[i][0]){
					return true;
					break;
				}
			}
			return false;
		}
		
		void display(){
			
			for (int i = 0; i <= next; i++){
				cout << arrTable[i][0] << " ";
				cout << (int)arrTable[i][1] << endl;
			}
		}
		//This function assumes the variable is in the table.
		//	If it is not it returns false.
		bool find(char var){
			bool foundChar;
			for (int i = 0; i < 28; i++){
				if (var == arrTable[i][0]){
					foundChar = arrTable[i][1];
					return foundChar;
				}
			}
			return false;
		}
		
		void initialize(){
			for (int i = 0; i < 28; i++){
				for (int j = 0; j < 2; j++){
					arrTable[i][j] = '\0';
				}
			}
			next = -1;
		}
		
	
	protected:
		//Private function for assigning the next boolean value into the table.
		void assign(char var, char boolVal){
				next++;
					arrTable[next][0] = var;
					arrTable[next][1] = boolVal;
		}
	
		char arrTable[28][2];
		int next = -1;
	
};


//Helper Functions
bool tokenize(string str, vector<string> &tkArray);
string getNextToken(string *&ptrToken);
string checkForVar(char varChar, varTable *& tbl);

//Recursive Descent Function
bool P(string *& ptrToken, stack *& stk, varTable *& tbl);
bool D(string *& ptrToken, stack *& stk,  varTable *& tbl);
bool V(string *& ptrToken, stack *& stk,  varTable *& tbl, char var);
bool B(string *& ptrToken, stack *& stk,  varTable *& tbl);
bool IT(string *& ptrToken,  stack *& stk,  varTable *& tbl);
bool IT_Tail(string *& ptrToken,  stack *& stk,  varTable *& tbl);
bool OT(string *& ptrToken,  stack *& stk,  varTable *& tbl);
bool OT_Tail(string *& ptrToken, stack *& stk,  varTable *& tbl);
bool AT(string *& ptrToken, stack *& stk,  varTable *& tbl );
bool AT_Tail(string *& ptrToken,  stack *& stk,  varTable *& tbl);
bool L(string *& ptrToken,  stack *& stk,  varTable *& tbl);
bool A(string *& ptrToken,  stack *& stk,  varTable *& tbl);

int main() {

	string theString;

	vector<string> tkArray;
	cout << "Enter a string:" << endl;
	getline(cin,theString);
	//ERROR CHECKING: cout << theString << endl;
	bool tokenTest;
	bool syntaxTest;
	bool semanticAnswer;
	
	//Initialize classes
	stack *stk;
	varTable *tbl;
	
	stk = new stack;
	tbl = new varTable;
	tbl->initialize();
	
	//Note - tokenTest is not used, but is assigned in case it is needed for error checking.
	tokenTest = tokenize(theString,tkArray);
	//ERROR CHECK: cout << tokenTest << endl;
	
	string* tk = tkArray.data();	// This makes vector data act more like a pointer
	syntaxTest = P(tk,stk,tbl);		// Begin recursive descent. 

	//Display the results of recursive descent.
	if (syntaxTest == true) {
		semanticAnswer = stk->pop();
		if (semanticAnswer == true){
			cout << "T" << endl;
		} else if (semanticAnswer == false){
			cout << "F" << endl;
		}
	
	//This will show undefined variable message before syntax error message. 
	//	In future versions, this should be fixed/updated.
	} else if (undefFlag == -1) {
		cout << "Undefined Variable" << endl;
	} else {
		cout << "Syntax Error" << endl;
	}
	
	//tbl->display();
	
	//Free all memory allocated.
	delete stk;
	delete tbl;
	
	return 0;
}

//The tokenizer will take the sentence and convert it to a vector of strings that have the tokens.
bool tokenize(string str,vector<string> &tkArray){
	int arrLoc = 0;
	bool errorFlag = false;
	//Loop through the input stream and push every token to a vector.
	for (string::iterator it=str.begin(); it!=str.end(); ++it){
		switch (*it){
			case '#':
				tkArray.push_back("#");
				break;
			
			case 'T':
				tkArray.push_back("T");
				break;
			
			case 'F':
				tkArray.push_back("F");
				break;
			
			case 'v':
				tkArray.push_back("v");
				break;
			
			case '^':
				tkArray.push_back("^");
				break;
			
			case '~':
				tkArray.push_back("~");
				break;
			
			case ' ':
				break;
			
			case '(':
				tkArray.push_back("(");
				break;
			
			case ')':
				tkArray.push_back(")");
				break;
			
			case '.':
				tkArray.push_back(".");
				break;
			
			case ';':
				tkArray.push_back(";");
				break;
			case '-':
				it++;
				if (*it == '>'){
					tkArray.push_back("->");
				}
				break;
			case ':':
				it++;
				if (*it == '='){
					tkArray.push_back(":=");
				}
				break;
			
			//Letters and Variables
			// Note: There may be a better way of doing this. In future versions,
			// 		you should see if there is a better way to check each letter.
			case 'a':
				tkArray.push_back("a");
				break;
			case 'b':
				tkArray.push_back("b");
				break;
			case 'c':
				tkArray.push_back("c");
				break;
			case 'd':
				tkArray.push_back("d");
				break;
			case 'e':
				tkArray.push_back("e");
				break;
			case 'f':
				tkArray.push_back("f");
				break;
			case 'g':
				tkArray.push_back("g");
				break;
			case 'h':
				tkArray.push_back("h");
				break;
			case 'i':
				tkArray.push_back("i");
				break;
			case 'j':
				tkArray.push_back("j");
				break;
			case 'k':
				tkArray.push_back("k");
				break;
			case 'l':
				tkArray.push_back("l");
				break;
			case 'm':
				tkArray.push_back("m");
				break;
			case 'n':
				tkArray.push_back("n");
				break;
			case 'o':
				tkArray.push_back("o");
				break;
			case 'p':
				tkArray.push_back("p");
				break;
			case 'q':
				tkArray.push_back("q");
				break;
			case 'r':
				tkArray.push_back("r");
				break;
			case 's':
				tkArray.push_back("s");
				break;
			case 't':
				tkArray.push_back("t");
				break;
			case 'u':
				tkArray.push_back("u");
				break;
			case 'w':
				tkArray.push_back("w");
				break;
			case 'x':
				tkArray.push_back("x");
				break;
			case 'y':
				tkArray.push_back("y");
				break;
			case 'z':
				tkArray.push_back("z");
				break;
			default:
				errorFlag = true;
				break;
		}
	}
	tkArray.push_back("\0");	//Used to prevent runtime errors if there are not any tokens. 
	return errorFlag;
}

//This function is the equivalent to lex.get. It is used to "consume" the next vector token.
//	It consumes the next vector by moving the pointer in ptrToken.
string getNextToken(string *& ptrToken){
    string tkStr = *ptrToken;
    if(*ptrToken != "\0"){
        ptrToken++;
	    tkStr = *ptrToken;
    }
	return tkStr;
}	

//This function will search the varTable for variable and return the tables 
//	boolean value as a string, either T or F.
string checkForVar(char varChar, varTable *& tbl){

	if (tbl->isInTable(varChar)) {
		if (tbl->find(varChar)) {
			//cout << tbl->find(varChar) << endl;
			return "T";
		} else {
			//cout << tbl->find(varChar) << endl;
			return "F";
		}
			
	}
	return "error";
}


//First of the recursive descent functions
bool P(string *& ptrToken, stack *& stk, varTable *& tbl) {
	if (D(ptrToken,stk,tbl)) {
		if (B(ptrToken,stk,tbl)) {
			return true;
		} else {
			return false;
		}
		
	} else {
		return false;
	}
}

//Check for Variable 
bool D(string *& ptrToken, stack *& stk, varTable *& tbl){
	char var;
	bool testVar;
	string str;
	
	if (*ptrToken == "#") {
		getNextToken(ptrToken);	
		str = *ptrToken;
		var = str[0];
		//If the character is ASCII 'a' - 'z' excluding 'v'
		if (var >96 && var < 123 && var != 118){
			getNextToken(ptrToken);
			if (*ptrToken == ":="){
			    getNextToken(ptrToken);
				if (V(ptrToken,stk,tbl,var)){
						if (*ptrToken == ";"){
						    getNextToken(ptrToken);    
						    if (D(ptrToken,stk,tbl)){
							    return true;
						    } else {
							    return false;
						    }
						    return true;
						} else {
						    return false;
						}
					return true;
				} else {
					return false;
				}
				return true;
			} else {
				return false;
			}
			return true;
		} else {
			return false;
		}
	}
	//Empty Set
	return true;
}

//Add variables and their boolean equivalents to the varTable.
bool V(string *& ptrToken, stack *& stk,  varTable *& tbl, char var){

	if (*ptrToken == "T") {
		tbl->modify(var,true);
		getNextToken(ptrToken);
		return true;
	} else if (*ptrToken == "F") {
		tbl->modify(var, false);
		getNextToken(ptrToken);
		return true;
	} else {
		return false;
	}
	return false;
}

//Begin the boolean checker
bool B(string *& ptrToken, stack *& stk,  varTable *& tbl){

	if (IT(ptrToken,stk,tbl)){
		if (*ptrToken == "."){
			return true;
		} else {
			return false;
		}
		
		return true;
	} else {
		return false;
	}
	return false;
}

//Imply_term
bool IT(string *& ptrToken,  stack *& stk,  varTable *& tbl){
	if (OT(ptrToken,stk,tbl)) {
		if (IT_Tail(ptrToken,stk,tbl)){
			return true;
		} else {
			return false;
		}
		return true;
	} else {
		return false;
	}
	return false;
}

//Imply_term (tail)
bool IT_Tail(string *& ptrToken,  stack *& stk,  varTable *& tbl){
	bool b1, b2;
	
	if (*ptrToken == "->"){
		getNextToken(ptrToken);
		if (OT(ptrToken,stk,tbl)){
			//Sematics for evaluating imply_term.
			b1 = stk->pop();
			b2 = stk->pop();
			
			if (b2 == true && b1 == false){
				stk->push(false);
			} else {
				stk->push(true);
			}
			//End semantics
			if (IT_Tail(ptrToken,stk,tbl)){
				return true;
			} else {
				return false;
			}
			return true;
		} else {
			return false;
		}
	return true;
	} else {
	//For the empty set
	return true;
	}
}

// Or term
bool OT(string *& ptrToken,  stack *& stk,  varTable *& tbl){
	if (AT(ptrToken,stk,tbl)){
		if (OT_Tail(ptrToken,stk,tbl)){
			return true;
		} else {
			return false;
		}
		return true;
	} else {
		return false;
	}
	return false;
}
// Or term (tail).
bool OT_Tail(string *& ptrToken, stack *& stk,  varTable *& tbl){
		bool b1, b2;

		if (*ptrToken == "v"){
			getNextToken(ptrToken);
			if (AT(ptrToken,stk,tbl)){
				//Or term semantics
				b1 = stk->pop();
				b2 = stk->pop();
				
				if (b1 == false && b2 == false){
					stk->push(false);
				} else {
					stk->push(true);
				}
				
				//End semantics
				if (OT_Tail(ptrToken,stk,tbl)){
					return true;
				} else {
					return false;
				}
				return true;
			} else {
				return false;
			}
			return true;
		} else {
			//This is the statement for the empty string
			return true;
		}
}

//And term checker
bool AT(string *& ptrToken, stack *& stk,  varTable *& tbl ){
	if (L(ptrToken,stk,tbl)){
		if (AT_Tail(ptrToken, stk,tbl)){
			return true;
		} else {
			return false;
		}
		return true;
	} else {
		return false;
	}
	
}

//And term (tail).
bool AT_Tail(string *& ptrToken,  stack *& stk,  varTable *& tbl){
	bool b1, b2;
	
	if (*ptrToken == "^") {
		getNextToken(ptrToken);
		if (L(ptrToken,stk,tbl)){
			//And term semantics
			b1 = stk->pop();
			b2 = stk->pop();
			
			if (b1 == true && b2 == true) {
				stk->push(true);
			} else {
				stk->push(false);
			}
			//End sematics
			if (AT_Tail(ptrToken,stk,tbl)){
				return true;
			} else {
				return false;
			}
			return true;
		} else {
			return false;
		}
		return true;
	} else {
		//This statement is for the empty string
		return true;
	}
}
// Literal checker
bool L(string *& ptrToken,  stack *& stk,  varTable *& tbl){
	bool b1;
	
	if (A(ptrToken, stk, tbl)){
		return true;
	} else if (*ptrToken == "~"){
		getNextToken(ptrToken);
		if (L(ptrToken,stk,tbl)){
			//Negation semantics
			b1 = stk->pop();
			
			if (b1 == true){
				stk->push(false);
			} else {
				stk->push(true);
			}
			//End semantics
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
	return false;
}

bool A(string *& ptrToken,  stack *& stk,  varTable *& tbl){	

	string str;
	
	//Check for T and F first
	if (*ptrToken == "T"){
		stk->push(true); //Semantic
		getNextToken(ptrToken);
		return true;
	} else if (*ptrToken == "F"){
		stk->push(false); //Semantic
		getNextToken(ptrToken);
		return true;
	}
	
	//Check for variable letters
	//NOTE: Attempted switch statement here, but C++ switch 
	//	statements must contain a number (or a 'char'). Therefore
	//	because of the implementation of strings, switch did not work.
	if (*ptrToken == "a"){
		str = checkForVar('a', tbl);
	} else if (*ptrToken == "b"){
		str = checkForVar('b', tbl);
	} else if (*ptrToken == "c"){	
		str = checkForVar('c', tbl);
	} else if (*ptrToken == "d"){
		str = checkForVar('d', tbl);
	} else if (*ptrToken == "e"){
		str = checkForVar('e', tbl);
	} else if (*ptrToken == "f"){
		str = checkForVar('f', tbl);
	} else if (*ptrToken == "g"){
		str = checkForVar('g', tbl);
	} else if (*ptrToken == "h"){
		str = checkForVar('h', tbl);
	} else if (*ptrToken == "i"){
		str = checkForVar('i', tbl);
	} else if (*ptrToken == "j"){
		str = checkForVar('j', tbl);
	} else if (*ptrToken == "k"){
		str = checkForVar('k', tbl);
	} else if (*ptrToken == "l"){
		str = checkForVar('l', tbl);
	} else if (*ptrToken == "m"){
		str = checkForVar('m', tbl);
	} else if (*ptrToken == "n"){
		str = checkForVar('n', tbl);
	} else if (*ptrToken == "o"){
		str = checkForVar('o', tbl);
	} else if (*ptrToken == "p"){
		str = checkForVar('p', tbl);
	} else if (*ptrToken == "q"){
		str = checkForVar('q', tbl);
	} else if (*ptrToken == "r"){
		str = checkForVar('r', tbl);
	} else if (*ptrToken == "s"){
		str = checkForVar('s', tbl);
	} else if (*ptrToken == "t"){
		str = checkForVar('t', tbl);
	} else if (*ptrToken == "u"){
		str = checkForVar('u', tbl);
	} else if (*ptrToken == "w"){
		str = checkForVar('w', tbl);
	} else if (*ptrToken == "x"){
		str = checkForVar('x', tbl);
	} else if (*ptrToken == "y"){
		str = checkForVar('y', tbl);
	} else if (*ptrToken == "z"){
		str = checkForVar('z', tbl);
	}
	
	//Convert the variable string T or F into boolean
	if (str == "T"){
		stk->push(true); //Semantic
		getNextToken(ptrToken);
		return true;
	} else if (str == "F"){
		stk->push(false); //Semantic
		getNextToken(ptrToken);
		return true;
	} else if (str == "error") {
		undefFlag = -1;
		return true;
	}
	
	//Parenthesis 
	if (*ptrToken == "(") {
		getNextToken(ptrToken);
		if (IT(ptrToken,stk,tbl)){
			if (*ptrToken == ")"){
				getNextToken(ptrToken);
				return true;
			} else {
				return false;
			}
			return true;
		} else {
			return false;
		}
		return true;
	} else {
		return false;
	}
	
	return false;
}





