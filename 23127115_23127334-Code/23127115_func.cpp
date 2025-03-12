#include "header_program.h"

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

void printProcessing(vector<vector<int>> &tableInfo, vector<int> &CPU,
                     vector<int> &R1, vector<int> &R2, int &time) {
  cout << "\nTime: " << time << endl;
  for (int i = 0; i < tableInfo.size(); i++) {
    for (int j = 0; j < 5; j++) {
      cout << tableInfo[i][j] << " ";
    }
    cout << endl;
  }
  cout << "CPU: ";
  for (int i = 0; i < CPU.size(); i++) cout << CPU[i] + 1 << " ";
  cout << endl;
  cout << "R1 : ";
  for (int i = 0; i < R1.size(); i++) cout << R1[i] + 1 << " ";
  cout << endl;
  cout << "R2 : ";
  for (int i = 0; i < R2.size(); i++) cout << R2[i] + 1 << " ";
  cout << endl;
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

bool pqueueContains(priority_queue<pair<int, pair<int, int>>> pq, int index) {
  while (!pq.empty()) {
    if (pq.top().second.second == -index) return true;
    pq.pop();
  }
  return false;
}

bool checkUseResource2(vector<int> &R2) {
  for (int i = 0; i < R2.size(); i++)
    if (R2[i] + 1) return true;
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
  if (checkUseResource2(R2)) {
    for (int i = 0; i < R2.size(); i++) {
      if (R2[i] + 1)
        fileout << R2[i] + 1 << " ";
      else
        fileout << "_ ";
    }
    fileout << endl;
  }
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
  priority_queue<pair<int, pair<int, int>>> readyQueue;
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
  bool noCPU = false;
  vector<int> lastUseCPU(tableInfo.size(), 1);

  while (!checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time)
        readyQueue.push(make_pair(-time, make_pair(lastUseCPU[i], -i)));
      if (stillInProcess(tableInfo, i) && tableInfo[i][0] <= time) TT[i]++;
    }

  CPU:
    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = -readyQueue.top().second.second;
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0)
        tableInfo[currentCPU][1]--;
      else if (tableInfo[currentCPU][2] > 0) {
        lastUseCPU[currentCPU] = time - 1;
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
        if (!tableInfo[currentR1][2]) {
          if (tableInfo[currentR1][3])
            readyQueue.push(make_pair(
                -(time + 1), make_pair(-lastUseCPU[currentR1], -currentR1)));
          else if (tableInfo[currentR1][4]) {
            if (resourceName[currentR1][1] == nameOfResource[0])
              R1Queue.push(currentR1);
            else
              R2Queue.push(currentR1);
            noCPU = true;
          }
          tempR1 = currentR1;
          currentR1 = -1;
        }
      } else if (tableInfo[currentR1][4] > 0) {
        tableInfo[currentR1][4]--;
        if (!tableInfo[currentR1][4]) {
          tempR1 = currentR1;
          currentR1 = -1;
        }
      }
    }

  R2:
    if (currentR2 == -1 && !R2Queue.empty() && !noCPU) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }
    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        tableInfo[currentR2][2]--;
        if (!tableInfo[currentR2][2]) {
          if (tableInfo[currentR2][3])
            readyQueue.push(make_pair(
                -(time + 1), make_pair(-lastUseCPU[currentR2], -currentR2)));
          else if (tableInfo[currentR2][4]) {
            if (resourceName[currentR2][1] == nameOfResource[0])
              R1Queue.push(currentR2);
            else
              R2Queue.push(currentR2);
          }
          tempR2 = currentR2;
          currentR2 = -1;
        }
      } else if (tableInfo[currentR2][4] > 0) {
        tableInfo[currentR2][4]--;
        if (!tableInfo[currentR2][4]) {
          tempR2 = currentR2;
          currentR2 = -1;
        }
      }
    }

    for (int i = 0; i < tableInfo.size(); i++)
      if (pqueueContains(readyQueue, i) && tableInfo[i][0] <= time &&
          currentCPU != i)
        if (tempR1 != i && tempR2 != i) WT[i]++;

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
    noCPU = false;
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}

void SJF(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
         vector<string> nameOfResource, char *namefileout) {
  priority_queue<pair<int, pair<int, int>>> readyQueue;
  queue<int> R1Queue;
  queue<int> R2Queue;
  vector<int> CPU;
  vector<int> R1;
  vector<int> R2;
  vector<int> TT(tableInfo.size(), 0);
  vector<int> WT(tableInfo.size(), 0);
  vector<int> lastUseCPU(tableInfo.size(), 1);
  int time = 0;
  int currentCPU = -1;
  int currentR1 = -1;
  int currentR2 = -1;
  int tempCPU = -1;
  int tempR1 = -1;
  int tempR2 = -1;
  bool noCPU = false;

  while (!checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time)
        readyQueue.push(
            make_pair(-tableInfo[i][1], make_pair(lastUseCPU[i], -i)));
      if (stillInProcess(tableInfo, i) && tableInfo[i][0] <= time) TT[i]++;
    }

  CPU:
    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = -readyQueue.top().second.second;
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0)
        tableInfo[currentCPU][1]--;
      else if (tableInfo[currentCPU][2] > 0) {
        lastUseCPU[currentCPU] = time - 1;
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
        if (!tableInfo[currentR1][2]) {
          if (tableInfo[currentR1][3])
            readyQueue.push(
                make_pair(-tableInfo[currentR1][3],
                          make_pair(-lastUseCPU[currentR1], -currentR1)));
          else if (tableInfo[currentR1][4]) {
            if (resourceName[currentR1][1] == nameOfResource[0])
              R1Queue.push(currentR1);
            else
              R2Queue.push(currentR1);
            noCPU = true;
          }
          tempR1 = currentR1;
          currentR1 = -1;
        }
      } else if (tableInfo[currentR1][4] > 0) {
        tableInfo[currentR1][4]--;
        if (!tableInfo[currentR1][4]) {
          tempR1 = currentR1;
          currentR1 = -1;
        }
      }
    }

  R2:
    if (currentR2 == -1 && !R2Queue.empty() && !noCPU) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }
    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        tableInfo[currentR2][2]--;
        if (!tableInfo[currentR2][2]) {
          if (tableInfo[currentR2][3])
            readyQueue.push(
                make_pair(-tableInfo[currentR2][3],
                          make_pair(-lastUseCPU[currentR2], -currentR2)));
          else if (tableInfo[currentR2][4]) {
            if (resourceName[currentR2][1] == nameOfResource[0])
              R1Queue.push(currentR2);
            else
              R2Queue.push(currentR2);
          }
          tempR2 = currentR2;
          currentR2 = -1;
        }
      } else if (tableInfo[currentR2][4] > 0) {
        tableInfo[currentR2][4]--;
        if (!tableInfo[currentR2][4]) {
          tempR2 = currentR2;
          currentR2 = -1;
        }
      }
    }

    for (int i = 0; i < tableInfo.size(); i++)
      if (pqueueContains(readyQueue, i) && tableInfo[i][0] <= time &&
          currentCPU != i)
        if (tempR1 != i && tempR2 != i) WT[i]++;

    if (tempCPU != -1) {
      CPU.push_back(tempCPU);
      tempCPU = -1;
    } else
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
    noCPU = false;
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}

void RR(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
        vector<string> nameOfResource, char *namefileout, int quantumTime) {
  priority_queue<pair<int, pair<int, int>>> readyQueue;
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
  bool noCPU = false;
  vector<int> lastProccess;
  vector<int> lastUseCPU(tableInfo.size(), 1);

  while (!checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time)
        readyQueue.push(make_pair(-time, make_pair(lastUseCPU[i], -i)));
      if (stillInProcess(tableInfo, i) && tableInfo[i][0] <= time) TT[i]++;
    }

  CPU:
    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = -readyQueue.top().second.second;
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0) {
        if (checkQuantumTime(lastProccess, quantumTime, currentCPU)) {
          lastUseCPU[currentCPU] = time - 1;
          readyQueue.push(make_pair(
              -time, make_pair(-lastUseCPU[currentCPU], -currentCPU)));
          currentCPU = -1;
          goto CPU;
        }
        tableInfo[currentCPU][1]--;
        lastProccess.push_back(currentCPU);
      } else if (tableInfo[currentCPU][2] > 0) {
        lastUseCPU[currentCPU] = time - 1;
        if (resourceName[currentCPU][0] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else if (tableInfo[currentCPU][3] > 0) {
        if (checkQuantumTime(lastProccess, quantumTime, currentCPU)) {
          lastUseCPU[currentCPU] = time - 1;
          readyQueue.push(make_pair(
              -time, make_pair(-lastUseCPU[currentCPU], -currentCPU)));
          currentCPU = -1;
          goto CPU;
        }
        tableInfo[currentCPU][3]--;
        lastProccess.push_back(currentCPU);
      } else if (tableInfo[currentCPU][4] > 0) {
        lastUseCPU[currentCPU] = time - 1;
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
        if (!tableInfo[currentR1][2]) {
          if (tableInfo[currentR1][3])
            readyQueue.push(make_pair(
                -(time + 1), make_pair(-lastUseCPU[currentR1], -currentR1)));
          else if (tableInfo[currentR1][4]) {
            if (resourceName[currentR1][1] == nameOfResource[0])
              R1Queue.push(currentR1);
            else
              R2Queue.push(currentR1);
            noCPU = true;
          }
          tempR1 = currentR1;
          currentR1 = -1;
        }
      } else if (tableInfo[currentR1][4] > 0) {
        tableInfo[currentR1][4]--;
        if (!tableInfo[currentR1][4]) {
          tempR1 = currentR1;
          currentR1 = -1;
        }
      }
    }

  R2:
    if (currentR2 == -1 && !R2Queue.empty() && !noCPU) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }
    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        tableInfo[currentR2][2]--;
        if (!tableInfo[currentR2][2]) {
          if (tableInfo[currentR2][3])
            readyQueue.push(make_pair(
                -(time + 1), make_pair(-lastUseCPU[currentR2], -currentR2)));

          else if (tableInfo[currentR2][4]) {
            if (resourceName[currentR2][1] == nameOfResource[0])
              R1Queue.push(currentR2);
            else
              R2Queue.push(currentR2);
          }
          tempR2 = currentR2;
          currentR2 = -1;
        }
      } else if (tableInfo[currentR2][4] > 0) {
        tableInfo[currentR2][4]--;
        if (!tableInfo[currentR2][4]) {
          tempR2 = currentR2;
          currentR2 = -1;
        }
      }
    }

    for (int i = 0; i < tableInfo.size(); i++)
      if (pqueueContains(readyQueue, i) && tableInfo[i][0] <= time &&
          currentCPU != i)
        if (tempR1 != i && tempR2 != i) WT[i]++;

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
    noCPU = false;
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}

void SRTN(vector<vector<int>> tableInfo, vector<vector<string>> resourceName,
          vector<string> nameOfResource, char *namefileout) {
  priority_queue<pair<int, pair<int, int>>> readyQueue;
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
  bool noCPU = false;
  vector<int> lastUseCPU(tableInfo.size(), 1);

  while (!checkDone(tableInfo)) {
    for (int i = 0; i < tableInfo.size(); i++) {
      if (tableInfo[i][0] == time)
        readyQueue.push(
            make_pair(-tableInfo[i][1], make_pair(lastUseCPU[i], -i)));
      if (stillInProcess(tableInfo, i) && tableInfo[i][0] <= time) TT[i]++;
    }

  CPU:
    if (currentCPU == -1 && !readyQueue.empty()) {
      currentCPU = readyQueue.top().second.second;
      readyQueue.pop();
    }
    if (currentCPU != -1) {
      if (tableInfo[currentCPU][1] > 0) {
        tableInfo[currentCPU][1]--;
        lastUseCPU[currentCPU] = time;
        readyQueue.push(
            make_pair(-tableInfo[currentCPU][1],
                      make_pair(-lastUseCPU[currentCPU], -currentCPU)));
      } else if (tableInfo[currentCPU][2] > 0) {
        lastUseCPU[currentCPU] = time - 1;
        if (resourceName[currentCPU][0] == nameOfResource[0])
          R1Queue.push(currentCPU);
        else
          R2Queue.push(currentCPU);
        currentCPU = -1;
        goto CPU;
      } else if (tableInfo[currentCPU][3] > 0) {
        tableInfo[currentCPU][3]--;
        lastUseCPU[currentCPU] = time;
        readyQueue.push(
            make_pair(-tableInfo[currentCPU][3],
                      make_pair(-lastUseCPU[currentCPU], -currentCPU)));
      }

      else if (tableInfo[currentCPU][4] > 0) {
        lastUseCPU[currentCPU] = time - 1;
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
        if (!tableInfo[currentR1][2]) {
          if (tableInfo[currentR1][3])
            readyQueue.push(
                make_pair(-tableInfo[currentR1][3],
                          make_pair(-lastUseCPU[currentR1], -currentR1)));
          else if (tableInfo[currentR1][4]) {
            if (resourceName[currentR1][1] == nameOfResource[0])
              R1Queue.push(currentR1);
            else
              R2Queue.push(currentR1);
            noCPU = true;
          }
          tempR1 = currentR1;
          currentR1 = -1;
        }
      } else if (tableInfo[currentR1][4] > 0) {
        tableInfo[currentR1][4]--;
        if (!tableInfo[currentR1][4]) {
          tempR1 = currentR1;
          currentR1 = -1;
        }
      }
    }

  R2:
    if (currentR2 == -1 && !R2Queue.empty() && !noCPU) {
      currentR2 = R2Queue.front();
      R2Queue.pop();
    }
    if (currentR2 != -1) {
      if (tableInfo[currentR2][2] > 0) {
        tableInfo[currentR2][2]--;
        if (!tableInfo[currentR2][2] && tableInfo[currentR2][3]) {
          readyQueue.push(
              make_pair(-tableInfo[currentR2][3],
                        make_pair(-lastUseCPU[currentR2], -currentR2)));
          tempR2 = currentR2;
          currentR2 = -1;
        }
      } else if (tableInfo[currentR2][4] > 0) {
        tableInfo[currentR2][4]--;
        if (!tableInfo[currentR2][4]) {
          tempR2 = currentR2;
          currentR2 = -1;
        }
      }
    }

    for (int i = 0; i < tableInfo.size(); i++)
      if (pqueueContains(readyQueue, i) && tableInfo[i][0] <= time &&
          currentCPU != i)
        if (tempR1 != i && tempR2 != i) WT[i]++;

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
    noCPU = false;
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}
