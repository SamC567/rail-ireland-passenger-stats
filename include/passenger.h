#ifndef PASSENGER_H
#define PASSENGER_H


#include "common.h"


typedef struct Passenger {
int PPS; // unique, sorted by PPS in main list
char firstName[MAX_NAME];
char secondName[MAX_NAME];
int yearBorn;
char email[MAX_EMAIL];
char area[MAX_AREA]; // Dublin, Leinster, Connacht, Ulster, Munster
char travelClass[MAX_CLASS]; // Economy, First Class
char tripsPerYear[MAX_TRIPS]; // <3, <5, >5 (stored as text per brief)
struct Passenger *next;
} Passenger;


// Core list
Passenger* load_passengers(const char *path);
bool save_passengers(const char *path, Passenger *head);


// CRUD + queries (list is kept sorted by PPS)
Passenger* add_passenger_sorted(Passenger **head, Passenger newP, bool *wasDuplicate);
Passenger* find_passenger(Passenger *head, int PPS);
bool delete_passenger(Passenger **head, int PPS);
void display_all(Passenger *head);
void display_one(const Passenger *p);


// Update fields (interactive)
void update_passenger_interactive(Passenger *p);


// Reports & statistics
void print_report(const char *path, Passenger *head);
void list_sorted_by_yearborn(Passenger *head);
void generate_stats_by_class(Passenger *head);


// Validation
bool validate_email(const char *email);
bool validate_area(const char *area);
bool validate_class(const char *cls);
bool validate_trips(const char *trips);


// Cleanup
void free_list(Passenger *head);
void input_passenger(Passenger *p);  // make input collector available to main


#endif // PASSENGER_H