/*
 * Homework 6
 * File:   program6.cc
 * Author: Rohan Arya
 * Email:  rxa160430@utdallas.edu
 */


#include <iostream>
#include "cdk.h"
#include <stdint.h>
#include <fstream>
#include <string>
#include <sstream>

#define MATRIX_WIDTH 3
#define MATRIX_HEIGHT 5
#define BOX_WIDTH 17
#define MATRIX_NAME_STRING "Binary File Contents"

using namespace std;

const int maxRecordStringLength = 25;

class BinaryFileHeader
{
public:

  uint32_t magicNumber;
  uint32_t versionNumber;
  uint64_t numRecords;

};

class BinaryFileRecord
{
public:

  uint8_t strLength;
  char stringBuffer[maxRecordStringLength];
};

int
main ()
{

  WINDOW *window;
  CDKSCREEN *cdkscreen;
  CDKMATRIX *myMatrix;		// CDK Screen Matrix
  BinaryFileHeader *temp = new BinaryFileHeader ();

  ifstream input_file ("cs3377.bin", ios::in | ios::binary);
  if(!input_file.is_open())
    {
      cout<<"Error opening file!\n";
      exit(-1);
    }
  input_file.read ((char *) temp, sizeof (BinaryFileHeader));
  // Remember that matrix starts out at 1,1.
  // Since arrays start out at 0, the first entries
  // below ("R0", and "C0") are just placeholders
  // 
  // Finally... make sure your arrays have enough entries given the
  // values you choose to set for MATRIX_WIDTH and MATRIX_HEIGHT
  // above.

  const char *rowTitles[] = { "0", "a", "b", "c", "d", "e", "f" };
  const char *columnTitles[] = { "0", "a", "b", "c", "d", "e", "f" };
  int boxWidths[] =
    { BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH, BOX_WIDTH };
  int boxTypes[] = { vMIXED, vMIXED, vMIXED, vMIXED, vMIXED, vMIXED };

  /*
   * Initialize the Cdk screen.
   *
   * Make sure the putty terminal is large enough
   */
  window = initscr ();
  cdkscreen = initCDKScreen (window);

  /* Start CDK Colors */
  initCDKColor ();

  /*
   * Create the matrix.  Need to manually cast (const char**) to (char **)
   */
  myMatrix =
    newCDKMatrix (cdkscreen, CENTER, CENTER, MATRIX_HEIGHT, MATRIX_WIDTH,
		  MATRIX_HEIGHT, MATRIX_WIDTH, MATRIX_NAME_STRING,
		  (char **) rowTitles, (char **) columnTitles, boxWidths,
		  boxTypes, 1, 1, ' ', ROW, true, true, false);

  if (myMatrix == NULL)
    {
      printf ("Error creating Matrix\n");
      _exit (1);
    }

  /* Display the Matrix */
  drawCDKMatrix (myMatrix, true);

  /*
   * Dipslay a message
   */

  string h1 = "Magic: 0x";
  stringstream ss;
  ss << hex << uppercase << temp->magicNumber;
  h1 += ss.str ();

  string h2 = "Version: ";
  stringstream ss1;
  ss1 << temp->versionNumber;
  h2 += ss1.str ();

  string h3 = "NumRecords: ";
  stringstream ss2;
  ss2 << temp->numRecords;
  h3 += ss2.str ();

  setCDKMatrixCell (myMatrix, 1, 1, h1.c_str ());
  setCDKMatrixCell (myMatrix, 1, 2, h2.c_str ());
  setCDKMatrixCell (myMatrix, 1, 3, h3.c_str ());
  drawCDKMatrix (myMatrix, true);

  int i = 2;

  BinaryFileRecord *temp1 = new BinaryFileRecord ();
  while (input_file.read ((char *) temp1, sizeof (BinaryFileRecord))
	 && i <= 5)
    {
      string s1, s2;
      stringstream ss1, ss2;
      ss2 << temp1->stringBuffer;
      s2 += ss2.str ();
      ss1 << "strlen: " << static_cast<uint16_t> (temp1->strLength);
      s1 += ss1.str ();

      setCDKMatrixCell (myMatrix, i, 1, s1.c_str ());
      setCDKMatrixCell (myMatrix, i, 2, s2.c_str ());
      i++;
    }



  drawCDKMatrix (myMatrix, true);	/* required  */

  /* So we can see results, pause until a key is pressed. */
  unsigned char x;
  cin >> x;
  input_file.close ();
  // Cleanup screen
  endCDK ();
}
