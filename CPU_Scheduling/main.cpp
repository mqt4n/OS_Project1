#include "func.cpp"

void main_1(int choice, ifstream& filein, char *namefileout) {
  vector<vector<int>> tableInfo;
  vector<vector<string>> resourceName;
  vector<string> nameOfResource;
  int quantumTime;

  if (choice == 2) filein >> quantumTime;
  loadTable(filein, tableInfo, resourceName, nameOfResource);
  if (choice == 1)
    FCFS(tableInfo, resourceName, nameOfResource, namefileout);
  else if (choice == 3)
    SJF(tableInfo, resourceName, nameOfResource, namefileout);
}

void main_2(int choice, char* filenamein, char* filenameout) {
  string fileNameIn = filenamein, fileNameOut = filenameout;
	vector<process> data; 
	vector<dataTime> res;
	int AlgorithmNumber = 0; 
	int TimeQuantum = 0; 

	string str1 = "", str2 = "";  
	data = getData(fileNameIn, AlgorithmNumber, TimeQuantum, str1, str2); 
	if (data.size() == 0) {
		return;
	}

	string CPU_illustration, R1_illustration, R2_illustration;
  if (choice == 2) {
    res = RoundRobin(data, TimeQuantum, CPU_illustration, R1_illustration, R2_illustration, str1, str2);
  } else if (choice == 4) {
    res = SRTN(data, CPU_illustration, R1_illustration, R2_illustration, str1, str2);
  }

  fstream fout;
  fout.open(fileNameOut, ios::out);
  if (!fout.is_open()) {
    cout << "can not open this file output" << endl; 
    return;
  }

  fout << CPU_illustration << "\n" << R1_illustration << "\n" << R2_illustration << "\n" ;

	for (int i = 0; i < res.size(); i ++) {
		fout << res[i].turn_around_time << " "; 
	}

	fout << endl; 

	for (int i = 0; i < res.size(); i++) {
		fout << res[i].waiting_time << " "; 
	}
}

int main(int argc, char *argv[]) {
  ifstream filein;
  filein.open(argv[1]);

  if(argc != 3) {
    cout << "Invalid number of arguments" << endl;
    return 0;
  }
  if (!filein) {
    cout << "File not found" << endl;
    return 0;
  }

  int choice;
  filein >> choice;
  if (choice == 1 || choice == 3) {
    main_1(choice, filein, argv[2]);
  } else if (choice == 2 || choice == 4) {
    main_2(choice, argv[1], argv[2]);
  }
  return 0;
}