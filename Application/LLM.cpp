/************************************************** Description
 * *******************************************************/
/*
    File : LLM.cpp
    Programmer : Mohammad Lotfi
    Used : analize packet
    Design Pattern : none
    Types of memory : Heap & Stack
    Total Tread : Nothing
    Site : https://www.mahsen.ir
    Tel : +989124662703
    Email : info@mahsen.ir
    Last Update : 2025/3/7
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
#include "LLM.hpp"
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
LLM llm;
/************************************************** Functions
 * *********************************************************/
static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                            void *userp) {
  ((std::string *)userp)->append((char *)contents, size * nmemb);
  return size * nmemb;
}
/*--------------------------------------------------------------------------------------------------------------------*/
// Callback function to process query results
static int sqlite3_select_testers_callback(void *NotUsed, int argc, char **argv,
                                           char **azColName) {
  llm.Respond_Data["Value"].push_back(string((char *)argv[1]) + ":" +
                                      string((char *)argv[2]));
  return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
// Callback function to process query results
static int sqlite3_select_devices_callback(void *NotUsed, int argc, char **argv,
                                           char **azColName) {
  llm.Respond_Data["Value"].push_back(string((char *)argv[1]));
  return 0;
}
/*--------------------------------------------------------------------------------------------------------------------*/
LLM::Status LLM::Analize(char *Request_Packet, int Request_Length,
                         void (*CallBack)(char *Respond_Packet)) {
  Status status;
  string Respond_Packet;
  string Command;
  string Value;
  status = Status::Success;

  CallBack((char *)"Content-type: application/json\n\n");

  Request_Data = json::parse(Request_Packet);
  Command = Request_Data["Command"].get<std::string>();
  Respond_Data["Command"] = Command;
  Respond_Data["Value"] = "Command_Not_Find";
  if (Command == "TEST") {
    Respond_Data["Value"] = "OK";
  } else if (Command == "QUESTION") {
    Respond_Data["Value"] = "Value_Not_Proccess";
    Value = Request_Data["Value"].get<std::string>();
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    // Default Ollama API endpoint
    std::string url = "http://localhost:11434/api/generate";
    json requestData;
    json respondData;
    requestData["model"] = "phi3:medium";
    requestData["stream"] = false;
    requestData["prompt"] = Value;

    curl = curl_easy_init();
    if (curl) {
      // Set Ollama API URL
      curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

      // JSON data for the request
      readBuffer = requestData.dump();
      const char *json_data = readBuffer.c_str();

      // Set HTTP headers
      struct curl_slist *headers = NULL;
      headers = curl_slist_append(headers, "Content-Type: application/json");
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

      // Set POST request
      curl_easy_setopt(curl, CURLOPT_POST, 1L);
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

      // Response handling
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

      // Perform the request
      res = curl_easy_perform(curl);
      if (res != CURLE_OK) {
        Println((char *)curl_easy_strerror(res));
        // std::cerr << "curl_easy_perform() failed: " <<
        // curl_easy_strerror(res) << std::endl;
      } else {
        // Find the last '{' and corresponding '}'
        size_t lastOpeningBrace = readBuffer.rfind('{');
        size_t lastClosingBrace = readBuffer.find('}', lastOpeningBrace);
        // Extract the second JSON string
        respondData = json::parse(readBuffer.substr(
            lastOpeningBrace, lastClosingBrace - lastOpeningBrace + 1));
        Respond_Data["Value"] =
            respondData["response"].get<std::string>().c_str();
      }
      // Cleanup
      curl_easy_cleanup(curl);
      curl_slist_free_all(headers);
    }
  } else if (Command == "ADD_TESTER") {
    sqlite3 *db;
    char *errorMessage = 0;
    /* Split IP:PORT */
    Value = Request_Data["Value"].get<std::string>();
    vector<string> IP_Port = DEFINE_split(Value, ':');
    // Open database (creates if not exists)
    if (sqlite3_open("database.db", &db)) {
      status = Status::Failed;
      return status;
    }
    /* Create Table if not exist */
    const char *sqlCreateTable =
        "CREATE TABLE IF NOT EXISTS Testers (ID INTEGER PRIMARY KEY, IP TEXT, "
        "Port INTEGER, Name TEXT);";
    sqlite3_exec(db, sqlCreateTable, 0, 0, &errorMessage);
    /* Insert to DB */
    string sqlInsertData = "INSERT INTO Testers (IP, Port, Name) VALUES ('" +
                           IP_Port[0] + "', " + IP_Port[1] + ", '" + "Tester-" +
                           IP_Port[0] + "');";
    sqlite3_exec(db, sqlInsertData.c_str(), 0, 0, &errorMessage);
    sqlite3_close(db);
  } else if (Command == "GET_TESTERS") {
    sqlite3 *db;
    char *errorMessage = 0;
    // Open database (creates if not exists)
    if (sqlite3_open("database.db", &db)) {
      status = Status::Failed;
      return status;
    }
    const char *sqlSelectData = "SELECT * FROM Testers;";
    Respond_Data["Value"] = nlohmann::json::array();
    sqlite3_exec(db, sqlSelectData, sqlite3_select_testers_callback, 0,
                 &errorMessage);
    sqlite3_close(db);
  } else if (Command == "RM_TESTER") {
    sqlite3 *db;
    char *errorMessage = 0;
    /* Split IP:PORT */
    Value = Request_Data["Value"].get<std::string>();
    vector<string> IP_Port = DEFINE_split(Value, ':');
    // Open database (creates if not exists)
    if (sqlite3_open("database.db", &db)) {
      status = Status::Failed;
      return status;
    }
    /* remove record of Table */
    string sqlRMTable = "DELETE FROM Testers WHERE IP = '" + IP_Port[0] +
                        "' AND Port = " + IP_Port[1] + ";";
    sqlite3_exec(db, sqlRMTable.c_str(), 0, 0, &errorMessage);
    sqlite3_close(db);
  } else if (Command == "UPLOAD_DEVICES") {
    sqlite3 *db;
    char *errorMessage = 0;
    string filename = Request_Data["filename"].get<std::string>();
    string content = Request_Data["content"].get<std::string>();
    /* Split Serials */
    vector<string> Serials = DEFINE_split(content, '\n');
    // Open database (creates if not exists)
    if (sqlite3_open("database.db", &db)) {
      status = Status::Failed;
      return status;
    }
    /* Create Table if not exist */
    const char *sqlCreateTable = "CREATE TABLE IF NOT EXISTS Devices (ID "
                                 "INTEGER PRIMARY KEY, Serial TEXT, "
                                 "Report TEXT);";
    sqlite3_exec(db, sqlCreateTable, 0, 0, &errorMessage);
    /* remove record of Table */
    string sqlRMTable = "DELETE FROM Devices;";
    sqlite3_exec(db, sqlRMTable.c_str(), 0, 0, &errorMessage);
    /* Insert to DB */
    for (string &s : Serials) {
      string sqlInsertData =
          "INSERT INTO Devices (Serial, Report) VALUES ('" + s + "', '');";
      sqlite3_exec(db, sqlInsertData.c_str(), 0, 0, &errorMessage);
    }
    sqlite3_close(db);
    Respond_Data["Value"] = "OK";
  } else if (Command == "GET_DEVICES") {
    sqlite3 *db;
    char *errorMessage = 0;
    // Open database (creates if not exists)
    if (sqlite3_open("database.db", &db)) {
      status = Status::Failed;
      return status;
    }
    const char *sqlSelectData = "SELECT * FROM Devices;";
    Respond_Data["Value"] = nlohmann::json::array();
    sqlite3_exec(db, sqlSelectData, sqlite3_select_devices_callback, 0,
                 &errorMessage);
    sqlite3_close(db);
  }

  Respond_Packet = Respond_Data.dump();
  CallBack((char *)Respond_Packet.c_str());

  return status;
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
