#include "common.h"
#include "login.h"
#include "passenger.h"
#include "utils.h"

#ifdef _WIN32
  #include <direct.h>
  #define MKDIR_OK _mkdir(DATA_DIR)
#else
  #include <sys/stat.h>
  #include <sys/types.h>
  #include <errno.h>
  static int mkdir_p_once(void){
      if (mkdir(DATA_DIR, 0777) == -1 && errno != EEXIST) { perror("mkdir"); return -1; }
      return 0;
  }
#endif

static void ensure_data_dir(void){
#ifdef _WIN32
    (void)MKDIR_OK; // creates if missing; ignored if exists
#else
    (void)mkdir_p_once();
#endif
}

static void menu_loop(Passenger **head){
    while(1){
        printf("\n==== Rail Ireland Passenger System ====\n");
        printf("1. Add passenger\n");
        printf("2. Display all passengers\n");
        printf("3. Display passenger details\n");
        printf("4. Update a passenger statistic\n");
        printf("5. Delete passenger\n");
        printf("6. Generate statistics by travel class\n");
        printf("7. Print all passengers into report file\n");
        printf("8. List all passengers in order of year born\n");
        printf("9. Save & Exit\n");
        int choice = prompt_int("Select option (1-9): ", 1, 9);

        if(choice==1){
            Passenger p={0};
            input_passenger(&p); // now properly exposed via header
            bool dup=false; Passenger *node = add_passenger_sorted(head, p, &dup);
            if(dup){
                printf("\nPPS already exists. Update this passenger instead? [Y/N]: ");
                char b[8]; fgets(b,sizeof(b),stdin);
                if(toupper((unsigned char)b[0])=='Y') update_passenger_interactive(node);
                else puts("No changes made.");
            } else {
                puts("Passenger added.");
            }
        } else if(choice==2){
            display_all(*head);
        } else if(choice==3){
            int pps = prompt_int("Enter PPS to view: ", 1, 2147483647);
            Passenger *p = find_passenger(*head, pps);
            if(!p) puts("Not found."); else display_one(p);
        } else if(choice==4){
            int pps = prompt_int("Enter PPS to update: ", 1, 2147483647);
            Passenger *p = find_passenger(*head, pps);
            if(!p) puts("Not found."); else update_passenger_interactive(p);
        } else if(choice==5){
            int pps = prompt_int("Enter PPS to delete: ", 1, 2147483647);
            if(delete_passenger(head, pps)) puts("Deleted."); else puts("Not found.");
        } else if(choice==6){
            generate_stats_by_class(*head);
        } else if(choice==7){
            print_report(REPORT_FILE, *head);
        } else if(choice==8){
            list_sorted_by_yearborn(*head);
        } else if(choice==9){
            save_passengers(PASSENGER_FILE, *head);
            puts("Saved. Goodbye.");
            break;
        }
    }
}

int main(void){
    ensure_data_dir();

    // Load login and authenticate
    LoginNode *logins = load_logins(LOGIN_FILE);
    if(!logins){ puts("Fatal: could not load login records."); return EXIT_FAILURE; }
    if(!authenticate(logins)){ puts("Access denied."); free_logins(logins); return EXIT_FAILURE; }

    // Load passengers list
    Passenger *head = load_passengers(PASSENGER_FILE);
    if(!head) head=NULL; // start empty if file missing

    menu_loop(&head);

    free_list(head);
    free_logins(logins);
    return 0;
}
