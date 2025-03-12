#pragma once
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;
//23127115
void loadTable(ifstream &filein, vector<vector<int>> &tableInfo,
               vector<vector<string>> &resourceName,
               vector<string> &nameOfResource);
void printProcessing(vector<vector<int>> &tableInfo, vector<int> &CPU,
                     vector<int> &R1, vector<int> &R2, int &time);
bool checkDone(vector<vector<int>> tableInfo);
bool stillInProcess(vector<vector<int>> tableInfo, int index);
bool queueContains(priority_queue<pair<int, int>> readyQueue, int index);
bool pqueueContains(priority_queue<pair<int, pair<int, int>>> pq, int index);
bool checkUseResource2(vector<int> &R2);
void write_to_file(vector<int> &CPU, vector<int> &R1, vector<int> &R2,
                   vector<int> &TT, vector<int> &WT, char *&namefileout);
bool checkQuantumTime(vector<int> &lastProccess, int quantumTime,
                      int currentCPU);
void FCFS(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
          vector<string> nameOfResource, char *namefileout);
void SJF(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
         vector<string> nameOfResource, char *namefileout);
void RR(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
        vector<string> nameOfResource, char *namefileout, int quantumTime);
void SRTN(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
          vector<string> nameOfResource, char *namefileout);
		  