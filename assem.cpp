#include<iostream>
#include<vector>
#include<fstream>
#include<string>
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

string Handle_Instruction_R(string instruction)
{
	string code;
	string opt, rs, rt, rd;
	stringstream is(instruction);
	is >> opt >> rd >> rs >> rt;
	rs = rs.substr(0, rs.size() - 1);
	rt = rs.substr(0, rt.size() - 1);
	rd = rd.substr(0, rd.size() - 1);
	switch(opt)
	{
		case("add"): code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100000";break;
		case("sub"): code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100010";break;
		case("and"): code = "000000" + reg_lookup(rs) + reg_lookup(rt) + reg_lookup(rd) + "00000" + "100100";break;
	}	
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
  else if (dollar == 2) return I_type;
  else return R_type;
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
		Instruction_type type;
		string Machine_code;
		type = getbuffer(*iter);
		switch(type)
		{
			case (J_type): Machine_code = Handle_Instruction_J(*iter); break;
			case (I_type): Machine_code = Handle_Instruction_I(*iter); break;
			case (R_type): Machine_code = Handle_Instruction_R(*iter); break;
			default : break;
		}
	}
  cout << Labels["start"];
}
