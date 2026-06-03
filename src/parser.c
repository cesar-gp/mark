#include <stdlib.h>
#include <string.h>
#include "utf8.h"
#include "parser.h"

/**
 *	Consume caracteres de una cadena de texto hasta
 *	encontrar un texto igual o distinto a la cadena
 *	pasada por parámetro.
 * 
 *	El parámetro 'consumer' define la condición de parada:
 * 
 *	-	REPEATER:	consume caracteres siempre y cuando la
 *					cadena siga coincidiendo con 'str'.
 *	-	DELIMITER:	consume caracteres siempre y cuando la
 *					cadena NO coincida con 'str'.
 * 
 *	El parámetro 'times' permite establecer un número de
 *	repeticiones, cuya función varía según el modo. Por
 *	ejemplo, si el parámetro 'times' tiene el valor 3:
 * 
 *	-	REPEATER:	parará al encontrar 'str' 3 veces,
 *					o si encuentra algo distinto.
 *	-	DELIMITER:	parará la tercera vez que encuentre
 *					'str', ignorando las cadenas distintas.
 * 
 *	Un parámetro 'times' con valor 0 equivale a un máximo o
 *	mínimo de coincidencias indefinido. Su función varía,
 *	de nuevo, según el modo:
 * 
 *	-	REPEATER:	avanzará hasta encontrar una cadena
 *					distinta de 'str' o el fin de cadena.
 *	-	DELIMITER:	avanzará hasta encontrar la última
 *					ocurrencia de 'str' o el fin de cadena.
 * 
 *	Nótese que usar un DELIMITER con límite 0 no es la
 *	forma más eficiente de llegar a la última coincidencia,
 *	pero sí resultar útil para saber el total de ellas.
 * 
 *	EJEMPLOS
 *	--
 * 
 *	-	consumestr(&":-:-:-Árbol-:-:-:", ":-", 3, REPEATER);
 *		Devuelve:		"Árbol-:-:-:".
 *		Explicación:	Consume los 3 ":-" seguidos.
 * 
 *	-	consumestr(&":-:-:-Árbol-:-:-:", "Árbol", 1, DELIMITER);
 *		Devuelve:		"-:-:-:".
 *		Explicación:	Consume hasta encontrar "Árbol".
 * 
 *	-	consumestr(&"[#]Uno[#]Dos[#]Tres", "[#]", 2, REPEATER);
 *		Devuelve:		"Uno[#]Dos[#]Tres".
 *		Explicación:	Consume un "[#]", no llega a
 *						encontrar 2.
 * 
 *	-	consumestr(&"[#]Uno[#]Dos[#]Tres", "[#]", 2, DELIMITER);
 *		Devuelve:		"Dos[#]Tres".
 *		Explicación:	Consume hasta encontrar dos "[#]",
 *						aunque no estén seguidos.
 * 
 *	-	consumestr(&"[#]Uno[#]Dos[#]Tres", "[#]", 0, REPEATER);
 *		Devuelve:		"Uno[#]Dos[#]Tres".
 *		Explicación:	Consume todos los "[#]" contiguos.
 *						Solo encuentra uno.
 * 
 *	-	consumestr(&"[#]Uno[#]Dos[#]Tres", "[#]", 0, DELIMITER);
 *		Devuelve:		"Tres".
 *		Explicación:	Consume toda la cadena hasta llegar
 *						al último "[#]".
 */
uint32_t consumestr(char** in, char* str, uint32_t times, consumer_t consumer) {
	uint32_t dummy;												// Codepoints.
	uint8_t clen;												// Long. de caracteres de '*in'.
	uint8_t strclen;											// Long. de caracteres de 'str'.
	uint8_t strfirstclen;										// Long. del primer caracter de 'str'.
	char next[4];												// Sig. caracter de 'in'.
	char strnext[4];											// Sig. caracter de 'str'.
	char strfirst[4];											// Primer caracteer de 'str'.
	uint32_t repe = 0;											// Apariciones de 'str' en '*in'.

	uint32_t stri = 0;											// Cursor para 'str'.
	char* last = *in;											// Pos. de última aparición.

	if(utf8decode(str, &dummy, &strfirstclen)) return 0;		// Leer 1º caracter de 'str'.
	memcpy(strfirst, str, strfirstclen);						// Copiar a 'strfirst'.

	while(**in && (!times || repe < times)) {					// Recorrer '*in' hasta aparición limite.
		if(utf8decode(*in, &dummy, &clen)) break;				// Leer sig. caracter de '*in'.
		memcpy(next, *in, clen);								// Copiar a 'next'.

		if(!str[stri]) {										// ¿Fin de 'str'?
			stri = 0;											//  | Restaurar cursor.
			last = *in;											//  | Apuntar aparición.
			if(++repe == times) break;							//  + ¿Aparición límite? Salir.
		}
		
		if(utf8decode(str + stri, &dummy, &strclen)) break;		// Leer sig. caracter de 'str'.
		memcpy(strnext, str + stri, strclen);					// Copiar a 'strnext'.

		bool diff = memcmp(next, strnext, strclen);				// Comparar caracteres.
		if(consumer == REPEATER && diff) break;					// ¿Distintos? Fin de REPEATER.

		if(diff) {												// ¿Distintos?
			if(consumer == REPEATER) break;						//  | Fin de REPEATER.
																//  |
			stri = 0;											//  | Resetear cursor de 'str'.
			if(!memcmp(next, strfirst, strfirstclen))			//  | ¿Es el primer caracter?
				stri += strfirstclen;							//  +  + Avanzar en 'str'.
		} else {
			stri += strclen;									// ¿Iguales? Avanzar en 'str'. 
		}

		*in += clen;											// Consumir caracter de '*in'.
	}

	if(!str[stri]) {											// ¿Fin de 'str'?
		stri = 0;												//  | Restaurar cursor.
		last = *in;												//  | Apuntar aparición.
		repe++;													//  + Contar aparición.
	}

	if(consumer == DELIMITER && !times)							// ¿DELIMITER de 0 apariciones?
		*in = last;												//  + Mover '*in' a última aparición.
	else														// ¿No?
		*in -= stri;											//  + Descartar parte de 'str' recorrida.

	return repe;												// Devolver apariciones.
}