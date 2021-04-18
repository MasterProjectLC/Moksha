#pragma once
#include <vector>
#include <string>

#ifndef ENUM_H
#define ENUM_H

enum { M, F, imprimir, avancar, nulo, mover, descansar, acaoNula, interagir, atacar, pegar, deixar,
		ouvir, mencionar, falar, conversar, abrir, tempo, espiar, checar, sondar };
static vector<string> stringEnum = vector<string>({ "M", "F", "print", "advance", "null", "move", "rest", "", "interact", "attack", "take", "leave", 
													"listen", "mention", "say", "talk", "open", "time", "peer", "check", "scan" } );

#endif