#include "globals.h"

/****************************************************************
 * Code file for the 'Globals' class for global constants and
 * functions.
 *
 * Documentation for the functions is in the header for those
 * functions.
 *
 * Author/copyright:  Duncan Buell
 * Used with permission and modified by: Stephen Volpe
 * Date: 1 November 2017
 *
**/

/******************************************************************************
 * Function 'BitStringToDec'.
 * Convert a bit string to a decimal value.
 * This is merely a wrapper for the 'stoi' function.
 *
 * Parameters:
 *   thebits - the ASCII array of "bits" to be converted
 *
 * Returns:
 *   the array 'thebits' converted to an 'int'
**/
int Globals::BitStringToDec(const string thebits) const {
#ifdef EBUG
  Utils::log_stream << "enter BitStringToDec\n"; 
#endif

  // Remember that the second parameter is for positioning the pointer
  // after doing the conversion (so we don't care here) and the third
  // says we are converting from a string of digits in binary.
  int stoivalue = std::stoi(thebits, nullptr, 2);
 
#ifdef EBUG
  Utils::log_stream << "leave BitStringToDec\n"; 
#endif

  return stoivalue;
}

/******************************************************************************
 * Function 'DecToBitString'.
 * This function converts a decimal 'int' to a string of 0s and 1s.
 *
 * We only allow conversion to a string of length 12 or of length 16
 * because we only allow an address (lessequal 4096 = 2^12) or a hex
 * operand of 16 bits.
 *
 * This is basically just a wrapper for the 'bitset' function.
 *
 * Parameters:
 *   value - the value to convert
 *   how_many_bits - the length of the result
 *
 * Returns:
 *   the 'string' of bits obtained from the 'value' parameter
**/
string Globals::DecToBitString(const int value, const int how_many_bits) const {
#ifdef EBUG
  Utils::log_stream << "enter DecToBitString\n";
#endif

  string bitsetvalue = "";
  if (how_many_bits == 12) {
    bitsetvalue = std::bitset<12>(value).to_string();
  }
  else if (how_many_bits == 16) {
    bitsetvalue = std::bitset<16>(value).to_string();
  }else if(how_many_bits == 8){
    bitsetvalue = std::bitset<8>(value).to_string();
  } else {
    Utils::log_stream << "ERROR DECTOBITSTRING " << value << " "
                      << how_many_bits << endl;
    exit(0);
  }



#ifdef EBUG
  Utils::log_stream << "leave DecToBitString\n";
#endif

  return bitsetvalue;
}
