#include "func.cpp"

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
  vector<string> nameOfResource;
  int quantumTime;
  int choice;
  filein >> choice;
  if (choice == 2) filein >> quantumTime;
  loadTable(filein, tableInfo, resourceName, nameOfResource);
  if (choice == 1)
    FCFS(tableInfo, resourceName, nameOfResource, argv[2]);
  else if (choice == 2)
    RR(tableInfo, resourceName, nameOfResource, argv[2], quantumTime);
  else if (choice == 3)
    SJF(tableInfo, resourceName, nameOfResource, argv[2]);
  else if (choice == 4)
    SRTN(tableInfo, resourceName, nameOfResource, argv[2]);

  return 0;
}