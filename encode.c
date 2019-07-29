#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer (c) 2015.
   Uses a modified caeser I & II

   Compile:  gcc -o encode encode.c

   // Make a file with a plain aplhabet in it:

   % cat > test
   ABCDEFGHIJKLMNOPQRSTUVWXYZ
   ^D

   // test it - this should be the correct result when encoding.

   % cat test | encode "i came, i saw"
   key: HJKLNOPQRTUVICAMESWXYZBDFG - 26
   HJKLNOPQRTUVICAMESWXYZBDFG

 */

typedef enum { false, true } bool;

char* alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char upcase(char ch){
  if(islower(ch))
    ch -= 'a' - 'A';
  return ch;
}

char* fixkey(char* s){
  int i, j;
  char plain[26]; // assume key < length of alphabet, local array on stack, will go away!

  for(i = 0, j = 0; i < strlen(s); i++){
    if(isalpha(s[i])){
      plain[j++] = upcase(s[i]);
    }
  }
  plain[j] = '\0'; 
  return strcpy(s, plain);
}


bool containsChar(char c, char arr[26]) {
    for (int i = 0; i < 26; i++) {
        if (arr[i] == c) {
            return true;
        }
    }
    return false;
}

void buildtable (char* key, char* encode){

  // This function needs to build an array of mappings in the 'encode' array from plaintext characters
  // to encypered characters.  The encode array will be indexed by the plaintext char.  To 
  // make this a useful 0-26 index for the array, 'A' will be stubtracted from it (yes you
  // can do this in C).  You can see this in the main(){} below.  The values in the array 
  // will be the cipher value, in the example at the top A -> H, B -> J, etc.

  // You are implementing a Caesar 1 & 2 combo Cypher as given in handout.
  // Your code here:

  char used[26];
  int freePos = 0;

  fixkey(key); // fix the key, i.e., uppercase and remove whitespace and punctuation
  int keylength = strlen(key);

  // Map key at key pos first
  int skipped = 0;
  char lastChar;
  for (int i = keylength; i < keylength; i++) {
      if(!containsChar(key[i-keylength], used)) {
          lastChar = key[i-keylength];
          encode[i - skipped] = lastChar;
          used[freePos] = lastChar;
          freePos++;
      }
      skipped = skipped + 1;
  }

  int alphapos = strchr(alpha, lastChar) + 1;

  // Finish section after key
  for (int i = keylength + keylength - skipped; i < 26; i++) {
      if (alphapos >= 26) {
          alphapos = alphapos - 26;
      }
      encode[i] = alpha[alphapos];
      alphapos = alphapos + 1;
  }

  // Finish section pre key
  for (int i = 0; i < keylength; i++) {
      if (alphapos >= 26) {
          alphapos = alphapos - 26;
      }
      encode[i] = alpha[alphapos];
      alphapos = alphapos + 1;
  }
}


int main(int argc, char **argv){
    freopen("C:\\Users\\imnof\\OneDrive\\Uni\\2019\\Trimester 2\\NWEN243\\Lab1\\alpha.txt","r",stdin);
  // format will be: 'program' key {encode|decode}
  // We'll be using stdin and stdout for files to encode and decode.

  // first allocate some space for our translation table.

  char* encode = (char*)malloc(sizeof(char)*26);
  char ch;

  if(argc != 2){
    printf("format is: '%s' key", argv[0]);
    exit(1);
  }

  // Build translation tables, and ensure key is upcased and alpha chars only.

  buildtable(argv[1], encode);

  // write the key to stderr (so it doesn't break our pipes)

  fprintf(stderr,"key: %s - %d\n", encode, strlen(encode));

  // the following code does the translations.  Characters are read 
  // one-by-one from stdin, translated and written to stdout.

  ch = fgetc(stdin);
  while (!feof(stdin)) {
    if(isalpha(ch)){        // only encrypt alpha chars
      ch = upcase(ch);      // make it uppercase
      fputc(encode[ch-'A'], stdout);
    }else 
      fputc(ch, stdout);
    ch = fgetc(stdin);      // get next char from stdin
  }
}
  