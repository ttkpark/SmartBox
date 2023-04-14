#include "pch.h"
#include "PacketCoder.h"


#pragma region Packet 암호화 매트릭스

char array890[9][8] = { 
                                     { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' } };
char array891[9][8] = {
                                     { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
};
char array892[9][8] = {
                                     { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
};
char array893[9][8] = {
                                     { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
};
char array894[9][8] = {
                                     { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
};
char array895[9][8] = {
                                     { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
};
char array896[9][8] = {
                                     { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
};
char array897[9][8] = {
                                     { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
};
char array898[9][8] = {
                                     { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
};
char array899[9][8] = {
                                     { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '=', 'P' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
};
char array900[8][9] = { 
                                     { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' } };
char array901[8][9] = {
                                     { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
};
char array902[8][9] = {
                                     { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
};
char array903[8][9] = {
                                     { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
};
char array904[8][9] = {
                                     { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
};
char array905[8][9] = {
                                     { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
};
char array906[8][9] = {
                                     { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
};
char array907[8][9] = {
                                     { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
};
char array908[8][9] = {
                                     { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
};
char array909[8][9] = {
                                     { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '=', 'n' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
};

char array100[10][8] = {
                                     { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' } };
char array101[10][8] = {
                                     { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
};
char array102[10][8] = {
                                     { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
};
char array103[10][8] = {
                                     { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
};
char array104[10][8] = {
                                     { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
};
char array105[10][8] = {
                                     { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
};
char array106[10][8] = {
                                     { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
};
char array107[10][8] = {
                                     { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
};
char array108[10][8] = {
                                     { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
                                   , { '+', '=', ';', '#', '@', '\'', ',', ':' }
};
char array109[10][8] = {
                                     { '+', '=', ';', '#', '@', '\'', ',', ':' }
                                   , { '/', '9', '3', 'q', 'F', 'X', 'm', 'e' }
                                   , { 'p', 'B', '^', 'U', 'f', '2', 'k', 'M' }
                                   , { '7', 'l', 'z', 'I', 'b', 'T', 'g', 'o' }
                                   , { 'E', '|', 'r', '!', 'v', '(', 'n', ')' }
                                   , { 'J', '1', 'c', 'y', 'H', 'Y', '~', 'P' }
                                   , { 'C', 'L', '_', 'V', 'h', '4', 'w', 'u' }
                                   , { 'N', 'O', 's', '5', 't', 'Q', 'Z', 'd' }
                                   , { 'K', 'G', 'R', '6', '-', 'x', 'A', 'S' }
                                   , { 'i', 'D', '8', 'a', '.', 'W', 'j', '0' }
};
char array110[8][10] = {
                                     { 'i', '+', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', '=', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', ';', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', '#', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', '@', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', '\'', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', ',', 'g', 'm', 'w', 'Z', 'k', 'A', '~', 'n' }
                                   , { '0', ':', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' } };
char array111[8][10] = {
                                     { '0', 'o', '+', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '=', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', ';', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '#', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', '@', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', '\'', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', ',', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', ':', 'm', 'w', 'Z', 'k', 'A', '~', 'n' }
};
char array112[8][10] = {
                                     { 'j', 'g', 'm', '+', 'w', 'Z', 'k', 'A', '~', 'n' }
                                   , { '0', 'o', 'e', '=', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', ';', 'C', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', '#', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '@', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', '\'', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', ',', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', ':', '4', 'Q', '2', 'x', 'Y', '(' }
};
char array113[8][10] = {
                                     { 'W', 'T', 'X', '4', '+', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', '=', 'Z', 'k', 'A', '~', 'n' }
                                   , { '0', 'o', 'e', 'u', ';', 'd', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', '#', 'N', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', '@', 'O', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', '\'', 's', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', ',', '5', 'U', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', ':', 't', 'f', '-', 'H', 'v' }
};
char array114[8][10] = {
                                     { '.', 'b', 'F', 'h', 't', '+', 'f', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '=', '2', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', ';', 'k', 'A', '~', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', '#', 'M', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', '@', 'p', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', '\'', 'B', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', ',', '^', 'R', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', ':', 'U', '6', 'y', '!' }
};
char array115[8][10] = {
                                     { 'a', 'I', 'q', 'V', '5', 'U', '+', '6', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '=', '-', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', ';', 'x', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', '#', 'A', '~', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', '@', 'S', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', '\'', 'K', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', ',', 'G', '1', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', ':', 'R', 'c', 'r' }
};
char array116[8][10] = {
                                     { '8', 'z', '3', '_', 's', '^', 'R', '+', 'c', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', '=', 'y', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', ';', 'H', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', '#', 'Y', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '@', '~', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', '\'', 'P', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', ',', 'J', 'E' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', ':', '1', '|' }
};
char array117[8][10] = {
                                     { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '+', '|' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', '=', 'r' }
                                   , { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', ';', '!' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', '#', 'v' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '@', '(' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '~', '\'', 'n' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', '\0', ')' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', '\0', 'E' }
};
char array118[8][10] = {
                                     { 'a', 'I', 'q', 'V', '5', 'U', '6', 'y', '!', '+' }
                                   , { 'i', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E', '=' }
                                   , { 'W', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(', ';' }
                                   , { 'D', 'l', '9', 'L', 'O', 'B', 'G', '1', '|', '#' }
                                   , { 'j', 'g', 'm', 'w', 'Z', 'k', 'A', '~', 'n', '@' }
                                   , { '8', 'z', '3', '_', 's', '^', 'R', 'c', 'r', '\'' }
                                   , { '.', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v', ',' }
                                   , { '0', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')', ':' }
};
char array119[8][10] = {
                                     { '0', '+', 'o', 'e', 'u', 'd', 'M', 'S', 'P', ')' }
                                   , { 'W', '=', 'T', 'X', '4', 'Q', '2', 'x', 'Y', '(' }
                                   , { 'a', ';', 'I', 'q', 'V', '5', 'U', '6', 'y', '!' }
                                   , { '8', '#', 'z', '3', '_', 's', '^', 'R', 'c', 'r' }
                                   , { 'D', '@', 'l', '9', 'L', 'O', 'B', 'G', '1', '|' }
                                   , { 'j', '\'', 'g', 'm', 'w', 'Z', 'k', 'A', '~', 'n' }
                                   , { '.', ',', 'b', 'F', 'h', 't', 'f', '-', 'H', 'v' }
                                   , { 'i', ':', '7', '/', 'C', 'N', 'p', 'K', 'J', 'E' }
};

#pragma endregion //Packet 암호화 매트릭스


#pragma region Packet 암 / 복호화
CString PacketCoder::DecodingPacketString(CString mty, CString str)
{
#pragma region
    CString rtn = _T("");
    char** ary = NULL;
    if (mty == "890") { ary = (char**)array890; }
    else if (mty == "891") { ary = (char**)array891; }
    else if (mty == "892") { ary = (char**)array892; }
    else if (mty == "893") { ary = (char**)array893; }
    else if (mty == "894") { ary = (char**)array894; }
    else if (mty == "895") { ary = (char**)array895; }
    else if (mty == "896") { ary = (char**)array896; }
    else if (mty == "897") { ary = (char**)array897; }
    else if (mty == "898") { ary = (char**)array898; }
    else if (mty == "899") { ary = (char**)array899; }
    else if (mty == "900") { ary = (char**)array900; }
    else if (mty == "901") { ary = (char**)array901; }
    else if (mty == "902") { ary = (char**)array902; }
    else if (mty == "903") { ary = (char**)array903; }
    else if (mty == "904") { ary = (char**)array904; }
    else if (mty == "905") { ary = (char**)array905; }
    else if (mty == "906") { ary = (char**)array906; }
    else if (mty == "907") { ary = (char**)array907; }
    else if (mty == "908") { ary = (char**)array908; }
    else if (mty == "909") { ary = (char**)array909; }
    else
    {
        return CString();
    }

    int nRow = sizeof(array890[0]), nCol = sizeof(array890)/ sizeof(array890[0]);
    if (mty.GetAt(0) == _T('9'))nRow = sizeof(array900[0]), nCol = sizeof(array900) / sizeof(array900[0]);

    for (int i = 0; i < str.GetLength(); i+=2)
    {
        rtn += GetDencodingCodePacketChar(ary, nRow, nCol, str.Mid(i, 2));
    }

    return rtn.Trim();
#pragma endregion
}

CString PacketCoder::GetDencodingCodePacketChar(char** ary, size_t sRow, size_t sCol, CString pvalue)
{
#pragma region
    CString result = _T("");

    result = ((char*)ary)[(_wtoi(pvalue.Mid(0, 1)) - 1) * sRow + (_wtoi(pvalue.Mid(1)) - 1)];
    result.Trim();

    return result;
#pragma endregion
}

/// <summary>
/// Packet 암호화. string enc = bizEncrypt.EncodingString("890", str);
/// </summary>
/// <param name="mty">890 ~ 909</param>
/// <param name="str">wzms_ko-KR_h00001/WorkZone01/bt77/er00^00^00/tp26.20^ox20.30^ps1018</param>
/// <returns></returns>
CString PacketCoder::EncodingPacketString(CString mty, CString str)
{
#pragma region
    CString rtn = _T("");
    char** ary = NULL;
    if (mty == "890") { ary = (char**)array890; }
    else if (mty == "891") { ary = (char**)array891; }
    else if (mty == "892") { ary = (char**)array892; }
    else if (mty == "893") { ary = (char**)array893; }
    else if (mty == "894") { ary = (char**)array894; }
    else if (mty == "895") { ary = (char**)array895; }
    else if (mty == "896") { ary = (char**)array896; }
    else if (mty == "897") { ary = (char**)array897; }
    else if (mty == "898") { ary = (char**)array898; }
    else if (mty == "899") { ary = (char**)array899; }
    else if (mty == "900") { ary = (char**)array900; }
    else if (mty == "901") { ary = (char**)array901; }
    else if (mty == "902") { ary = (char**)array902; }
    else if (mty == "903") { ary = (char**)array903; }
    else if (mty == "904") { ary = (char**)array904; }
    else if (mty == "905") { ary = (char**)array905; }
    else if (mty == "906") { ary = (char**)array906; }
    else if (mty == "907") { ary = (char**)array907; }
    else if (mty == "908") { ary = (char**)array908; }
    else if (mty == "909") { ary = (char**)array909; }
    else
    {
        return CString();
    }

    int nRow = sizeof(array890[0]), nCol = sizeof(array890) / sizeof(array890[0]);
    if (mty.GetAt(0) == _T('9'))nRow = sizeof(array900[0]), nCol = sizeof(array900) / sizeof(array900[0]);

    for (int i = 0; i < str.GetLength(); i++)
    {
        rtn += GetEncodingCodePacketChar(ary, nRow, nCol, str.Mid(i, 1));
    }

    rtn += mty;
    return rtn.Trim();
#pragma endregion
}

CString PacketCoder::GetEncodingCodePacketChar(char** ary, size_t sRow, size_t sCol, CString pvalue)
{
#pragma region
    int idx[2] = { 0, 0 };
    CString result = _T("");
    char* point = (char*)ary;

    for (int i = 0; i < sCol; i++)
    {
        for (int j = 0; j < sRow; j++)
        {
            if (pvalue.Trim() == point[i * sRow + j])
            {
                idx[0] = i + 1; idx[1] = j + 1;
            }
        }
    }
    result.Format(_T("%d%d"),idx[0],idx[1]);

    return result;
#pragma endregion
}
#pragma endregion //Packet 암/복호화

#pragma region Packet 암 / 복호화 10
/// <summary>
/// Packet 복호화. string denc = bizEncrypt.DecodingPacketString10("110", str);
/// </summary>
/// <param name="mty">110 ~ 119</param>
/// <param name="str">472337534367287571~</param>
/// <returns></returns>
CString PacketCoder::DecodingPacketString10(CString mty, CString str)
{
#pragma region
    CString rtn = _T("");
    char** ary = NULL;
    if (mty == "100") { ary = (char**)array100; }
    else if (mty == "101") { ary = (char**)array101; }
    else if (mty == "102") { ary = (char**)array102; }
    else if (mty == "103") { ary = (char**)array103; }
    else if (mty == "104") { ary = (char**)array104; }
    else if (mty == "105") { ary = (char**)array105; }
    else if (mty == "106") { ary = (char**)array106; }
    else if (mty == "107") { ary = (char**)array107; }
    else if (mty == "108") { ary = (char**)array108; }
    else if (mty == "109") { ary = (char**)array109; }
    else if (mty == "110") { ary = (char**)array110; }
    else if (mty == "111") { ary = (char**)array111; }
    else if (mty == "112") { ary = (char**)array112; }
    else if (mty == "113") { ary = (char**)array113; }
    else if (mty == "114") { ary = (char**)array114; }
    else if (mty == "115") { ary = (char**)array115; }
    else if (mty == "116") { ary = (char**)array116; }
    else if (mty == "117") { ary = (char**)array117; }
    else if (mty == "118") { ary = (char**)array118; }
    else if (mty == "119") { ary = (char**)array119; }
    else
    {
        return CString();
    }

    int nRow = sizeof(*array100), nCol = sizeof(array100) / sizeof(*array100);
    if (mty.GetAt(1) == _T('1')) nRow = sizeof(*array110), nCol = sizeof(array110) / sizeof(*array110);

    for (int i = 0; i < str.GetLength(); i+=4)
    {
        //Console.WriteLine("i = " + i.ToString());
        rtn += GetDencodingCodePacketChar10(ary, nRow, nCol, str.Mid(i, 4));
    }

    return rtn.Trim();
#pragma endregion
}

CString PacketCoder::GetDencodingCodePacketChar10(char** ary, size_t sRow, size_t sCol, CString pvalue)
{
#pragma region
    CString result = _T("");

    //Console.WriteLine("pvalue = " + pvalue);
    result = ((char*)ary)[(_wtoi(pvalue.Mid(0, 2)) - 1) * sRow + (_wtoi(pvalue.Mid(2)) - 1)]; //.Trim() 공백문자 때문에...
    //Console.WriteLine("GetDencodingCodePacketChar = " + result);\
    
    result.Trim();

    return result;
#pragma endregion
}

/// <summary>
/// Packet 암호화. CString enc = bizEncrypt.EncodingString10("110", str);
/// </summary>
/// <param name="mty">110 ~ 119</param>
/// <param name="str">wzms_ko-KR_h00001/WorkZone01/bt77/er00^00^00/tp26.20^ox20.30^ps1018</param>
/// <returns></returns>
CString PacketCoder::EncodingPacketString10(CString mty, CString str)
{
#pragma region
    CString rtn = _T("");
    char** ary = NULL;
    if (mty == "100") { ary = (char**)array100; }
    else if (mty == "101") { ary = (char**)array101; }
    else if (mty == "102") { ary = (char**)array102; }
    else if (mty == "103") { ary = (char**)array103; }
    else if (mty == "104") { ary = (char**)array104; }
    else if (mty == "105") { ary = (char**)array105; }
    else if (mty == "106") { ary = (char**)array106; }
    else if (mty == "107") { ary = (char**)array107; }
    else if (mty == "108") { ary = (char**)array108; }
    else if (mty == "109") { ary = (char**)array109; }
    else if (mty == "110") { ary = (char**)array110; }
    else if (mty == "111") { ary = (char**)array111; }
    else if (mty == "112") { ary = (char**)array112; }
    else if (mty == "113") { ary = (char**)array113; }
    else if (mty == "114") { ary = (char**)array114; }
    else if (mty == "115") { ary = (char**)array115; }
    else if (mty == "116") { ary = (char**)array116; }
    else if (mty == "117") { ary = (char**)array117; }
    else if (mty == "118") { ary = (char**)array118; }
    else if (mty == "119") { ary = (char**)array119; }
    else
    {
        return CString();
    }

    int nRow = sizeof(*array100), nCol = sizeof(array100) / sizeof(*array100);
    if (mty.GetAt(1) == _T('1')) nRow = sizeof(*array110), nCol = sizeof(array110) / sizeof(*array110);


    for (int i = 0; i < str.GetLength(); i++)
    {
        rtn += GetEncodingCodePacketChar10(ary, nRow, nCol, str.Mid(i, 1));
    }
    rtn += mty;
    return rtn.Trim();
#pragma endregion
}

CString PacketCoder::GetEncodingCodePacketChar10(char** ary, size_t sRow, size_t sCol, CString pvalue)
{
#pragma region
    int idx[2] = { 0, 0 };
    CString result = _T("");
    char* point = (char*)ary;

    for (int i = 0; i < sCol; i++)
    {
        for (int j = 0; j < sRow; j++)
        {
            if (pvalue.Trim() == point[i*sRow+j])
            {
                idx[0] = i + 1; idx[1] = j + 1;
            }
        }
    }
    result.Format(_T("%02d%02d"),idx[0],idx[1]);
    //Console.WriteLine("GetEncodingCodePacketChar = " + result);

    return result;
#pragma endregion
}
#pragma endregion //Packet 암/복호화


CString PacketCoder::Encode(CString str, int mty)
{
    CString sEncode = _T("");
    CString Mty = _T(""); Mty.Format(_T("%d"), mty);
    if (890 <= mty && mty <= 909)
        sEncode = EncodingPacketString(Mty, str);
    else if (100 <= mty && mty <= 119)
        sEncode = EncodingPacketString10(Mty, str);
    return sEncode;
}
CString PacketCoder::Decode(CString str)
{
    CString Mty = str.Right(3);
    int mty = _wtoi(Mty);
    CString sPacketData;
    str = str.Mid(0, str.GetLength() - 3);

    if (890 <= mty && mty <= 909)
        sPacketData = DecodingPacketString(Mty, str);
    else if (100 <= mty && mty <= 119)
        sPacketData = DecodingPacketString10(Mty, str);
    return sPacketData;
}