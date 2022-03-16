#include "ConsoleInterface.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "Constants.h"
#include "Character.h"
#include "CommandMap.h"

#define EXIT_COMMAND "exit"
#define PRINT_MAIN_MENU_COMMAND "print menu"
#define SEE_STOCK_COMMAND "see stock"
#define ADD_ITEM_COMMAND "add item"
#define UPDATE_ITEM_COMMAND "update item"
#define REMOVE_ITEM_COMMAND "remove item"

#define FILTER_COMMAND "filter"
#define SORT_COMMAND "sort"

#define SET_FILTER "set filter"
#define SET_SORT "set sort"
#define SET_ORDER "set order"

#define PRICE_CRITERIA "price"
#define QUANTITY_CRITERIA "quantity"
#define MANUFACTURER_CRITERIA "manufacturer"


#define ASCENDING "ascending"
#define DESCENDING "descending"

#define NUMBER_OF_COMMANDS 11
#define MAX_SIZE_OF_COMMAND 30

static char command_list[][MAX_SIZE_OF_COMMAND] = { EXIT_COMMAND,
		PRINT_MAIN_MENU_COMMAND,
		ADD_ITEM_COMMAND,
		REMOVE_ITEM_COMMAND,
		UPDATE_ITEM_COMMAND,
		SEE_STOCK_COMMAND,
		FILTER_COMMAND,
		SORT_COMMAND,
		SET_FILTER,
		SET_SORT,
		SET_ORDER
};

struct console_interface_struct
{
	shop_service_t* shop_service;
	command_map_t* command_map;

	command_map_t* filter_command_map;
	command_map_t* sort_command_map;
	char filter_field_criteria[MAX_SIZE_OF_COMMAND];
	char sort_criteria[MAX_SIZE_OF_COMMAND];
	char order_criteria[MAX_SIZE_OF_COMMAND];
};


char* console_interface_create_string_with_no_leading_and_trailing_spaces(list_t* list_of_character_t)
{
	char* user_string;

	list_iterator_t* begin = list_iterator_create(list_of_character_t);

	size_t i = 0;
	for (;
		list_iterator_is_valid(begin) && character_get_value((character_t*)list_iterator_get(begin)) == ' ';
		list_iterator_next(begin), i++);

	if (list_iterator_is_valid(begin))
	{
		list_iterator_t* end = list_iterator_create(list_of_character_t);
		list_iterator_last(end);

		size_t j = 0;
		for (;
			list_iterator_is_valid(end) && character_get_value((character_t*)list_iterator_get(end)) == ' ';
			list_iterator_previous(end), j++);
		list_iterator_next(end);

		size_t len = list_size(list_of_character_t) - i - j;
		user_string = (char*)calloc(len + 1, sizeof(char));
		if (!user_string)
		{
			perror("Out of heap memory");
			exit(1);
		}

		for (size_t index = 0;
			list_iterator_get(begin) != list_iterator_get(end) && index < len;
			list_iterator_next(begin))

			user_string[index++] = character_get_value((character_t*)list_iterator_get(begin));

		list_iterator_destroy(end);
	}
	else
	{
		user_string = calloc(1, sizeof(char));
		if (!user_string)
		{
			perror("Out of heap memory");
			exit(1);
		}
	}
	list_iterator_destroy(begin);
	return user_string;
}

char* console_interface_read_string_from_user()
{
	char c;
	character_t* cc = character_create('\0');
	list_t* list_of_char = list_create(character_copy, character_destroy);
	scanf_s("%c", &c, 1);
	while (c != '\n')
	{
		character_set_value(cc, c);
		list_add(list_of_char, cc);
		scanf_s("%c", &c, 1);
	}
	char* user_string = console_interface_create_string_with_no_leading_and_trailing_spaces(list_of_char);

	list_destroy(list_of_char);
	character_destroy(cc);
	return user_string;
}

int console_interface_is_string_int(char* string)
{
	if (!strlen(string))
		return false;

	if (*string == '-' || *string == '+')
	{
		string++;
		if (*string == 0)
			return false;
	}

	while (*string != 0)
	{
		if (*string < '0' || *string > '9')
			return false;
		string++;
	}
	return true;
}

int console_interface_read_int_from_user(int* destination, char* error_message)
{
	char* raw_int_string = console_interface_read_string_from_user();
	if (!console_interface_is_string_int(raw_int_string))
	{
		printf("%s\n", error_message);
		free(raw_int_string);
		return false;
	}
	*destination = atoi(raw_int_string);
	free(raw_int_string);
	return true;
}

int console_interface_is_whole_part_int(char* string)
{
	char* pointer_to_dot = strchr(string, '.');

	if (!pointer_to_dot)
		return false;

	size_t size_of_whole_part = pointer_to_dot - string;
	if (size_of_whole_part > 0)
	{
		char* whole_part_string = (char*)calloc(size_of_whole_part + 1, sizeof(char));
		if (!whole_part_string)
		{
			perror("Out of heap memory");
			exit(1);
		}
		strncpy_s(whole_part_string, size_of_whole_part + 1, string, size_of_whole_part);

		if (!console_interface_is_string_int(whole_part_string))
		{
			free(whole_part_string);
			return false;
		}
		free(whole_part_string);
	}
	return true;
}

int console_interface_is_decimal_part_int(char* string)
{
	char* pointer_to_dot = strchr(string, '.');
	
	if (!pointer_to_dot)
		return false;

	size_t size_of_decimal_part = strlen(string) - (pointer_to_dot - string) - 1;
	if (size_of_decimal_part > 0)
	{
		char* decimal_part_string = (char*)calloc(size_of_decimal_part + 1, sizeof(char));
		if (!decimal_part_string)
		{
			perror("Out of heap memory");
			exit(1);
		}
		strncpy_s(decimal_part_string, size_of_decimal_part + 1, pointer_to_dot + 1, size_of_decimal_part);

		if (!console_interface_is_string_int(decimal_part_string))
		{
			free(decimal_part_string);
			return false;
		}
		free(decimal_part_string);
	}
	return true;
}


int console_interface_is_string_double(char* string)
{
	if (console_interface_is_string_int(string))
		return true;
	if (*string == '.' && *(string + 1) == 0)
		return false;
	return console_interface_is_whole_part_int(string) &
		console_interface_is_decimal_part_int(string);
}

int console_interface_read_double_from_user(double* destination, char* error_message)
{
	char* raw_string_float = console_interface_read_string_from_user();

	if (!console_interface_is_string_double(raw_string_float))
	{
		printf(error_message);
		free(raw_string_float);
		return false;
	}

	*destination = atof(raw_string_float);
	free(raw_string_float);
	return true;
}

void console_interface_print_main_menu(console_interface_t* console_interface)
{
	printf("\n\n");
	printf("		SHOP MANAGER APPLICATION\n");
	printf("	COMMANDS:\n");
	printf("	>> print menu\n");
	printf("	>> see stock\n");
	printf("	>> add item\n");
	printf("	>> update item\n");
	printf("	>> remove item\n");
	printf("	>> filter\n");
	printf("	>> sort\n");
	printf("	>> set filter\n");
	printf("	>> set sort\n");
	printf("	>> set order\n");
	printf("	Report is set to <%s>\n", console_interface->filter_field_criteria);
	printf("	Sort items by <%s>. Order of items is set to <%s>\n", console_interface->sort_criteria, console_interface->order_criteria);
	printf("\n\n");
}

void console_interface_print_invalid_item_messages(shop_item_validation_result_t* result)
{
	printf("	INVALID ITEM EXCEPTION\n");
	if (!shop_item_validation_result_is_type_valid(result))
		printf("Type can't be empty\n");
	if (!shop_item_validation_result_is_manufacturer_valid(result))
		printf("Manufacturer can't be empty\n");
	if (!shop_item_validation_result_is_model_valid(result))
		printf("Model can't be empty\n");
	if (!shop_item_validation_result_is_price_valid(result))
		printf("Price can't be negative\n");
	if (!shop_item_validation_result_is_quantity_valid(result))
		printf("Quantity can't be negative\n");
}

void console_interface_add_item(console_interface_t* console_interface)
{
	printf("	ADD ITEM\n");
	printf("Type : ");
	char* type = console_interface_read_string_from_user();
	printf("Manufacturer: ");
	char* manufacturer = console_interface_read_string_from_user();
	printf("Model: ");
	char* model = console_interface_read_string_from_user();

	double price;
	printf("Price: ");
	int result_of_price_read = console_interface_read_double_from_user(&price, "Invalid numerical value for price\n");
	int quantity;
	printf("Quantity: ");
	int result_of_quantity_read = console_interface_read_int_from_user(&quantity, "Invalid numerical value for quantity\n");
	if (result_of_price_read && result_of_quantity_read)
	{
		shop_item_validation_result_t* validation_result = shop_item_validation_result_create();
		add_shop_item_result_t add_result = shop_service_add_item_in_stock(
			console_interface->shop_service,
			validation_result,
			type,
			manufacturer,
			model,
			price,
			quantity
		);
		if (add_result == ADD_INVALID)
			console_interface_print_invalid_item_messages(validation_result);
		else if (add_result == ADD_ADDED)
			printf("The new product Type=%s | Manufacturer=%s | Model=%s has been added to the stock\n",
				type,
				manufacturer,
				model);
		else if (add_result == ADD_INCREASED)
			printf("The quantity of product Type=%s | Manufacturer=%s | Model=%s has been increased by %d\n",
				type, 
				manufacturer,
				model,
				quantity);
		shop_item_validation_result_destroy(validation_result);
	}
	free(type);
	free(manufacturer);
	free(model);
}

void console_interface_print_item(shop_item_t* item)
{
	printf("Id=%d | Type=%s | Manufacturer=%s | Model=%s | Price=%f | Quantity=%d\n",
		shop_item_get_id(item),
		shop_item_get_type(item),
		shop_item_get_manufacturer(item),
		shop_item_get_model(item),
		shop_item_get_price(item),
		shop_item_get_quantity(item));
}

void console_interface_see_stock(console_interface_t* console_interface)
{
	printf("	STOCK\n");
	list_t* stock = shop_service_get_stock(console_interface->shop_service);
	
	if (list_size(stock))
		list_for_each(stock, console_interface_print_item);
	else
		printf("Stock is empty\n");
	list_destroy(stock);
}

int console_interface_is_user_command_valid(char* user_command)
{
	for (size_t i = 0; i < NUMBER_OF_COMMANDS; i++)
		if (!strcmp(command_list[i], user_command))
			return true;
	return false;
}


void console_interface_update_item(console_interface_t* console_interface)
{
	printf("	UPDATE ITEM\n");
	int id;
	printf("Id: ");
	int result_of_id_read = console_interface_read_int_from_user(&id, "Invalid numerical value for id\n");

	double price;
	printf("Price: ");
	int result_of_price_read = console_interface_read_double_from_user(&price, "Invalid numerical value for price\n");

	int quantity;
	printf("Quantity: ");
	int result_of_quantity_read = console_interface_read_int_from_user(&quantity, "Invalid numerical value for quantity\n");

	if (result_of_price_read && result_of_quantity_read && result_of_id_read)
	{
		shop_item_validation_result_t* validation_result = shop_item_validation_result_create();
		update_shop_item_result_t update_result = shop_service_update_item(console_interface->shop_service,
			id,
			validation_result,
			price,
			quantity);

		if (update_result == UPDATE_INVALID)
			console_interface_print_invalid_item_messages(validation_result);
		else if (update_result == UPDATE_NOT_FOUND)
			printf("Item with id %d doesn't exist\n", id);
		else if (update_result == UPDATE_UPDATED)
			printf("Item has been updated\n");
		shop_item_validation_result_destroy(validation_result);
	}
}

void console_interface_remove_item(console_interface_t* console_interface)
{
	printf("Id: ");
	int id;
	int result_of_id_read = console_interface_read_int_from_user(&id, "Invalid numerical value for id\n");

	if (result_of_id_read)
	{
		int result_of_remove = shop_service_remove_item_by_id(console_interface->shop_service, id);
		if (result_of_remove)
			printf("Item with id %d has been removed\n", id);
		else
			printf("Item with id %d doesn't exist\n", id);
	}
}



void console_interface_filter_items(console_interface_t* console_interface)
{
	command_map_run(console_interface, console_interface->filter_command_map, console_interface->filter_field_criteria);
}

void console_interface_sort_items(console_interface_t* console_interface)
{
	command_map_run(console_interface, console_interface->sort_command_map, console_interface->sort_criteria);
}

void console_interface_filter_items_by_price(console_interface_t* console_interface)
{
	printf("Find items with smaller price\nEnter price:");

	double price;
	int is_valid = console_interface_read_double_from_user(&price, "Invalid value for price!");

	if (is_valid)
	{
		list_t* items = shop_service_find_items_with_price_smaller_than(console_interface->shop_service, &price);
		list_for_each(items, console_interface_print_item);
		list_destroy(items);
	}
}

void console_interface_filter_items_by_quantity(console_interface_t* console_interface)
{
	printf("Find items with smaller quantity\nEnter quantity:");

	int quantity;
	int is_valid = console_interface_read_int_from_user(&quantity, "Invalid value for quantity!");

	if (is_valid)
	{
		list_t* items = shop_service_find_items_with_quantity_smaller_than(console_interface->shop_service, &quantity);
		list_for_each(items, console_interface_print_item);
		list_destroy(items);
	}
}

void console_interface_filter_items_by_manufacturer(console_interface_t* console_interface)
{
	printf("Find items produced by manufacturer\nEnter manufacturer:");

	char* manufacturer = console_interface_read_string_from_user();

	list_t* items = shop_service_find_items_of_manufacturer(console_interface->shop_service, manufacturer);
	list_for_each(items, console_interface_print_item);
	list_destroy(items);
	free(manufacturer);
}


void console_interface_initialize_filter_command_map(console_interface_t* console_interface)
{
	command_map_put(console_interface->filter_command_map, PRICE_CRITERIA, console_interface_filter_items_by_price);
	command_map_put(console_interface->filter_command_map, MANUFACTURER_CRITERIA, console_interface_filter_items_by_manufacturer);
	command_map_put(console_interface->filter_command_map, QUANTITY_CRITERIA, console_interface_filter_items_by_quantity);
}

void console_interface_sort_items_by_price(console_interface_t* console_interface)
{
	list_t* items;

	if (!strcmp(console_interface->order_criteria, ASCENDING))
		items = shop_service_sort_items_by_price_ascending(console_interface->shop_service);
	else
		items = shop_service_sort_items_by_price_descending(console_interface->shop_service);

	list_for_each(items, console_interface_print_item);
	list_destroy(items);
}

void console_interface_sort_items_by_quantity(console_interface_t* console_interface)
{
	list_t* items;

	if (!strcmp(console_interface->order_criteria, ASCENDING))
		items = shop_service_sort_items_by_quantity_ascending(console_interface->shop_service);
	else
		items = shop_service_sort_items_by_quantity_descending(console_interface->shop_service);

	list_for_each(items, console_interface_print_item);
	list_destroy(items);
}

void console_interface_intialize_sort_command_map(console_interface_t* console_interface)
{
	command_map_put(console_interface->sort_command_map, PRICE_CRITERIA, console_interface_sort_items_by_price);
	command_map_put(console_interface->sort_command_map, QUANTITY_CRITERIA, console_interface_sort_items_by_quantity);
}


static char filter_options[][MAX_SIZE_OF_COMMAND] = { PRICE_CRITERIA,
	MANUFACTURER_CRITERIA,
	QUANTITY_CRITERIA };
void console_interface_set_filter(console_interface_t* console_interface)
{
	printf("Filter:");
	char* user_option = console_interface_read_string_from_user();

	int is_valid = false;
	for (int i = 0; i < 3 && !is_valid; i++)
		if (!strcmp(user_option, filter_options[i]))
			is_valid = true;

	if (is_valid)
		strcpy_s(console_interface->filter_field_criteria, strlen(user_option) + 1, user_option);
	else
		printf("Invalid options. Filter options: <%s> <%s> <%s>\n", PRICE_CRITERIA, QUANTITY_CRITERIA, MANUFACTURER_CRITERIA);
	free(user_option);
}

static char sort_options[][MAX_SIZE_OF_COMMAND] = {PRICE_CRITERIA,
	QUANTITY_CRITERIA};
void console_interface_set_sort_criteria(console_interface_t* console_interface)
{
	printf("Sort: ");
	char* user_option = console_interface_read_string_from_user();

	int is_valid = false;
	for (int i = 0; i < 3 && !is_valid; i++)
		if (!strcmp(user_option, sort_options[i]))
			is_valid = true;

	if (is_valid)
		strcpy_s(console_interface->sort_criteria, strlen(user_option) + 1, user_option);
	else
		printf("Invalid options. Sort options: <%s> <%s>\n", PRICE_CRITERIA, QUANTITY_CRITERIA);
	free(user_option);
}

static char order_options[][MAX_SIZE_OF_COMMAND] = { ASCENDING, DESCENDING};
void console_interface_set_order_criteria(console_interface_t* console_interface)
{
	printf("Order:");
	char* user_option = console_interface_read_string_from_user();

	int is_valid = false;
	for (int i = 0; i < 3 && !is_valid; i++)
		if (!strcmp(user_option, order_options[i]))
			is_valid = true;

	if (is_valid)
		strcpy_s(console_interface->order_criteria, strlen(user_option) + 1, user_option);
	else
		printf("Invalid options. Order options: <%s> <%s>\n", ASCENDING, DESCENDING);
	free(user_option);
}

void console_interface_initialize_command_map(console_interface_t* console_interface)
{
	command_map_put(console_interface->command_map, ADD_ITEM_COMMAND, console_interface_add_item);
	command_map_put(console_interface->command_map, SEE_STOCK_COMMAND, console_interface_see_stock);
	command_map_put(console_interface->command_map, PRINT_MAIN_MENU_COMMAND, console_interface_print_main_menu);
	command_map_put(console_interface->command_map, UPDATE_ITEM_COMMAND, console_interface_update_item);
	command_map_put(console_interface->command_map, REMOVE_ITEM_COMMAND, console_interface_remove_item);
	command_map_put(console_interface->command_map, FILTER_COMMAND, console_interface_filter_items);
	command_map_put(console_interface->command_map, SORT_COMMAND, console_interface_sort_items);
	command_map_put(console_interface->command_map, SET_FILTER, console_interface_set_filter);
	command_map_put(console_interface->command_map, SET_SORT, console_interface_set_sort_criteria);
	command_map_put(console_interface->command_map, SET_ORDER, console_interface_set_order_criteria);
}


console_interface_t* console_interface_create(shop_service_t* shop_service)
{
	console_interface_t* console_interface = (console_interface_t*)calloc(1, sizeof(console_interface_t));
	if (!console_interface)
	{
		perror("Out of heap memory");
		exit(1);
	}
	console_interface->shop_service = shop_service;
	console_interface->command_map = command_map_create();
	console_interface->sort_command_map = command_map_create();
	console_interface->filter_command_map = command_map_create();

	console_interface_initialize_command_map(console_interface);
	console_interface_initialize_filter_command_map(console_interface);
	console_interface_intialize_sort_command_map(console_interface);

	strcpy_s(console_interface->filter_field_criteria, strlen(PRICE_CRITERIA) + 1, PRICE_CRITERIA);
	strcpy_s(console_interface->sort_criteria, strlen(PRICE_CRITERIA) + 1, PRICE_CRITERIA);
	strcpy_s(console_interface->order_criteria, strlen(ASCENDING) + 1, ASCENDING);
	return console_interface;
}

void console_interface_destroy(console_interface_t* console_interface)
{
	command_map_destroy(console_interface->command_map);
	command_map_destroy(console_interface->filter_command_map);
	command_map_destroy(console_interface->sort_command_map);
	console_interface->command_map = NULL;
	console_interface->shop_service = NULL;
	free(console_interface);
	console_interface = NULL;
}




void console_interface_run(console_interface_t* console_interface)
{
	while (true)
	{
		console_interface_print_main_menu(console_interface);
		printf(">> ");
		char* user_command = console_interface_read_string_from_user();
		
		if (strlen(user_command) > 0)
		{
			if (!strcmp(user_command, EXIT_COMMAND))
			{
			    free(user_command);
				return;
			}
			if (console_interface_is_user_command_valid(user_command))
				command_map_run(console_interface, console_interface->command_map, user_command);
			else
				printf("Invalid command\n");
		}
		free(user_command);
	}
}



