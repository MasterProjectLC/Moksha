#pragma once
#include <vector>
#include <string>

#ifndef ENUM_H
#define ENUM_H

enum { M, F, imprimir, avancar, mover, descansar, acaoNula, ouvir, mencionar, falar, conversar, interagir, atacar, checar, sondar };
static vector<string> stringEnum = vector<string>({ "M", "F", "print", "advance",
										  "move", "rest", "", "listen", "mention", "say", "talk", "interact", "attack", "check", "scan" } );

#endif