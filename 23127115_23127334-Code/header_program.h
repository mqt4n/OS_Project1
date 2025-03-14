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
bool pqueueContains(priority_queue<pair<int, pair<int, int>>> pq, int index);
bool checkUseResource(vector<int> &R);
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
		  
//23127334
struct process {
  int ArriveTime = 0;
  int cpu1 = 0;
  int cpu2 = 0;
  pair<int, string> r1 = {0, ""};
  pair<int, string> r2 = {0, ""};
};

struct dataTime {
  int turn_around_time;
  int waiting_time;
};

struct Compare {
  bool operator()(pair<process, pair<int, int>> &a, pair<process, pair<int, int>> &b) {
    if (a.first.cpu1 > 0 && b.first.cpu1 > 0) {
      if (a.first.cpu1 == b.first.cpu1) return a.second.second > b.second.second;
      return a.first.cpu1 > b.first.cpu1;
    } else if (a.first.cpu1 == 0 && a.first.cpu2 > 0 && b.first.cpu1 > 0) {
      if (a.first.cpu2 == b.first.cpu1) return a.second.second > b.second.second;
      return a.first.cpu2 > b.first.cpu1;
    } else if (a.first.cpu1 > 0 && b.first.cpu1 == 0 && b.first.cpu2 > 0) {
      if (a.first.cpu1 == b.first.cpu2) return a.second.second > b.second.second;
      return a.first.cpu1 > b.first.cpu2;
    } else {
      if (a.first.cpu2 == b.first.cpu2) return a.second.second > b.second.second;
      return a.first.cpu2 > b.first.cpu2;
    }
  }
};

vector<process> getData(string filename, int &Alg, int &TimeQuantum, string &a,
                        string &b);

bool existsInQueue(queue<pair<process, int>> q, int target);

bool existsInPriorityQueue(
    priority_queue<pair<process, int>, vector<pair<process, int>>, Compare> q,
    int target);

vector<dataTime> SRTN(vector<process> data, string &CPU_illustration,
                      string &R1_illustration, string &R2_illustration,
                      string a, string b);

vector<dataTime> RoundRobin(vector<process> data, int TimeQuantum,
                            string &CPU_illustration, string &R1_illustration,
                            string &R2_illustration, string a, string b);
