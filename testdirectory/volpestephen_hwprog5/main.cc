#include "main.h"

/****************************************************************
 * Main program for Pullet16 Interpreter program.
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Used with permission and modified by: Stephen Volpe
 * Date: 1 November 2017
 *
 * NOTE that none of the input parameters have file extensions.
 *
**/

static const string kTag = "Main: ";

int main(int argc, char *argv[]) {
  string exec_filename = "dummyexecname";
  string binary_filename = "dummybinaryname";
  string data_filename = "dummydataname";
  string out_filename = "dummyoutname";
  string log_filename = "dummylogname";

  Scanner exec_scanner;
  Scanner data_scanner;
  ofstream out_stream;

  Interpreter interpreter;

  Utils::CheckArgs(4, argc, argv, "execfilename datafilename outfilename logfilename");
  exec_filename = static_cast<string>(argv[1]) + ".txt";
  binary_filename = static_cast<string>(argv[1]) + ".bin";
  data_filename = static_cast<string>(argv[2]) + ".txt";
  out_filename = static_cast<string>(argv[3]) + ".txt";
  log_filename = static_cast<string>(argv[4]) + ".txt";

  Utils::LogFileOpen(log_filename);
  exec_scanner.OpenFile(exec_filename);
  data_scanner.OpenFile(data_filename);
  Utils::FileOpen(out_stream, out_filename);

  Utils::log_stream << kTag << "Beginning execution" << endl;
  Utils::log_stream.flush();

  Utils::log_stream << kTag << "logfile '" << log_filename << "'" << endl;

  interpreter.Load(exec_scanner, binary_filename);
  exec_scanner.Close();
  interpreter.Interpret(data_scanner, out_stream);

  Utils::log_stream << kTag << "Ending execution" << endl;
  Utils::log_stream.flush();

  Utils::FileClose(out_stream);
  Utils::FileClose(Utils::log_stream);

  return 0;
}

