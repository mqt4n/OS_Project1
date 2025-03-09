#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

void loadTable(ifstream &filein, vector<vector<int>> &tableInfo,
               vector<vector<string>> &resourceName,
               vector<string> &nameOfResource) {
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
        string temp = token.substr(pos + 1, token.size() - pos - 2);
        resourceName[i][index_name++] = temp;
        bool flag = false;
        for (int j = 0; j < nameOfResource.size(); j++)
          if (nameOfResource[j] == temp) flag = true;
        if (!flag) nameOfResource.push_back(temp);

      } else
        tableInfo[i][index_num++] = stoi(token);
    }
  }
}

bool checkDone(vector<vector<int>> tableInfo) {
  for (int i = 0; i < tableInfo.size(); i++)
    for (int j = 1; j < 5; j++)
      if (tableInfo[i][j]) return false;
  return true;
}

bool stillInProcess(vector<vector<int>> tableInfo, int index) {
  for (int i = 1; i < 5; i++)
    if (tableInfo[index][i]) return true;
  return false;
}

bool queueContains(queue<int> q, int index) {
  while (!q.empty()) {
    if (q.front() == index) return true;
    q.pop();
  }
  return false;
}

bool pqueueContains(priority_queue<pair<int, int>> pq, int index) {
  while (!pq.empty()) {
    if (pq.top().second == index) return true;
    pq.pop();
  }
  return false;
}

void write_to_file(vector<int> &CPU, vector<int> &R1, vector<int> &R2,
                   vector<int> &TT, vector<int> &WT, char *&namefileout) {
  ofstream fileout;
  fileout.open(namefileout);
  for (int i = 0; i < CPU.size(); i++) {
    if (CPU[i] + 1)
      fileout << CPU[i] + 1 << " ";
    else
      fileout << "_ ";
  }
  fileout << endl;
  for (int i = 0; i < R1.size(); i++) {
    if (R1[i] + 1)
      fileout << R1[i] + 1 << " ";
    else
      fileout << "_ ";
  }
  fileout << endl;
  for (int i = 0; i < R2.size(); i++) {
    if (R2[i] + 1)
      fileout << R2[i] + 1 << " ";
    else
      fileout << "_ ";
  }
  fileout << endl;
  for (int i = 0; i < TT.size(); i++) {
    fileout << TT[i] << " ";
  }
  fileout << endl;
  for (int i = 0; i < WT.size(); i++) {
    fileout << WT[i] << " ";
  }
  fileout.close();
}

bool checkQuantumTime(vector<int> &lastProccess, int quantumTime,
                      int currentCPU) {
  if (lastProccess.size() < quantumTime) return false;
  for (int i = 0; i < quantumTime; i++)
    if (lastProccess[lastProccess.size() - 1 - i] != currentCPU) return false;
  lastProccess.clear();
  return true;
}

void FCFS(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
          vector<string> nameOfResource, char *namefileout) {
  queue<int> readyQueue;
  queue<int> R1Queue;
  queue<int> R2Queue;
  vector<int> CPU;
  vector<int> R1;
  vector<int> R2;
  vector<int> TT(tableInfo.size(), 0);
  vector<int> WT(tableInfo.size(), 0);
  int time = 0;
  int currentCPU = -1;
  int currentR1 = -1;
  int currentR2 = -1;
  int tempR1 = -1;
  int tempR2 = -1;

  while (!checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time) readyQueue.push(i);
      if (stillInProcess(tableInfo, i) && tableInfo[i][0] <= time) TT[i]++;
    }

  CPU:
    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = readyQueue.front();
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0)
        tableInfo[currentCPU][1]--;
      else if (tableInfo[currentCPU][2] > 0) {
        if (resourceName[currentCPU][0] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else if (tableInfo[currentCPU][3] > 0)
        tableInfo[currentCPU][3]--;
      else if (tableInfo[currentCPU][4] > 0) {
        if (resourceName[currentCPU][1] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else {
        currentCPU = -1;
        goto CPU;
      }
    }

  R1:
    if (currentR1 == -1 && !R1Queue.empty()) {
      currentR1 = R1Queue.front();
      R1Queue.pop();
    }
    if (currentR1 != -1) {
      if (tableInfo[currentR1][2] > 0) {
        tableInfo[currentR1][2]--;
        if (!tableInfo[currentR1][2] && tableInfo[currentR1][3]) {
          readyQueue.push(currentR1);
          tempR1 = currentR1;
          currentR1 = -1;
        }
      } else if (tableInfo[currentR1][4] > 0)
        tableInfo[currentR1][4]--;
      else {
        currentR1 = -1;
        goto R1;
      }
    }

  R2:
    if (currentR2 == -1 && !R2Queue.empty()) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }
    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        tableInfo[currentR2][2]--;
        if (!tableInfo[currentR2][2] && tableInfo[currentR2][3]) {
          readyQueue.push(currentR2);
          tempR2 = currentR2;
          currentR2 = -1;
        }
      } else if (tableInfo[currentR2][4] > 0)
        tableInfo[currentR2][4]--;
      else {
        currentR2 = -1;
        goto R2;
      }
    }

    for (int i = 0; i < tableInfo.size(); i++)
      if (queueContains(readyQueue, i) && tableInfo[i][0] <= time &&
          currentCPU != i)
        WT[i]++;

    CPU.push_back(currentCPU);
    if (tempR1 != -1) {
      R1.push_back(tempR1);
      tempR1 = -1;
    } else
      R1.push_back(currentR1);
    if (tempR2 != -1) {
      R2.push_back(tempR2);
      tempR2 = -1;
    } else
      R2.push_back(currentR2);
    time++;
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}

void SJF(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
         vector<string> nameOfResource, char *namefileout) {
  priority_queue<pair<int, int>> readyQueue;
  queue<int> R1Queue;
  queue<int> R2Queue;
  vector<int> CPU;
  vector<int> R1;
  vector<int> R2;
  vector<int> TT(tableInfo.size(), 0);
  vector<int> WT(tableInfo.size(), 0);
  int time = 0;
  int currentCPU = -1;
  int currentR1 = -1;
  int currentR2 = -1;
  int tempR1 = -1;
  int tempR2 = -1;

  while (!checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time)
        readyQueue.push(make_pair(-tableInfo[i][1], i));
      if (stillInProcess(tableInfo, i) && tableInfo[i][0] <= time) TT[i]++;
    }

  CPU:
    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = readyQueue.top().second;
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0)
        tableInfo[currentCPU][1]--;
      else if (tableInfo[currentCPU][2] > 0) {
        if (resourceName[currentCPU][0] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else if (tableInfo[currentCPU][3] > 0)
        tableInfo[currentCPU][3]--;
      else if (tableInfo[currentCPU][4] > 0) {
        if (resourceName[currentCPU][1] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else {
        currentCPU = -1;
        goto CPU;
      }
    }

  R1:
    if (currentR1 == -1 && !R1Queue.empty()) {
      currentR1 = R1Queue.front();
      R1Queue.pop();
    }
    if (currentR1 != -1) {
      if (tableInfo[currentR1][2] > 0) {
        tableInfo[currentR1][2]--;
        if (!tableInfo[currentR1][2] && tableInfo[currentR1][3]) {
          readyQueue.push(make_pair(-tableInfo[currentR1][3], currentR1));
          tempR1 = currentR1;
          currentR1 = -1;
        }
      } else if (tableInfo[currentR1][4] > 0)
        tableInfo[currentR1][4]--;
      else {
        currentR1 = -1;
        goto R1;
      }
    }

  R2:
    if (currentR2 == -1 && !R2Queue.empty()) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }
    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        tableInfo[currentR2][2]--;
        if (!tableInfo[currentR2][2] && tableInfo[currentR2][3]) {
          readyQueue.push(make_pair(-tableInfo[currentR2][3], currentR2));
          tempR2 = currentR2;
          currentR2 = -1;
        }
      } else if (tableInfo[currentR2][4] > 0)
        tableInfo[currentR2][4]--;
      else {
        currentR2 = -1;
        goto R2;
      }
    }

    for (int i = 0; i < tableInfo.size(); i++)
      if (pqueueContains(readyQueue, i) && tableInfo[i][0] <= time &&
          currentCPU != i)
        WT[i]++;

    CPU.push_back(currentCPU);
    if (tempR1 != -1) {
      R1.push_back(tempR1);
      tempR1 = -1;
    } else
      R1.push_back(currentR1);
    if (tempR2 != -1) {
      R2.push_back(tempR2);
      tempR2 = -1;
    } else
      R2.push_back(currentR2);
    time++;
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}

void RR(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
        vector<string> nameOfResource, char *namefileout, int quantumTime) {
  queue<int> readyQueue;
  queue<int> R1Queue;
  queue<int> R2Queue;
  vector<int> CPU;
  vector<int> R1;
  vector<int> R2;
  vector<int> TT(tableInfo.size(), 0);
  vector<int> WT(tableInfo.size(), 0);
  int time = 0;
  int currentCPU = -1;
  int currentR1 = -1;
  int currentR2 = -1;
  int tempR1 = -1;
  int tempR2 = -1;
  vector<int> lastProccess;

  while (!checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time) readyQueue.push(i);
      if (stillInProcess(tableInfo, i) && tableInfo[i][0] <= time) TT[i]++;
    }

  CPU:
    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = readyQueue.front();
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0) {
        if (checkQuantumTime(lastProccess, quantumTime, currentCPU)) {
          readyQueue.push(currentCPU);
          currentCPU = -1;
          goto CPU;
        }
        tableInfo[currentCPU][1]--;
        lastProccess.push_back(currentCPU);
      } else if (tableInfo[currentCPU][2] > 0) {
        if (resourceName[currentCPU][0] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else if (tableInfo[currentCPU][3] > 0) {
        if (checkQuantumTime(lastProccess, quantumTime, currentCPU)) {
          readyQueue.push(currentCPU);
          currentCPU = -1;
          goto CPU;
        }
        tableInfo[currentCPU][3]--;
        lastProccess.push_back(currentCPU);
      } else if (tableInfo[currentCPU][4] > 0) {
        if (resourceName[currentCPU][1] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else {
        currentCPU = -1;
        goto CPU;
      }
    }

  R1:
    if (currentR1 == -1 && !R1Queue.empty()) {
      currentR1 = R1Queue.front();
      R1Queue.pop();
    }
    if (currentR1 != -1) {
      if (tableInfo[currentR1][2] > 0) {
        tableInfo[currentR1][2]--;
        if (!tableInfo[currentR1][2] && tableInfo[currentR1][3]) {
          readyQueue.push(currentR1);
          tempR1 = currentR1;
          currentR1 = -1;
        }
      } else if (tableInfo[currentR1][4] > 0)
        tableInfo[currentR1][4]--;
      else {
        currentR1 = -1;
        goto R1;
      }
    }

  R2:
    if (currentR2 == -1 && !R2Queue.empty()) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }
    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        tableInfo[currentR2][2]--;
        if (!tableInfo[currentR2][2] && tableInfo[currentR2][3]) {
          readyQueue.push(currentR2);
          tempR2 = currentR2;
          currentR2 = -1;
        }
      } else if (tableInfo[currentR2][4] > 0)
        tableInfo[currentR2][4]--;
      else {
        currentR2 = -1;
        goto R2;
      }
    }
    for (int i = 0; i < tableInfo.size(); i++)
      if (queueContains(readyQueue, i) && tableInfo[i][0] <= time &&
          currentCPU != i)
        WT[i]++;

    CPU.push_back(currentCPU);
    if (tempR1 != -1) {
      R1.push_back(tempR1);
      tempR1 = -1;
    } else
      R1.push_back(currentR1);
    if (tempR2 != -1) {
      R2.push_back(tempR2);
      tempR2 = -1;
    } else
      R2.push_back(currentR2);
    time++;
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}

void SRTN(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
          vector<string> nameOfResource, char *namefileout) {
  priority_queue<pair<int, int>> readyQueue;
  queue<int> R1Queue;
  queue<int> R2Queue;
  vector<int> CPU;
  vector<int> R1;
  vector<int> R2;
  vector<int> TT(tableInfo.size(), 0);
  vector<int> WT(tableInfo.size(), 0);
  int time = 0;
  int currentCPU = -1;
  int currentR1 = -1;
  int currentR2 = -1;
  int tempR1 = -1;
  int tempR2 = -1;

  while (!checkDone(tableInfo)) {

    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time)
        readyQueue.push(make_pair(-tableInfo[i][1], i));
      if (stillInProcess(tableInfo, i) && tableInfo[i][0] <= time) TT[i]++;
    }

  CPU:
    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = readyQueue.top().second;
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0) {
        tableInfo[currentCPU][1]--;
        readyQueue.push(make_pair(-tableInfo[currentCPU][1], currentCPU));
      } else if (tableInfo[currentCPU][2] > 0) {
        if (resourceName[currentCPU][0] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else if (tableInfo[currentCPU][3] > 0) {
        tableInfo[currentCPU][3]--;
        readyQueue.push(make_pair(-tableInfo[currentCPU][3], currentCPU));
      }

      else if (tableInfo[currentCPU][4] > 0) {
        if (resourceName[currentCPU][1] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else {
        currentCPU = -1;
        goto CPU;
      }
    }

  R1:
    if (currentR1 == -1 && !R1Queue.empty()) {
      currentR1 = R1Queue.front();
      R1Queue.pop();
    }
    if (currentR1 != -1) {
      if (tableInfo[currentR1][2] > 0) {
        tableInfo[currentR1][2]--;
        if (!tableInfo[currentR1][2] && tableInfo[currentR1][3]) {
          readyQueue.push(make_pair(-tableInfo[currentR1][3], currentR1));
          tempR1 = currentR1;
          currentR1 = -1;
        }
      } else if (tableInfo[currentR1][4] > 0)
        tableInfo[currentR1][4]--;
      else {
        currentR1 = -1;
        goto R1;
      }
    }

  R2:
    if (currentR2 == -1 && !R2Queue.empty()) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }
    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        tableInfo[currentR2][2]--;
        if (!tableInfo[currentR2][2] && tableInfo[currentR2][3]) {
          readyQueue.push(make_pair(-tableInfo[currentR2][3], currentR2));
          tempR2 = currentR2;
          currentR2 = -1;
        }
      } else if (tableInfo[currentR2][4] > 0)
        tableInfo[currentR2][4]--;
      else {
        currentR2 = -1;
        goto R2;
      }
    }

    for (int i = 0; i < tableInfo.size(); i++)
      if (pqueueContains(readyQueue, i) && tableInfo[i][0] <= time &&
          currentCPU != i)
        WT[i]++;

    CPU.push_back(currentCPU);
    if (tempR1 != -1) {
      R1.push_back(tempR1);
      tempR1 = -1;
    } else
      R1.push_back(currentR1);
    if (tempR2 != -1) {
      R2.push_back(tempR2);
      tempR2 = -1;
    } else
      R2.push_back(currentR2);
    currentCPU = -1;
    time++;
    // cout<<endl;
    // cout<<time-1<<endl;
    // for (int i = 0; i<tableInfo.size();i++){
    //   for (int j = 0; j<5;j++){
    //     cout << tableInfo[i][j] << " ";
    //   }
    //   cout << endl;
    // }

    // for(int i = 0; i<CPU.size();i++){
    //   cout << CPU[i]+1 << " ";
    // }
    // cout << endl;
    // for(int i = 0; i<R1.size();i++){
    //   cout << R1[i]+1 << " ";
    // }
    // cout << endl;
    // for(int i = 0; i<R2.size();i++){
    //   cout << R2[i]+1 << " ";
    // }
    // cout << endl;
    // for(int i = 0; i<TT.size();i++){
    //   cout << TT[i] << " ";
    // }
    // cout << endl;
    // for(int i = 0; i<WT.size();i++){
    //   cout << WT[i] << " ";
    // }
    // cout << endl;
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}