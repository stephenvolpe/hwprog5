/****************************************************************
 * Header file for the Pullet16 interpreter.
 *
 * Author/copyright:  Duncan Buell
 * Used with permission and modified by: Stephen Volpe
 * Date: 1 November 2017
 *
**/

#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <fstream>
#include <cstdio>
#include <iostream>
#include <vector>

using namespace std;

#include "../../Utilities/scanner.h"
#include "../../Utilities/scanline.h"
#include "../../Utilities/utils.h"

#include "globals.h"
#include "hex.h"

class Interpreter {
  public:
    Interpreter();
    virtual ~Interpreter();

    void Interpret(Scanner& data_scanner, ofstream& out_stream);
    void Load(Scanner& exec_scanner, string binary_filename);

  private:
    static const int kMaxInstrCount = 128;
    static const int kPCForStop = 7777;

    int pc_;
    int accum_;

    string ToString();

    vector<string> memory_;
    Globals globals_;

    void DoADD(string addr, string target);
    void DoAND(string addr, string target);
    void DoBAN(string addr, string target);
    void DoBR(string addr, string target);
    void DoLD(string addr, string target);
    void DoRD(Scanner& data_scanner);
    void DoSTC(string addr, string target);
    void DoSTP();
    void DoSUB(string addr, string target);
    void DoWRT(ofstream& out_stream);
    void Execute(string opcode, string addr, string target,
                 Scanner& data_scanner, ofstream& out_stream);
    void FlagAddressOutOfBounds(int address);
    int GetTargetLocation(string label, string address, string target);
    int TwosComplementInteger(int value);
};
#endif
