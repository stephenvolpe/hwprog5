#include "pullet16interpreter.h"
/*
*
*  This is Dr. Buells code, as represented by the author: Duncan. Buell.
*
*/
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Class 'Interpreter' for interpreting the Pullet16 code.
 *
 * This class does the interpretation of ASCII versions of 'executable'
 * files for the Pullet16.
 *
 * The 'Load' function loads an executable into memory, and then the
 * 'Interpret' function simulates execution of the Pullet16 using the
 * code loaded into the memory.
 *
 * Documentation for details of the functions is done as headers for
 * the functions.
 *
 * Notes:
 *   Notice throughout that 'GetTargetLocation' does the error checking
 *   for whether a target address is in fact out of bounds.
 *
 * Author: Duncan A. Buell
 * Used with permission and modified by: Stephen Volpe
 * Date: 1 November 2017
**/

/******************************************************************************
 * Constructor
**/
Interpreter::Interpreter() {
}

/******************************************************************************
 * Destructor
**/
Interpreter::~Interpreter() {
}

/******************************************************************************
 * Accessors and Mutators
**/

/******************************************************************************
 * General functions.
**/

/******************************************************************************
 * Function 'DoADD'.
 * This top level function interprets the 'ADD' opcode.
 *
 * Load the contents from the 'target', taking indirection into account.
 * Convert the 16-bit value to its 2s complement version as a 32-bit value.
 * Add, storing the result in the accumulator.
**/
void Interpreter::DoADD(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoADD\n"; 
#endif

  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "ADD        " 
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("ADD FROM", addr, target);
  int valuetoadd = globals_.BitStringToDec(memory_.at(location));
  int twoscomplement = this->TwosComplementInteger(valuetoadd);
  Utils::log_stream << "ADD VALUE " << memory_.at(location) 
                    << " " << twoscomplement << endl;
  Utils::log_stream << endl;

  accum_ = (accum_ + valuetoadd) % 65536;

#ifdef EBUG
  Utils::log_stream << "leave DoADD\n"; 
#endif
}

/******************************************************************************
 * Function 'DoAND'.
 * This top level function interprets the 'AND' opcode.
 *
 * Load the contents from the 'target', taking indirection into account.
 * AND, storing the result in the accumulator.
**/
void Interpreter::DoAND(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoAND\n"; 
#endif
  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "AND "
                    << addr << " " << target << endl;
  int location = this->GetTargetLocation("AND WITH", addr, target);
  int valuetoand = globals_.BitStringToDec(memory_.at(location));
  Utils::log_stream << "AND VALUE " << memory_.at(location) << endl; 
  Utils::log_stream << endl;

  accum_ = accum_ & valuetoand;

#ifdef EBUG
  Utils::log_stream << "leave DoAND\n"; 
#endif
}

/******************************************************************************
 * Function 'DoBAN'.
 * This top level function interprets the 'BAN' opcode.
 *
 * If the accumulator value is negative, branch to the target location.
 * Otherwise, just continue on continuing on.
**/
void Interpreter::DoBAN(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoBAN\n"; 
#endif
  Utils::log_stream << "OPCODE ADDR TARGET " << "BAN " << addr << " " 
                    << target << endl;

  // We are faking the twos-complement, so the 16 bit twos-complement
  // accumulator is negative if the high bit is set, which means as an
  // ordinary integer it will be greater equal 32768 = 2^{15}.
  if (accum_ >= 32768) {

    int location = this->GetTargetLocation("BRANCH TO", addr, target);

    pc_ = location - 1; // a hack because we always increment later
  }

#ifdef EBUG
  Utils::log_stream << "leave DoBAN\n"; 
#endif
}

/******************************************************************************
 * Function 'DoBR'.
 * This top level function interprets the 'BR' opcode.
 *
 * Branch unconditionally to the target location.
**/
void Interpreter::DoBR(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoBR\n"; 
#endif
  Utils::log_stream << "OPCODE ADDR TARGET " << "BR  " << addr << " " 
                    << target << endl;

  int location = this->GetTargetLocation("BRANCH TO", addr, target);

    pc_ = location - 1; // a hack because we always increment later

#ifdef EBUG
  Utils::log_stream << "leave DoBR\n"; 
#endif
}

/******************************************************************************
 * Function 'DoLD'.
 * This top level function interprets the 'LD' opcode.
 *
 * Load the accumulator with the contents of the target location.
**/
void Interpreter::DoLD(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoLD\n"; 
#endif
  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "LD         " 
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("LOAD FROM", addr, target);
  int loadvalue = globals_.BitStringToDec(memory_.at(location));
  int twoscomplement = this->TwosComplementInteger(loadvalue);
  Utils::log_stream << "LOAD VALUE " << twoscomplement << endl;
  Utils::log_stream << endl;

  accum_ = loadvalue;

#ifdef EBUG
  Utils::log_stream << "leave DoLD\n"; 
#endif
}

/******************************************************************************
 * Function 'DoRD'.
 * This top level function interprets the 'RD' opcode.
 *
 * If there is more data:
 *   scan the next line for input
 *   convert from the hex character format input into an int value
 *   store the int value in the accumulator
 * Else:
 *   crash on read past end of file
**/
void Interpreter::DoRD(Scanner& data_scanner) {
#ifdef EBUG
  Utils::log_stream << "enter DoRD\n"; 
#endif
  Utils::log_stream << "OPCODE " << "RD  " << endl;

  if (data_scanner.HasNext()) {
    string inputstring = data_scanner.Next();
    Hex hex = Hex(inputstring, globals_);

    if (hex.HasAnError()) {
      Utils::log_stream << "\nERROR -- INVALID INPUT " << hex.ToString() << endl;
      Utils::log_stream << "PROGRAM TERMINATING" << endl;
      exit(0);
    } else {
      accum_ = hex.GetValue();
    }
  } else {
    Utils::log_stream << "\nERROR -- READ PAST END OF FILE" << endl;
    Utils::log_stream << "PROGRAM TERMINATING" << endl;
    exit(0);
  }

#ifdef EBUG
  Utils::log_stream << "leave DoRD\n"; 
#endif
}

/******************************************************************************
 * Function 'DoSTC'.
 * This top level function interprets the 'STC' opcode.
 *
 * Get the target location.
 * Store the accumulator at that location.
 * Zero the accumulator.
 *
 * This assumes that 'GetTargetLocation' does the error checking for invalid
 * addresses.
**/
void Interpreter::DoSTC(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoSTC\n"; 
#endif
  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "STC        " 
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("STORE TO", addr, target);
  string storevalue = globals_.DecToBitString(accum_, 16);
  // We can use 'at' here because the 'GetTargetLocation' will have
  // crashed if 'location' isn't a valid address.
  memory_.at(location) = storevalue;
  Utils::log_stream << "STORE VALUE " << storevalue << endl;
  Utils::log_stream << endl;

  accum_ = 0;

#ifdef EBUG
  Utils::log_stream << "leave DoSTC\n"; 
#endif
}

/******************************************************************************
 * Function 'DoSTP'.
 * This top level function interprets the 'STP' opcode.
 *
 * Since the function one higher up interprets the 'kPCForStop' value as
 * the indicator to stop execution, all we need to do here is assign that
 * constant to the program counter.
**/
void Interpreter::DoSTP() {
#ifdef EBUG
  Utils::log_stream << "enter DoSTP\n"; 
#endif
  Utils::log_stream << "OPCODE " << "STP " << endl;

  pc_ = kPCForStop;

#ifdef EBUG
  Utils::log_stream << "leave DoSTP\n"; 
#endif
}

/******************************************************************************
 * Function 'DoSUB'.
 * This top level function interprets the 'SUB' opcode.
**/
void Interpreter::DoSUB(string addr, string target) {
#ifdef EBUG
  Utils::log_stream << "enter DoSUB\n"; 
#endif

  Utils::log_stream << "EXECUTE:    OPCODE ADDR TARGET " << "SUB        " 
                    << addr << " " << target << endl;

  int location = this->GetTargetLocation("SUB FROM", addr, target);
  int valuetosub = globals_.BitStringToDec(memory_.at(location));
  int twoscomplement = this->TwosComplementInteger(valuetosub);
  Utils::log_stream << "SUB VALUE " << memory_.at(location) 
                    << " " << twoscomplement << endl;
  Utils::log_stream << endl;

  accum_ = (accum_ - valuetosub + 65536) % 65536;

#ifdef EBUG
  Utils::log_stream << "leave DoSUB\n"; 
#endif
}

/******************************************************************************
 * Function 'DoWRT'.
 * This top level function interprets the 'WRT' opcode.
 *
 * Convert the 16-bit accumulator to a 32-bit 2s complement value.
 * Write that value to standard output.
 *
 * Note that we actually write more than just the value itself so we can do
 * better tracing. This could/should be fixed in a final version of this code.
**/
void Interpreter::DoWRT(ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter DoWRT\n"; 
#endif
  Utils::log_stream << "EXECUTE:    OPCODE             " << "WRT" << endl;

  string s = "WRITE OUTPUT ";
  int twoscomplement = this->TwosComplementInteger(accum_);
  s += Utils::Format(twoscomplement, 8) + " " + globals_.DecToBitString(accum_, 16);

  Utils::log_stream << s << endl;

  out_stream << s << endl;

#ifdef EBUG
  Utils::log_stream << "leave DoWRT\n"; 
#endif
}

/******************************************************************************
 * Function 'Interpret'.
 * This top level function interprets the code.
 *
 * Note that errors in execution will result in the program being terminated
 * from functions other than this one.
 *
 * We run a loop until we either hit the bogus PC value for the STP or we
 * encounter an error, which can include having the PC go past 4095.
 * while true
 *   decode the instruction pointed to by the PC
 *   execute the instruction
 *   check for invalid PC or infinite loop
**/
void Interpreter::Interpret(Scanner& data_scanner, ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Interpret\n"; 
#endif

  ////////////////////////////////////////////////////////////////////////////
  // Loop through the memory.
  // Unpack the instruction.
  // Execute the instruction.
  //
  // The only gotcha in this program is that we ALWAYS bump the PC by 1 at
  // the bottom of this loop, so the execute function takes this into account
  // and bumps by one too few.
  int instructioncount = 0;
  pc_ = 0;
  while (true) { // run forever, break below for STP or instructioncount
    string line = memory_.at(pc_);
    string opcode = line.substr(0, 3);
    string addr = line.substr(3, 1);
    string target = line.substr(4);
    Utils::log_stream << "INTERPRET: PC OPCODE ADDR TARGET " 
                      << Utils::Format(pc_, 6) << " " << opcode 
                      << " " << addr << " " << target << endl;
    this->Execute(opcode, addr, target, data_scanner, out_stream);

    // If we have hit the stop we will have returned a flag value that says
    // we should stop execution. Note that if we happen to want to branch
    // to an invalid location that is exactly the same as the 'kPCForStop'
    // value we will already have crashed in the 'BR' or 'BAN' instruction
    // before we get here.
    if (pc_ == kPCForStop) {
      break;
    }

    ++pc_;
    // If we have executed but the PC is now incremented past the end of
    // memory, we have an execution error.
    if (pc_ >= memory_.size()) {
      Utils::log_stream << "***** ERROR -- PC BEYOND MEMORY BOUND" << endl;
      break;
    }

    // This is an interpreter thing. We prevent infinite loops from being
    // interpreted by having a timeout feature on instruction count.
    ++instructioncount;
    if (instructioncount >= kMaxInstrCount) {
      Utils::log_stream << "PROGRAM TIMED OUT" << endl;
      break;
    }
  } // while (true)

#ifdef EBUG
  Utils::log_stream << "leave Interpret\n"; 
#endif
}

/******************************************************************************
 * Function 'Execute'.
 * This top level function executes the code.
 *
 * Execution is basically a switch statement based on the opcode value.
 *
 * Parameters:
 *   opcode - the three "bit" opcode to be executed
 *   addr - the bit indicating indirect addressing or not
 *   target - the 12-bit address in the instruction to be executed
 *   data_scanner - the 'Scanner', needed for the 'RD' instruction
 *   out_stream - the output stream , needed for the 'WRT' instruction
**/
void Interpreter::Execute(string opcode, string addr, string target,
                       Scanner& data_scanner, ofstream& out_stream) {
#ifdef EBUG
  Utils::log_stream << "enter Execute\n"; 
#endif

  if (opcode == "000") {
    this->DoBAN(addr, target);
  } else if (opcode == "001") {
    this->DoSUB(addr, target);
  } else if (opcode == "010") {
    this->DoSTC(addr, target);
  } else if (opcode == "011") {
    this->DoAND(addr, target);
  } else if (opcode == "100") {
    this->DoADD(addr, target);
  } else if (opcode == "101") {
    this->DoLD(addr, target);
  } else if (opcode == "110") {
    this->DoBR(addr, target);
  } else if (opcode == "111") {
    if (target == "000000000001") {
      this->DoRD(data_scanner);
    } else if (target == "000000000010") {
      this->DoSTP();
    } else if (target == "000000000011") {
      this->DoWRT(out_stream);
    } else {
      Utils::log_stream << "***** ERROR -- ILLEGAL OPCODE " << opcode
                        << " AND TARGET " << target << endl;
      Utils::log_stream << "PROGRAM TERMINATING" << endl;
      exit(0);
    }
  } else {
    Utils::log_stream << "***** ERROR -- ILLEGAL OPCODE " << opcode
                      << " AND TARGET " << target << endl;
    Utils::log_stream << "PROGRAM TERMINATING" << endl;
    exit(0);
  }

  Utils::log_stream << "MACHINE IS NOW" << endl << this->ToString() << endl;
  Utils::log_stream << endl;

#ifdef EBUG
  Utils::log_stream << "leave Execute\n"; 
#endif
}

/******************************************************************************
 * Function 'FlagAddressOutOfBounds'.
 * Check to see if an address is between 0 and 'kMaxMemory' inclusive and
 * die if this isn't the case.
 *
 * Parameter:
 *   address - the address to check for out of bounds
**/
void Interpreter::FlagAddressOutOfBounds(int address) {
#ifdef EBUG
  Utils::log_stream << "enter FlagAddressOutOfBounds\n"; 
#endif

  if ((address < 0) || (address > globals_.kMaxMemory)) {
    string s = "";
    s += "***** ERROR -- ADDRESS "; 
    s += Utils::Format(address, 8);
    s += " IS OUT OF BOUNDS"; 
    Utils::log_stream << s << endl;
    exit(0);
  }

#ifdef EBUG
  Utils::log_stream << "leave FlagAddressOutOfBounds\n"; 
#endif
}

/******************************************************************************
 * Function 'GetTargetLocation'.
 * Get the target location, perhaps through indirect addressing.
 *
 * Note that this function crashes the program if the target location is out
 * of bounds for this simulated computer.
 *
 * Parameter:
 *   label - the label for our tracing output
 *   address - is this indirect or not?
 *   target - the target to look up
**/
int Interpreter::GetTargetLocation(string label, string address, string target) {
#ifdef EBUG
  Utils::log_stream << "enter GetTargetLocation\n"; 
#endif

  int location = 0;
  if (address == "0") {
    location = globals_.BitStringToDec(target);
    this->FlagAddressOutOfBounds(location);
    Utils::log_stream << endl;
    Utils::log_stream << label << " LOCATION " << location << endl;
    Utils::log_stream << endl;
  } else {
    location = globals_.BitStringToDec(target);
    this->FlagAddressOutOfBounds(location);
    int indirectlocation = globals_.BitStringToDec(memory_.at(location));
    this->FlagAddressOutOfBounds(indirectlocation);
    Utils::log_stream << endl;
    Utils::log_stream << label << " LOCATION " << location << endl;
    Utils::log_stream << label << " INDIRECT " << indirectlocation << endl;
    Utils::log_stream << endl;
    location = indirectlocation;
  }

#ifdef EBUG
  Utils::log_stream << "leave GetTargetLocation\n"; 
#endif

  return location;
}

/******************************************************************************
 * Function 'Load'.
 * This top level function loads the machine code into memory.
 *
 * We also open and read the binary executable file, and we verify that the
 * binary that we read matches the ASCII 0 and 1 file that we read as the
 * "machine code" into the memory.
 *
 * Parameters:
 *   in_scanner - the scanner to read for source code
 *   binary_filename - the name of the file of the binary executable
**/
void Interpreter::Load(Scanner& in_scanner, string binary_filename) {
#ifdef EBUG
  Utils::log_stream << "enter Load\n"; 
#endif
  globals_ = Globals();
  accum_ = 0;
  pc_ = 0;
  // Read the lines of the ASCII version of the executable and put the
  // ASCII into a 'vector' of 'string' data.
  //This is for homework 5, part 1 of 3
  int linesub = 0;
  while (in_scanner.HasNext()) {
    string line = in_scanner.NextLine();
    memory_.push_back(line);
    ++linesub;
  }
  
  //Part 2 of homework 5
  //I want to take my memory of 16 bit string bits
  //And push them onto a vector of shorts

  vector<short> short_vec;
  for(int i = 0; i < memory_.size(); ++i){
    string temp_string = memory_.at(i);
    int temp_int = globals_.BitStringToDec(temp_string);
    short temp_short = static_cast<short>(temp_int);
    short_vec.push_back(temp_short);
  }




  //I now need to write the vector of shorts
  //To a binary file
  FILE * fp;
  fp = fopen("test.bin", "w");
  for(auto iter = short_vec.begin(); iter != short_vec.end(); ++iter){
    short n = *iter;
    fwrite(&n, 2, 1, fp);
  }
  fclose(fp);


  //I now need to read the binary I just wrote
  //This is for part 3 of assignment 5
  vector<string> check_values;
  fp = fopen("test.bin", "r");

  for(auto iter = short_vec.begin(); iter != short_vec.end(); ++iter){
    short temp_short;
    fread(&temp_short, 2, 1, fp);
    check_values.push_back(globals_.DecToBitString(temp_short, 16));
  }

  /*
  *
  *  This is where i need to compare what I had
  *  TO what I have and i cant figure out why
  *  I am still writing ascii to the bin file
  *
  */
  //Expand the binary into
  //Asci


  /*
  * SI Session
  string ascii_input = "";
  vector<string> binary_strings;
  Scanner binary_scanner;
  char character = ' ';
  int character_to_int = 0;
  string binary_str = "";
  //Open the Utilities binary file
  //binary file name was included in main.cc and passed in
  //as a parameter to this function
  binary_scanner.OpenFile(binary_filename);
  while(binary_scanner.HasNext()){
    ascii_input += binary_scanner.Next();
  }
  for(auto iter = ascii_input.begin(); iter != ascii_input.end(); ++iter){
    character = *iter;
    character_to_int = static_cast<int>(character);
    binary_str = globals_.DecToBitString(character_to_int, 8); 
    binary_strings.push_back(binary_str);
    


    Utils::log_stream << "Character is " << Utils::Format(character) 
      << " is in ASCII: " << binary_str << endl;
    //Utilize the binary to decimal function
    //static cast back to character
    //back_to_character
    //ascii_input and compare the characters

  }
  binary_scanner.Close();
  //Write time to log_stream
  */
  
  Utils::log_stream << "MACHINE IS NOW" << endl << this->ToString() << endl;

#ifdef EBUG
  Utils::log_stream << "leave Load\n"; 
#endif
}

/******************************************************************************
 * Function 'ToString'.
 *
 * This outputs in the prettyprinted string:
 *   PC, the program counter
 *   ACC, the accumulator, as an integer and as a bitstring
 *   a dump of memory from 0 through the max memory for this program
 *
 * Returns:
 *   the prettyprint string for printing
**/
string Interpreter::ToString() {
#ifdef EBUG
  Utils::log_stream << "enter ToString\n"; 
#endif

  string s = "";

  s += "PC    " + Utils::Format(pc_, 8) + "\n";
  
  int twoscomplement = this->TwosComplementInteger(accum_);
  s += "ACCUM " + Utils::Format(twoscomplement, 8)
                + " " + globals_.DecToBitString(accum_, 16)
                + "\n\n";

  int memorysize = memory_.size();
  for (int outersub = 0; outersub < memorysize; outersub += 4) {
    s += "MEM " + Utils::Format(outersub, 4)
                + "-"
                + Utils::Format(outersub+3, 4);
    for (int innersub = outersub; innersub < outersub + 4; ++innersub) {
      if (innersub < memorysize) {
        s += " " + memory_.at(innersub);
      }
    }
    s += "\n";
  }

#ifdef EBUG
  Utils::log_stream << "leave ToString\n"; 
#endif

  return s;
}
/******************************************************************************
 * Function 'TwosComplementInteger'.
 *
 * This converts a 16 bit integer into the plus or minus 15 bit integer
 * that is the integer of the 2s complement bit pattern.
 *
 * Parameter:
 *   what - the integer value to convert to the integer that is the 2s compl
 *
 * Returns:
 *   the converted value
**/
int Interpreter::TwosComplementInteger(int what) {
#ifdef EBUG
  Utils::log_stream << "enter TwosComplementInteger\n"; 
#endif

  int twoscomplement = (what > 32768) ? what - 65536 : what;

#ifdef EBUG
  Utils::log_stream << "leave TwosComplementInteger\n"; 
#endif

  return twoscomplement;
}
