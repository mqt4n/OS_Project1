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
      if (pos != string::npos) {
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
    for (int j = 1; j < 5; j++) {
      if (tableInfo[i][j] != 0) return false;
    }
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

  while (!checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time) {
        readyQueue.push(i);
      }
    }

    cout<<"\nTime: "<<time<<endl;
    cout<<"Before: \n";
    for (int i = 0; i < tableInfo.size(); i++) {
      for (int j = 1; j < 5; j++) {
        cout << tableInfo[i][j] << " ";
      }
      cout << endl;
    }
    for (int i = 0; i < CPU.size(); i++) {
      cout << CPU[i] + 1 << " ";
    }
    cout << endl;
    for (int i = 0; i < R1.size(); i++) {
      cout << R1[i] + 1 << " ";
    }
    cout << endl;
    for (int i = 0; i < R2.size(); i++) {
      cout << R2[i] + 1 << " ";
    }
    cout << endl;

    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = readyQueue.front();
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0) {
        CPU.push_back(currentCPU);
        tableInfo[currentCPU][1]--;
      } else if (tableInfo[currentCPU][2] > 0) {
        if (resourceName[currentCPU][0] == "R1") {
          R1Queue.push(currentCPU);
        } else {
          R2Queue.push(currentCPU);
        }
        currentCPU = -1;
        continue;
      } else if (tableInfo[currentCPU][3] > 0) {
        CPU.push_back(currentCPU);
        tableInfo[currentCPU][3]--;
      } else if (tableInfo[currentCPU][4] > 0) {
        if (resourceName[currentCPU][1] == "R1") {
          R1Queue.push(currentCPU);
        } else {
          R2Queue.push(currentCPU);
        }
        currentCPU = -1;
        continue;
      } else {
        currentCPU = -1;
      }
    } else {
      CPU.push_back(-1);
    }

    if (currentR1 == -1 && !R1Queue.empty()) {
      currentR1 = R1Queue.front();
      R1Queue.pop();
    }
    if (currentR1 != -1) {
      if (tableInfo[currentR1][2] > 0) {
        R1.push_back(currentR1);
        tableInfo[currentR1][2]--;
      } else if (tableInfo[currentR1][3] > 0) {
        readyQueue.push(currentR1);
        currentR1 = -1;
        continue;
      } else if (tableInfo[currentR1][4] > 0) {
        R1.push_back(currentR1);
        tableInfo[currentR1][4]--;
      } else {
        currentR1 = -1;
      }
    } else {
      R1.push_back(-1);
    }

    if (currentR2 == -1 && !R2Queue.empty()) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }

    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        R2.push_back(currentR2);
        tableInfo[currentR2][2]--;
      } else if (tableInfo[currentR2][3] > 0) {
        readyQueue.push(currentR2);
        currentR2 = -1;
        continue;
      } else if (tableInfo[currentR2][4] > 0) {
        R2.push_back(currentR2);
        tableInfo[currentR2][4]--;
      } else {
        currentR2 = -1;
      }
    } else {
      R2.push_back(-1);
    }

    cout<<"\nAfter: \n";

    for (int i = 0; i < tableInfo.size(); i++) {
      for (int j = 1; j < 5; j++) {
        cout << tableInfo[i][j] << " ";
      }
      cout << endl;
    }

    for (int i = 0; i < CPU.size(); i++) {
      cout << CPU[i] + 1 << " ";
    }
    cout << endl;
    for (int i = 0; i < R1.size(); i++) {
      cout << R1[i] + 1 << " ";
    }
    cout << endl;
    for (int i = 0; i < R2.size(); i++) {
      cout << R2[i] + 1 << " ";
    }
    cout << endl;
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
  if (choice == 1) {
    FCFS(tableInfo, resourceName);
  }

  return 0;
}