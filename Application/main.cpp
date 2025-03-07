/************************************************** Description
 * *******************************************************/
/*
    File : main.cpp
    Programmer : Mohammad Lotfi
    Used : main
    Design Pattern : none
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2025/2/27
*/
/************************************************** Warnings
 * **********************************************************/
/*
    Only for learning
*/
/************************************************** Wizards
 * ***********************************************************/
/*
    Nothing
*/
/************************************************** Includes
 * **********************************************************/
#include "Core/Define.hpp"
#include "LLM.hpp"
#include "Module/Print.hpp"
/************************************************** Defineds
 * **********************************************************/
/*
    Nothing
*/
/************************************************** Names
 * *************************************************************/
/*
    Nothing
*/
/************************************************** Variables
 * *********************************************************/
/*
    Nothing
*/
/************************************************** Opjects
 * ***********************************************************/
/*
    Nothing
*/
/************************************************** Functions
 * *********************************************************/
/* The main function start of program in cpp language */
int main() {
  /* Get the content data by length */
  char *Packet_Length_str = getenv("CONTENT_LENGTH");
  int Packet_Length = Packet_Length_str ? atoi(Packet_Length_str) : 0;
  if (Packet_Length > 0) {
    char *Packet_Request = (char *)malloc(Packet_Length + 1);
    fread(Packet_Request, 1, Packet_Length, stdin);
    Packet_Request[Packet_Length] = '\0';
    /* send data to analize module */
    LLM::Status status = llm.Analize(Packet_Request, Packet_Length, Print);
    return status;
  } else {
    Print((char *)"Content-type: application/json\n\n");
    Print((char *)"{\"error\": \"No data received\"}\n");
  }
  return 0;
}
/************************************************** Tasks
 * *************************************************************/
/*
    Nothing
*/
/************************************************** Vectors
 * ***********************************************************/
/*
    Nothing
*/
/**********************************************************************************************************************/
