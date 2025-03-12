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
  }
  write_to_file(CPU, R1, R2, TT, WT, namefileout);
}

struct process{
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

vector<process> getData(string filename, int& Alg, int& TimeQuantum, string& a, string& b) {
	fstream fin(filename, ios::in); 
	if (!fin) {
		cout << "Can open this file" << endl; 
		return {}; 
	}

	vector<process> infor; 

	int NumberProcess; 
	fin >> Alg;
	if (Alg == 2) {
		fin >> TimeQuantum >> NumberProcess; 
	} else {
		fin >> NumberProcess;
	}
	fin.ignore();
	for (int i = 0; i < NumberProcess; i++) {
		string line;  
		getline(fin, line);  
		stringstream ss(line); 
		process p; 

		ss >> p.ArriveTime >> p.cpu1; 
		string tmp; 
		while (ss >> tmp) { 
			if (tmp.find("(") != string::npos) {
				int index = tmp.find("("); 
				if (a == "") a = tmp.substr(index + 1, (tmp.size() - 1) - index - 1);
				else if (b == "" && tmp.substr(index + 1, (tmp.size() - 1) - index - 1) != a) {
					b = tmp.substr(index + 1, (tmp.size() - 1) - index - 1); 
					cout << "good morning" << endl;
				}
				
				if (p.r1.first == 0) {
					p.r1.first = stoi(tmp.substr(0, index + 1)); 
					p.r1.second = tmp.substr(index + 1, tmp.size() - 1 - index - 1);
				} else {
					p.r2.first = stoi(tmp.substr(0, index + 1)); 
					p.r2.second = tmp.substr(index + 1, tmp.size() - 1 - index - 1);
				}

			} else {
				p.cpu2 = stoi(tmp);  
			}
		}
		infor.push_back(p);
	}
	return infor; 
} 

struct Compare {
    bool operator()(const std::pair<process, int>& a, const std::pair<process, int>& b) {
		if (a.first.cpu1 > 0 && b.first.cpu1 > 0) {
			return a.first.cpu1 > b.first.cpu1; 
		} else if (a.first.cpu1 == 0 && a.first.cpu2 > 0 && b.first.cpu1 > 0) {
			return a.first.cpu2 > b.first.cpu1; 
		} else if (a.first.cpu1 > 0 && b.first.cpu1 == 0 && b.first.cpu2 > 0) {
			return a.first.cpu1 > b.first.cpu2; 
		} else {
			return a.first.cpu2 > b.first.cpu2; 
		}
    }
};

bool existsInQueue(queue<pair<process, int>> q, int target) {
    while (!q.empty()) {
        if (q.front().second == target) {
            return true;
        }
        q.pop();
    }
    return false;
}

bool existsInPriorityQueue(priority_queue<pair<process, int>, vector<pair<process, int >>, Compare> q, int target) {
    while (!q.empty()) {
        if (q.top().second == target) {
            return true;
        }
        q.pop();
    }
    return false;
}


vector<dataTime> SRTN(vector<process> data, string& CPU_illustration,string& R1_illustration,string& R2_illustration, string a, string b) {

	priority_queue<pair<process, int>, vector<pair<process, int>>, Compare> ReadyQueueCPU; 
	queue<pair<process, int>> ResourceQueue1; 
	queue<pair<process, int>> ResourceQueue2; 

	vector<dataTime> dt;
	for (int i = 0; i < data.size(); i++) {
		dt.push_back({0,0});
	}

	int count_time = 1;

	for (int i = 0; i < data[0].ArriveTime; i++) {
		CPU_illustration += " _"; 
		R1_illustration += " _";
		R2_illustration += " _"; 
		count_time += 1;
	}
	ReadyQueueCPU.push({data[0], 1});
	// Real time 
	vector<bool> trace = {0};
	trace[0] = 1;

	pair<process, int> CPU_process;
	pair<process, int> R1_process; 
	pair<process, int> R2_process; 

	bool CPU_CHECK_EMPTY = 0, R1_CHECK_EMPTY = 1, R2_CHECK_EMPTY = 1;

	while (CPU_CHECK_EMPTY == 0 || R1_CHECK_EMPTY == 0 || R2_CHECK_EMPTY == 0 || !ReadyQueueCPU.empty()) {
		bool timeOut = 0;
		int curValueCPU = 0;
		int len_time = 1;
		bool isOne = 1; 

		CPU_process.second = 0;
		if (!ReadyQueueCPU.empty()) {
			CPU_process = ReadyQueueCPU.top(); 
			ReadyQueueCPU.pop();
			CPU_CHECK_EMPTY = 0; 
			if (CPU_process.first.cpu1 != 0) {
				isOne = 1;
				curValueCPU = CPU_process.first.cpu1;
			} else if(CPU_process.first.cpu2 != 0) {
				isOne = 0;
				curValueCPU = CPU_process.first.cpu2;
			}
		
		} else {
			CPU_CHECK_EMPTY = 1; 
		}

		for (int i = 0; i < dt.size(); i++) {
			if (existsInPriorityQueue(ReadyQueueCPU, i + 1) == 1 && trace[i] == 1) {
				dt[i].waiting_time += 1;
			}
		}
		
		// using time quantum for running scheduling algorithsm
		for (int j = 1; j < data.size(); j++) {
			if (data[j].ArriveTime == count_time && trace[j] == 0) {
				ReadyQueueCPU.push({data[j], j + 1}); 
				trace[j] = 1; 
			}
		}
		
		// solving problem from CPU 
		if (CPU_CHECK_EMPTY == 0) {

			// CPU time 1 OR CPU2 time 2
			if (CPU_process.first.cpu1 != 0) {
				isOne = 1; 
			} else if(CPU_process.first.cpu2 != 0) {
				isOne = 0; 
			}

			// The process use cpu have done 
			if (curValueCPU == 1) {

				CPU_illustration += (" " + to_string(CPU_process.second)); 

				if (isOne) {
					CPU_process.first.cpu1 = 0;
				} else {
					CPU_process.first.cpu2 = 0; 
				}

				if (isOne) {
					if (CPU_process.first.r1.first != 0) {
						if (CPU_process.first.r1.second == a) {
							ResourceQueue1.push(CPU_process); 
						}

						if (CPU_process.first.r1.second == b) {
							ResourceQueue2.push(CPU_process); 
						}
					}
				} else {
					if (CPU_process.first.r2.first != 0) {
						if (CPU_process.first.r2.second == a) {
							ResourceQueue1.push(CPU_process); 
						}

						if (CPU_process.first.r2.second == b) {
							ResourceQueue2.push(CPU_process); 
						}
					}
				}
				if (CPU_process.first.cpu1 == 0 && CPU_process.first.cpu2 == 0 && CPU_process.first.r1.first == 0 && CPU_process.first.r2.first == 0) {
					dt[CPU_process.second - 1].turn_around_time = count_time - CPU_process.first.ArriveTime; 
				}
				CPU_CHECK_EMPTY = 1;

			// continue use CPU
			} else {
				CPU_illustration += (" " + to_string(CPU_process.second));
				if (isOne) {
					CPU_process.first.cpu1 -= 1; 
				} else {
					CPU_process.first.cpu2 -= 1; 
				}
				ReadyQueueCPU.push(CPU_process);
			}

		} else {
			// if Ready queue is empty 
			CPU_illustration += " _"; 
			timeOut = 1; 
		}

		
		// SOLVING PROBLEM FROM RESOURCE 1 
		if (R1_CHECK_EMPTY == 0) {
			if (R1_process.first.r1.second == a && R1_process.first.r1.first != 0) {
				//  R1 have been almost empty 
				if (R1_process.first.r1.first == 1) {
					
					R1_illustration += " " + to_string(R1_process.second);
					R1_process.first.r1.first = 0; 
					if (R1_process.first.cpu2 != 0) {	
						ReadyQueueCPU.push(R1_process); 
					} 
					else if (R1_process.first.r2.first != 0) {
						if (R1_process.first.r2.second == a) {
							ResourceQueue1.push(R1_process); 
						}
						else ResourceQueue2.push(R1_process); 
					}

					if (R1_process.first.cpu1 == 0 && R1_process.first.cpu2 == 0 && R1_process.first.r1.first == 0 && R1_process.first.r2.first == 0) {
						dt[R1_process.second - 1].turn_around_time = count_time - R1_process.first.ArriveTime; 
					}

					// condition make sure resource queue is empty or not 
					if (!ResourceQueue1.empty()) {
						R1_process = ResourceQueue1.front(); 
						ResourceQueue1.pop(); 
						R1_CHECK_EMPTY = 0; 
					} else {
						R1_CHECK_EMPTY = 1;
					} 

				} else if (R1_process.first.r1.first > 1) {
					R1_process.first.r1.first -= 1;
					R1_illustration += " " + to_string(R1_process.second);
				}

			}

			else if (R1_process.first.r2.second == a && R1_process.first.r2.first != 0) {
				
				if (R1_process.first.r2.first == 1) {
					R1_illustration += " " + to_string(R1_process.second);
					R1_process.first.r2.first = 0; 

					if (R1_process.first.cpu1 == 0 && R1_process.first.cpu2 == 0 && R1_process.first.r1.first == 0 && R1_process.first.r2.first == 0) {
						dt[R1_process.second - 1].turn_around_time = count_time - R1_process.first.ArriveTime; 
					}
					 
					if (!ResourceQueue1.empty()) {
						R1_process = ResourceQueue1.front(); 
						ResourceQueue1.pop(); 
						R1_CHECK_EMPTY = 0; 
					} else {
						R1_CHECK_EMPTY = 1;
					}

				} else if (R1_process.first.r2.first > 1) {
					R1_process.first.r2.first -= 1;
					R1_illustration += " " + to_string(R1_process.second);
				}
			}
		}

		else if (R1_CHECK_EMPTY == 1) {
			R1_illustration += " _";
			if (!ResourceQueue1.empty()) {
				R1_process = ResourceQueue1.front(); 
				ResourceQueue1.pop(); 
				R1_CHECK_EMPTY = 0; 
			} else {
				R1_CHECK_EMPTY = 1;
			}
		}

		// Solving problem from resource 2
		if (R2_CHECK_EMPTY == 0) {

			if (R2_process.first.r1.second == b && R2_process.first.r1.first != 0) {

				if (R2_process.first.r1.first == 1) {
					R2_illustration += " " + to_string(R2_process.second);
					R2_process.first.r1.first = 0; 
					if (R2_process.first.cpu2 != 0) {
						ReadyQueueCPU.push(R2_process); 
					} 
					else if (R2_process.first.r2.first != 0) {
						ResourceQueue2.push(R2_process); 
					}

					if (R2_process.first.cpu1 == 0 && R2_process.first.cpu2 == 0 && R2_process.first.r1.first == 0 && R2_process.first.r2.first == 0) {
						dt[R2_process.second - 1].turn_around_time = count_time - R2_process.first.ArriveTime; 
					}

					if (!ResourceQueue2.empty()) {
						R2_process = ResourceQueue2.front(); 
						ResourceQueue2.pop(); 
						R2_CHECK_EMPTY = 0; 
					} else {
						R2_CHECK_EMPTY = 1;
					} 

				} else if (R2_process.first.r1.first > 1) {
					R2_process.first.r1.first -= 1;
					R2_illustration += " " + to_string(R2_process.second);
				}
			}

			else if (R2_process.first.r2.second == b && R2_process.first.r2.first != 0) {
				if (R2_process.first.r2.first == 1) {
					R2_illustration += " " + to_string(R2_process.second);
					R2_process.first.r2.first = 0; 

					if (R2_process.first.cpu1 == 0 && R2_process.first.cpu2 == 0 && R2_process.first.r1.first == 0 && R2_process.first.r2.first == 0) {
						dt[R2_process.second - 1].turn_around_time = count_time - R2_process.first.ArriveTime; 
					}

					if (!ResourceQueue2.empty()) {
						R2_process = ResourceQueue2.front(); 
						ResourceQueue2.pop(); 
						R2_CHECK_EMPTY = 0; 
					} else {
						R2_CHECK_EMPTY = 1;
					} 

				} else if (R2_process.first.r2.first > 1) {
					R2_process.first.r2.first -= 1;
					R2_illustration += " " + to_string(R2_process.second);
				}

			}
		}

		else if (R2_CHECK_EMPTY == 1) {
			R2_illustration += " _";
			if (!ResourceQueue2.empty()) {
				R2_process = ResourceQueue2.front();
				ResourceQueue2.pop();
				R2_CHECK_EMPTY = 0;
			} else {
				R2_CHECK_EMPTY = 1;
			}
		}

		count_time += 1;

	}

	return dt; 
}


vector<dataTime> RoundRobin(vector<process> data, int TimeQuantum, string& CPU_illustration,string& R1_illustration,string& R2_illustration, string a, string b) {

	queue<pair<process, int>> ReadyQueueCPU; 
	queue<pair<process, int>> ResourceQueue1; 
	queue<pair<process, int>> ResourceQueue2; 

	vector<dataTime> dt; 
	for (int i = 0; i < data.size(); i++) {
		dt.push_back({0, 0});
	}

	int count_time = 1;
	for (int i = 0; i < data[0].ArriveTime; i++) {
		CPU_illustration += " -";
		R1_illustration += " -";
		R2_illustration += " -";  
		count_time += 1;
	}

	ReadyQueueCPU.push({data[0], 1});
	// Real time 
	vector<bool> trace = {0};
	trace[0] = 1;

	pair<process, int> CPU_process; 
	pair<process, int> R1_process; 
	pair<process, int> R2_process; 

	bool CPU_CHECK_EMPTY = 0, R1_CHECK_EMPTY = 1, R2_CHECK_EMPTY = 1;

	while (CPU_CHECK_EMPTY == 0 || R1_CHECK_EMPTY == 0 || R2_CHECK_EMPTY == 0 || !ReadyQueueCPU.empty()) {
		bool timeOut = 0;

		if (!ReadyQueueCPU.empty()) {
			CPU_process = ReadyQueueCPU.front(); 
			ReadyQueueCPU.pop();
			CPU_CHECK_EMPTY = 0; 
		} else {
			CPU_CHECK_EMPTY = 1; 
		}
		
		// using time quantum for running scheduling algorithsm
		for (int i = 0; i < TimeQuantum; i++) {
			for (int j = 0; j < dt.size(); j++) {
				if (existsInQueue(ReadyQueueCPU, j + 1) == 1) {
					dt[j].waiting_time += 1;
				}
			}

			for (int j = 1; j < data.size(); j++) {
				if (data[j].ArriveTime == count_time && trace[j] == 0) {
					ReadyQueueCPU.push({data[j], j + 1}); 
					trace[j] = 1; 
				}
			}
			
			// solving problem from CPU 
			if (CPU_CHECK_EMPTY == 0) {
				bool isOne = 1; 
				int ConsumeCPU = 0; 

				// CPU time 1 OR CPU2 time 2
				if (CPU_process.first.cpu1 != 0) {
					isOne = 1; 
					ConsumeCPU = CPU_process.first.cpu1; 
				} else if(CPU_process.first.cpu2 != 0) {
					isOne = 0; 
					ConsumeCPU = CPU_process.first.cpu2;
				}

				// Time Quantum time out
				if (ConsumeCPU > 1 && i == TimeQuantum - 1) {
					if (isOne) {
						CPU_process.first.cpu1 -= 1; 
					} else {
						CPU_process.first.cpu2 -= 1; 
					}
					CPU_illustration += (" " + to_string(CPU_process.second));
					// Make sure process have not used the cpu for the long time 
					
					for (int j = 1; j < data.size(); j++) {
						if (data[j].ArriveTime == count_time && trace[j] == 0) {
							ReadyQueueCPU.push({data[j], j + 1}); 
							trace[j] = 1;
						}
					}

					ReadyQueueCPU.push(CPU_process);
				}

				// The process use cpu have done 
				else if (ConsumeCPU == 1) {
	
					CPU_illustration += (" " + to_string(CPU_process.second)); 
	
					if (isOne) {
						CPU_process.first.cpu1 = 0;
					} else {
						CPU_process.first.cpu2 = 0; 
					}
	
					if (isOne) {
						if (CPU_process.first.r1.first != 0) {
							if (CPU_process.first.r1.second == a) {
								ResourceQueue1.push(CPU_process); 
							}
	
							if (CPU_process.first.r1.second == b) {
								ResourceQueue2.push(CPU_process); 
							}
						}
					} else {
						if (CPU_process.first.r2.first != 0) {
							if (CPU_process.first.r2.second == a) {
								ResourceQueue1.push(CPU_process); 
							}
	
							if (CPU_process.first.r2.second == b) {
								ResourceQueue2.push(CPU_process); 
							}
						}
					}
					CPU_CHECK_EMPTY = 1;

					// Calculate turn around time 
					if (CPU_process.first.cpu1 == 0 && CPU_process.first.cpu2 == 0 && CPU_process.first.r1.first == 0 && CPU_process.first.r2.first == 0) {
						dt[CPU_process.second - 1].turn_around_time = count_time - CPU_process.first.ArriveTime; 
					}

					// Condition for make sure three sentences will be added in the same time 
					if (i < TimeQuantum - 1) {
						timeOut = 1; 
					}


				// continue use CPU
				} else if (ConsumeCPU > 1) {
					CPU_illustration += (" " + to_string(CPU_process.second));

					if (isOne) {
						CPU_process.first.cpu1 -= 1; 
					} else {
						CPU_process.first.cpu2 -= 1; 
					}
				}

			} else {
				// if Ready queue is empty 
				CPU_illustration += " _"; 
				timeOut = 1; 
			}

			
			// SOLVING PROBLEM FROM RESOURCE 1 
			if (R1_CHECK_EMPTY == 0) {

				// CHECK r1 or r2 (R time 1 or R time 2) 
				if (R1_process.first.r1.second == a && R1_process.first.r1.first != 0) {
					//  R1 have been almost empty 
					if (R1_process.first.r1.first == 1) {
						R1_illustration += " " + to_string(R1_process.second);
						R1_process.first.r1.first = 0; 
						if (R1_process.first.cpu2 != 0) {
							ReadyQueueCPU.push(R1_process); 
						} 
						else if (R1_process.first.r2.first != 0) {
							ResourceQueue2.push(R1_process); 
						}

						if (R1_process.first.cpu1 == 0 && R1_process.first.cpu2 == 0 && R1_process.first.r1.first == 0 && R1_process.first.r2.first == 0) {
							dt[R1_process.second - 1].turn_around_time = count_time - R1_process.first.ArriveTime; 
						}

						// condition make sure resource queue is empty or not 
						if (!ResourceQueue1.empty()) {
							R1_process = ResourceQueue1.front(); 
							ResourceQueue1.pop(); 
							R1_CHECK_EMPTY = 0; 
						} else {
							R1_CHECK_EMPTY = 1;
						} 

					} else if (R1_process.first.r1.first > 1) {
						R1_process.first.r1.first -= 1;
						R1_illustration += " " + to_string(R1_process.second);
					}
	
				}

				else if (R1_process.first.r2.second == a && R1_process.first.r2.first != 0) {
					
					if (R1_process.first.r2.first == 1) {
						R1_illustration += " " + to_string(R1_process.second);
						R1_process.first.r2.first = 0; 
	
						if (!ResourceQueue1.empty()) {
							R1_process = ResourceQueue1.front(); 
							ResourceQueue1.pop(); 
							R1_CHECK_EMPTY = 0; 
						} else {
							R1_CHECK_EMPTY = 1;
						}

						if (R1_process.first.cpu1 == 0 && R1_process.first.cpu2 == 0 && R1_process.first.r1.first == 0 && R1_process.first.r2.first == 0) {
							dt[R1_process.second - 1].turn_around_time = count_time - R1_process.first.ArriveTime; 
						}
	
					} else if (R1_process.first.r2.first > 1) {
						R1_process.first.r2.first -= 1;
						R1_illustration += " " + to_string(R1_process.second);
					}
				}
			}

			else if (R1_CHECK_EMPTY == 1) {
				R1_illustration += " _";
				if (!ResourceQueue1.empty()) {
					R1_process = ResourceQueue1.front(); 
					ResourceQueue1.pop(); 
					R1_CHECK_EMPTY = 0; 
				} else {
					R1_CHECK_EMPTY = 1;
				}
			}

			// Solving problem from resource 2
			if (R2_CHECK_EMPTY == 0) {

				if (R2_process.first.r1.second == b && R2_process.first.r1.first != 0) {

					if (R2_process.first.r1.first == 1) {
						R2_illustration += " " + to_string(R2_process.second);
						R2_process.first.r1.first = 0; 
						if (R2_process.first.cpu2 != 0) {
							ReadyQueueCPU.push(R2_process); 
						} 
						else if (R2_process.first.r2.first != 0) {
							ResourceQueue2.push(R2_process); 
						}

						if (R2_process.first.cpu1 == 0 && R2_process.first.cpu2 == 0 && R2_process.first.r1.first == 0 && R2_process.first.r2.first == 0) {
							dt[R2_process.second - 1].turn_around_time = count_time - R2_process.first.ArriveTime; 
						}
	
						if (!ResourceQueue2.empty()) {
							R2_process = ResourceQueue2.front(); 
							ResourceQueue2.pop(); 
							R2_CHECK_EMPTY = 0; 
						} else {
							R2_CHECK_EMPTY = 1;
						} 
	
					} else if (R2_process.first.r1.first > 1) {
						R2_process.first.r1.first -= 1;
						R2_illustration += " " + to_string(R2_process.second);
					}
	
				}

				else if (R2_process.first.r2.second == b && R2_process.first.r2.first != 0) {
					if (R2_process.first.r2.first == 1) {
						R2_illustration += " " + to_string(R2_process.second);
						R2_process.first.r2.first = 0; 

						if (R2_process.first.cpu1 == 0 && R2_process.first.cpu2 == 0 && R2_process.first.r1.first == 0 && R2_process.first.r2.first == 0) {
							dt[R2_process.second - 1].turn_around_time = count_time - R2_process.first.ArriveTime; 
						}
	
	
						if (!ResourceQueue2.empty()) {
							R2_process = ResourceQueue2.front(); 
							ResourceQueue2.pop(); 
							R2_CHECK_EMPTY = 0; 
						} else {
							R2_CHECK_EMPTY = 1;
						} 
	
					} else if (R2_process.first.r2.first > 1) {
						R2_process.first.r2.first -= 1;
						R2_illustration += " " + to_string(R2_process.second);
					}
	
				}
			}

			else if (R2_CHECK_EMPTY == 1) {
				R2_illustration += " _";
				if (!ResourceQueue2.empty()) {
					R2_process = ResourceQueue2.front();
					ResourceQueue2.pop();
					R2_CHECK_EMPTY = 0;
				} else {
					R2_CHECK_EMPTY = 1;
				}
			}

			count_time += 1; 
			if (timeOut == 1 or (CPU_CHECK_EMPTY == 1 && R1_CHECK_EMPTY == 1 && R2_CHECK_EMPTY == 1)) {
				break; 
			}
		}	
	}
	return dt;
}