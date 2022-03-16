#include "ShopItemValidator.h"

#include <stdlib.h>
#include <string.h>

#include "Constants.h"

struct shop_item_validation_result_struct
{
	int is_valid;
	int is_type_valid;
	int is_manufacturer_valid;
	int is_model_valid;
	int is_price_valid;
	int is_quantity_valid;
};

shop_item_validation_result_t* shop_item_validation_result_create()
{
	shop_item_validation_result_t* result = (shop_item_validation_result_t*)calloc(1, sizeof(shop_item_validation_result_t));
	if (result == NULL)
	{
		perror("Out of heap memory\n");
		exit(1);
	}
	shop_item_validation_result_initialize(result);
	return result;
}

void shop_item_validation_result_initialize(shop_item_validation_result_t* result)
{
	result->is_valid					= true;
	result->is_type_valid				= true;
	result->is_model_valid				= true;
	result->is_manufacturer_valid		= true;
	result->is_price_valid				= true;
	result->is_quantity_valid			= true;
}

void shop_item_validation_result_destroy(shop_item_validation_result_t* result)
{
	free(result);
	result = NULL;
}

int shop_item_validation_result_is_shop_item_valid(shop_item_validation_result_t* result)
{
	return result->is_valid;
}

int shop_item_validation_result_is_type_valid(shop_item_validation_result_t* result)
{
	return result->is_type_valid;
}

int shop_item_validation_result_is_manufacturer_valid(shop_item_validation_result_t* result)
{
	return result->is_manufacturer_valid;
}

int shop_item_validation_result_is_model_valid(shop_item_validation_result_t* result)
{
	return result->is_model_valid;
}

int shop_item_validation_result_is_price_valid(shop_item_validation_result_t* result)
{
	return result->is_price_valid;
}

int shop_item_validation_result_is_quantity_valid(shop_item_validation_result_t* result)
{
	return result->is_quantity_valid;
}


void shop_item_validation_result_validate_shop_item(shop_item_t* item, shop_item_validation_result_t* validationResult)
{
	shop_item_validation_result_initialize(validationResult);
	if (!strlen(shop_item_get_type(item)))
		validationResult->is_type_valid = false;
	if (!strlen(shop_item_get_manufacturer(item)))
		validationResult->is_manufacturer_valid = false;
	if (!strlen(shop_item_get_model(item)))
		validationResult->is_model_valid = false;
	if (shop_item_get_price(item) <= 0.0f)
		validationResult->is_price_valid = false;
	if (shop_item_get_quantity(item) < 0)
		validationResult->is_quantity_valid = false;

	validationResult->is_valid = validationResult->is_type_valid &
		validationResult->is_manufacturer_valid &
		validationResult->is_model_valid &
		validationResult->is_price_valid &
		validationResult->is_quantity_valid;
}
