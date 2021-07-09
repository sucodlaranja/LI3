#ifndef QUERIES_H
#define QUERIES_H

#include "sgr.h"
#include "Table.h"


/**
 * @brief 2. Determinar a lista de nomes de negócios e o número total de negócios cujo nome
 * inicia por uma dada letra. (procura não deverá ser case sensitive).
 * 
 * @param sgr
 * @param letter
 * @return TABLE
 */
TABLE businesses_started_by_letter(SGR sgr, char letter);

/**
 * @brief 3. Dado um id de negócio, determinar a sua informação: nome, cidade, estado, stars,
 * e número total reviews.
 * 
 * @param sgr 
 * @param business_id 
 * @return TABLE 
 */
TABLE businesses_info(SGR sgr, char *business_id);

/**
 * @brief 4. Dado um id de utilizador, determinar a lista de negócios aos quais fez review. 
 * A informação associada a cada negócio deve ser o id e o nome.
 * 
 * @param sgr 
 * @param user_id 
 * @return TABLE 
 */
TABLE businesses_reviewed(SGR sgr, char *user_id);

/**
 * @brief 5. Dado um número n de stars e uma cidade, determinar a lista de negócios com n
 * ou mais stars na dada cidade. A informação associada a cada negócio deve ser o seu id e nome.
 * 
 * @param sgr 
 * @param stars 
 * @param city 
 * @return TABLE 
 */
TABLE businesses_with_stars_and_city(SGR sgr, float stars, char *city);

/**
 * @brief 6. Dado um número inteiro n, determinar a lista dos top n negócios (tendo em conta
 * o número médio de stars) em cada cidade. A informação associada a cada negócio deve
 * ser o seu id, nome, e número de estrelas.
 * 
 * @param sgr 
 * @param top 
 * @return TABLE 
 */
TABLE top_businesses_by_city(SGR sgr, int top);

/**
 * @brief 7. Determinar a lista de ids de utilizadores e o número total de utilizadores que
 * tenham visitado mais de um estado, i.e. que tenham feito reviews em negócios de diferentes estados.
 * 
 * @param sgr 
 * @return TABLE 
 */
TABLE international_users(SGR sgr);

/**
 * @brief 8. Dado um número inteiro n e uma categoria, determinar a lista dos top n negócios
 * (tendo em conta o número médio de stars) que pertencem a uma determinada categoria. A
 * informação associada a cada negócio deve ser o seu id, nome, e número de estrelas.
 * 
 * @param sgr 
 * @param top 
 * @param category 
 * @return TABLE 
 */
TABLE top_businesses_with_category(SGR sgr, int top, char *category);

/**
 * @brief 9. Dada uma palavra, determinar a lista de ids de reviews que a referem no campo
 * text. Note que deverá ter em conta os vários possíveis símbolos de pontuação para
 * delimitar cada palavra que aparece no texto.
 * 
 * @param sgr 
 * @param word 
 * @return TABLE 
 */
TABLE reviews_with_word(SGR sgr,char *word);

#endif