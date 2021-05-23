#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "md5.h"

using namespace std;

class LoginManager{
    public:
    	void displayMenu(){
			clear();
			cout<<"\033[94;1;4;5m\n\t\t**AUTHENTICATION MECHANISM** v1.0\n\t\t\t\tby Ispas Andrei\033[0m"<<endl<<endl;
            cout<<"Choose an option:"<<endl<<endl<<"1. LogIn\t\t"<<"2. Register"<<endl;
		}

        //clear screen using ANSI esacape codes
        void clear(){
            cin.get();
            // \033 is the C-style octal sequence for the escape character.
            cout << "\033[2J\033[1;1H";
        }
        
        int count = 0;
        void login(){
        	clear();
            cout<<"\033[94;1;5m\n\t\tLogin:\033[0m"<<endl<<endl;
            cout << "Username:";
            
            cin >> usernameInput;
            int userID = readFile(usernameInput, "users.db");
            
            if(userID != 0){
                cout << "Password:";
                
                cin >> passwordInput;
                int passwordID = readPasswdFile(passwordInput, "passwd.db", userID);
                
                if(userID == passwordID){
                    cout << "Logged in successfully!"<<endl<<endl;
                    return;
                } else {
                    cout<<"Wrong Password!"<<endl<<endl;
                }
                cin.get();
            } else {
                cout<<"Invalid Username!" << endl<<endl;
                cin.get();
            }
            
            //Anti brute force counter
            count++;
            if(count < 3){
            	login();
            } else {
            	cout<<"Too many wrong attempts!"<<endl<<"Bye!";
            }
        }
        
        int readFile(string input, const char* fileName){
            input = md5(input);
        	string line;
        	fstream file;
        	string checker;
            
            //fix last newline issue and deal with empty folders
            file.seekg(0, ios::end);
        	if(file.tellg() == 0){
        		file << input;
                file << "\n$KEY:1";
                return 1;
        	}
        	file.seekg(0, ios::beg);

        	file.open(fileName, ios::in);
        	while(true){
        		getline(file, checker);
        		if(checker.find("$KEY:") != string::npos){
        			if(input == line){
        				file.close();
        				checker.erase(0, 5);
        				int id;
        				istringstream(checker) >> id;
        				return id;
        			} else {
        				line.erase(line.begin(), line.end());
        			}
        		} else {
        			line = checker;
        			checker = "";
        		}

        		if(file.peek() == EOF){
        			file.close();
        			return 0;
        		}
        	}
        }
        
        int readPasswdFile(string input, const char* fileName, int userID){
            input = md5(input);
        	string line;
        	fstream file;
        	string checker;
            
            file.open(fileName, ios::in);
        	while(true){
        		getline(file, checker);
        		if(checker.find("$KEY:") != string::npos){
                    checker.erase(0, 5);
                    int intContainer;
                    istringstream(checker) >> intContainer;
        			if(input == line && intContainer == userID){
        				file.close();
        				int id;
        				istringstream(checker) >> id;
        				return id;
        			} else {
        				line.erase(line.begin(), line.end());
        			}
        		} else {
        			line = checker;
        			checker = "";
        		}

        		if(file.peek() == EOF){
        			file.close();
        			return 0;
        		}
        	}
        }

        void addUser(string username, string password){
        	if(readFile(username, "users.db") != 0){
        		cout<<"\nUsername already taken!";
        		return;
        	}
        	
        	int id = 1 + getLastID();
        	writeFile(username, "users.db", id);
        	writeFile(password, "passwd.db", id);
        	cout<<"\nAccount created successfully.";
        }

        int getLastID(){
        	fstream file;
        	file.open("users.db", ios::in);
        	file.seekg(0, ios::end);

        	if(file.tellg() == 0){
        		return 0;
        	}
        	
        	string s;
        	for(int i = -1; s.find("$") == string::npos; i--){
        		file.seekg(i, ios::end);
        		file >> s;
		    }
        		file.close();
        		s.erase(0, 5);
        		int id;
        		istringstream(s) >> id;

        		return id;
        }

        void writeFile(string line, const char* fileName, const int& id){
        	fstream file;
        	file.open(fileName, ios::app);

        	file.seekg(0, ios::end);
        	if(file.tellg() != 0){
        		file << "\n";
        	}
        	file.seekg(0, ios::beg);

        	file << md5(line);
        	file << "\n";
        	file << "$KEY:"<< id;
        	file.close();
        }

    private:
        string usernameInput;
        string passwordInput;
};


int main()
{
    LoginManager app;
    int option;
    string username, password, passwordCheck;

    while(true){
    	app.displayMenu();
    	cin >> option;
    	
    	if(option != 1 || option != 2){
    		cout << "\nInvalid option!"<<endl;
    	}

    	if(option == 1){
    		app.login();
    		break;
    	} else if(option == 2){
    		app.clear();
    		cout << "\033[94;1;5m\n\t\tRegister an account:\n\n\033[0m";
    		cout << "Username: ";
    		cin >> username;
    		cout << "Password: ";
    		cin >> password;
    		cout << "Re-enter Password: ";
    		cin >> passwordCheck;

    		if(password != passwordCheck){
    			cout<<"\nPasswords don't match!"<<endl;
    		} else {
    			app.addUser(username, password);
    		}
    	}
    	
    	cin.get();
    }

    cin.get();
}

/*

In the next versions:
-delete user from database
-hide password when typing it
-password recovery system

*/
