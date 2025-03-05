#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void loadTable(ifstream &filein, vector<vector<int>> &tableInfo,
               vector<vector<string>> &resourceName) {
  int number_of_processes;
  filein >> number_of_processes;
  string c;
  getline(filein, c);  // remove endline after number_of_processes
  cout << number_of_processes << endl;

  for (int i = 0; i < number_of_processes; i++) {
    tableInfo.push_back(vector<int>(5));
    resourceName.push_back(vector<string>(2));
  }
  for (int i = 0; i < number_of_processes; i++) {
    int index_num = 0, index_name = 0;
    string line;
    getline(filein, line);
    if (line[line.size() - 1] == '\n') line.pop_back();
    stringstream ss(line);
    string token;
    while (ss >> token) {
      int pos = token.find('(');
      if (pos >= 0) {
        tableInfo[i][index_num++] = stoi(token.substr(0, pos));
        resourceName[i][index_name++] =
            token.substr(pos + 1, token.size() - pos - 2);
      } else
        tableInfo[i][index_num++] = stoi(token);
    }
  }
}

bool checkDone(vector<vector<int>> tableInfo) {
  for (int i = 0; i < tableInfo.size(); i++) {
    if (tableInfo[i][4] != 0) return false;
  }
  return true;
}

void FCFS(vector<vector<int>> tableInfo, vector<vector<string>> resourceName) {
  queue<int> readyQueue;
  queue<int> R1Queue;
  queue<int> R2Queue;
  vector<int> CPU;
  vector<int> R1;
  vector<int> R2;
  int time = 0;
  int currentCPU = -1;
  int currentR1 = -1;
  int currentR2 = -1;

  while (checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][1] == time) {
        readyQueue.push(i);
      }
    }
	if(currentCPU == -1){
		if(!readyQueue.empty()){
			currentCPU = readyQueue.front();
			readyQueue.pop();
		}
	}

	if(currentR1 == -1){
		if(!R1Queue.empty()){
			currentR1 = R1Queue.front();
			R1Queue.pop();
		}
	}
	if(currentR2 == -1){
		if(!R2Queue.empty()){
			currentR2 = R2Queue.front();
			R2Queue.pop();
		}
	}

    time++;
  }
}

int main(int argc, char *argv[]) {
  int number = argc;
  ifstream filein;
  filein.open(argv[1]);

  if (!filein) {
    cout << "File not found" << endl;
    return 0;
  }
  vector<vector<int>> tableInfo;
  vector<vector<string>> resourceName;
  int choice;
  filein >> choice;
  loadTable(filein, tableInfo, resourceName);
  for (int i = 0; i < tableInfo.size(); i++) {
    for (int j = 0; j < 5; j++) {
      cout << tableInfo[i][j] << " ";
    }
    for (int j = 0; j < 2; j++) {
      cout << resourceName[i][j] << " ";
    }
    cout << endl;
  }

  return 0;
}