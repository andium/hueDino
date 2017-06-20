#include "Client.h"
#include "RestClient.h"

//#define HTTP_DEBUG true
#ifdef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifndef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string)
#endif

RestClient::RestClient(void) {
  
}

RestClient::RestClient(Client& netClient, const char* _host) {
  host = _host;
  port = 80;
  num_headers = 0;
  contentType = "x-www-form-urlencoded";	// default
  this->client = &netClient;
  this->responseBody = "";
  this->timeout = 1000;     // default. TODO: add a setter function
}

RestClient::RestClient(Client& netClient, const char* _host, int _port) {
  host = _host;
  port = _port;
  num_headers = 0;
  contentType = "application/x-www-form-urlencoded";	// default
  this->client = &netClient;
}

// GET path
int RestClient::get(String path){
  return request("GET", path, "");
}

// POST path and body
int RestClient::post(String path, String body){
  return request("POST", path, body);
}

// PUT path and body
int RestClient::put(String path, String body){
  return request("PUT", path, body);
}

// DELETE path
int RestClient::del(String path){
  return request("DELETE", path, "");
}

// DELETE path and body
int RestClient::del(String path, String body ){
  return request("DELETE", path, body);
}

void RestClient::setHeader(String header){
  headers[num_headers] = header;
  num_headers++;
}

void RestClient::setContentType(String contentTypeValue){
  contentType = contentTypeValue;
}

// The mother- generic request method.
//
int RestClient::request(const char* method, String path, String body){

  HTTP_DEBUG_PRINT("HTTP: connect\n");

  if(client->connect(host, port)){
    HTTP_DEBUG_PRINT("HTTP: connected\n");
    HTTP_DEBUG_PRINT("REQUEST: \n");
    // Make a HTTP request line:
    String requestString = method;
    requestString += " ";
    requestString += path;
    requestString += " HTTP/1.1";
    requestString += "\r\n";
    for(int i=0; i<num_headers; i++){
      requestString += headers[i];
      requestString += "\r\n";
    }
    requestString += "Host: ";
    requestString += host;
    requestString += "\r\n";
    requestString += "Connection: close";
    requestString += "\r\n";
    //TODO: deal with binary content
    if(body != ""){
      requestString += "Content-Length: ";
      requestString += body.length();
      requestString += "\r\n";
      requestString += "Content-Type: ";
      requestString += contentType;
      requestString += "\r\n\r\n";
      requestString += body;
    }
    requestString += "\r\n\r\n";
    client->print(requestString);
    HTTP_DEBUG_PRINT(requestString);

    // make sure you've sent all bytes. Ugly hack.
    // TODO: check output buffer instead?
    delay(50);

    HTTP_DEBUG_PRINT("HTTP: call getResponse\n");
    int statusCode = getResponse();
    HTTP_DEBUG_PRINT("HTTP: return getResponse\n");

    //cleanup
    // only stop if server disconnected. Otherwise you can
    // get in an infinite loop in getResponse:
    if (!client->connected()) {
      HTTP_DEBUG_PRINT("HTTP: stop client\n");
      num_headers = 0;
      //client->stop();
      HTTP_DEBUG_PRINT("HTTP: client stopped\n");
    }
    return statusCode;
  }else{
    HTTP_DEBUG_PRINT("HTTP Connection failed\n");
    return 0;
  }
}

int RestClient::getResponse() {
  this->requestStart = millis();
  // an http request ends with a blank line
  boolean currentLineIsBlank = true;
  boolean httpBody = false;
  boolean inStatus = false;
  // clear response string:
  this->responseBody = "";

  char statusCode[4];
  int i = 0;
  int code = 0;

  HTTP_DEBUG_PRINT("HTTP: RESPONSE: \n");
  while (client->connected()) {
  //  HTTP_DEBUG_PRINT(".");
    if (client->available()) {
    //  HTTP_DEBUG_PRINT(",");

      char c = client->read();
      HTTP_DEBUG_PRINT(c);

      if(c == ' ' && !inStatus){
        inStatus = true;
      }

      if(inStatus && i < 3 && c != ' '){
        statusCode[i] = c;
        i++;
      }
      if(i == 3){
        statusCode[i] = '\0';
        code = atoi(statusCode);
      }

      if(httpBody){
        // add this char to the responseBody
        this->responseBody += c;
      }
      else
      {
          if (c == '\n' && currentLineIsBlank) {
            httpBody = true;
          }

          if (c == '\n') {
            // you're starting a new line
            currentLineIsBlank = true;
          }
          else if (c != '\r') {
            // you've gotten a character on the current line
            currentLineIsBlank = false;
          }
      }
    } else {
      // there is a condition where client connects
      // and available() always <= 0. So timeout is here to catch that:
      if (millis() - this->requestStart > this->timeout) return 0;
    }
  }

  HTTP_DEBUG_PRINT("HTTP: return getResponse3\n");
  return code;
}
