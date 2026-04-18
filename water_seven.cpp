#include "water_seven.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

bool isName(string s){
    return s == "LUFFY" || s == "ZORO" || s == "SANJI" || s == "NAMI" || s == "CHOPPER" || s == "USOPP" || s == "ROBIN" ? true : false; 
}
int appear(char character[FIXED_CHARACTER][MAX_NAME], string s, int count){
    for(int i = 0; i < count; i++){
        if(!strcmp(character[i], s.c_str())) return i;
    }
    return -1;
}
void modifyHp(int &t){
    if(t < 0) t = 0;
    else if(t > 1000) t = 1000;
    return;
}
void modifySkill(int &t){
    if(t < 0) t = 0;
    else if(t > 100) t = 100;
    return;
}
void modifyRepair(int &t){
    if(t < 0) t = 0;
    else if(t > 3000) t = 3000;
    return;
}


//Task 0
bool readInput(
    const string &filename,
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], 
    int skill[FIXED_CHARACTER], int &shipHP, int &repairCost){
        ifstream Open(filename);
        string name;
        int val1, val2;
        int count = 0;
        if(!Open.is_open()) return false;
        while(Open >> name >> val1 >> val2){
            modifyHp(val1);
            if(isName(name)){
                modifySkill(val2);
                int k = appear(character, name, count);
                if(k != -1){
                    hp[k] = val1;
                    skill[k] = val2;
                }else{
                    strcpy(character[count], name.c_str()); //NOTE: strcpy is used to copy name.c_str() into character[count], c_str() convert string to C-string
                    hp[count] = val1;
                    skill[count] = val2;
                    count++;
                }
            }else if(name == "GOING_MERRY"){
                modifyRepair(val2);
                shipHP = val1;
                repairCost = val2;
            }
        }
        return true;
}

bool Perfect(int n){
    if(n <= 0) return false;
    int t = 0;
    while(n > 0){
        t += n%10;
        n /= 10;
    }
    int k = 0;
    for(int i = 1; i < t; i++){
        if(!(t%i)) k+=i;
    }
    return t == k;
}

// Task 1
int damageEvaluation(int shipHP, int repairCost){
    int res = shipHP < 455 && Perfect(shipHP) ? (repairCost % 2 == 0? repairCost + repairCost/2 : repairCost + (repairCost-1)/2 + 1):repairCost;
    return (res > 3000) ? 3000 : res; 
}

// Task 2
int conflictSimulation(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int shipHP, int repairCost){
        int LS = 0;
        int US = 0;
        int k = 0;
//        while(k < FIXED_CHARACTER && character[k][0] != '\0'){ //Check if a string is empty: '\0'
//            k++;
//        }
        for(int i = 0; i < FIXED_CHARACTER; i++){
            if(!strcmp(character[i], "LUFFY")) LS = skill[i]; //NOTE: strcmp is String Comparision 
            if(!strcmp(character[i], "USOPP")) US = skill[i];
        }
        double conflict = LS - US + repairCost / 100.0 + (500 - shipHP) / 50.0;
        int conflictIndex = ceil(conflict);
        int n = 0;
        while(n < 10 && conflictIndex < 255){
            switch(((conflictIndex % 6) + 6) % 6){
                case 0:
                    conflictIndex += 255;
                    break;
                case 1:
                    conflictIndex += 20;
                    break;
                case 2:
                    conflictIndex += 50;
                    break;
                case 3:
                    conflictIndex += 70;
                    break;
                case 4:
                    conflictIndex += 90;
                    break;
                case 5:
                    conflictIndex += 100;
                    break; 
                default:
                    break;   
            }
            n++;
        }
        return conflictIndex;
    }

// Task 3
void resolveDuel(
    char character[FIXED_CHARACTER][MAX_NAME], int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER],
    int conflictIndex, int repairCost, char duel[FIXED_CHARACTER][MAX_NAME]){
        int k = 0;
        int m = 0;
        while(k < FIXED_CHARACTER && character[k][0] != '\0'){ //Check if a string is empty: '\0'
            if(!strcmp(character[k], "LUFFY") || !strcmp(character[k], "USOPP")) m++;
            k++;
        }
        if(m != 2) return;
        int interventionCost[k - m];
        int supportLevel[k - m];
        int realIndex[k - m];
        int U = 0;
        int L = 0;
        int s = 0;
        for(int i = 0; i < k; i++){
            if(!strcmp(character[i], "LUFFY")){
                L = i;
                continue;
            }else if(!strcmp(character[i], "USOPP")){
                U = i;
                continue;
            }
            realIndex[s] = i;
            interventionCost[s] = hp[i]%10 + 1;
            supportLevel[s] = skill[i];
            s++;
        }
        int Us = skill[U] + conflictIndex/20 + repairCost/500;
        int res = Us - skill[L];
        if(res <= 0) return;

        //k: Size of the character array
        //s: Size of the support members array

        int arr[5]{0};
        //Case 1
        int index1 = -1;
        if((k - m) - 1 >= 0){
            int minCost1 = INT_MAX;
            for(int i = 0; i < s; i++){
                if(supportLevel[i] >= res){
                    if(minCost1 > interventionCost[i]){
                        minCost1 = interventionCost[i];
                        index1 = i;
                    }
                }
            }
            if(index1 != -1) arr[0] = minCost1;
        }

        //Case 2
        int index2_1 = -1;
        int index2_2 = -1;
        if((k - m) - 2 >= 0){
            int minCost2 = INT_MAX;
            for(int i = 0; i < s; i++){
                for(int j = i + 1; j < s; j++){
                    if(supportLevel[i] + supportLevel[j] >= res){
                        if(minCost2 > interventionCost[i] + interventionCost[j]){
                            minCost2 = interventionCost[i] + interventionCost[j];
                            index2_1 = i;
                            index2_2 = j;
                        }
                    }
                }
            }
            if(index2_1 != -1 && index2_2 != -1) arr[1] = minCost2; 
        }

        //Case 3
        int index3_1 = -1;
        int index3_2 = -1;
        int index3_3 = -1;
        if((k - m) - 3 >= 0){
            int minCost3 = INT_MAX;
            for(int i = 0; i < s; i++){
                for(int j = i + 1; j < s; j++){
                    for(int t = j + 1; t < s; t++){
                        if(supportLevel[i] + supportLevel[j] + supportLevel[t] >= res && minCost3 > interventionCost[i] + interventionCost[j] + interventionCost[t]){
                            minCost3 = interventionCost[i] + interventionCost[j] + interventionCost[t];
                            index3_1 = i;
                            index3_2 = j;
                            index3_3 = t;
                        }
                    }
                }
            }
            if(index3_1 != -1 && index3_2 != -1 && index3_3 != -1) arr[2] = minCost3; 
        }
        //Case 4
        int index4_1 = -1;
        int index4_2 = -1;
        int index4_3 = -1;
        int index4_4 = -1;

        if ((k - m) - 4 >= 0) {   // at least 4 helpers
            int minCost4 = INT_MAX;

            for (int i = 0; i < s; i++) {
                for (int j = i + 1; j < s; j++) {
                    for (int t = j + 1; t < s; t++) {
                        for (int u = t + 1; u < s; u++) {
                            int totalSupport = supportLevel[i] + supportLevel[j] + supportLevel[t] + supportLevel[u];
                            int totalCost = interventionCost[i] + interventionCost[j] + interventionCost[t] + interventionCost[u];
                            if (totalSupport >= res && totalCost < minCost4) {
                                minCost4 = totalCost;
                                index4_1 = i;
                                index4_2 = j;
                                index4_3 = t;
                                index4_4 = u;
                            }
                        }
                    }
                }
            }

            if (index4_1 != -1 && index4_2 != -1 && index4_3 != -1 && index4_4 != -1)
                arr[3] = minCost4;
        }

        //Case 5
        int minCost5 = 0;
        int SP5 = 0;
        for(int i = 0; i < s; i++){
            minCost5 += interventionCost[i];
            SP5 += supportLevel[i];
        }
        if(SP5 >= res) arr[4] = minCost5;

        int finalCase = -1;
        int finalCost = INT_MAX;
        for(int i = 0; i < 5; i++){
            if(arr[i] != 0 && arr[i] == finalCost){
                finalCase = min(finalCase, i);
            }else if(arr[i] != 0 && arr[i] < finalCost){
                finalCase = i;
                finalCost = arr[i];
            }
        }
        if(finalCase == -1) return;

        for (int i = 0; i < FIXED_CHARACTER; i++) {
            duel[i][0] = '\0';
        }
        switch(finalCase) {
        case 0: // Case 1: 1 helper
            strcpy(duel[0], character[realIndex[index1]]);
            break;

        case 1: // Case 2: 2 helpers
            strcpy(duel[0], character[realIndex[index2_1]]);
            strcpy(duel[1], character[realIndex[index2_2]]);
            break;

        case 2: // Case 3: 3 helpers
            strcpy(duel[0], character[realIndex[index3_1]]);
            strcpy(duel[1], character[realIndex[index3_2]]);
            strcpy(duel[2], character[realIndex[index3_3]]);
            break;

        case 3: // Case 4: 4 helpers
            strcpy(duel[0], character[realIndex[index4_1]]);
            strcpy(duel[1], character[realIndex[index4_2]]);
            strcpy(duel[2], character[realIndex[index4_3]]);
            strcpy(duel[3], character[realIndex[index4_4]]);
            break;

        case 4: // Case 5: all helpers
            {
                int p = 0;
                for (int i = 0; i < s; i++) {
                    strcpy(duel[p], character[realIndex[i]]);
                    p++;
                }
            }
            break;
        }
        return;
    }

string Reverse(string s){
    int n = s.size();
    string res = "";
    for(int i = n - 1; i >= 0; i--){
        res += s[i];
    }
    return res;
}

bool isValid(string s){
    return s.find("CP9") != string::npos || s.find("ENIESLOBBY") != string::npos;
}

// Task 4
void decodeCP9Message(char character[FIXED_CHARACTER][MAX_NAME], 
    int hp[FIXED_CHARACTER], int skill[FIXED_CHARACTER], int conflictIndex, 
    int repairCost, char cipherText[], char resultText[]){
        int key = (conflictIndex + repairCost)%26;
        int B = key%5 + 4;
        int size = strlen(cipherText);
        int sum = 0;
        int num = (cipherText[size - 2] - '0') * 10 + (cipherText[size - 1] - '0');
        string message = "";
        for(int i = 0; i < size - 3; i++){
            sum += cipherText[i];
            message += cipherText[i];
        }
        //CheckSum
        if(sum%100 != num){
            resultText[0] = '\0';
            return;
        }
        //Block-wise reversal
        string tmp = "";
        for(int i = 0; i < message.size(); i+=B){
            tmp += i + B < message.size() ? Reverse(message.substr(i, B)) : Reverse(message.substr(i));
        }
        message = tmp;
        //Reverse caesar shift decoding
        for(auto &x: message){
            if(x >= 48 && x <= 57){
                x = ((x - '0') - key%10 >= 0 ? ((x - '0') - key%10) + '0' : ((x - '0') - key%10 + 10) + '0');
            } else if(x >= 'A' && x <= 'Z') {
                x = ((x - 'A')) - key >= 0 ? ((x - 'A') - key) + 'A' : ((x - 'A') - key + 26) + 'A';
            } else if(x >= 'a' && x <= 'z') {
                x = ((x - 'a')) - key >= 0 ? ((x - 'a') - key) + 'a' : ((x - 'a') - key + 26) + 'a';
            }
        }
        message = isValid(message) ? message += "_TRUE" : message += "_FALSE";
        strcpy(resultText, message.c_str());
    }

// Task 5
int analyzeDangerLimit(int grid[MAX_GRID][MAX_GRID], int rows, int cols){
    int maxRowSum = 0;
    int maxCell = 0;
    for(int i = 0; i < rows; i++){
        int tmp = 0;
        for(int j = 0; j < cols; j++){
            tmp += grid[i][j] != -1 ? grid[i][j] : 0;
            maxCell = max(maxCell, grid[i][j]);
        }
        maxRowSum = max(tmp, maxRowSum);
    }
    return maxRowSum + maxCell;
}


bool evaluateRoute(int grid[MAX_GRID][MAX_GRID], int rows, int cols, int dangerLimit){
    if(grid[0][0] == -1 || grid[rows - 1][cols - 1] == -1) return false;
    int dp[rows][cols];
    dp[0][0] = grid[0][0];
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < cols; j++){
            if(i == 0 && j == 0) continue;
            if(grid[i][j] != -1){
            if(i == 0) dp[i][j] = grid[i][j] + dp[i][j - 1];
            else if(j == 0) dp[i][j] = grid[i][j] + dp[i - 1][j];
            else dp[i][j] = grid[i][j] + min(dp[i][j - 1], dp[i - 1][j]);
            }else dp[i][j] = 1000000000;
        }
    }
    return dp[rows - 1][cols - 1] <= dangerLimit;
}


////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
