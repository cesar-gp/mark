#include "utf8.h"
#include <stdlib.h>

/**
 *	Devuelve la longitud de un caracter UTF-8 o el valor
 *	cero (0) si el caracter no es válido.
 * 
 *	Entrada:	in:		pointer hacia caracter UTF-8.
 * 
 *	Salida:		0:		caracter inválido.
 *				[1,4]:	longitud del caracter en bytes.
 */
uint8_t utf8len(const char* in) {
	if((*in & 0x80) == 0x00) return 1;		// Caracter ASCII.
	else if((*in & 0xE0) == 0xC0) return 2;	// De 2 bytes.
	else if((*in & 0xF0) == 0xE0) return 3;	// De 3 bytes.
	else if((*in & 0xF8) == 0xF0) return 4;	// De 4 bytes.
	else return 0;							// Primer byte inválido.
}

/**
 *	Devuelve el número de caracteres de una cadena de texto
 *	codificada en UTF-8 terminada en un byte nulo ('\0').
 * 
 *	Si hay un caracter de longitud indefinida o de longitud
 *	menor a la indicada, cada uno de sus bytes será contado
 *	como un caracter distinto.
 * 
 *	Los bytes de continuación sueltos serán interpretados
 *	también como un caracter de un byte. Las codificaciones
 *	demasiado largas y caracteres de sustitución se tratan
 *	de la misma forma que un caracter UTF-8 válido.
 * 
 *	Entrada:	in:		pointer hacia cadena UTF-8.
 * 
 *	Salida:		número de caracteres de la cadena.
 */
uint32_t utf8chars(const char* in) {
	uint32_t ret;
	rec: for(ret = 0; *in; ret++) {
		// Conseguir longitud y consumir primer byte.
		uint8_t len = utf8len(in);
		if(len == 0) len = 1;
		in++;

		// Recorrer bytes de continuación.
		while(--len)
			if((*in & 0xC0) != 0x80) continue rec;
			else in++;
	}

	return ret;
}

/**
 *	Decodifica un caracter UTF-8 y extrae su longitud y su
 *	codepoint. La función devuelve 0 si el caracter es
 *	válido y números distintos en otros casos.
 * 
 *	El buffer *out contendrá el codepoint calculado para el
 *	caracter *in, incluso si el codepoint es inválido. Para
 *	comprobar validez se pueden usar los códigos de salida.
 * 
 *	Es indiferente si el parámetro *in contiene una cadena
 *	o solamente el caracter a decodificar. De hecho, la
 *	función se puede usar para recorrer una cadena letra
 *	por letra utilizando el buffer *len para realizar los
 *	saltos entre caracteres.
 * 
 *	El buffer *len contiene siempre el número de bytes
 *	leidos. Es decir, si la codificación de un caracter
 *	indica una longitud de 4 bytes pero el caracter acaba
 *	al leer 2 bytes, *len contendrá el valor 2, no 4.
 * 
 *	Entrada:	in:		pointer hacia caracter UTF-8.
 *				out:	buffer para codepoint.
 *				len:	buffer para longitud.
 * 
 *	Salida:		0:		es válido.
 *				1:		su primer byte es inválido.
 *				2:		mide menos de lo indicado.
 *				3:		mide más de lo indicado.
 *				4:		su codificación es demasiado larga.
 *				5:		es un caracter de sustitución.
 *				6:		su codepoint sobrepasa el límite.
 * 
 *				Se sobreescriben *out y *len.
 */
uint8_t utf8decode(const char* in, uint32_t* out, uint8_t* len) {
	// Calcular longitud de caracter y consumir primer byte.
	*len = utf8len(in);

	switch(*len) {
		case 1:
			// Caracter ASCII. Devolver caracter directamente.
			*out = (uint32_t) in[0];
			return 0;
		case 0:
			// Primer byte inválido. Devolver error.
			*len = 1;
			return 1;
		default:
			// Caracter UTF-8. Extraer bits para codepoint.
			*out = (uint32_t) (in[0] & (0xFF >> (*len + 1)));
	}

	// Decodificar los bytes de continuación.
	uint8_t read = 1;
	for(; read < *len && (in[read] & 0xC0) == 0x80; read++)
		*out = (*out << 6) | (uint32_t) (in[read] & 0x3F);

	// ¿Mide menos de lo previsto? Error.
	if(read < *len) {
		*len = read;
		return 2;
	}

	// ¿Mide más de lo previsto? Error.
	if((in[read] & 0xC0) == 0x80) return 3;

	switch((uint8_t) in[0]) {
		case 0xC0:
		case 0xC1:
			// Overlong encoding de 2 bytes.
			return 4;
		case 0xE0:
			// Overlong encoding de 3 bytes.
			if((uint8_t) in[1] < 0xA0) return 4;
			break;
		case 0xED:
			// Caracter de sustitución (surrogate).
			if((uint8_t) in[1] >= 0xA0) return 5;
			break;
		case 0xF0:
			// Overlong encoding de 4 bytes.
			if((uint8_t) in[1] < 0x90) return 4;
	}

	// ¿Codepoint fuera de límites? Error.
	if(*out > 0x0010FFFF) return 6;

	return 0;
}