﻿/***************************************************************************
*  $MCI M�dulo de implementa��o: LIS  Lista duplamente encadeada
*
*  Arquivo gerado:              LISTA.c
*  Letras identificadoras:      LIS
*
*  Nome da base de software:    Arcabou�o para a automa��o de testes de programas redigidos em C
*  Arquivo da base de software: D:\AUTOTEST\PROJETOS\LISTA.BSW
*
*  Projeto: INF 1301 / 1628 Automatiza��o dos testes de m�dulos C
*  Gestor:  LES/DI/PUC-Rio
*  Autores: avs
*
*  $HA Hist�rico de evolu��o:
*     Vers�o  Autor    Data     Observa��es
*	  5 	  lp,ja,tb 20/set/2016
*     4       avs   01/fev/2006 criar linguagem script simb�lica
*     3       avs   08/dez/2004 uniformiza��o dos exemplos
*     2       avs   07/jul/2003 unifica��o de todos os m�dulos em um s� projeto
*     1       avs   16/abr/2003 in�cio desenvolvimento
*
***************************************************************************/

#include   <stdio.h>
#include   <string.h>
#include   <memory.h>
#include   <stdlib.h>
#include   <assert.h>

#define LISTA_OWN
#include "LISTA.h"
#undef LISTA_OWN

#define DEBUG
/***********************************************************************
*
*  $TC Tipo de dados: LIS Elemento da lista
*
*
***********************************************************************/

typedef struct tagElemLista {

	void *pValor;
	/* Ponteiro para o valor contido no elemento */

	struct tagElemLista * pAnt;
	/* Ponteiro para o elemento predecessor */

	struct tagElemLista * pProx;
	/* Ponteiro para o elemento sucessor */
#ifdef _DEBUG  
	LIS_tpLista *pCab;
	/*Ponteiro para a cabeca da lista*/
	int tamanho;
	/*Tamaho ocupado pela estrutura que a lista aponta*/
#endif


} tpElemLista;

/***********************************************************************
*
*  $TC Tipo de dados: LIS Descritor da cabeca de lista
*
*
***********************************************************************/

typedef struct LIS_tagLista {

	tpElemLista * pOrigemLista;
	/* Ponteiro para a origem da lista */

	tpElemLista * pFimLista;
	/* Ponteiro para o final da lista */

	tpElemLista * pElemCorr;
	/* Ponteiro para o elemento corrente da lista */

	int numElem;
	/* Numero de elementos da lista */

	char idLista[15];
	/*Identificação da lista*/

	// void(*ExcluirValor) (void * pValor);                                              Acho q n tem necessidade disso.
	/* Ponteiro para a função de destruição do valor contido em um elemento */

} LIS_tpLista;

/***** Prototipos das fun��es encapuladas no m�dulo *****/

static void LiberarElemento(LIS_tppLista   pLista,
	tpElemLista  * pElem);

static  tpElemLista * CriarElemento(LIS_tppLista pLista, void * pValor);

static void LimparCabeca(LIS_tppLista pLista);

/*****  C�digo das fun��es exportadas pelo m�dulo  *****/

/***************************************************************************
*
*  Funcao: LIS  &Criar lista
*  ****/

LIS_tpCondRet LIS_CriarLista(LIS_tppLista *pLista, char* idLista) {

	LIS_tpLista * aux;

	aux = (LIS_tpLista *)malloc(sizeof(LIS_tpLista));
	if (aux == NULL)
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */

	LimparCabeca(aux);

	strcpy(aux->idLista, idLista);

	*pLista = aux;


	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Criar lista */

  /***************************************************************************
  *
  *  Funcao: LIS  &Destruir lista
  *  ****/

LIS_tpCondRet LIS_DestroiLista(LIS_tppLista pLista)
{
	if (pLista == NULL)
		return LIS_CondRetListaNaoExiste;

	if (pLista->pElemCorr == NULL)
		return LIS_CondRetListaVazia;

	LIS_EsvaziarLista(pLista);

	free(pLista);

	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Destruir lista */

  /***************************************************************************
  **
  *  Funcao: LIS  &ObeterId
  *  *************************************************************************/

LIS_tpCondRet LIS_ObterId(LIS_tppLista pLista, char *idLista) {

	strcpy(idLista, pLista->idLista);

	return LIS_CondRetOK;

} /* Fim Fun��o: LIS  &ObterId */

  /***************************************************************************
  *
  *  Funcao: LIS  &Esvaziar lista
  *
  *  *************************************************************************/

void LIS_EsvaziarLista(LIS_tppLista pLista)
{

	tpElemLista * pElem;
	tpElemLista * pProx;

#ifdef _DEBUG
	assert(pLista != NULL);
#endif

	pElem = pLista->pOrigemLista;

	while (pElem != NULL)
	{

		pProx = pElem->pProx;

		LiberarElemento(pLista, pElem);

		pElem = pProx;
	} /* while */

	LimparCabeca(pLista);

} /* Fim fun��o: LIS  &Esvaziar lista */

  /***************************************************************************
  *
  *  Função: LIS  &Inserir elemento após
  *  ****/

LIS_tpCondRet LIS_InserirNo(LIS_tppLista pLista,
	void * pValor)

{

	tpElemLista * pElem;

#ifdef _DEBUG
	assert(pLista != NULL);
#endif

	/* Criar elemento a inerir após */

	pElem = CriarElemento(pLista, pValor);
	if (pElem == NULL)
	{
		return LIS_CondRetFaltouMemoria;
	} /* if */

	  /* Encadear o elemento após o elemento */

	if (pLista->pElemCorr == NULL)
	{
		pLista->pOrigemLista = pElem;
		pLista->pFimLista = pElem;
	}
	else
	{
		if (pLista->pElemCorr->pProx != NULL)
		{
			pElem->pProx = pLista->pElemCorr->pProx;
			pLista->pElemCorr->pProx->pAnt = pElem;
		}
		else
		{
			pLista->pFimLista = pElem;
		} /* if */

		pElem->pAnt = pLista->pElemCorr;
		pLista->pElemCorr->pProx = pElem;

	} /* if */

	pLista->pElemCorr = pElem;

	return LIS_CondRetOK;

} /* Fim função: LIS  &Inserir elemento após */

  /***************************************************************************
  *
  *  Funcao: LIS  &Excluir elemento
  *
  *  *************************************************************************/

LIS_tpCondRet LIS_ExcluirNoCorrente(LIS_tppLista pLista)
{
	tpElemLista * pElem;

	if (pLista->pElemCorr == NULL)
	{
		return LIS_CondRetListaVazia;
	} /* if */

	pElem = pLista->pElemCorr;

	/* Desencadeia à esquerda */

	if (pElem->pAnt != NULL)
	{
		pElem->pAnt->pProx = pElem->pProx;
		pLista->pElemCorr = pElem->pAnt;
	}
	else {
		pLista->pElemCorr = pElem->pProx;
		pLista->pOrigemLista = pLista->pElemCorr;
	} /* if */

	  /* Desencadeia à direita */

	if (pElem->pProx != NULL)
	{
		pElem->pProx->pAnt = pElem->pAnt;
	}
	else
	{
		pLista->pFimLista = pElem->pAnt;
	} /* if */

	LiberarElemento(pLista, pElem);

	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Excluir elemento */

  /***************************************************************************
  *
  *  Funcao: LIS  &Obter refer�ncia para o valor contido no elemento
  *
  *  *************************************************************************/

LIS_tpCondRet LIS_ObterNo(LIS_tppLista pLista, void** pValor) {

	if (pLista->pElemCorr == NULL)
	{
		return LIS_CondRetListaVazia;
	} /* if */
	*pValor = pLista->pElemCorr->pValor;

	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Obter refer�ncia para o valor contido no elemento */

  /***************************************************************************
  *
  *  Funcao: LIS  &Avan�ar elemento
  *
  *  ************************************************************************/

LIS_tpCondRet LIS_AvancarElementoCorrente(LIS_tppLista pLista, int numElem)
{

	int i;

	tpElemLista * pElem;

#ifdef _DEBUG
	assert(pLista != NULL);
#endif

	/* Tratar lista vazia */

	if (pLista->pElemCorr == NULL)
	{

		return LIS_CondRetListaVazia;

	} /* fim ativa: Tratar lista vazia */

	  /* Tratar avançar para frente */

	if (numElem > 0)
	{

		pElem = pLista->pElemCorr;
		for (i = numElem; i > 0; i--)
		{
			if (pElem == NULL)
			{
				break;
			} /* if */
			pElem = pElem->pProx;
		} /* for */

		if (pElem != NULL)
		{
			pLista->pElemCorr = pElem;
			return LIS_CondRetOK;
		} /* if */

		pLista->pElemCorr = pLista->pFimLista;
		return LIS_CondRetFimLista;

	} /* fim ativa: Tratar avançar para frente */

	  /* Tratar avançar para trás */

	else if (numElem < 0)
	{

		pElem = pLista->pElemCorr;
		for (i = numElem; i < 0; i++)
		{
			if (pElem == NULL)
			{
				break;
			} /* if */
			pElem = pElem->pAnt;
		} /* for */

		if (pElem != NULL)
		{
			pLista->pElemCorr = pElem;
			return LIS_CondRetOK;
		} /* if */

		pLista->pElemCorr = pLista->pOrigemLista;
		return LIS_CondRetFimLista;

	} /* fim ativa: Tratar avançar para trás */

	  /* Tratar não avançar */

	return LIS_CondRetOK;

} /* Fim função: LIS  &Avançar elemento */


  /***************************************************************************
  *
  *  Funcao: LIS  &Voltar elemento
  *
  *  *************************************************************************/

LIS_tpCondRet LIS_VoltarElementoCorrente(LIS_tppLista pLista)
{

	int i;

#ifdef _DEBUG
	assert(pLista != NULL);
#endif

	/* Tratar lista vazia */

	if (pLista->pElemCorr == NULL)
	{

		return LIS_CondRetListaVazia;

	} /* fim ativa: Tratar lista vazia */

	  /* Tratar avan�ar para tras */
	if (pLista->pElemCorr->pAnt != NULL) {

		pLista->pElemCorr = pLista->pElemCorr->pAnt;

		return LIS_CondRetOK;
	}

	/* fim ativa: Tratar avan�ar para tras */

	/* Tratar No corrente é o primeiro */
	else if (pLista->pElemCorr->pAnt == NULL)
		return LIS_CondRetInicioLista;

} /* Fim fun��o: LIS  &Avan�ar elemento */
  /***************************************************************************
  *
  *  Funcao: LIS  &Alterar elemento corrente
  *
  *  *************************************************************************/

LIS_tpCondRet LIS_AlterarElementoCorrente(LIS_tppLista pLista, void* pValor)
{
#ifdef _DEBUG
	assert(pLista != NULL);
#endif

	/* Tratar se Lista Existe */
	if (pLista == NULL) {

		return LIS_CondRetListaNaoExiste;
	}
	/* fim ativa: Tratar se Lista Existe */

	/* Tratar lista vazia */
	if (pLista->pElemCorr == NULL)
	{
		return LIS_CondRetListaVazia;

	} /* fim ativa: Tratar lista vazia */

	  /* Tratar troca */
	pLista->pElemCorr->pValor = pValor;

	/* fim ativa: Tratar troca */

	return LIS_CondRetOK;

} /* Fim fun��o: LIS  &Avan�ar elemento */

  /***********************************************************************
  *
  *  Funcao: LIS  &Ir para o elemento inicial
  *  *********************************************************************/

LIS_tpCondRet LIS_IrInicioLista(LIS_tppLista pLista)
{
	if (pLista->pElemCorr == NULL)
	{
		return LIS_CondRetListaVazia;
	} /* if */

	pLista->pElemCorr = pLista->pOrigemLista;

	return LIS_CondRetOK;

} /* Fim função: LIS  &Ir para o elemento inicial */

  /***********************************************************************
  *
  *  Função: LIS  &Obter numero de elementos
  ***********************************************************************/

LIS_tpCondRet LIS_ObterNumElementos(LIS_tppLista pLista, int *numElementos) {
	if (pLista == NULL) {
		return LIS_CondRetListaNaoExiste;
	}

	*numElementos = pLista->numElem;

	return LIS_CondRetOK;
} /* Fim função: LIS  -Obter numero de elementos */

  /***********************************************************************
  *
  *  Função: LIS  &Ir para o elemento final
  ***********************************************************************/
void LIS_IrFinalLista(LIS_tppLista pLista)
{

	pLista->pElemCorr = pLista->pFimLista;

} /* Fim função: LIS  &Ir para o elemento final */


  /***************************************************************************
  *
  *  $FC Funcao: LIS  -Liberar elemento da lista (ESTATICA)
  *
  *  $ED Descri��o da fun��o
  *     Elimina os espa�os apontados pelo valor do elemento e o
  *     pr�prio elemento.
  *
  ***********************************************************************/

void LiberarElemento(LIS_tppLista   pLista,
	tpElemLista  * pElem)
{
	free(pElem);

	pLista->numElem--;

} /* Fim fun��o: LIS  -Liberar elemento da lista */


  /***********************************************************************
  *
  *  $FC Funcao: LIS  -Criar o elemento (ESTATICA)
  *
  ***********************************************************************/

tpElemLista * CriarElemento(LIS_tppLista pLista, void * pValor)
{

	tpElemLista * pElem;

	pElem = (tpElemLista *)malloc(sizeof(tpElemLista));
	if (pElem == NULL)
	{
		return NULL;
	} /* if */

	pElem->pValor = pValor;
	pElem->pAnt = NULL;
	pElem->pProx = NULL;

	pLista->numElem++;

	return pElem;

} /* Fim fun��o: LIS  -Criar o elemento */


  /***********************************************************************
  *
  *  $FC Funcao: LIS  -Limpar a cabe�a da lista (ESTATICA)
  *
  ***********************************************************************/

void LimparCabeca(LIS_tppLista pLista)
{
	pLista->pOrigemLista = NULL;
	pLista->pFimLista = NULL;
	pLista->pElemCorr = NULL;
	pLista->numElem = 0;

} /* Fim fun��o: LIS  -Limpar a cabe�a da lista */

  /********** Fim do m�dulo de implementa��o: LIS  Lista duplamente encadeada **********/

  //