#include <bits/stdc++.h>

using namespace std;

const int maxLen = 15;
const string banner =
"888b    888                                      888 888          \n\
8888b   888                                      888 888          \n\
88888b  888                                      888 888          \n\
888Y88b 888  8888b.  88888b.d88b.   .d88b.   .d88888 888  .d88b.  \n\
888 Y88b888     \"88b 888 \"888 \"88b d8P  Y8b d88\" 888 888 d8P  Y8b \n\
888  Y88888 .d888888 888  888  888 88888888 888  888 888 88888888\n\
888   Y8888 888  888 888  888  888 Y8b.     Y88b 888 888 Y8b.     \n\
888    Y888 \"Y888888 888  888  888  \"Y8888   \"Y88888 888  \"Y8888  \n";

vector<set<string>> names(maxLen+1);

string lower(string s) {
  string res = "";
  for (char c : s) {
    res += tolower(c);
  }

  return res;
}

void initNames() {
  for (int year = 1880; year <= 2023; year++) {
    ifstream input("names/yob"+to_string(year)+".txt");

    string line;
    while (getline(input, line)) {
      string name = "";
      for (int i = 0; i < line.size(); i++) {
        if (line[i] == ',') {
          break;
        }
        name += line[i];
      }
      names[name.size()].insert(lower(name));
    }
    input.close();
  }
}

bool isNumber(string ans) {
  for (char c : ans) {
    if (!isdigit(c)) return false;
  }
  return true;
}

int isBool(string ans) {
  if (lower(ans) == "yes" || lower(ans) == "y") {
    return 1;
  } else if (lower(ans) == "no" || lower(ans) == "n") {
    return 0;
  }
  return -1;
}

string generateRandomName(int len) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> distr(0, names[len].size()-1);

  int rand = distr(gen);
  auto it = names[len].begin();
  for (int i = 0; i < rand; i++) {
    it++;
  }

  return lower(*it);
}

int inputLength() {
  string prompt = "Length of name to guess (2-" + to_string(maxLen) + "): ";
  cout << prompt;
  string ans;
  cin >> ans;
  int iAns;
  while (true) {
    if (!isNumber(ans)) {
      cout << "Not a number!!\n\n";
    } else {
      iAns = stoi(ans);
      if (iAns > maxLen) {
        cout << "Too long\n\n";
      } else if (iAns < 2) {
        cout << "Too short\n\n";
      } else {
        break;
      }
    }

    cout << prompt;
    cin >> ans;
  }

  return iAns;
}

int inputNumberOfGuesses() {
  string prompt = "Number of guesses (at least 1): ";
  cout << prompt;
  string ans;
  cin >> ans;
  int iAns;
  while (true) {
    if (!isNumber(ans)) {
      cout << "Not a number!!\n\n";
    } else {
      iAns = stoi(ans);
      if (iAns < 1) {
        cout << "Too short\n\n";
      } else {
        break;
      }
    }

    cout << prompt;
    cin >> ans;
  }

  return iAns;
}

string validateAnswer(int len, string guess, string secret) {
  vector<int> arr(26);
  for (int i = 0; i < len; i++) {
    arr[secret[i]-'a']++;
  }

  string s(len, 'x');
  for (int i = 0; i < len; i++) {
    if (guess[i] == secret[i]) {
      arr[guess[i]-'a']--;
      s[i] = '.';
    }
  }

  for (int i = 0; i < len; i++) {
    if (guess[i] != secret[i] && arr[guess[i]-'a'] > 0) {
      arr[guess[i]-'a']--;
      s[i] = 'o';
    }
  }

  return s;
}

void startRound(int len, string secret, int guesses) {
  cout << "\nYou have " + to_string(guesses) + " tries to guess a " + to_string(len) + "-letter word.\n\n";
  cout << ". = right letter, correct place" << endl;
  cout << "o = right letter, incorrect place" << endl;
  cout << "x = wrong letter\n\n";
  cout << "Type 'quit' to quit.\n\n";

  string prompt = "Your guess:\n";
  int g = guesses;
  bool won = false;
  while (g > 0) {
    cout << g << " guesses left." << endl;
    cout << prompt;

    string guess; cin >> guess; guess = lower(guess);
    if (guess == "quit") {
      break;
    } else if (guess.size() < len) {
      cout << "Too short! " << "Enter " << len << " letters.\n\n";
    } else if (guess.size() > len) {
      cout << "Too long! " << "Enter " << len << " letters.\n\n";
    } else if (names[len].find(guess) == names[len].end()) {
      cout << "Not a valid name!\n\n";
    } else {
      g--;
      string res = validateAnswer(len, lower(guess), secret);
      cout << res << "\n\n";
      if (res == string(len, '.')) {
        // right answer!
        won = true;
        break;
      }
    }
  }

  if (won) {
    cout << "\nWoohoo! You guessed it in "<< guesses - g << " tries!\n\n";
  } else {
    cout << "\n0 guesses left." << endl;
    cout << "The name was " << secret << ".\n\n";
  }
}

void startGame() {
  cout << banner << endl;
  cout << "by Leran (Alan) Tao. Inspired by Wordle.\n\n\n";

  cout << "Adding names..." << endl;
  auto t1 = chrono::high_resolution_clock::now();
  
  initNames();

  auto t2 = chrono::high_resolution_clock::now();
  auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2 - t1);

  cout << "Done in " << ms_int.count() << "ms.\n\n";

  while (true) {
    int len = inputLength();
    string secret = generateRandomName(len);
    int guesses = inputNumberOfGuesses();

    startRound(len, secret, guesses);

    string ans = "";
    while (isBool(ans) == -1) {
      cout << "\nPlay again (yes/no): ";
      cin >> ans;
    }
    
    int bAns = isBool(ans);
    if (bAns == 0) {
      cout << "\nThanks for playing!" << endl;
      break;
    }

    cout << "\n\n";
  }
}

int main() {
  
  startGame();

  return 0;
}