#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>   //stoi
#include <sstream>  //used for parsing the input
#include <algorithm>//used for parsing the input
#include <iterator> //used for parsing the input

using namespace std;

struct PCB{
    PCB *parent;
    vector<PCB> child;
    int burstTime,
        quantumTime,
        PID;
    int waitingEvent; //if the process waiting event, this is the number of the event
};

void Create(int n, int burstTime);
void waitProcess(int n, PCB &runningProcess);
void Destroy(int n);
void terminateFamily(PCB parent);
void Display();


vector<PCB> ready; //ready queue
vector<PCB> waiting;
vector<int> EventWaitList;
PCB runningProcess;
PCB PID0;

int main(){
    bool exit = false;
    int q = 3;
    PID0.PID = 0;
    PID0.burstTime = 1000000;

    runningProcess.quantumTime = 0;

    cout << "PID 0 running" << endl;
    Display();

    while(!exit){
        //if there is no process in ready queue, set PID 0 as running process

        //get the input from the user
        string input;
        getline(cin, input);
        //istringstream iss(input);
        vector<string> args;

        std::stringstream iss(input);

        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(args)
        );

        // for(int i=0; i < args.size(); i++){
        //     cout << i << " - " << args[i] << endl;
        // }


        if(args[0] == "C"){
            int n = stoi(args[1]);
            int burstTime = stoi(args[2]);
            Create(n, burstTime);
        } else if(args[0] == "D"){
            int n = stoi(args[1]);
            Destroy(n);
        } else if(args[0] == "I"){

        } else if(args[0] == "W"){

        } else if(args[0] == "E"){

        } else if(args[0] == "X"){
            exit = true;
        } else {
            cout << "Invalid input!" << endl;
        }

        if(runningProcess.burstTime == 0){
            //terminate
            //call Destroy(runningProcess.PID)
        }

        if(runningProcess.quantumTime == 0){
            if(runningProcess.PID != 0){
                ready.push_back(runningProcess); //add this process to ready que
                cout << "PID " << runningProcess.PID << " placed on Ready Queue" << endl;
            }

            if(!ready.empty()){
                runningProcess = ready[0];
                ready.erase(ready.begin());

            } else{
                runningProcess = PID0;
            }
            runningProcess.quantumTime = 3;

        }

        if(runningProcess.PID != 0) {
            cout << "PID " << runningProcess.PID << " " << runningProcess.burstTime
                 << " running with " << runningProcess.quantumTime << " left" << endl;
        }
        Display();

        if(runningProcess.quantumTime != 0){
            runningProcess.quantumTime--;
            runningProcess.burstTime--;
        }

    }

    return 0;
}

void Create(int n, int burstTime){

    //create a new process
    PCB process;
    //assign n to process ID
    process.PID = n;
    //assign burstTime to process burstTime
    process.burstTime = burstTime;
    //set the quntum time to 3 currently
    process.quantumTime = 3;
    //set the current running process to be the parent of the new process
    process.parent = &runningProcess;
    //set the new process as child to the running process
    runningProcess.child.push_back(process);
    //append the new process to ready queue
    ready.push_back(process);
    cout << "PID " << process.PID << " placed on Ready Queue" << endl;

}

void Destroy(int n){
    //find the process n
    PCB process;
    //is it the running process
    if(runningProcess.PID == n){
        process = runningProcess;
        terminateFamily(process);
        runningProcess = PID0; //make sure running process is destroyed ...
    }
    //is it in the waiting q
    for(int i=0; i < waiting.size(); i++){
        if(waiting[i].PID == n){
            terminateFamily(waiting[i]);
            waiting.erase(waiting.begin()+i);
        }
    }
    //is it in the ready q
    for(int i=0; i < ready.size(); i++){
        if(ready[i].PID == n){
            terminateFamily(ready[i]);
            ready.erase(ready.begin()+i);
        }
    }
    //delete the children of process n
    //process.child.clear();
    //delete the process n
}

void terminateFamily(PCB parent){

    cout << "PID " << parent.PID << " " << parent.burstTime << " terminated" << endl;

    for(int i=0; i < parent.child.size(); i++){
        //cout << "PID " << parent.child[i].PID << " " << parent.child[i].burstTime << " terminated" << endl;
        Destroy(parent.child[i].PID);
    }
    parent.child.clear(); //termniate all children
}

void waitProcess(int n, PCB &runningProcess){
    //move the runningProcess to wait que
    waiting.push_back(runningProcess);
    //set n to waitEvent
    runningProcess.waitingEvent = n;
    //add n to EventWait list
    EventWaitList.push_back(n);
    //set the first element on the ready que as the runningProcess
    runningProcess = ready[0];
}

// void Interrupt(){

// }

// void Event(){

// }

void Display(){
    cout << "Ready Queue: ";
    for(int i=0; i < ready.size(); i++){
        cout << "PID" << ready[i].PID << " " << ready[i].burstTime << " ";
    }
    cout << endl;
    cout << "Wait Queue: ";
    cout << endl;
}

