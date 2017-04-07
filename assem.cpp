#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <bitset>
#include <map>
#include <sstream>
#define Instruction_type int

#define EMPTY_type 0x00
#define J_type 0x01
#define I_type 0x02
#define R_type 0x03

using namespace std;

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
  if (st == "$zero") Num = "00000";
  else if (st == "$at") Num = "00001";
  else if (st == "$v0") Num = "00010";  
  else if (st == "$v1") Num = "00011";
  else if (st == "$a0") Num = "00100";  
  else if (st == "$a1") Num = "00101";
  else if (st == "$a2") Num = "00110";  
  else if (st == "$a3") Num = "00111";
  else if (st == "$t0") Num = "01000";
  else if (st == "$t1") Num = "01001";
  else if (st == "$t2") Num = "01010";  
  else if (st == "$t3") Num = "01011";
  else if (st == "$t4") Num = "01100";  
  else if (st == "$t5") Num = "01101";
  else if (st == "$t6") Num = "01110";  
  else if (st == "$t7") Num = "01111";
  else if (st == "$s0") Num = "10000";    
  else if (st == "$s1") Num = "10001";
  else if (st == "$s2") Num = "10010";  
  else if (st == "$s3") Num = "10011";
  else if (st == "$s4") Num = "10100";  
  else if (st == "$s5") Num = "10101";
  else if (st == "$s6") Num = "10110";  
  else if (st == "$s7") Num = "10111";
  else if (st == "$t8") Num = "11000";
  else if (st == "$t9") Num = "11001";
  else if (st == "$k0") Num = "11010";
  else if (st == "$k1") Num = "11011";
  else if (st == "$gp") Num = "11100";
  else if (st == "$sp") Num = "11101";
  else if (st == "$fp") Num = "11110"; 
  else if (st == "$ra") Num = "11111";       
  return Num ;    
}

 string BinToHex(const string &strBin)
 {
     string strHex = "";
     for (int i = 0; i < 8;  ++ i)
     {
      string tmp = strBin.substr(i * 4, i * 4 + 4);
      int tmpint = (tmp[0] - '0') * 8 + (tmp[1] - '0') * 4 + (tmp[2] - '0') * 2 + (tmp[3] - '0') * 1;
      strHex += tmpint >= 10 ? tmpint - 10 + 'a' : tmpint + '0';
     }
     return strHex;
 }

 string DectoBin(const string &strDec)
 {
    string strBin;
    bitset<16> t;
    t = atoi(strDec.c_str());
    strBin = t.to_string();
    return strBin;
 }

string Handle_Instruction_R(string instruction)
{
	string code;
	string opt, rs, rt, rd;
	stringstream is(instruction);
	is >> opt >> rd >> rs >> rt;
	rs = rs.substr(0, rs.size() - 1);
	rt = rt.substr(0, rt.size() - 1);
	rd = rd.substr(0, rd.size() - 1);
  if (opt == "add") 
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100000";
  else if (opt == "addu")
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100001";
  else if (opt == "sub")
		code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100010";
  else if (opt == "subu")
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100011";
  else if (opt == "slt")
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "101010";
  else if (opt == "sltu")
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "101011";
  else if (opt == "and")
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100100";
	else if (opt == "or") 
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100101";
  else if (opt == "xor")
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100110";
  else if (opt == "nor")
    code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100111";
  return code;
}

string Handle_Instruction_I(string instruction, int pc, map<string, int> Labels)
{
  string code;
  string opt, rs, rt, imm;
  stringstream is(instruction);
  is >> opt >> rt >> rs >> imm;
  rs = rs.substr(0, rs.size() - 1);
  rt = rt.substr(0, rt.size() - 1);
  imm = imm.substr(0, imm.size() - 1);
  /*alu   immediate */
  if (opt == "addi")
    code = "001000" + reg_lookup(rs) + reg_lookup(rt) + DectoBin(imm);
  else if (opt == "addiu")
    code = "001001" + reg_lookup(rs) + reg_lookup(rt) + DectoBin(imm);
  else if (opt == "andi")
    code = "001100" + reg_lookup(rs) + reg_lookup(rt) + DectoBin(imm);
  else if (opt == "ori")
    code = "001101" + reg_lookup(rs) + reg_lookup(rt) + DectoBin(imm);
  else if (opt == "xori")
    code = "001110" + reg_lookup(rs) + reg_lookup(rt) + DectoBin(imm);
  else if (opt == "slti")
    code = "001010" + reg_lookup(rs) + reg_lookup(rt) + DectoBin(imm);
  else if (opt == "sltiu")
    code = "001011" + reg_lookup(rs) + reg_lookup(rt) + DectoBin(imm);
  else if (opt == "lui")
    code = "00111100000" + reg_lookup(rt) + DectoBin(rs);
  else if (opt == "beq")
    code = "000100" + reg_lookup(rs) + reg_lookup(rt) + (Labels.find(imm) == Labels.end() ? DectoBin(imm) : DectoBin(to_string(Labels[imm] - pc - 4)) );
  else if (opt == "bne")
    code = "000101" + reg_lookup(rs) + reg_lookup(rt) + (Labels.find(imm) == Labels.end() ? DectoBin(imm) : DectoBin(to_string(Labels[imm] - pc - 4)) );
  else if (opt == "blez")
    code = "000110" + reg_lookup(rt) + "00000" + (Labels.find(rt) == Labels.end() ? DectoBin(rt) : DectoBin(to_string(Labels[rt] - pc - 4)) );
  else if (opt == "bgtz")
    code = "000111" + reg_lookup(rt) + "00000" + (Labels.find(rt) == Labels.end() ? DectoBin(rt) : DectoBin(to_string(Labels[rt] - pc - 4)) );
  else if (opt == "bltz")
    code = "000001" + reg_lookup(rt) + "00000" + (Labels.find(rt) == Labels.end() ? DectoBin(rt) : DectoBin(to_string(Labels[rt] - pc - 4)) );
  else if (opt == "bgez")
    code = "000001" + reg_lookup(rt) + "10001" + (Labels.find(rt) == Labels.end() ? DectoBin(rt) : DectoBin(to_string(Labels[rt] - pc - 4)) );
  else if (opt == "lw")
  {
    string of = rs.substr(rs.find('(') + 1, rs.find(')') - rs.find('(') - 1), offset = rs.substr(0, rs.find('('));
    code = "100011" + reg_lookup(of) + reg_lookup(rt) + DectoBin(offset);
  }
  else if (opt == "lb")
  {
    string of = rs.substr(rs.find('(') + 1, rs.find(')') - rs.find('(') - 1), offset = rs.substr(0, rs.find('('));
    code = "100000" + reg_lookup(of) + reg_lookup(rt) + DectoBin(offset);
  }
  else if (opt == "sw")
  {
    string of = rs.substr(rs.find('(') + 1, rs.find(')') - rs.find('(') - 1), offset = rs.substr(0, rs.find('('));
    code = "101011" + reg_lookup(of) + reg_lookup(rt) + DectoBin(offset);
  }
  return code;
}

string Handle_Instruction_J(string instruction, int pc, map<string, int> Labels)
{
  string code;
  string opt, target;
  stringstream is(instruction);
  is >> opt >> target;
  opt = opt.substr(0, opt.size() - 1);
  target = target.substr(0, target.size() - 1);
  if (opt == "jr")
    code = "000000" + reg_lookup(target) + "000000000000000001000" ;
  else if (opt == "j")
    code = "000010" + DectoBin(to_string(Labels[target] >> 2));
  else if (opt == "jal")
    code = "000011" + DectoBin(to_string(Labels[target] >> 2));
  else if (opt == "jalr")
    code = "000000" + reg_lookup(target) + "000000000000000001000" ;
  return code;
}

int getmacro(string &s)
{
  if (s.find('#') != string::npos)
    return 1;
  else
    return 0;
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
  if (dollar == 0) return J_type;
  else if (dollar == 3) return R_type;
  else return I_type;
}

void String_space_filter(string &s)
{
  int index = 0;
  if (!s.empty())
  {
      while( (index = s.find(' ', index)) != string::npos)
      {
          s.erase(index, 1);
      }
  }
}


int main(int argc, char** argv)
{
  vector<string> Instructions;
  map<string, int> Labels;
  int instruction_i, label_i, pc, initial_pc;
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
  for (vector<string>::iterator iter = Instructions.begin(); iter != Instructions.end(); ++ iter)
	{
    pc = initial_pc;
		Instruction_type type;
		string Machine_code;
		type = gettype(*iter);
		switch(type)
		{
			case (J_type): {
        Machine_code = Handle_Instruction_I(*iter, pc, Labels); 
        cout << BinToHex(Machine_code) << endl;
        break;
      }
			case (I_type): {
        Machine_code = Handle_Instruction_I(*iter, pc, Labels); 
        cout << BinToHex(Machine_code) << endl;
        break;
      }
			case (R_type): {
          Machine_code = Handle_Instruction_R(*iter); 
          cout << BinToHex(Machine_code) << endl;
          break;
      }
			default : break;
		}
    pc += 4;
	}
}
