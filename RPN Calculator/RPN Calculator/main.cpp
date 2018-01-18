#include<iostream>
#include<stack>
#include<queue>
#include<string>
#include<vector>
#include<stdexcept>
#include<typeinfo>
#include<cmath>
#include<sstream>
using namespace std;


#define EULER 2.718281
#define PI 3.141592


vector<string> split(const string & str, const string & delimiter=" ", bool noempty = true){
    vector<string> result;
    size_t prev=0;
    size_t next=0;
    
    while(next=str.find_first_of(delimiter,prev),next!= string::npos){
        if (!noempty || next>prev) {
            result.push_back(str.substr(prev,next-prev)); //Extracts substring at prev with length next-prev;
        }
        prev=next+1;// Move
    }
    //Last possible token
    if(prev<str.size()){
        result.push_back(str.substr(prev)); //push into vector.
    }
    
    return result;
}

int value(const string& s) {    //assigns a value to each math operator
    if (s == "+") {
        return 0;
    } else if (s == "-") {
        return 0;
    } else if (s == "*") {
        return 1;
    } else if (s == "/") {
        return 1;
    } else if (s == "^") {
        return 2;
    } else {
        return -1;
    }
}


bool is_symbol( const string& s) {  //checks if string s is a valid mathematical operator/symbol - for shunting yard
    if (s == "+" || s == "-" || s == "*" || s == "/" || s == "^" ||
        s == "(" || s == ")" ) {
        return true;
    } else {
        return false;
    }
}

bool is_symbol(const char& c) {     //checks if char c is a valid mathematical operator/symbol - for calculator
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^' ||
        c == 's' || c == 'c' || c == 't' || c == 'l' || c== 'z' ||
        c == 'p' || c == 'e') {
        return true;
    } else {
        return false;
    }
}


bool is_trig(const string& s) {     //checks if string s is a valid trig function
    if (s == "log" || s == "sin" || s == "cos" || s == "tan" || s == "ln" ||
        s == "e" || s == "pi") {
        return true;
    }
    return false;
}


int compare(const string& s, const stack<string>& op) {     // compares two operators and their values
    if (op.empty() || op.top() == "(" || op.top() == ")") {
        return -100;
    } else if (value(s)==-1) {  //throw error if it's not a valid operator
        throw logic_error("Invalid operator");
    } else if (value(s) > value(op.top())) {    // if token is > than top operator
        return 1;
    }else if (value(s) < value(op.top())) {     // if token is < than top operator
        return 0;
    } else {
        return -1;      // if two operators are equal rank, return -1. main function handles -1 condition
    }
}


void operation(const char& c, stack<double>& in) {  //calculates the operation, given the operator as char c and the inputs as stack in
    double left, right, x;
    
    switch(c) {
        case '+':
            left = in.top();
            in.pop();
            right = in.top();
            in.pop();
            in.push(left + right);
            break;
            
        case '-':
            right = in.top();
            in.pop();
            left = in.top();
            in.pop();
            in.push(left-right);
            break;
            
        case '*':
            left = in.top();
            in.pop();
            right = in.top();
            in.pop();
            in.push(left * right);
            break;
            
        case '/':
            right = in.top();
            in.pop();
            left = in.top();
            in.pop();
            in.push(left/right);
            break;
            
        case '^':
            right = in.top();
            in.pop();
            left = in.top();
            in.pop();
            in.push(pow(left, right));
            break;
            
        case 's':
            x = in.top();
            in.pop();
            in.push(sin(x));
            break;
            
        case 'c':
            x = in.top();
            in.pop();
            in.push(cos(x));
            break;
            
        case 't':
            x = in.top();
            in.pop();
            in.push(tan(x));
            break;
            
        case 'l': //log case
            x = in.top();
            in.pop();
            in.push(log10(x));
            break;
            
        case 'z':  //ln(x) case;
            x = in.top();
            in.pop();
            in.push(log(x));
            break;
    }
}

void shunting_yard(vector<string>& tok, queue<string>& in, stack<string>& op) {
    
    for(int i = 0; i < tok.size() ; ++i) {
        if (is_trig(tok[i])) {
            if (tok[i] == "pi" || tok[i] == "e") {
                in.push(tok[i]);
            } else {
                op.push(tok[i]);
            }
        } else if(is_symbol(tok[i])) {
            
            if(tok[i]==")" ){     //function take care the ( and )
                while(op.top() != "(") {
                    in.push(op.top());//remove top from op and push into input
                    op.pop();
                }
                op.pop(); //remove the (
            }
            
            else if (tok[i]=="(") {
                op.push(tok[i]);
            } else if (compare(tok[i], op) == 1) {  // if token is > than top operator
                op.push(tok[i]);  //push the new operator into stack
            } else if (compare(tok[i], op) == 0){   // if token is < than top operator
                while(compare(tok[i], op) == 0) {   // until top operator is > or = to token,
                    in.push(op.top());              // if token is < top operator then the while keep pop untile the one = or > then self.
                    op.pop();           //pop top operator and push into queue
                }
                op.push(tok[i]);//after pop operator greater then self and push self into operator
            } else if (compare(tok[i], op) == -1) { //if token is = to top operator
                while(compare(tok[i], op) == -1) { // until top operator is < or > to token, push top operator into queue.
                    in.push(op.top());
                    op.pop();
                }
                op.push(tok[i]);
            } else {    //push token in - only for first token
                op.push(tok[i]);// this is the condition to  check first op. if we dont have we can not compare.
            }
            
        } else if(!is_symbol(tok[i]) && typeid(stoi(tok[i])) != typeid(int)) {
            throw invalid_argument("Invalid argument");
        } else {
            in.push(tok[i]);    //assume string is a number if it's not an operator
        }
    }
    while (!op.empty()) {   // when all tokens have been check, push all remaining operators into the input queue
        in.push(op.top());
        op.pop();
    }
}


double calculator(stack<double>& in, const string & s) {
    for(int i = 0; i <= s.length() ; ++i) {
        if (is_symbol(s[i]) ) {
            if( isalpha(s[i])) {
                if (s[i+1] == 'n') {
                    operation('z', in);//ln
                    i+=1;
                } else if (s[i] == 'p') {   //pi
                    in.push(PI);// which define on top
                    i+=1;
                } else if (s[i] == 'e') {   //e
                    in.push(EULER);
                } else {
                    operation(s[i], in);//log, sin ,cos, tan
                    i+=2;
                }
            } else { //calculates using regular operators
                operation(s[i], in);
            }
            
        } else if (isdigit(s[i])) {       //if character is a number, change into integer and store in input_stack
            string st = string(1, s[i]);
            for (int j = i+1; j <= s.length(); ++j) {    //checks to see if character has more than one digit
                if (isdigit(s[j])) {
                    string a = string(1, s[j]);//here we add digits on
                    st+=a;
                    i=j; //override from original for loop
                } else {
                    break; //if there are no additional digits at the end, break out of for loop.
                }
            }
            int x = stoi (st);       //convert string into int
            in.push(x);    //push int onto stack
        }
    }
    
    return in.top();
}



int main(){
    
    string s;
    string str;
    stringstream ss;
    queue<string> input_q;    // queue for our output
    stack<string> operator_s; //stack for operators
    
    cout<<"Enter in mathematical expression, with a space between each value/operator/symbol: ";
    getline(cin, s);
    
    vector<string> token = split(s);//to split user into a vector
    //split the code we use in the class
    
    shunting_yard(token, input_q, operator_s);//call the shunting yard function
    
    
    cout<<endl<<"The RPN equation is: ";
    
    while (!input_q.empty()) {        // prints out queue
        cout << input_q.front() << " ";
        ss << input_q.front();  //converts string array into one string
        ss << " \n";
        input_q.pop();
    }
    
    str = ss.str();
    
    // Calculates new RPN equation
    stack<double> input_stack;
    double x = calculator(input_stack, str);
    
    cout<<"The answer is "<<input_stack.top()<<endl;
    
    
}
