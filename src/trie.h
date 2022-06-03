/** @file
 * Interfejs modułu drzew Trie.
 * @author Mikołaj Piróg
 * @copyright Mikołaj Piróg
 * @date 2022
 */

#ifndef _TRIE_
#define _TRIE_

/**
 * Makro opisujące wielkość alfabetu drzew Trie.
 */
#define ALPHABET_SIZE 12

#include "my_string.h"
#include "phnum_ptr_int_pair.h"

/**
 * Typ reprezentujący drzewa Trie.
 */
typedef struct Trie Trie;

/** @brief Inicjuje węzeł Trie.
 * Inicjuje węzeł Trie, ustawiając @p prefix jako prefiks oraz @p parent
 * jako rodzica. Jako wartość przechowywaną w węźle ustawia pusty napis. Zakłada
 * poprawność argumentów.
 * @param[in, out] trie - wskaźnik na wskaźnik na strukturę Trie;
 * @param[in] prefix - prefiks inicjowanego węzła;
 * @param[in] parent - rodzica inicjowanego węzła;
 * @param[in] is_reverse - zmienna typu bool, informująca, czy drzewo jest
 * drzewm przekierowań, czy odwrotności;
 * @return @p Wartość true, jeśli alokacja pamięci powiedzie się. Wartość @p
 * false w przeciwnym wypadku.
 */
bool init_trie(Trie **trie, char prefix, Trie *parent, bool is_reverse);

/** @brief Zwalnia węzeł i wszystkiego jego dzieci.
 * Zwalnia węzeł i wszystkiego jego dzieci. Funkcja zawsze się powiedzie.
 * @param[in,out] trie - wskaźnik na zwalniany węzeł;
 * @param[in] update_reverse - zmienna typu bool, informująca, czy należy
 * uaktualnić drzewo odwrotności;
 */
void free_trie(Trie *trie, bool update_reverse);

/** @brief Dodaje wartość do drzewa Trie.
 * Dodaje wartość do drzewa Trie. Miejsce, gdzie wartość ma zostać dodana
 * określane jest przez @p route. Nadpisuje ewentualną zastaną wartość.
 * Przejmuje @p value na własność. Funkcja zakłada poprawność danych.
 * @param[in,out] root - wskaźnik na korzeń drzewa Trie;
 * @param[in] route - ścieżka do miejsca, w którym dodajemy wartość;
 * @param[in] value - dodawana wartość;
 * @param[in] ptr_to_reverse_trie_value - wskaźnik na parę @p
 * PhoneNumbersPtrSizeTPair w drzewie odwrotności;
 * @return Wartość @p true, jeśli wszystkie operacje powiodą się. Wartość @p
 * false w przeciwnym wypadku, to jest gdy dowolna alokacja pamięci nie
 * powiedzie się. W wypadku błędu drzewo z korzeniem @p root nadal jest
 * poprawnym drzewem.
 */
Trie *
add_value_normal_trie(Trie *root, const String *route, String *value,
                      PhoneNumbersPtrSizeTPair *ptr_to_reverse_trie_value);

/** @brief Dodaje wartość do drzewa Trie, przechowującego odwrotności
 * przekierowań. Dodaje wartość do drzewa Trie. Miejsce, gdzie wartość ma zostać
 * dodana określane jest przez @p route. Dodaje @p value do struktury
 * przechowującej wartości. Przejmuje @p value na własność. Funkcja zakłada
 * poprawność danych.
 * @param[in,out] root - wskaźnik na korzeń drzewa Trie;
 * @param[in] route - ścieżka do miejsca, w którym dodajemy wartość;
 * @param[in] value - dodawana wartość;
 * @param[in,out] ptr_to_added_node - wskaźnik na wskaźnik, do którego wpiszemy
 * wskaźnik na dodany węzeł;
 * @return Wartość @p true, jeśli wszystkie operacje powiodą się. Wartość @p
 * false w przeciwnym wypadku, to jest gdy dowolna alokacja pamięci nie
 * powiedzie się. W wypadku błędu drzewo z korzeniem @p root nadal jest
 * poprawnym drzewem.
 */
PhoneNumbersPtrSizeTPair *add_value_reverse_trie(Trie *root,
                                                 const String *route,
                                                 String *value,
                                                 Trie **ptr_to_added_node);

/** @brief Usuwa poddrzewo.
 * Usuwa poddrzewo, do którego prowadzi ścieżka @p route_to_subtree. Funkcja
 * zakłada poprawność argumentu @p root i różność @p route_to_subtree od @p
 * NULL. Jeśli @p route_to_subtree nie jest poprawnym numerem lub nie wskazuje
 * na istniejące poddrzewo, funkcja nic nie robi. Funkcja nie może się nie udać.
 * @param[in,out] root - wskaźnik na wskaźnik na korzeń drzewa Trie;
 * @param[in] route_to_subtree - wskaźnik na ciąg stanowiący potencjalną drogę;
 * do usuwanego poddrzewa.
 */
void remove_subtree(Trie **root, char const *route_to_subtree);

/** @brief Wyznacza przekierowanie numeru @p num.
 * Wyznacza przekierowanie numeru @p num. Przekierowanie to numer będący
 * wynikiem operacji zamiany pierwszych n znaków numeru @p num, na najgłębszą,
 * niebędącą @p NULL-em, wartość, gdzie n oznacza głębokość, na której
 * znaleziono wartość. Jeśli taka wartość nie istnieje, wynikiem przekierowania
 * jest napis reprezentowany przez
 * @p num. Wartość wyniku przekierowania jest kopiowana do @p result. Funkcja
 * zakłada poprawność argumentów. Jeśli funkcja nie powiedzie się, @p result
 * może nie reprezentować numeru.
 * @param[in] root - wskaźnik na korzeń drzewa Trie;
 * @param[in] num - przekierowywany numer będący także ścieżką do wartości
 * przekierowania w drzewie o korzeniu @p root;
 * @param[in,out] result - wskaźnik na strukturę reprezentującą napis będący
 * wynikiem operacji przekierowania;
 * @return Wartość @p true jeśli wszystkie operacja powiodą się. Jeśli wystąpi
 * błąd alokacji pamięci, wynikiem jest wartość @p false.
 */
bool get_num_forward_from_trie(Trie *root, const String *num, String *result);

/** @brief Wyznacza odwrotności przekierowań numeru reprezentowanego przez @p
 * route.
 * Wyznacza odwrotności przekierowań numeru reprezentowanego przez @p
 * route. Odwrotność jest rozumiana jako takie num, że phfwd(pf, num) daje
 * strukturę @p PhoneNumbers przechowująca numer równy @p route.
 * @param[in] reverse_trie_root - wskaźnik na korzeń drzewa odwrotnośći;
 * @param[in] route - wskaźnik na numer, którego odwrotności wyznaczamy.
 * @return Wartość @p true jeśli wszystkie operacja powiodą się. Jeśli wystąpi
 * błąd alokacji pamięci, wynikiem jest wartość @p false.
 */
PhoneNumbers *get_reversed_numbers(Trie *reverse_trie_root,
                                   const String *route);

/** @brief Odłącza węzeł od reszty drzewa.
 * Odłącza węzeł od reszty drzewa.
 * @param[in] node - wskaźnik na węzeł;
 */
void detach_node_from_trie(Trie *node);

#endif//_TRIE_