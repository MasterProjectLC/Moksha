#include "conceito.h"

Conceito::Conceito(string nome) {
	this->nome = nome;
	this->descricao = "";
}

Conceito::Conceito(string nome, string descricao) {
	this->nome = nome;
	this->descricao = descricao;
}