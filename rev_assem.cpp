#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <map>
#include <sstream>
#include <cstdio>

using namespace std;

void Semicolon_Comma_filter(string &);
string reg_lookup(string );
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

string reg_lookup(string  st)
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
 string BinToHex(const string &strBin)
 {
     string strHex = "";
     for (int i = 0; i < strBin.size() / 4;  ++ i)
     {
      string tmp = strBin.substr(i * 4, i * 4 + 4);
      int tmpint = (tmp[0] - '0') * 8 + (tmp[1] - '0') * 4 + (tmp[2] - '0') * 2 + (tmp[3] - '0') * 1;
      strHex += tmpint >= 10 ? tmpint - 10 + 'A' : tmpint + '0';
     }
     return strHex;
 }

int BinToDec(const string &strBin)
{
  int _2 = 0, tot = 0;
  int tmp = strBin.length() - 1;
  _2 = 1;
  if (strBin[0] == '0')
  {
  while (tmp > 0)
  {
    tot += (strBin[tmp --] - '0') * _2;
    _2 *= 2; 
  }
  return tot;
  }
  else
  {
    while (tmp > 0)
    {
      tot += (strBin[tmp -- ] == '1' ? 0: 1) * _2;
      _2 *= 2;
    }
    return -(tot + 1);
  }
}


 string HexToBin(const string &strHex)
 {
    string ReturnStr = "";
    for (int i = 0; i < 8;  ++ i)
     {
      string strBin = "";
      int tmp = strHex[i] - '0' > 10 ? strHex[i] - 'A' + 10 : strHex[i] - '0';
      if (tmp / 8) strBin += "1"; else strBin += "0"; tmp %= 8;
      if (tmp / 4) strBin += "1"; else strBin += "0"; tmp %= 4;
      if (tmp / 2) strBin += "1"; else strBin += "0"; tmp %= 2;
      if (tmp / 1) strBin += "1"; else strBin += "0"; tmp %= 1;
      ReturnStr += strBin;
     }
     return ReturnStr;
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

string Handle_MachineCode(string & machine, map<int, string> Labels, int pc)
{
  string assem;
  string opcode = machine.substr(0, 6), shamt = machine.substr(21, 5), funct = machine.substr(26, 6);
  if (opcode == "000000")
  {
    if (funct == "100000" && shamt == "00000")
      assem = "add " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "100001" && shamt == "00000")
      assem = "addu " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "100010" && shamt == "00000") 
      assem = "sub " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "100011" && shamt == "00000") 
      assem = "subu " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "101010" && shamt == "00000") 
      assem = "slt " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "101011" && shamt == "00000") 
      assem = "sltu " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "100100" && shamt == "00000") 
      assem = "and " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "100101" && shamt == "00000") 
      assem = "or " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "100110" && shamt == "00000") 
      assem = "xor " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else  if (funct == "100111" && shamt == "00000") 
      assem = "nor " + reg_lookup(machine.substr(16, 5)) +  ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + reg_lookup(machine.substr(11, 5));
    else if (funct == "001000")
      assem = "jr " + reg_lookup(machine.substr(6, 5));
    else if (funct == "001001")
      assem = "jalr " + reg_lookup(machine.substr(6, 5));
  }
  else if (opcode == "001000")
      assem = "addi " + reg_lookup(machine.substr(11, 5)) + ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16)));
  else if (opcode == "001001")
      assem = "addu " + reg_lookup(machine.substr(11, 5)) + ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16)));
  else if (opcode == "001101")
      assem = "ori " + reg_lookup(machine.substr(11, 5)) + ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16)));
  else if (opcode == "001100")
      assem = "xori " + reg_lookup(machine.substr(11, 5)) + ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16)));
  else if (opcode == "001010")
      assem = "slti " + reg_lookup(machine.substr(11, 5)) + ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16)));
  else if (opcode == "001101")
      assem = "sltiu " + reg_lookup(machine.substr(11, 5)) + ", " + 
              reg_lookup(machine.substr(6, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16)));
  else if (opcode == "001111")
      assem = "lui " + reg_lookup(machine.substr(11, 5)) + ", " + "0x" + BinToHex(machine.substr(16, 16));
  else if (opcode == "000100")
      assem = "beq " + reg_lookup(machine.substr(6, 5)) + ", " + 
              reg_lookup(machine.substr(11, 5)) + ", " + Labels[(BinToDec(machine.substr(16, 16)) << 2) + pc + 4];
  else if (opcode == "000101")
      assem = "bne " + reg_lookup(machine.substr(6, 5)) + ", " + 
              reg_lookup(machine.substr(11, 5)) + ", " + Labels[(BinToDec(machine.substr(16, 16)) << 2) + pc + 4];
  else if (opcode == "000110")
      assem = "blez " + reg_lookup(machine.substr(6, 5)) + " " + 
              Labels[BinToDec(machine.substr(16, 16)) << 2];
  else if (opcode == "000111")
      assem = "bgtz " + reg_lookup(machine.substr(6, 5)) + " " + 
              Labels[BinToDec(machine.substr(16, 16)) << 2];
  else if (opcode == "000110" && machine.substr(11, 5) == "00000")
      assem = "bltz " + reg_lookup(machine.substr(6, 5)) + " " + 
              Labels[BinToDec(machine.substr(16, 16)) << 2];      
  else if (opcode == "000110" && machine.substr(11, 5) == "10001")
      assem = "bgez " + reg_lookup(machine.substr(6, 5)) + " " + 
              Labels[BinToDec(machine.substr(16, 16)) << 2];
  else if (opcode == "100011")
      assem = "lw " + reg_lookup(machine.substr(11, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16))) + "(" 
              + reg_lookup(machine.substr(6, 5)) + ")";
  else if (opcode == "101011")
      assem = "sw " + reg_lookup(machine.substr(11, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16))) + "(" 
              + reg_lookup(machine.substr(6, 5)) + ")";
  else if (opcode == "100000")
      assem = "lb " + reg_lookup(machine.substr(11, 5)) + ", " + to_string(BinToDec(machine.substr(16, 16))) + "(" 
              + reg_lookup(machine.substr(6, 5)) + ")"; 
  else if (opcode == "000010")
      assem = "j " +  Labels[BinToDec(machine.substr(6, 26)) << 2];
  else if (opcode == "000011")
      assem = "jal " +  Labels[BinToDec(machine.substr(6, 26)) << 2];         
  return assem;
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
  map<int, string> Labels;
  int instruction_i, label_i = 0, pc = 0, initial_pc = 0;
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
/* Scan the whole .bin file, generate the instructions and the label tables; */
  while (!inFile.eof())
  {
    string buffer;
    getline(inFile, buffer);
    buffer = HexToBin(buffer);
    Semicolon_Comma_filter(buffer);
    if (buffer.substr(0, 6) == "000100" || buffer.substr(0, 6) == "000101" ||
          buffer.substr(0, 6) == "000110" || buffer.substr(0, 6) == "000111" || buffer.substr(0, 6) == "000001"
          || buffer.substr(0, 6) == "000010" || buffer.substr(0, 6) == "000011")
      {
        if ((buffer.substr(0, 6) == "000010" || buffer.substr(0, 6) == "000011" ) && 
              Labels.find(BinToDec(buffer.substr(6, 26)) * 4) == Labels.end())
        {
            label_i ++ ;
            Labels.insert(pair<int,string>(BinToDec(buffer.substr(6, 26)) * 4, "label" + to_string(label_i)));
        }
        else if ((buffer.substr(0, 6) != "000010" && buffer.substr(0, 6) != "000011" ) &&
                    Labels.find(BinToDec(buffer.substr(6, 26)) * 4 + pc + 4) == Labels.end())
        {
            label_i ++ ;
            Labels.insert(pair<int,string>(pc + 4 + (BinToDec(buffer.substr(16, 16)) * 4), "label" + to_string(label_i)));
        }
      }
    Instructions.push_back(buffer);
    pc += 4;
  }
    pc = initial_pc;
  for (vector<string>::iterator iter = Instructions.begin(); iter != Instructions.end(); ++ iter)
	{
    string assemcode;
    assemcode = Handle_MachineCode(*iter, Labels, pc);
    if (Labels.find(pc) != Labels.end()) outFile << Labels[pc] << ":" << endl;
    outFile << assemcode << endl;
    pc += 4;
	}
}

