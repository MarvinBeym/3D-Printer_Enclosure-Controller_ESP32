/**
 * A Simple class to interact with the nextion display.
 * Copyright (C) 2020  Marvin Beym
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

#include "SimpleNextion.h"

SimpleNextion::SimpleNextion(HardwareSerial &nextionSerial, uint32_t baudrate): nextionHWSerial(&nextionSerial){
  nextionHWSerial->begin(baudrate);
}

String SimpleNextion::ReadCommand(){
  char byte;
  String cmd;
  int countEnd = 0;
  while(Serial2.available()>0){
    delay(10);
    if(Serial2.available()>0){
      byte = Serial2.read();
      cmd += byte;
      if(byte == 0xff){countEnd++;}
      if(countEnd == 3){break;}
    }
  }
  String cmdConstruct = "";
  switch(cmd[0]){
    case 'e'://0x65
      countEnd = 0;
      for(int i = 0; i < cmd.length(); i++){
        if(cmd[i] == 0xff){countEnd++;}
        cmdConstruct += String(cmd[i], HEX);
        if(countEnd == 3){return cmdConstruct;}
        cmdConstruct += " ";
      }
      break;
    case 'f'://0x66
      return String(cmd[1], DEC);
      break;
    case 'g'://0x67
      cmd = String(cmd[2], DEC) + "," + String(cmd[4], DEC) +","+ String(cmd[5], DEC);
	    return cmd;
      break;
    case 'h'://0x68
      cmd = String(cmd[2], DEC) + "," + String(cmd[4], DEC) +","+ String(cmd[5], DEC);
      cmd = "68 " + cmd;	
      return cmd;
      break;
    case 'p'://0x70
      cmd = cmd.substring(1, cmd.length()-3);
      cmd = "70 " + cmd;
      return cmd;
      break;
    default: 
      return cmd;
    break;
  }
  return "";
}
void SimpleNextion::SendCommand(const char* cmdToSend){
  while(Serial2.available()){ Serial2.read(); }
  Serial2.print(cmdToSend);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);
}
void SimpleNextion::SetPage(const char* page){
  String cmd = "page " + String(page);
  SendCommand(cmd.c_str());
  delay(200);
}
void SimpleNextion::SetCompValue(String compName, int value){
  String newValue = compName + ".val=" + value;
  SendCommand(newValue.c_str());
}
unsigned int SimpleNextion::GetCompValue(String compName){
  unsigned int value = 0;
  String cmd = "get " + compName + ".val";
  SendCommand(cmd.c_str());
  Serial2.setTimeout(20);
  uint8_t recievedBytes[8] = {0};
  if(sizeof(recievedBytes) != Serial2.readBytes((char *)recievedBytes, sizeof(recievedBytes))){
    return -1;
  }

  //Check if value is the value requested
  if(recievedBytes[0] == 0x71 && recievedBytes[5] == 0xff && recievedBytes[6] == 0xff && recievedBytes[7] == 0xff){
    value = (recievedBytes[4] << 24) | (recievedBytes[3] << 16) | (recievedBytes[2] << 8) | (recievedBytes[1]);//Little-endian convertion
  }
  return value;
}
void SimpleNextion::SetCompText(String compName, String value){
  String newText = compName + ".txt=\"" + value + "\"";
  SendCommand(newText.c_str());
}