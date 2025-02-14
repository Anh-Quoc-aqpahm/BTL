#include "tay_nguyen_campaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

const int MAX_LINES = 5;         
const int MAX_LINE_LENGTH = 100;

// Task 0: Read input file
bool readFile(const string &filename, int LF1[], int LF2[], int &EXP1, int &EXP2, int &T1, int &T2, int &E) {
  char data[MAX_LINES][MAX_LINE_LENGTH];
  int numLines = 0;

  ifstream ifs(filename);
  if (!ifs.is_open()) {
      return false;
  }

  while (numLines < MAX_LINES && ifs.getline(data[numLines], MAX_LINE_LENGTH)) {
      numLines++;
  }
  ifs.close();

  if (numLines < MAX_LINES) {
      return false;
  }

  stringstream ss1(data[0]);
  char discard;
  for (int i = 0; i < 17; i++) {
      ss1 >> discard >> LF1[i];
  }

  stringstream ss2(data[1]);
  for (int i = 0; i < 17; i++) {
      ss2 >> discard >> LF2[i];
  }

  stringstream ss3(data[2]);
  ss3 >> EXP1 >> EXP2;

  stringstream ss4(data[3]);
  ss4 >> T1 >> T2;

  stringstream ss5(data[4]);
  ss5 >> E;

  return true;
}

// Task 1
int gatherForces(int LF1[], int LF2[]) {
  const int rankWeights[] = {1,  2,  3,  4,  5,  7,  8,  9, 10,
                              12, 15, 18, 20, 30, 40, 50, 70};
  const int numRanks = sizeof(rankWeights) / sizeof(rankWeights[0]);

  int totalForce1 = 0, totalForce2 = 0;

  for (int i = 0; i < numRanks; ++i) {
      totalForce1 += LF1[i] * rankWeights[i];
      totalForce2 += LF2[i] * rankWeights[i];
  }

  totalForce1 = (totalForce1 < 0) ? 0 : (totalForce1 > 1000 ? 1000 : totalForce1);
  totalForce2 = (totalForce2 < 0) ? 0 : (totalForce2 > 1000 ? 1000 : totalForce2);

  int LF = totalForce1 + totalForce2;

  return LF;
}

// Task 2
string determineRightTarget(const string &target) {
  int numbers[3], count = 0;

  for (size_t i = 0; i < target.size() && count < 3; i++) {
      if (isdigit(target[i])) {
          int num = target[i] - '0';
          while (i + 1 < target.size() && isdigit(target[i + 1])) {
              num = num * 10 + (target[++i] - '0');
          }
          if (num >= 0 && num <= 100) {
              numbers[count++] = num;
          }
      }
  }

  if (count == 0) return "INVALID";

  int id = 3; 
  if (count == 1) {
      id = numbers[0];
  } else if (count == 2) {
      id = (numbers[0] + numbers[1]) % 5 + 3;
  } else {

      int maxValue = numbers[0];
      if (numbers[1] > maxValue) maxValue = numbers[1];
      if (numbers[2] > maxValue) maxValue = numbers[2];

      id = (maxValue % 5) + 3;
  }

  const string MAIN_TARGETS[] = {
      "Buon Ma Thuot", "Duc Lap", "Dak Lak", "National Route 21", "National Route 14"
  };

  if (id >= 3 && id <= 7) return MAIN_TARGETS[id - 3];
  return "INVALID";
}

string decodeTarget(const string &message, int EXP1, int EXP2) {
  if (EXP1 < 0) EXP1 = 0;
  if (EXP1 > 600) EXP1 = 600;
  if (EXP2 < 0) EXP2 = 0;
  if (EXP2 > 600) EXP2 = 600;

  string decodedMessage;

  if (EXP1 >= 300 && EXP2 >= 300) {
      int shift = (EXP1 + EXP2) % 26;
      for (size_t i = 0; i < message.length(); i++) {
          char c = message[i];
          if (isalpha(c)) {
              char base = isupper(c) ? 'A' : 'a';
              decodedMessage += char((c - base - shift + 26) % 26 + base);
          } else {
              decodedMessage += c;
          }
      }
  } else {
      decodedMessage = string(message.rbegin(), message.rend());
  }

  bool capitalize = true;
  for (size_t i = 0; i < decodedMessage.length(); i++) {
      if (isalpha(decodedMessage[i])) {
          if (capitalize) {
              decodedMessage[i] = toupper(decodedMessage[i]);
              capitalize = false;
          } else {
              decodedMessage[i] = tolower(decodedMessage[i]);
          }
      } else {
          capitalize = true;
      }
  }

  const string MAIN_TARGETS[] = {
      "Buon Ma Thuot", "Duc Lap", "Dak Lak",
      "National Route 21", "National Route 14"
  };

  for (size_t i = 0; i < 5; i++) {
      if (decodedMessage == MAIN_TARGETS[i]) {
          return MAIN_TARGETS[i];
      }
  }

  return "INVALID";
}

// Task 3
void manageLogistics(int LF1, int LF2, int EXP1, int EXP2, int &T1, int &T2, int E) {
    
  double totalSupply = T1 + T2;

  double deltaT1 = (static_cast<double>(LF1) / (LF1 + LF2)) * totalSupply * (1 + (static_cast<double>(EXP1 - EXP2) / 100));
  int roundedDeltaT1 = static_cast<int>(ceil(deltaT1));

  if (roundedDeltaT1 < 0) roundedDeltaT1 = 0;
  if (roundedDeltaT1 > 3000) roundedDeltaT1 = 3000;

  int deltaT2 = static_cast<int>(ceil(totalSupply - roundedDeltaT1));

  if (E == 0) {

      T1 = roundedDeltaT1;
      T2 = deltaT2;
  } else if (E >= 1 && E <= 9) {
      double adjustedT1 = T1 * (1 - E * 0.01);
      double adjustedT2 = T2 * (1 - E * 0.005);
      T1 = ceil(adjustedT1);
      T2 = ceil(adjustedT2);
  } else if (E >= 10 && E <= 29) {
      T1 += E * 50;
      T2 += E * 50;
  } else if (E >= 30 && E <= 59) {
      double adjustedT1 = T1 * (1 + E * 0.005);
      double adjustedT2 = T2 * (1 + E * 0.002);
      T1 = ceil(adjustedT1);
      T2 = ceil(adjustedT2);
  }

  T1 = (T1 < 0) ? 0 : (T1 > 3000) ? 3000 : T1;
  T2 = (T2 < 0) ? 0 : (T2 > 3000) ? 3000 : T2;
}


// Task 4
int planAttack(int LF1, int LF2, int EXP1, int EXP2, int T1, int T2, int battleField[10][10]) {

  int S = (LF1 + LF2) + (EXP1 + EXP2) * 5 + (T1 + T2) * 2;

  for (int i = 0; i < 10; i++) {
      for (int j = 0; j < 10; j++) {
          double damage = battleField[i][j];
          if (i % 2 == 0) {
              damage *= 2.0/3.0;
          } else {
              damage *= 1.5;
          }
          S -= static_cast<int>(ceil(damage));
      }
  }
  return S;
}

// Task 5
int resupply(int shortfall, int supply[5][5]) {
  int minTotal = INT_MAX;

  for (int a = 0; a < 25; a++) {
      for (int b = a + 1; b < 25; b++) {
          for (int c = b + 1; c < 25; c++) {
              for (int d = c + 1; d < 25; d++) {
                  for (int e = d + 1; e < 25; e++) {

                      int values[5] = {
                          supply[a / 5][a % 5],
                          supply[b / 5][b % 5],
                          supply[c / 5][c % 5],
                          supply[d / 5][d % 5],
                          supply[e / 5][e % 5]
                      };

                      int total = 0;
                      for (int i = 0; i < 5; i++) {
                          total += values[i];
                      }

                      if (total >= shortfall && total < minTotal) {
                          minTotal = total;
                      }
                  }
              }
          }
      }
  }

  return minTotal;
}
////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
