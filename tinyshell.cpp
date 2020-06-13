
#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <chrono>
#include <cstring>
#include <string>
#include <math.h>

using namespace std;

struct Com{
    string command_com;
    double time_com;
};

string remove_spaces(string old_command);
void output(list<Com> & recall);

bool operator<(const Com & a, const Com & b){
    return (a.time_com>b.time_com);}



int main() {
    string command;
    list <Com> recall;

    cout << "tinyshell>";
    getline(cin, command);
    int command_count=1;



    while (command != "exit"){
        command = remove_spaces(command);
       
        //if (command == "exit"){
         //      exit(0);}// if end

        Com temp;
        temp.command_com = command;


        if (command != "history" && command != "history -sbu" ){
          
            const char *input = command.c_str();
            pid_t pid;
            pid = fork();
            auto time_1 = chrono::high_resolution_clock::now();
               if (pid <0){
                cout << "Fork failed" << endl;}
               else if (pid ==0){
                   system(input);
                _exit(0);}
               else {
                waitpid(pid, NULL, 0);}

            auto time_2 = chrono::high_resolution_clock::now();
            auto time_diff = chrono::duration_cast<chrono::nanoseconds>(time_2-time_1).count();
            temp.time_com = time_diff;
            recall.push_front(temp);
            cout << "tinyshell>";
            getline(cin, command);
            command_count++;
           }//end if

        else if (command == "history"){
		
	    pid_t pid;
            pid = fork();
               auto time_1 = chrono::high_resolution_clock::now();
               if (pid <0){
                cout << "Fork failed" << endl;}
               else if (pid ==0){
               	output(recall);
          
                _exit(0);
               }
               else {
                waitpid(pid, NULL, 0);     

            }

            auto time_2 = chrono::high_resolution_clock::now();
            auto time_diff = chrono::duration_cast<chrono::nanoseconds>(time_2-time_1).count();

            temp.time_com = time_diff;
           
            recall.push_front(temp);
            cout << "tinyshell>";
            getline(cin, command);
            command_count++;
        }//end if

        else if (command == "history -sbu"){

	    pid_t pid;
            pid = fork();
            auto time_1 = chrono::high_resolution_clock::now();
            if (pid <0){
                cout << "Fork failed" << endl;}
            else if (pid ==0){
            	list <Com> a;
		list<Com>:: iterator itr;
		if (recall.size()>5){
			itr = recall.begin();
			for (int i =0; i<5 ; i++){
				a.push_back(*itr);
				itr++;
			}//end of for
		}
		else {

			for (itr = recall.begin(); itr!=recall.end(); itr++){
				a.push_back(*itr);
			}//end of for
		}



            	a.sort();
		output(a);
                _exit(0);
            }
               else {
                waitpid(pid, NULL, 0);     

            }
		 
            auto time_2 = chrono::high_resolution_clock::now();
            auto time_diff = chrono::duration_cast<chrono::nanoseconds>(time_2-time_1).count();

            temp.time_com = time_diff;
            
            recall.push_front(temp);
            cout << "tinyshell>";
            getline(cin, command);
            command_count++;
        }//end if


    }//while end
exit(0);

}//main end

void output(list<Com> & recall){
	list<Com>:: iterator itr;
	if (recall.size()>5){
		itr = recall.begin();
		for (int i =0; i<5 ; i++){
			cout << itr->command_com << "  " << round(((itr->time_com)/1000000000)*1000.0)/1000.0<< "s" <<endl;
			itr++;
		}//end of for
	}
	else {

		for (itr = recall.begin(); itr!=recall.end(); itr++){
			 cout << itr->command_com << "  " << round(((itr->time_com)/1000000000)*1000.0)/1000.0<< "s" <<endl;
		}//end of for
	}

}

string remove_spaces(string old_command){
    string new_command1, new_command2;
    int start = old_command.find_first_not_of(" ");
    int end = old_command.find_last_not_of(" ");
    int length = end - start + 1;
    new_command1 = old_command.substr(start, length);

    char arr[new_command1.length()+1];
    strcpy(arr, new_command1.c_str());

    for (int i=0; i < new_command1.length(); i++){
        if (arr[i] == ' ' && arr[i+1] == ' '){
               continue;
         }
         else{
               new_command2 += arr[i];
         }
    }

    return new_command2;
}






