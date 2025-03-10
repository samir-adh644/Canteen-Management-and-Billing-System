/*
    PROJECT NAME --> CANTEEN MANAGEMENT AND BILLING SYSTEM
    TEAM MEMBERS --> ROJAN BHATTARAI , SANDEEP JAISWAL , SAMIR ADHIKARI , RISHAV JAISWAL
    
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define OWNER_PASSWORD "admin"


struct Menu {
    int id;
    char name[50];
    float price;
};


void addItem() {
    struct Menu items[100], item;
    int count = 0, i;
    FILE *fp = fopen("canteen.txt", "r");

   
    if (fp != NULL) {
        while (fscanf(fp, "%d %s %f", &items[count].id, items[count].name, &items[count].price) != EOF) {
            count++;
        }
        fclose(fp);
    }

    
    printf("Enter Item ID: ");
    scanf("%d", &item.id);

    
    for (i = 0; i < count; i++) {
        if (items[i].id == item.id) {
            printf("Item ID already exists! Try again.\n");
            return;
        }
    }

    printf("Enter Item Name(use '_' instead of spaces): ");
    scanf(" %[^\n]s", item.name);
    printf("Enter Item Price: ");
    scanf("%f", &item.price);

   
    items[count++] = item;
    for (i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (items[i].id > items[j].id) {
                struct Menu temp = items[i];
                items[i] = items[j];
                items[j] = temp;
            }
        }
    }

   
    fp = fopen("canteen.txt", "w");
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d %s %.2f\n", items[i].id, items[i].name, items[i].price);
    }
    fclose(fp);

    printf("Item added successfully!\n");
}



void viewItems() {
    struct Menu item;
    FILE *fp = fopen("canteen.txt", "r");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("\nID\tName\tPrice\n");
    printf("----------------------------\n");
    while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.price) != EOF) {
        printf("%d\t%s\t%.2f\n", item.id, item.name, item.price);
    }

    fclose(fp);
}



void deleteItem() {
    struct Menu items[100];
    int count = 0, deleteId, i, found = 0;
    FILE *fp = fopen("canteen.txt", "r");

    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }

    
    while (fscanf(fp, "%d %s %f", &items[count].id, items[count].name, &items[count].price) != EOF) {
        count++;
    }
    fclose(fp);
    viewItems();
    
    printf("Enter Item ID to delete: ");
    scanf("%d", &deleteId);

   
    for (i = 0; i < count; i++) {
        if (items[i].id == deleteId) {
            found = 1;
            for (int j = i; j < count - 1; j++) {
                items[j] = items[j + 1];
            }
            count--;
            break;
        }
    }

    if (!found) {
        printf("Item not found!\n");
        return;
    }

    fp = fopen("canteen.txt", "w");
    for (i = 0; i < count; i++) {
        fprintf(fp, "%d %s %.2f\n", items[i].id, items[i].name, items[i].price);
    }
    fclose(fp);

    printf("Item deleted successfully!\n");
}



int ownerLogin() {
    char password[50];
    printf("Enter Admin Password: ");
    scanf("%s", password);

    if (strcmp(password, OWNER_PASSWORD) == 0) {
        return 1;
    } else {
        printf("Incorrect password! Access denied.\n");
        return 0;
    }
}



void ownerSection() {
    if (!ownerLogin()) {
        return; 
    }

    int choice;
    while (1) {
        printf("\nOwner Section\n");
        printf("1. Add Item\n");
        printf("2. Delete Item\n");
        printf("3. View Items\n");
        printf("4. Exit Owner Section\n");
        printf("Enter your choice(numbers only): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addItem();
                break;
            case 2:
                deleteItem();
                break;
            case 3:
                viewItems();
                break;
            case 4:
                return; 
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}





void placeOrder() {
    struct Menu item;
    int orderId, quantity;
    float total = 0;
    FILE *fp = fopen("canteen.txt", "r");
    FILE *orderFile = fopen("orders.txt", "a"); 

    if (fp == NULL) {
        printf("Error opening menu file!\n");
        return;
    }
    if (orderFile == NULL) {
        printf("Error opening orders file!\n");
        fclose(fp);
        return;
    }

    printf("\nAvailable Menu:\n");
    viewItems();

    printf("\nEnter Item ID to order (0 to finish): ");
    scanf("%d", &orderId);

    while (orderId != 0) {
        printf("Enter Quantity: ");
        scanf("%d", &quantity);

        int found = 0;
        rewind(fp); 
        while (fscanf(fp, "%d %s %f", &item.id, item.name, &item.price) != EOF) {
            if (item.id == orderId) {
                found = 1;
                printf("Added %d x %s (%.2f each)\n", quantity, item.name, item.price);
                total += item.price * quantity;

               
                fprintf(orderFile, "Order ID: %d, Food Name: %s, Quantity: %d, Total Price: %.2f\n",
                        item.id, item.name, quantity, item.price * quantity);
                break;
            }
        }

        if (!found) {
            printf("Item not found!\n");
        }

        printf("\nEnter Item ID to order (0 to finish): ");
        scanf("%d", &orderId);
    }

    if (total > 0) {
        
        fprintf(orderFile, "Total Order Price: %.2f\n", total);
        
        fprintf(orderFile, "----------------------------------------\n");
        printf("\nYour total bill is: %.2f\n", total);
    } else {
        printf("No items ordered.\n");
    }

    fclose(fp);
    fclose(orderFile);
}



void orderSection() {
    int choice;
    while (1) {
        printf("\nOrder Section\n");
        printf("1. View Menu\n");
        printf("2. Place Order\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewItems();
                break;
            case 2:
                placeOrder();
                break;
            case 3:
                return; 
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}



int main() {
    int choice;

    
    while (1) {
        
        printf("\n");
        printf("========================================\n");
        printf("    **** CANTEEN MANAGEMENT ****\n");
        printf("       **** AND BILLING ****\n");
        printf("         **** SYSTEM ****\n");
        printf("========================================\n");
        printf("\n");

        printf("\n");
        printf("1. Owner Login\n");
        printf("2. Menu and Order\n");
        printf("3. Exit\n");
        printf("Enter your choice(numbers only): ");
     
         

         if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); 
            continue; 
            
        }

        switch (choice) {
            case 1:
                ownerSection();
                break;
            case 2:
                orderSection();
                break;
            case 3:
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
               
        }
    }

    return 0;
}
