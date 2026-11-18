/*inicio ← chave mod m;

para i de 0 até m−1:

    j ← (inicio + i) mod m;

    se T[j].estado = NUNCA_USADA,
       retornar NÃO_ENCONTRADO;

    se T[j].estado = OCUPADA e T[j].chave = chave,
       retornar ENCONTRADO.

*/