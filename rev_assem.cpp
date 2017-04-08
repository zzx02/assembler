#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <map>
#include <sstream>
#include <cstdio>

#define Instruction_type int

#define EMPTY_type 0x00
#define J_type 0x01
#define I_type 0x02
#define R_type 0x03

using namespace std;

void Semicolon_Comma_filter(string &);
 /**************************************************** 
  The instruction strctures 
  R-type:  
     +------------------------------------------------+ 
     | opcode | rs    | rt    | rd    | shamt | funct |     
     |--------+-------+-------+-------+-------+-------| 
     | 6-bit  | 5-bit | 5-bit | 5-bit | 5-bit | 6-bit | 
     +------------------------------------------------+ 
  I-type: 
     +------------------------------------------------+ 
     | opcode | rs    | rt    | immediate             | 
     |--------+-------+-------+-----------------------| 
     | 6-bit  | 5-bit | 5-bit | 16-bit                | 
     +------------------------------------------------+ 
  J-type: 
     +------------------------------------------------+ 
     | opcode | jump address                          | 
     |--------+---------------------------------------| 
     | 6-bit  | 26-bit                                | 
     +------------------------------------------------+ 
 */  

string reg_lookup(string & st)
{
	string Num;
  if (st == "00000") Num = "$zero";
  else if (st == "00001") Num = "$at";
  else if (st == "00010") Num = "$v0";  
  else if (st == "00011") Num = "$v1";
  else if (st == "00100") Num = "$a0";  
  else if (st == "00101") Num = "$a1";
  else if (st == "00110") Num = "$a2";  
  else if (st == "00111") Num = "$a3";
  else if (st == "01000") Num = "$t0";
  else if (st == "01001") Num = "$t1";
  else if (st == "01010") Num = "$t2";  
  else if (st == "01011") Num = "$t3";
  else if (st == "01100") Num = "$t4";  
  else if (st == "01101") Num = "$t5";
  else if (st == "01110") Num = "$t6";  
  else if (st == "01111") Num = "$t7";
  else if (st == "10000") Num = "$s0";    
  else if (st == "10001") Num = "$s1";
  else if (st == "10010") Num = "$s2";  
  else if (st == "10011") Num = "$s2";
  else if (st == "10100") Num = "$s4";  
  else if (st == "10101") Num = "$s5";
  else if (st == "10110") Num = "$s6";  
  else if (st == "10111") Num = "$s7";
  else if (st == "11000") Num = "$t8";
  else if (st == "11001") Num = "$t9";
  else if (st == "11010") Num = "$k0";
  else if (st == "11011") Num = "$k1";
  else if (st == "11100") Num = "$gp";
  else if (st == "11101") Num = "$sp";
  else if (st == "11110") Num = "$fp"; 
  else if (st == "11111") Num = "$ra";       
  return Num ;    
}

 string HexToBin(const string &strHex)
 {
    for (int i = 0; i < 7;  ++ i)
     {
      string strBin = "";
      int tmp = strHex[i] - '0' > 10 ? strHex[i] - 'A' : strHex[i] - '0';
      if (tmp / 8) strBin += "1"; else strBin += "0"; tmp %= 8;
      if (tmp / 4) strBin += "1"; else strBin += "0"; tmp %= 4;
      if (tmp / 2) strBin += "1"; else strBin += "0"; tmp %= 2;
      if (tmp / 1) strBin += "1"; else strBin += "0"; tmp %= 1;
     }
     return strBin;
 }

 string DectoBin(const string &strDec, bool _26 = false)
 {
    string strBin;
    unsigned int tmp;
    if (!_26)
    {
      bitset<16> t;
      if (strDec.substr(0, 2) != "0x")
        t = atoi(strDec.c_str());
      else
      {
        sscanf(strDec.c_str(), "%x", &tmp);
        t = tmp;
      }
      strBin = t.to_string();
    }
    else
    {
      bitset<26> t;
      if (strDec.substr(0, 2) != "0x")
        t = atoi(strDec.c_str());
      else
      {
        sscanf(strDec.c_str(), "%x", &tmp);
        t = tmp;
      }
      strBin = t.to_string();
    }
    return strBin;
 }

int getlabel(string &s, string &label)
{
  int index = 0;
  if ((index = s.find(':')) != string::npos)
  {
      label = s.substr(0, index);
      s = s.substr(index + 1, s.size());
      return 1;
  }
  else
      {
        label = "";
        return 0;
      }

}

int gettype(const string &s)
{
  int dollar = 0;
  if (s == "" || s.find('#') != string::npos) return EMPTY_type;
  for (int it = 0; it != s.size(); it ++)
  {
    if (s[it] == '$') dollar ++;
  }
  if (dollar == 0 || s.substr(0, 2) == "jr" || s.substr(0, 4) == "jalr") return J_type;
  else if (dollar == 3) return R_type;
  else return I_type;
}

void Semicolon_Comma_filter(string &s)
{
  int index = 0;
  if (!s.empty())
  {
      while( (index = s.find(';', index)) != string::npos)
      {
        s.erase(index, 1);
      }
  }
  index = 0;
  if (!s.empty())
  {
      while( (index = s.find(',', index)) != string::npos)
      {
        s.erase(index, 1);
      }
  }
}



int main(int argc, char** argv)
{
  vector<string> Instructions;
  map<string, int> Labels;
  int instruction_i, label_i, pc = 0, initial_pc = 0;
  string label;
  if (argc != 3)
  {
    cout << "Parameters are not correct, please type the name of input and output" << endl;
    return -1;
  }
  ifstream inFile(argv[1]);
  ofstream outFile(argv[2]);
  if (!inFile || !outFile)
  {
    if (!inFile)
      cout << "Can't open " << argv[1] << ", Try again" << endl;
    else
      cout << "Can't open " << argv[2] << ", Try again" << endl;
    return -1;
  }
/* Scan the whole .asm file, generate the instructions and the label tables; */
  while (!inFile.eof())
  {
    string buffer, macro = "";
    Instruction_type type;
    getline(inFile, buffer);
    if (getmacro(buffer)) {
        stringstream is(buffer);
        string temp;
        int p;
        is >> temp >> p;
        pc = initial_pc = p;
    }
    if (getlabel(buffer, label)) Labels.insert(pair<string,int>(label, pc));
    if ((type = gettype(buffer)) == EMPTY_type) continue;
	Instructions.push_back(buffer);
    pc += 4;
  }
    pc = initial_pc;
  for (vector<string>::iterator iter = Instructions.begin(); iter != Instructions.end(); ++ iter)
	{
		Instruction_type type;
		string Machine_code;
		type = gettype(*iter);
		switch(type)
		{
			case (J_type): {
        Machine_code = Handle_Instruction_J(*iter, pc, Labels); 
        outFile << BinToHex(Machine_code) << endl;
        break;
      }
			case (I_type): {
        Machine_code = Handle_Instruction_I(*iter, pc, Labels); 
        outFile << BinToHex(Machine_code) << endl;
        break;
      }
			case (R_type): {
          Machine_code = Handle_Instruction_R(*iter); 
          outFile << BinToHex(Machine_code) << endl;
          break;
      }
			default : break;
		}
    pc += 4;
	}
}
