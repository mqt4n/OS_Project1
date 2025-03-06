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
  int choice;
  filein >> choice;
  loadTable(filein, tableInfo, resourceName);
  if (choice == 1)
    FCFS(tableInfo, resourceName, argv[2]);
  else if (choice == 3)
    SJF(tableInfo, resourceName, argv[2]);

    return 0;
}