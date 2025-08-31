// src/passenger.c
#include "passenger.h"
#include "utils.h"

// ---- local helpers ---------------------------------------------------------

static Passenger* create_node(const Passenger *src){
    Passenger *n = (Passenger*)malloc(sizeof(Passenger));
    if(!n){ perror("malloc"); exit(EXIT_FAILURE); }
    *n = *src;
    n->next = NULL;
    return n;
}

// case-insensitive substring (portable replacement for strcasestr)
static int contains_ci(const char *hay, const char *needle){
    if(!hay || !needle) return 0;
    size_t nlen = strlen(needle);
    if(nlen == 0) return 1;
    for(const char *p = hay; *p; ++p){
        size_t i = 0;
        while(i < nlen && p[i] &&
              tolower((unsigned char)p[i]) == tolower((unsigned char)needle[i])){
            ++i;
        }
        if(i == nlen) return 1;
    }
    return 0;
}

// ---- validation ------------------------------------------------------------

bool validate_email(const char *email){
    if(!email) return false;
    const char *at = strchr(email,'@');
    const char *dot = strrchr(email,'.');
    size_t len = strlen(email);
    if(!at || !dot) return false;
    if(len < 5) return false;                  // minimal a@b.c
    if(len < 4 || strcmp(email+len-4, ".com")!=0) return false; // must end .com
    if(at==email || at==email+len-1) return false;
    if(dot < at) return false;                 // dot must appear after @
    return true;
}

bool validate_area(const char *area){
    if(!area) return false;
    return contains_ci(area,"Dublin")   ||
           contains_ci(area,"Leinster") ||
           contains_ci(area,"Connacht") ||
           contains_ci(area,"Ulster")   ||
           contains_ci(area,"Munster");
}

bool validate_class(const char *cls){
    if(!cls) return false;
    return contains_ci(cls,"Economy") || contains_ci(cls,"First Class");
}

bool validate_trips(const char *trips){
    if(!trips) return false;
    return contains_ci(trips, "Less than three") ||
           contains_ci(trips, "Less than five")  ||
           contains_ci(trips, "More than five");
}

// ---- input / edit ----------------------------------------------------------

void input_passenger(Passenger *p){
    char buf[128];
    memset(p, 0, sizeof(*p));

    // PPS
    while(1){
        printf("Enter PPS (integer): ");
        if(!fgets(buf, sizeof(buf), stdin)) continue;
        char *end=NULL; long v=strtol(buf,&end,10);
        if(end==buf){ puts("Invalid."); continue; }
        p->PPS=(int)v; break;
    }

    // Names
    prompt_string("First Name: ", p->firstName, MAX_NAME);
    prompt_string("Second Name: ", p->secondName, MAX_NAME);

    // Year
    while(1){
        printf("Year Born (1900-2025): ");
        if(!fgets(buf,sizeof(buf),stdin)) continue;
        char *end=NULL; long v=strtol(buf,&end,10);
        if(end==buf || v<1900 || v>2025){ puts("Out of range."); continue; }
        p->yearBorn=(int)v; break;
    }

    // Email
    while(1){
        prompt_string("Email (.com): ", p->email, MAX_EMAIL);
        if(validate_email(p->email)) break;
        puts("Invalid email.");
    }

    // Area
    while(1){
        prompt_string("Area [Dublin/Leinster/Connacht/Ulster/Munster]: ", p->area, MAX_AREA);
        if(validate_area(p->area)) break;
        puts("Invalid area.");
    }

    // Class
    while(1){
        prompt_string("Class [Economy/First Class]: ", p->travelClass, MAX_CLASS);
        if(validate_class(p->travelClass)) break;
        puts("Invalid class.");
    }

    // Trips
    while(1){
        prompt_string(
          "Trips per year [Less than three times per year / Less than five times per year / More than five times per year]: ",
          p->tripsPerYear, MAX_TRIPS);
        if(validate_trips(p->tripsPerYear)) break;
        puts("Invalid choice.");
    }
}

void update_passenger_interactive(Passenger *p){
    if(!p) return;
    char buf[8];

    printf("Update fields for PPS %d. Press Y to change each field.\n", p->PPS);

    printf("Change First Name (current: %s)? [Y/N]: ", p->firstName);
    if(fgets(buf,sizeof(buf),stdin) && toupper((unsigned char)buf[0])=='Y')
        prompt_string("New First Name: ", p->firstName, MAX_NAME);

    printf("Change Second Name (current: %s)? [Y/N]: ", p->secondName);
    if(fgets(buf,sizeof(buf),stdin) && toupper((unsigned char)buf[0])=='Y')
        prompt_string("New Second Name: ", p->secondName, MAX_NAME);

    printf("Change Year Born (current: %d)? [Y/N]: ", p->yearBorn);
    if(fgets(buf,sizeof(buf),stdin) && toupper((unsigned char)buf[0])=='Y'){
        char l[32];
        while(1){
            printf("Year Born (1900-2025): ");
            if(!fgets(l,sizeof(l),stdin)) continue;
            char *end=NULL; long v=strtol(l,&end,10);
            if(end==l || v<1900||v>2025){ puts("Out of range."); continue; }
            p->yearBorn=(int)v; break;
        }
    }

    printf("Change Email (current: %s)? [Y/N]: ", p->email);
    if(fgets(buf,sizeof(buf),stdin) && toupper((unsigned char)buf[0])=='Y'){
        while(1){
            prompt_string("Email (.com): ", p->email, MAX_EMAIL);
            if(validate_email(p->email)) break;
            puts("Invalid email.");
        }
    }

    printf("Change Area (current: %s)? [Y/N]: ", p->area);
    if(fgets(buf,sizeof(buf),stdin) && toupper((unsigned char)buf[0])=='Y'){
        while(1){
            prompt_string("Area [Dublin/Leinster/Connacht/Ulster/Munster]: ", p->area, MAX_AREA);
            if(validate_area(p->area)) break;
            puts("Invalid area.");
        }
    }

    printf("Change Class (current: %s)? [Y/N]: ", p->travelClass);
    if(fgets(buf,sizeof(buf),stdin) && toupper((unsigned char)buf[0])=='Y'){
        while(1){
            prompt_string("Class [Economy/First Class]: ", p->travelClass, MAX_CLASS);
            if(validate_class(p->travelClass)) break;
            puts("Invalid class.");
        }
    }

    printf("Change Trips/Year (current: %s)? [Y/N]: ", p->tripsPerYear);
    if(fgets(buf,sizeof(buf),stdin) && toupper((unsigned char)buf[0])=='Y'){
        while(1){
            prompt_string(
                "Trips per year [Less than three times per year / Less than five times per year / More than five times per year]: ",
                p->tripsPerYear, MAX_TRIPS);
            if(validate_trips(p->tripsPerYear)) break;
            puts("Invalid choice.");
        }
    }
}

// ---- core list ops ---------------------------------------------------------

Passenger* add_passenger_sorted(Passenger **head, Passenger newP, bool *wasDuplicate){
    if(wasDuplicate) *wasDuplicate=false;
    Passenger **cur = head;
    while(*cur && (*cur)->PPS < newP.PPS) cur = &(*cur)->next;

    if(*cur && (*cur)->PPS == newP.PPS){
        if(wasDuplicate) *wasDuplicate=true;
        return *cur; // return existing node so caller can update
    }

    Passenger *node = create_node(&newP);
    node->next = *cur;
    *cur = node;
    return node;
}

Passenger* find_passenger(Passenger *head, int PPS){
    for(Passenger *n=head; n; n=n->next)
        if(n->PPS==PPS) return n;
    return NULL;
}

bool delete_passenger(Passenger **head, int PPS){
    Passenger **cur=head;
    while(*cur && (*cur)->PPS!=PPS) cur=&(*cur)->next;
    if(!*cur) return false;
    Passenger *del=*cur;
    *cur=del->next;
    free(del);
    return true;
}

// ---- display / report ------------------------------------------------------

void display_one(const Passenger *p){
    if(!p){ puts("Not found."); return; }
    printf("\nPPS: %d\nName: %s %s\nYear Born: %d\nEmail: %s\nArea: %s\nClass: %s\nTrips/Year: %s\n\n",
        p->PPS,p->firstName,p->secondName,p->yearBorn,p->email,p->area,p->travelClass,p->tripsPerYear);
}

void display_all(Passenger *head){
    if(!head){ puts("No passengers loaded."); return; }
    for(Passenger *n=head; n; n=n->next) display_one(n);
}

static int cmp_yearborn(const void *a, const void *b){
    const Passenger *pa=*(const Passenger* const*)a;
    const Passenger *pb=*(const Passenger* const*)b;
    if(pa->yearBorn!=pb->yearBorn) return (pa->yearBorn<pb->yearBorn)?-1:1;
    // tie-break by PPS
    return (pa->PPS<pb->PPS)?-1: (pa->PPS>pb->PPS);
}

void list_sorted_by_yearborn(Passenger *head){
    int n=0; for(Passenger *p=head; p; p=p->next) n++;
    if(n==0){ puts("No passengers."); return; }
    Passenger **arr = (Passenger**)malloc((size_t)n*sizeof(Passenger*));
    if(!arr){perror("malloc"); return;}
    int i=0; for(Passenger *p=head; p; p=p->next) arr[i++]=p;
    qsort(arr, (size_t)n, sizeof(Passenger*), cmp_yearborn);
    for(i=0;i<n;i++) display_one(arr[i]);
    free(arr);
}

void print_report(const char *path, Passenger *head){
    FILE *f = fopen(path, "w");
    if(!f){ perror("report"); return; }
    fprintf(f, "Rail Ireland — Passenger Report\n\n");
    int count=0;
    for(Passenger *p=head; p; p=p->next){
        fprintf(f, "PPS:%d | %s %s | YOB:%d | %s | %s | %s | %s\n",
            p->PPS,p->firstName,p->secondName,p->yearBorn,p->email,p->area,p->travelClass,p->tripsPerYear);
        count++;
    }
    fprintf(f, "\nTotal passengers: %d\n", count);
    fclose(f);
    printf("Report written to %s\n", path);
}

// ---- stats -----------------------------------------------------------------

void generate_stats_by_class(Passenger *head){
    if(!head){ puts("No data."); return; }
    char cls[MAX_CLASS];
    while(1){
        printf("Select travel class [Economy/First Class]: ");
        if(!fgets(cls,sizeof(cls),stdin)) continue;
        trim_newline(cls);
        if(validate_class(cls)) break;
        puts("Invalid.");
    }

    int total=0, d=0,l=0,c=0,u=0,m=0;
    for(Passenger *n=head; n; n=n->next){
        if(ci_equal(n->travelClass, cls)){
            total++;
            if(ci_equal(n->area,"Dublin"))     d++;
            else if(ci_equal(n->area,"Leinster")) l++;
            else if(ci_equal(n->area,"Connacht")) c++;
            else if(ci_equal(n->area,"Ulster"))   u++;
            else if(ci_equal(n->area,"Munster"))  m++;
        }
    }
    if(total==0){ printf("No passengers in class '%s'.\n", cls); return; }

    printf("\nStatistics for class: %s (N=%d)\n", cls, total);
    printf("Dublin:   %.2f%%\n", 100.0*d/total);
    printf("Leinster: %.2f%%\n", 100.0*l/total);
    printf("Connacht: %.2f%%\n", 100.0*c/total);
    printf("Ulster:   %.2f%%\n", 100.0*u/total);
    printf("Munster:  %.2f%%\n\n", 100.0*m/total);
}

// ---- file I/O --------------------------------------------------------------

Passenger* load_passengers(const char *path){
    FILE *f = fopen(path, "r");
    if(!f){
        perror("open passenger file");
        return NULL;
    }
    Passenger *head=NULL;
    char line[512];

    while(fgets(line, sizeof(line), f)){
        trim_newline(line);
        if(line[0]=='\0') continue;

        Passenger p={0};
        char *tok;

        tok=strtok(line, "|"); if(!tok) continue; p.PPS=atoi(tok);
        tok=strtok(NULL, "|"); if(!tok) continue; strncpy(p.firstName,tok,MAX_NAME-1);
        tok=strtok(NULL, "|"); if(!tok) continue; strncpy(p.secondName,tok,MAX_NAME-1);
        tok=strtok(NULL, "|"); if(!tok) continue; p.yearBorn=atoi(tok);
        tok=strtok(NULL, "|"); if(!tok) continue; strncpy(p.email,tok,MAX_EMAIL-1);
        tok=strtok(NULL, "|"); if(!tok) continue; strncpy(p.area,tok,MAX_AREA-1);
        tok=strtok(NULL, "|"); if(!tok) continue; strncpy(p.travelClass,tok,MAX_CLASS-1);
        tok=strtok(NULL, "|"); if(!tok) continue; strncpy(p.tripsPerYear,tok,MAX_TRIPS-1);

        // ensure NUL-termination
        p.firstName[MAX_NAME-1] = '\0';
        p.secondName[MAX_NAME-1] = '\0';
        p.email[MAX_EMAIL-1] = '\0';
        p.area[MAX_AREA-1] = '\0';
        p.travelClass[MAX_CLASS-1] = '\0';
        p.tripsPerYear[MAX_TRIPS-1] = '\0';

        bool dup=false;
        add_passenger_sorted(&head, p, &dup); //	load keeps PPS order
    }
    fclose(f);
    return head;
}

bool save_passengers(const char *path, Passenger *head){
    FILE *f = fopen(path, "w");
    if(!f){ perror("write passenger file"); return false; }
    for(Passenger *n=head; n; n=n->next){
        fprintf(f, "%d|%s|%s|%d|%s|%s|%s|%s\n",
                n->PPS, n->firstName, n->secondName, n->yearBorn,
                n->email, n->area, n->travelClass, n->tripsPerYear);
    }
    fclose(f);
    return true;
}

// ---- cleanup ---------------------------------------------------------------

void free_list(Passenger *head){
    while(head){
        Passenger *n=head->next;
        free(head);
        head=n;
    }
}
