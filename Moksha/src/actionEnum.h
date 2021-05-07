#pragma once
#include <vector>
#include <string>

#ifndef ENUM_H
#define ENUM_H

enum { avancar, nulo, mover, descansar, acaoNula, interagir, atacar, transformar, pegar, deixar,
		ouvir, mencionar, falar, conversar, abrir, tempo, espiar, revelar, checar, sondar, atualizar_vizinhos };
static vector<string> stringEnum = vector<string>({ "advance", "null", "move", "rest", "", "interact", "attack", "transform", "take", "leave", 
													"listen", "mention", "say", "talk", "open", "time", "peer", "reveal", "check", "scan", "update_n" } );

#endif