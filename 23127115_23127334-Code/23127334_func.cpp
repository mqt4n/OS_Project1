#include "header_program.h"

vector<process> getData(string filename, int &Alg, int &TimeQuantum, string &a,
                          string &b) {
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
        if (a == "")
          a = tmp.substr(index + 1, (tmp.size() - 1) - index - 1);
        else if (b == "" &&
                 tmp.substr(index + 1, (tmp.size() - 1) - index - 1) != a) {
          b = tmp.substr(index + 1, (tmp.size() - 1) - index - 1);
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

bool existsInQueue(queue<int> q, int target) {
  while (!q.empty()) {
    if (q.front() + 1 == target) {
      return true;
    }
    q.pop();
  }
  return false;
}

bool existsInPriorityQueue(
    priority_queue<pair<process, int>, vector<pair<process, int>>, Compare> q,
    int target) {
  while (!q.empty()) {
    if (q.top().second == target) {
      return true;
    }
    q.pop();
  }
  return false;
}

bool isComplete(vector<process> data) {
  for (int i = 0; i < data.size(); i++) {
    if (data[i].cpu1 != 0 || data[i].cpu2 != 0 || data[i].r1.first != 0 || data[i].r2.first != 0) return 0;
  }
  return 1;
}

vector<dataTime> SRTN(vector<process> data, string &CPU_illustration,
                      string &R1_illustration, string &R2_illustration,
                      string a, string b) {
  priority_queue<pair<process, int> , vector<pair<process, int>>, Compare> ReadyQueueCPU;
  queue<pair<process, int>> IOQueue1;
  queue<pair<process, int>> IOQueue2;

  vector<dataTime> dt;
  for (int i = 0; i < data.size(); i++) {
    dt.push_back({0, 0});
  }

  int count_time = 1;
  int index_first_come = 0;
  for (int i = 1; i < data.size(); i++) {
    if (data[index_first_come].ArriveTime > data[i].ArriveTime) {
      index_first_come = i;
    }
  }

  for (int i = 0; i < data[index_first_come].ArriveTime; i++) {
    CPU_illustration += " _";
    R1_illustration += " _";
    R2_illustration += " _";
    count_time += 1;
  }
  
  // Real time
  vector<bool> trace(data.size(), false);
  trace[index_first_come] = 1;

  ReadyQueueCPU.push({data[index_first_come], index_first_come + 1});

  for (int i = 0; i < data.size(); i++) {
    if (data[index_first_come].ArriveTime == data[i].ArriveTime && index_first_come != i) {
      ReadyQueueCPU.push({data[i], i + 1});
      trace[i] = 1;
    }
  }

  pair<process, int> CPU_process;
  pair<process, int> R1_process;
  pair<process, int> R2_process;

  bool CPU_Operating = 0, IO_1_Operating = 1, IO_2_Operating = 1;

  while (!isComplete(data)) {
    bool timeOut = 0;
    int curValueCPU = 0;
    int len_time = 1;
    bool isOne = 1, prior = 0;

    CPU_process.second = 0;
    if (!ReadyQueueCPU.empty()) {
      CPU_process = ReadyQueueCPU.top();
      ReadyQueueCPU.pop();
      CPU_Operating = 0;
      if (CPU_process.first.cpu1 != 0) {
        isOne = 1;
        curValueCPU = CPU_process.first.cpu1;
      } else if (CPU_process.first.cpu2 != 0) {
        isOne = 0;
        curValueCPU = CPU_process.first.cpu2;
      }

    } else {
      CPU_Operating = 1;
    }

    for (int i = 0; i < dt.size(); i++) {
      if (existsInPriorityQueue(ReadyQueueCPU, i + 1) == 1 && trace[i] == 1) {
        dt[i].waiting_time += 1;
      }
    }

    // using time quantum for running scheduling algorithsm
    for (int j = 0; j < data.size(); j++) {
      if (data[j].ArriveTime == count_time && trace[j] == 0) {
        ReadyQueueCPU.push({data[j], j + 1});
        trace[j] = 1;
      }
    }

    // solving problem from CPU
    if (CPU_Operating == 0) {
      // CPU time 1 OR CPU2 time 2
      if (CPU_process.first.cpu1 != 0) {
        isOne = 1;
      } else if (CPU_process.first.cpu2 != 0) {
        isOne = 0;
      }

      // The process use cpu have done
      if (curValueCPU == 1) {
        CPU_illustration += (" " + to_string(CPU_process.second));

        if (isOne) {
          CPU_process.first.cpu1 = 0;
          data[CPU_process.second - 1].cpu1 = 0;
        } else {
          CPU_process.first.cpu2 = 0;
          data[CPU_process.second - 1].cpu2 = 0;
        }

        if (isOne) {
          if (CPU_process.first.r1.first != 0) {
            if (CPU_process.first.r1.second == a) {
              IOQueue1.push(CPU_process);
            }

            if (CPU_process.first.r1.second == b) {
              IOQueue2.push(CPU_process);
            }
          }
        } else {
          if (CPU_process.first.r2.first != 0) {
            if (CPU_process.first.r2.second == a) {
              IOQueue1.push(CPU_process);
            }

            if (CPU_process.first.r2.second == b) {
              IOQueue2.push(CPU_process);
            }
          }
        }
        if (CPU_process.first.cpu1 == 0 && CPU_process.first.cpu2 == 0 &&
            CPU_process.first.r1.first == 0 &&
            CPU_process.first.r2.first == 0) {
          dt[CPU_process.second - 1].turn_around_time =
              count_time - CPU_process.first.ArriveTime;
        }
        CPU_Operating = 1;

        // continue use CPU
      } else {
        CPU_illustration += (" " + to_string(CPU_process.second));
        if (isOne) {
          CPU_process.first.cpu1 -= 1;
          data[CPU_process.second - 1].cpu1 -= 1;
        } else {
          CPU_process.first.cpu2 -= 1;
          data[CPU_process.second - 1].cpu2 -= 1;
        }
        prior = 1;
      }

    } else {
      // if Ready queue is empty
      CPU_illustration += " _";
      timeOut = 1;
    }

    // SOLVING PROBLEM FROM RESOURCE 1
    if (IO_1_Operating == 0) {
      if (R1_process.first.r1.second == a && R1_process.first.r1.first != 0) {
        //  R1 have been almost empty
        if (R1_process.first.r1.first == 1) {
          R1_illustration += " " + to_string(R1_process.second);

          R1_process.first.r1.first = 0;
          data[R1_process.second - 1].r1.first = 0;

          if (R1_process.first.cpu2 != 0) {
            ReadyQueueCPU.push(R1_process);
          } else if (R1_process.first.r2.first != 0) {
            if (R1_process.first.r2.second == a) {
              IOQueue1.push(R1_process);
            } else
              IOQueue2.push(R1_process);
          }

          if (R1_process.first.cpu1 == 0 && R1_process.first.cpu2 == 0 &&
              R1_process.first.r1.first == 0 &&
              R1_process.first.r2.first == 0) {
            dt[R1_process.second - 1].turn_around_time =
                count_time - R1_process.first.ArriveTime;
          }

          // condition make sure resource queue is empty or not
          if (!IOQueue1.empty()) {
            R1_process = IOQueue1.front();
            IOQueue1.pop();
            IO_1_Operating = 0;
          } else {
            IO_1_Operating = 1;
          }

        } else if (R1_process.first.r1.first > 1) {
          R1_process.first.r1.first -= 1;
          data[R1_process.second - 1].r1.first -= 1;
          R1_illustration += " " + to_string(R1_process.second);
        }

      }

      else if (R1_process.first.r2.second == a &&
               R1_process.first.r2.first != 0) {
        if (R1_process.first.r2.first == 1) {
          R1_illustration += " " + to_string(R1_process.second);
          R1_process.first.r2.first = 0;
          data[R1_process.second - 1].r2.first = 0;

          if (R1_process.first.cpu1 == 0 && R1_process.first.cpu2 == 0 &&
              R1_process.first.r1.first == 0 &&
              R1_process.first.r2.first == 0) {
            dt[R1_process.second - 1].turn_around_time =
                count_time - R1_process.first.ArriveTime;
          }

          if (!IOQueue1.empty()) {
            R1_process = IOQueue1.front();
            IOQueue1.pop();
            IO_1_Operating = 0;
          } else {
            IO_1_Operating = 1;
          }

        } else if (R1_process.first.r2.first > 1) {
          R1_process.first.r2.first -= 1;
          data[R1_process.second - 1].r2.first -= 1;
          R1_illustration += " " + to_string(R1_process.second);
        }
      }
    }

    else if (IO_1_Operating == 1) {
      R1_illustration += " _";
      if (!IOQueue1.empty()) {
        R1_process = IOQueue1.front();
        IOQueue1.pop();
        IO_1_Operating = 0;
      } else {
        IO_1_Operating = 1;
      }
    }

    // Solving problem from resource 2
    if (IO_2_Operating == 0) {
      if (R2_process.first.r1.second == b && R2_process.first.r1.first != 0) {
        if (R2_process.first.r1.first == 1) {
          R2_illustration += " " + to_string(R2_process.second);

          R2_process.first.r1.first = 0;
          data[R2_process.second - 1].r1.first = 0;

          if (R2_process.first.cpu2 != 0) {
            ReadyQueueCPU.push(R2_process);
          } else if (R2_process.first.r2.first != 0) {
            IOQueue2.push(R2_process);
          }

          if (R2_process.first.cpu1 == 0 && R2_process.first.cpu2 == 0 &&
              R2_process.first.r1.first == 0 &&
              R2_process.first.r2.first == 0) {
            dt[R2_process.second - 1].turn_around_time =
                count_time - R2_process.first.ArriveTime;
          }

          if (!IOQueue2.empty()) {
            R2_process = IOQueue2.front();
            IOQueue2.pop();
            IO_2_Operating = 0;
          } else {
            IO_2_Operating = 1;
          }

        } else if (R2_process.first.r1.first > 1) {
          R2_process.first.r1.first -= 1;
          data[R2_process.second - 1].r1.first -= 1;

          R2_illustration += " " + to_string(R2_process.second);
        }
      }

      else if (R2_process.first.r2.second == b &&
               R2_process.first.r2.first != 0) {
        if (R2_process.first.r2.first == 1) {
          R2_illustration += " " + to_string(R2_process.second);
          R2_process.first.r2.first = 0;
          data[R2_process.second - 1].r2.first = 0;

          if (R2_process.first.cpu1 == 0 && R2_process.first.cpu2 == 0 &&
              R2_process.first.r1.first == 0 &&
              R2_process.first.r2.first == 0) {
            dt[R2_process.second - 1].turn_around_time =
                count_time - R2_process.first.ArriveTime;
          }

          if (!IOQueue2.empty()) {
            R2_process = IOQueue2.front();
            IOQueue2.pop();
            IO_2_Operating = 0;
          } else {
            IO_2_Operating = 1;
          }

        } else if (R2_process.first.r2.first > 1) {
          R2_process.first.r2.first -= 1;
          data[R2_process.second - 1].r2.first -= 1;
          R2_illustration += " " + to_string(R2_process.second);
        }
      }
    }

    else if (IO_2_Operating == 1) {
      R2_illustration += " _";
      if (!IOQueue2.empty()) {
        R2_process = IOQueue2.front();
        IOQueue2.pop();
        IO_2_Operating = 0;
      } else {
        IO_2_Operating = 1;
      }
    }

    if (prior == 1) {
      ReadyQueueCPU.push(CPU_process);
      prior = 0;
    }

    count_time += 1;
  }

  return dt;
}

vector<dataTime> RoundRobin(vector<process> data, int TimeQuantum,
                            string &CPU_illustration, string &R1_illustration,
                            string &R2_illustration, string a, string b) {
  queue<int> ReadyQueueCPU;
  queue<int> IOQueue1;
  queue<int> IOQueue2;

  vector<dataTime> dt;
  for (int i = 0; i < data.size(); i++) {
    dt.push_back({0, 0});
  }

  int count_time = 1; //Time to finish not to start (Mean: if process two have the time arrive == count time => push into queue have not run process yet) 

  int idx_fir_come = 0; 
  for (int i = 1; i < data.size(); i++) {
    if (data[i].ArriveTime < data[idx_fir_come].ArriveTime) {
      idx_fir_come = i;
    }
  }

  for (int i = 0; i < data[idx_fir_come].ArriveTime; i++) {
    CPU_illustration += " _";
    R1_illustration += " _";
    R2_illustration += " _";
    count_time += 1;
  }

  vector<bool> trace(data.size(), false);
  trace[idx_fir_come] = 1;

  ReadyQueueCPU.push(idx_fir_come);
  for (int i = 0; i < data.size(); i++) {
    if (data[idx_fir_come].ArriveTime == data[i].ArriveTime && idx_fir_come != i) {
      ReadyQueueCPU.push(i);
      trace[i] = 1;
    }
  }

  // Real time
  int CPU_Process_Index = 0, R1_Process_Index = 0, R2_Process_Index = 0;
  bool CPU_Operating = 0, IO_1_Operating = 1, IO_2_Operating = 1; 
  while (!isComplete(data)) {
    bool timeOut = 0;
    bool prior = 0;
    if (!ReadyQueueCPU.empty()) {
      CPU_Process_Index = ReadyQueueCPU.front();
      ReadyQueueCPU.pop();
      CPU_Operating = 0;
    } else {
      CPU_Operating = 1;
    }

    // using time quantum for running scheduling algorithsm
    for (int i = 0; i < TimeQuantum; i++) {
      for (int j = 0; j < dt.size(); j++) {
        if (existsInQueue(ReadyQueueCPU, j + 1) == 1) {
          dt[j].waiting_time += 1;
        }
      }

      for (int j = 0; j < data.size(); j++) {
        if (data[j].ArriveTime == count_time && trace[j] == 0) {
          ReadyQueueCPU.push(j);
          trace[j] = 1;
        }
      }

      // solving problem from CPU
      if (CPU_Operating == 0) {
        bool isOne = 1;
        int ConsumeCPU = 0;

        // CPU time 1 OR CPU2 time 2
        if (data[CPU_Process_Index].cpu1 != 0) {
          isOne = 1;
          ConsumeCPU = data[CPU_Process_Index].cpu1;
        } else if (data[CPU_Process_Index].cpu2 != 0) {
          isOne = 0;
          ConsumeCPU = data[CPU_Process_Index].cpu2;
        }

        // Time Quantum time out
        if (ConsumeCPU > 1 && i == TimeQuantum - 1) {
          if (isOne) {
            data[CPU_Process_Index].cpu1 -= 1;
          } else {
            data[CPU_Process_Index].cpu2 -= 1;
          }
          CPU_illustration += (" " + to_string(CPU_Process_Index + 1));
          // Make sure process have not used the cpu for the long time

          for (int j = 0; j < data.size(); j++) {
            if (data[j].ArriveTime == count_time && trace[j] == 0) {
              ReadyQueueCPU.push(j + 1);
              trace[j] = 1;
            }
          }

          prior = 1;
        }

        // The process use cpu have done
        else if (ConsumeCPU == 1) {
          CPU_illustration += (" " + to_string(CPU_Process_Index + 1));

          if (isOne) {
            data[CPU_Process_Index].cpu1 = 0;
          } else {
            data[CPU_Process_Index].cpu2 = 0;
          }

          if (isOne) {
            if (data[CPU_Process_Index].r1.first != 0) {
              if (data[CPU_Process_Index].r1.second == a) {
                IOQueue1.push(CPU_Process_Index);
              }

              if (data[CPU_Process_Index].r1.second == b) {
                IOQueue2.push(CPU_Process_Index);
              }
            }
          } else {
            if (data[CPU_Process_Index].r2.first != 0) {
              if (data[CPU_Process_Index].r2.second == a) {
                IOQueue1.push(CPU_Process_Index);
              }

              if (data[CPU_Process_Index].r2.second == b) {
                IOQueue2.push(CPU_Process_Index);
              }
            }
          }
          CPU_Operating = 1;

          // Calculate turn around time
          if (data[CPU_Process_Index].cpu1 == 0 && data[CPU_Process_Index].cpu2 == 0 &&
            data[CPU_Process_Index].r1.first == 0 &&
            data[CPU_Process_Index].r2.first == 0) {
            dt[CPU_Process_Index].turn_around_time =
                count_time - data[CPU_Process_Index].ArriveTime;
          }

          // Condition for make sure three sentences will be added in the same
          // time
          if (i < TimeQuantum - 1) {
            timeOut = 1;
          }

          // continue use CPU
        } else if (ConsumeCPU > 1) {
          CPU_illustration += (" " + to_string(CPU_Process_Index + 1));

          if (isOne) {
            data[CPU_Process_Index].cpu1 -= 1;
          } else {
            data[CPU_Process_Index].cpu2 -= 1;
          }
        }

      } else {
        // if Ready queue is empty
        CPU_illustration += " _";
        timeOut = 1;
      }

      // SOLVING PROBLEM FROM RESOURCE 1
      if (IO_1_Operating == 0) {
        // CHECK r1 or r2 (R time 1 or R time 2)
        if (data[R1_Process_Index].r1.second == a && data[R1_Process_Index].r1.first != 0) {
          //  R1 have been almost empty
          if (data[R1_Process_Index].r1.first == 1) {
            R1_illustration += " " + to_string(R1_Process_Index + 1);
            data[R1_Process_Index].r1.first = 0;
            if (data[R1_Process_Index].cpu2 != 0) {
              ReadyQueueCPU.push(R1_Process_Index);
            } else if (data[R1_Process_Index].r2.first != 0) {
              IOQueue2.push(R1_Process_Index);
            }

            if (data[R1_Process_Index].cpu1 == 0 && data[R1_Process_Index].cpu2 == 0 &&
                data[R1_Process_Index].r1.first == 0 &&
                data[R1_Process_Index].r2.first == 0) {
              dt[R1_Process_Index].turn_around_time =
                  count_time - data[R1_Process_Index].ArriveTime;
            }

            // condition make sure resource queue is empty or not
            if (!IOQueue1.empty()) {
              R1_Process_Index = IOQueue1.front();
              IOQueue1.pop();
              IO_1_Operating = 0;
            } else {
              IO_1_Operating = 1;
            }

          } else if (data[R1_Process_Index].r1.first > 1) {
            data[R1_Process_Index].r1.first -= 1;
            R1_illustration += " " + to_string(R1_Process_Index + 1);
          }

        }

        else if (data[R1_Process_Index].r2.second == a &&
            data[R1_Process_Index].r2.first != 0) {
          if (data[R1_Process_Index].r2.first == 1) {
            R1_illustration += " " + to_string(R1_Process_Index + 1);
            data[R1_Process_Index].r2.first = 0;

            if (!IOQueue1.empty()) {
              R1_Process_Index = IOQueue1.front();
              IOQueue1.pop();
              IO_1_Operating = 0;
            } else {
              IO_1_Operating = 1;
            }

            if (data[R1_Process_Index].cpu1 == 0 && data[R1_Process_Index].cpu2 == 0 &&
              data[R1_Process_Index].r1.first == 0 &&
              data[R1_Process_Index].r2.first == 0) {
              dt[R1_Process_Index].turn_around_time =
                  count_time - data[R1_Process_Index].ArriveTime;
            }

          } else if (data[R1_Process_Index].r2.first > 1) {
            data[R1_Process_Index].r2.first -= 1;
            R1_illustration += " " + to_string(R1_Process_Index + 1);
          }
        }
      }

      else if (IO_1_Operating == 1) {
        R1_illustration += " _";
        if (!IOQueue1.empty()) {
          R1_Process_Index = IOQueue1.front();
          IOQueue1.pop();
          IO_1_Operating = 0;
        } else {
          IO_1_Operating = 1;
        }
      }

      // Solving problem from resource 2
      if (IO_2_Operating == 0) {
        if (data[R2_Process_Index].r1.second == b && data[R2_Process_Index].r1.first != 0) {
          if (data[R2_Process_Index].r1.first == 1) {
            R2_illustration += " " + to_string(R2_Process_Index + 1);
            data[R2_Process_Index].r1.first = 0;
            if (data[R2_Process_Index].cpu2 != 0) {
              ReadyQueueCPU.push(R2_Process_Index);
            } else if (data[R2_Process_Index].r2.first != 0) {
              IOQueue2.push(R2_Process_Index);
            }

            if (data[R2_Process_Index].cpu1 == 0 && data[R2_Process_Index].cpu2 == 0 &&
              data[R2_Process_Index].r1.first == 0 &&
              data[R2_Process_Index].r2.first == 0) {
              dt[R2_Process_Index].turn_around_time =
                  count_time - data[R2_Process_Index].ArriveTime;
            }

            if (!IOQueue2.empty()) {
              R2_Process_Index = IOQueue2.front();
              IOQueue2.pop();
              IO_2_Operating = 0;
            } else {
              IO_2_Operating = 1;
            }

          } else if (data[R2_Process_Index].r1.first > 1) {
            data[R2_Process_Index].r1.first -= 1;
            R2_illustration += " " + to_string(R2_Process_Index + 1);
          }

        }

        else if (data[R2_Process_Index].r2.second == b &&
          data[R2_Process_Index].r2.first != 0) {
          if (data[R2_Process_Index].r2.first == 1) {
            R2_illustration += " " + to_string(R2_Process_Index + 1);
            data[R2_Process_Index].r2.first = 0;

            if (data[R2_Process_Index].cpu1 == 0 && data[R2_Process_Index].cpu2 == 0 &&
              data[R2_Process_Index].r1.first == 0 &&
              data[R2_Process_Index].r2.first == 0) {
              dt[R2_Process_Index].turn_around_time =
                  count_time - data[R2_Process_Index].ArriveTime;
            }

            if (!IOQueue2.empty()) {
              R2_Process_Index = IOQueue2.front();
              IOQueue2.pop();
              IO_2_Operating = 0;
            } else {
              IO_2_Operating = 1;
            }

          } else if (data[R2_Process_Index].r2.first > 1) {
            data[R2_Process_Index].r2.first -= 1;
            R2_illustration += " " + to_string(R2_Process_Index + 1);
          }
        }
      }

      else if (IO_2_Operating == 1) {
        R2_illustration += " _";
        if (!IOQueue2.empty()) {
          R2_Process_Index = IOQueue2.front();
          IOQueue2.pop();
          IO_2_Operating = 0;
        } else {
          IO_2_Operating = 1;
        }
      }
      if (prior == 1) {
        ReadyQueueCPU.push(CPU_Process_Index);
        prior = 0;
      }
      count_time += 1;
      if (timeOut == 1 or isComplete(data)) {
        break;
      }
    }
  }
  return dt;
}