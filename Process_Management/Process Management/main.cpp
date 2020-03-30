#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>   //stoi
#include <sstream>  //used for parsing the input
#include <algorithm>//used for parsing the input
#include <iterator> //used for parsing the input
#include <fstream>
#include <string>

using namespace std;

struct PCB{
    PCB *parent;
    vector<PCB> child;
    int burstTime,
        quantumTime,
        PID;
    int waitingEvent = 0; //if the process waiting event, this is the number of the event
};

void Create(int n, int burstTime, ofstream &outFile, int q);
void waitProcess(int n, PCB &runningProcess, ofstream &outFile);
void Destroy(int n, ofstream &outFile);
void terminateFamily(PCB parent, ofstream &outFile);
void Display(ofstream &outFile);
void Event(int n, ofstream &outFile);


vector<PCB> ready; //ready queue
vector<PCB> waiting;
vector<int> EventWaitList;
PCB runningProcess;
PCB PID0;

int main(){
    bool Exit = false;
    PID0.PID = 0;
    PID0.burstTime = 1000000;
    PID0.quantumTime = 0;
    string input;



    ifstream inFile("input.txt");
    ofstream outFile("output.txt");


    if (!inFile)
    {
        cout << "ERROR: The input or output file could NOT openned!" << endl;
        exit(0);
    }


    getline(inFile, input);
    int q = stoi(input);

    runningProcess.quantumTime = 0;
    runningProcess = PID0;

    Display(outFile);

    while(!Exit){
        //if there is no process in ready queue, set PID 0 as running process

        //get the input from the user

        getline(inFile, input);
        outFile << input << endl;
        vector<string> args;

        std::stringstream iss(input);

        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(args)
        );

        if(args[0] == "C"){
            int n = stoi(args[1]);
            int burstTime = stoi(args[2]);
            Create(n, burstTime, outFile, q);
        } else if(args[0] == "D"){
            int n = stoi(args[1]);
            Destroy(n, outFile);
        } else if(args[0] == "I"){

        } else if(args[0] == "W"){
            int n = stoi(args[1]);
            waitProcess(n, runningProcess, outFile);

        } else if(args[0] == "X") {
            outFile << "Current state of simulation:" << endl;
            Exit = true;
            runningProcess.quantumTime++;
            runningProcess.burstTime++;
            Display(outFile);
            break;
        }
//        } else {
//            cout << "Invalid input!" << endl;
//        }

        if(runningProcess.burstTime == 0){
            //terminate
            Destroy(runningProcess.PID, outFile);
        }

        if(runningProcess.quantumTime == 0){
            if(runningProcess.PID != 0){
                ready.push_back(runningProcess); //add this process to ready que
                outFile << "PID " << runningProcess.PID << " "
                        << runningProcess.burstTime
                        << " placed on Ready Queue" << endl;
            }

            if(!ready.empty()){
                runningProcess = ready[0];
                ready.erase(ready.begin());

            } else{
                runningProcess = PID0;
            }
            if(runningProcess.PID != 0) {
                runningProcess.quantumTime = q;
            }

        }

        if(args[0] == "E"){
            int n = stoi(args[1]);
            for(int i=0; i < EventWaitList.size(); i++){
                if(EventWaitList[i] == n){
                    Event(n, outFile);
                }
            }
        }

        Display(outFile);

        if(runningProcess.quantumTime != 0){
            runningProcess.quantumTime--;
            runningProcess.burstTime--;
        }

    }

    inFile.close();
    outFile.close();
    return 0;
}

void Create(int n, int burstTime, ofstream &outFile, int q){

    //create a new process
    PCB process;
    //assign n to process ID
    process.PID = n;
    //assign burstTime to process burstTime
    process.burstTime = burstTime;
    //set the quntum time to 3 currently
    process.quantumTime = q;
    //set the current running process to be the parent of the new process
    process.parent = &runningProcess;
    //set the new process as child to the running process
    runningProcess.child.push_back(process);
    //append the new process to ready queue
    ready.push_back(process);
    outFile << "PID " << process.PID << " " << process.burstTime
            << " placed on Ready Queue" << endl;

}

void Destroy(int n, ofstream &outFile){
    //find the process n
    PCB process;
    //is it the running process
    if(runningProcess.PID == n){
        process = runningProcess;
        terminateFamily(process, outFile);
        runningProcess = PID0; //make sure running process is destroyed ...
    }
    //is it in the waiting q
    for(int i=0; i < waiting.size(); i++){
        if(waiting[i].PID == n){
            terminateFamily(waiting[i], outFile);
            waiting.erase(waiting.begin()+i);
        }
    }
    //is it in the ready q
    for(int i=0; i < ready.size(); i++){
        if(ready[i].PID == n){
            terminateFamily(ready[i], outFile);
            ready.erase(ready.begin()+i);
        }
    }
    //delete the children of process n
    //process.child.clear();
    //delete the process n
    runningProcess = PID0;
}

void terminateFamily(PCB parent, ofstream &outFile){

    outFile << "PID " << parent.PID << " " << parent.burstTime << " terminated" << endl;

    for(int i=0; i < parent.child.size(); i++){
        Destroy(parent.child[i].PID, outFile);
    }
    parent.child.clear(); //termniate all children
}

void waitProcess(int n, PCB &runningProcess, ofstream &outFile){
    //set n to waitEvent
    runningProcess.waitingEvent = n;
    //move the runningProcess to wait que
    waiting.push_back(runningProcess);

    //add n to EventWait list
    EventWaitList.push_back(n);
    //set PID0 as running process, if some process in ready it will be setted.
    outFile << "PID " << runningProcess.PID << " " << runningProcess.burstTime
            << " placed on Wait Queue" << endl;
    runningProcess = PID0;

}

void Event(int n, ofstream &outFile){
    //move the element with n waitingEvent from waiting que to ready que
    for(int i=0; waiting.size(); i++){
        if(waiting[i].waitingEvent == n){
            ready.push_back(waiting[i]);
            waiting.erase(waiting.begin()+i);
            outFile << "PID " << waiting[i].PID << " " << waiting[i].burstTime
                    << " placed on Ready Queue" << endl;
        }
    }
}

void Display(ofstream &outFile){
    if(runningProcess.PID != 0) {
        outFile << "PID " << runningProcess.PID << " " << runningProcess.burstTime
                << " running with " << runningProcess.quantumTime << " left" << endl;
    } else{
        outFile << "PID 0 running" << endl;
    }

    outFile << "Ready Queue: ";
    for(int i=0; i < ready.size(); i++){
        outFile << "PID " << ready[i].PID << " " << ready[i].burstTime << " ";
    }
    outFile << endl;
    outFile << "Wait Queue: ";
    for(int i=0; i < waiting.size(); i++){
        outFile << "PID " << waiting[i].PID << " " << waiting[i].burstTime
                << " " << waiting[i].waitingEvent;
    }
    outFile << endl;
}

