/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int j = 0; j < length; j++) {
    result += key[j];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
    if (table == NULL) {
        return;
    }

    for(int j = 0; j < HT_SIZE ; j++){
        (*table)[j] = NULL;
    }
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
    if (table == NULL) {
        return NULL;
    }
    //index do tabulky
    int index = get_hash(key);
    ht_item_t *search = (*table)[index];

    //dokym neprejdeme vsetko
    while(search != NULL){
        //ak sa rovnaju
        if(search->key == key){
            return search;
        }
        //ideme na dalsi prvok
        search = search->next;
    }
    return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
    if (table == NULL) {
        return;
    }

    ht_item_t *item_exist = ht_search(table, key);

    if(item_exist!= NULL){
        item_exist->value = value;
    }
    else{
        //alokujeme si pamat
        ht_item_t *new_item = malloc(sizeof (ht_item_t));
        if(new_item == NULL){
            return;
        }
        //priradime hodnoty do noveho pridaneho prvku
        new_item->value = value;
        new_item->next = NULL;
        new_item->key = key;

        int index = get_hash(key);

        item_exist = (*table)[index];
        if(item_exist){
            new_item->next = item_exist;
        }
        (*table)[index] = new_item;
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
    if (table == NULL) {
        return NULL;
    }

    ht_item_t *elem = ht_search(table,key);

    if(elem  != NULL){
        return &elem->value;
    }else{
        return NULL;
    }
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {

    if (table == NULL) {
        return;
    }

    int hash = get_hash(key);
    ht_item_t *deleted = (*table)[hash];
    ht_item_t *previous_item = NULL;
    bool is_deleted = false;

    //ideme dokym sme ho nevymazali alebo nie je vymazany
    while (is_deleted != true && deleted != NULL ){
        if(deleted->key == key){
            if(previous_item == NULL){
               deleted = deleted->next;
            } else{
                previous_item->next = deleted->next;
            }
            free(deleted);
            is_deleted = true;
        }
        previous_item = deleted;
        deleted = deleted->next;
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {

    if (table == NULL) {
        return;
    }

    ht_item_t  *element, *delete;

    for (int j = 0; j < HT_SIZE; j++) {
        element = (*table)[j];
        if(element){
            delete = element;
            element = element->next;
            free(delete);
        }
        (*table)[j] = NULL;
    }
}
