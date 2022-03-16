#ifndef SHOP_ITEM_VALIDATOR_H
#define SHOP_ITEM_VALIDATOR_H

#include "ShopItem.h"

typedef struct shop_item_validation_result_struct shop_item_validation_result_t;

shop_item_validation_result_t*	shop_item_validation_result_create();
void							shop_item_validation_result_initialize(shop_item_validation_result_t* result);
void							shop_item_validation_result_destroy(shop_item_validation_result_t* result);

int								shop_item_validation_result_is_shop_item_valid(shop_item_validation_result_t* result);
int								shop_item_validation_result_is_type_valid(shop_item_validation_result_t* result);
int								shop_item_validation_result_is_manufacturer_valid(shop_item_validation_result_t* result);
int								shop_item_validation_result_is_model_valid(shop_item_validation_result_t* result);
int								shop_item_validation_result_is_price_valid(shop_item_validation_result_t* result);
int								shop_item_validation_result_is_quantity_valid(shop_item_validation_result_t* result);

void							shop_item_validation_result_validate_shop_item(shop_item_t* item, shop_item_validation_result_t* result);

#endif
