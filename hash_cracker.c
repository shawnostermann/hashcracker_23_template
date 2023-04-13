/* File:     hash_cracker.c
 * Ostermann's example solution
 * April 11, 2023
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "sha256.h"
#include <mpi.h>

// routine definitions
void Get_input(int my_rank, int comm_sz, char *salt, char *hash, char *alphabet, int *ppwd_len);
void ix_to_password(char *alphabet, int pwd_length, int ix, char *guess);
int FindHash(int my_rank, char *salt, char *hash, char *alphabet, int pwd_length, int first_pwd_ix, int num_pwds);

const int bufsize=256;

int debug = 0;  // must be turned OFF for github grading


// return base ^^ degree
unsigned power(unsigned base, unsigned degree)
{
    unsigned result = 1;
    unsigned term = base;
    while (degree) {
        if (degree & 1)
            result *= term;
        term *= term;
        degree = degree >> 1;
    }
    return result;
}


int main(int argc, char *argv[]) {
   int my_rank;            // my rank, CPU number
   int comm_sz;            // number of CPUs in the group
   char salt[bufsize];     // the salt used to generate the hash (a string, like "foobar")
   char hash[bufsize];     // the password hash we're trying to find (a string, like 
                           //      "AiVIwiOWe.ZrPkJTJs30soPiP2dYmpbNm8faGkAMBr8")
   char alphabet[bufsize]; // the alphabet that the passwords are taken from (a string, like "0123456789")
   int password_length;    // how many characters (from that alphabet) are in the password
   int ix1;                // the first password index that _I_ am supposed to start testing
   int ix2;                // the last password index that _I_ am supposed to test
   int total_passwords;    // how many do we need to check?
   int my_answer_ix;       // the answer that I came up with
   int answer = -1;        // the best answer from anybody

   
   /* Let the system do what it needs to start up MPI */
   MPI_Init(NULL, NULL);

   /* Get my process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Find out how many processes are being used */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   if ((argc == 2) && (strcmp(argv[1],"-d")==0))
      ++debug;

   /* Print the status */
   if (debug && my_rank == 0) {
      printf("Comm size: %d\n", comm_sz);
   }

   Get_input(my_rank, comm_sz, salt, hash, alphabet, &password_length);

   /* Print the status */
   if (debug && my_rank == 0) {
      printf("Target hash: '%s'\n", hash);      
      printf("Salt used: '%s'\n", salt);
      printf("Password alphabet: %s\n", alphabet);
      printf("Password Length: %d\n", password_length);
   }

   // calculate the total number of possible passwords for the given 
   // alphabet and password length
   // how many total passwords do we need to check?
   // you will need to calculate this!
   total_passwords = 666;
   fprintf(stderr,"total_passwords not finished\n");

   // Print the workload
   if (debug && my_rank == 0) {
      printf("Total passwords: %d\n", total_passwords);
      printf("Passwords per rank: %d\n", total_passwords / comm_sz);
   }

   // what work does THIS cpu need to do
   // you will need to calculate this
   ix1 = 42;    // first password index to check
   ix2 = 99;    // last password to check
   fprintf(stderr,"ix1 and ix2 not finished\n");

   // perform the assigned work
   my_answer_ix = FindHash(my_rank, salt, hash, alphabet, password_length, ix1, ix2);

   if (debug)
      printf("CPU %d returns answer %d\n", my_rank, my_answer_ix);

   // gather up the max password ix found */
   // you will need to finish this
   // you need to use MPI_Reduce to find the MAXIMUM value of my_answer_ix of all the CPUs
   // MPI_Reduce( /* fill in these fields */, 0, MPI_COMM_WORLD);
   fprintf(stderr,"MPI_Reduce not finished\n");

   /* Print the result */
   if (my_rank == 0) {
      if (answer >= 0) {
         char guess[bufsize];
         ix_to_password(alphabet, password_length, answer, guess);
         if (debug) {
            printf("Found Answer: %d (%s)\n", answer, guess);
         } else { 
            printf("%d: '%s'\n", answer, guess);
         }
      } else {
            printf("not found\n");
      }
   }

   /* Shut down MPI */
   MPI_Finalize();

   return 0;
} /*  main  */


// routine to get user input from process 0 and 
// broadcast the input to all other processes
void Get_input(int my_rank, int comm_sz, char *salt, 
               char *hash, char *alphabet, int *ppwd_len) {

   if (my_rank == 0) {
      int ret;
      if (isatty(0))
         fprintf(stderr,"Enter salt, hash, alphabet, and passwdlength\n");
      ret = scanf("%s %s %s %d", salt, hash, alphabet, ppwd_len);
      if (ret != 4) {
         fprintf(stderr,"Invalid arguments provided (%d)\n", ret);
         exit(0);
      }
   } 

   // you will need to complete the 4 MPI_Bcast calls needed to share those 4 parameters with everybody...
   // MPI_Bcast(salt,      /* fill in */     0, MPI_COMM_WORLD);
   // MPI_Bcast(hash,      /* fill in */     0, MPI_COMM_WORLD);
   // MPI_Bcast(alphabet,  /* fill in */     0, MPI_COMM_WORLD);
   // MPI_Bcast(ppwd_len,  /* fill in */     0, MPI_COMM_WORLD);
   fprintf(stderr,"MPI_Bcast not finished\n");
} 




// you need to write this routine VERY carefully
// its purpose is to fill in the array "guess" with the ix'th 
// password from the given alphabet
// you need to write this routine VERY carefully
// its purpose is to fill in the array "guess" with the ix'th password from the given alphabet
void ix_to_password(char *alphabet, int pwd_length, int ix, char *guess) {
   
   fprintf(stderr,"ix_to_password not finished\n");

   strcpy(guess,"xxx");
   guess[pwd_length] = '\00';  // make sure it's NULL terminated
}


// you need to finish this routine
// it needs to loop and generate each password from first_pwd_ix to last_pwd_ix
// for each one, compare against "target_hash" to check for a match
// if it matches, you should immediately return the ix of the matching password
// if none found, return -1
// a routine that generates the password at each index from ix1 to ix2, hashes 
// said password with the given salt, then checks it versus the target hash
// returns -1 if not found, if found, returns index immediately
int FindHash(int my_rank, char *salt, char *target_hash, char *alphabet, int pwd_length, int first_pwd_ix, int last_pwd_ix) {
   char guess[pwd_length+2];
   char genhash[bufsize];

   fprintf(stderr,"FindHash not finished\n");

   for (int ix=first_pwd_ix; ix <= last_pwd_ix; ++ix) {
      // finish me

      // 1: generate the "guess" that goes with that index...


      // 2: do the real work to turn salt/guess into the generated hash in genhash
      // using this function:
      // sha_hashonly(guess, salt, genhash);

      if (debug){
         printf("%4d: %s\t\t\'%s\'\t\t\t\'%s\'\n", ix, guess, genhash, target_hash);
      }

   }

   return(-1);
}
