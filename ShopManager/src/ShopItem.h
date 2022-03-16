#ifndef SHOP_ITEM_H
#define SHOP_ITEM_H

typedef struct shop_item_struct shop_item_t;

/*
	int id;
	char* type;
	char* manufacturer;
	char* model;
	float price;
	int quantity;
*/

shop_item_t*	shop_item_create(int id,
				char* type,
				char* manufacturer,
				char* model,
				double price,
				int quantity);
void			shop_item_destroy(shop_item_t* item);
shop_item_t*	shop_item_copy(shop_item_t* other);

int				shop_item_have_same_type_manufacturer_model(shop_item_t* item1, shop_item_t* item2);

int				shop_item_get_id(shop_item_t* item);
char*			shop_item_get_type(shop_item_t* item);
char*			shop_item_get_manufacturer(shop_item_t* item);
char*			shop_item_get_model(shop_item_t* item);
double			shop_item_get_price(shop_item_t* item);
int				shop_item_get_quantity(shop_item_t* item);

void			shop_item_increase_quantity_by(shop_item_t* item, int value);
void			shop_item_set_quantity(shop_item_t* item, int new_quantity);
void			shop_item_set_price(shop_item_t* item, double new_price);

int				shop_item_has_id(shop_item_t* item, int* id);
int				shop_item_is_manufactured_by(shop_item_t* item, char* manufacturer);
int				shop_item_has_price_smaller_than(shop_item_t* item, double* price);
int				shop_item_has_price_greater_than(shop_item_t* item, double* price);
int				shop_item_has_quantity_smaller_than(shop_item_t* item, int* quantity);
int				shop_item_has_quantity_greater_than(shop_item_t* item, int* quantity);

int				shop_item_compare_by_price_ascending(shop_item_t* item1, shop_item_t* item2);
int				shop_item_compare_by_price_descending(shop_item_t* item1, shop_item_t* item2);
int				shop_item_compare_by_quantity_ascending(shop_item_t* item1, shop_item_t* item2);
int				shop_item_compare_by_quantity_descending(shop_item_t* item1, shop_item_t* item2);

#endif 