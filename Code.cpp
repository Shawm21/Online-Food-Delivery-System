
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declaring structure for representing a food item
typedef struct FoodItem {
    char name[50];
    float price;
    int quantity;
    struct FoodItem *next;
} FoodItem;

// Declaring function for creating a new food item
FoodItem* createFoodItem(const char *name, float price) {
    FoodItem *newItem = (FoodItem*)malloc(sizeof(FoodItem));
    if (newItem == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    strcpy(newItem->name, name);
    newItem->price = price;
    newItem->quantity = 0;
    newItem->next = NULL;
    return newItem;
}

// Structure to represent a node in the stack
typedef struct StackNode {
    FoodItem *item;
    struct StackNode *next;
} StackNode;

// Creating a new node to the stack
StackNode* createStackNode(FoodItem *item) {
    StackNode *newNode = (StackNode*)malloc(sizeof(StackNode));
    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    newNode->item = item;
    newNode->next = NULL;
    return newNode;
}

// Defining Stack
typedef struct Stack {
    StackNode *top;
} Stack;

Stack* initializeStack() {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    stack->top = NULL;
    return stack;
}

// Function to add a food item to the stack
void push(Stack *stack, FoodItem *item) {
    StackNode *newNode = createStackNode(item);
    newNode->next = stack->top;
    stack->top = newNode;
}

// Function for removing a food item from the stack.
FoodItem* pop(Stack *stack) {
    if (stack->top == NULL) {
        printf("Order is empty!\n");
        return NULL;
    }
    StackNode *temp = stack->top;
    FoodItem *poppedItem = temp->item;
    stack->top = temp->next;
    free(temp);
    return poppedItem;
}

// Function for displaying the order
void displayOrder(Stack *order) {
    printf("Your Order:\n");
    float total = 0.0;
    StackNode *current = order->top;
    int itemNum = 1;
    while (current != NULL) {
        printf("%d. %s - BDT %.2f x %d\n", itemNum++, current->item->name, current->item->price, current->item->quantity);
        total += current->item->price * current->item->quantity;
        current = current->next;
    }
    printf("Total: BDT %.2f\n", total);
}

// Function to display  menu
void displayMenu(FoodItem *menu) {
    system("cls");
    printf("Menu:\n");
    int itemNum = 1;
    while (menu != NULL) {
        printf("%d. %s - BDT %.2f\n", itemNum++, menu->name, menu->price);
        menu = menu->next;
    }
    printf("\nPress Enter to return to the main menu...\n");
    getchar();
    getchar();
}

// Function for displaying the welcome message in first page.
void displayWelcome() {
    printf("Welcome to House of Ramen's Online Food Delivery App!\n");
    printf("\nPlease choose your action from below:\n");
}

// Function for removing an item
void removeItem(Stack *order, int itemNumber, int quantity) {
    if (order->top == NULL) {
        printf("Order is empty!\n");
        return;
    }

    StackNode *current = order->top;
    StackNode *prev = NULL;

    for (int i = 1; i < itemNumber && current != NULL; i++) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Invalid choice!\n");
        return;
    }

    if (current->item->quantity >= quantity) {
        current->item->quantity -= quantity;
        printf("%d %s removed from your order.\n", quantity, current->item->name);
        if (current->item->quantity == 0) {
            // If quantity becomes 0, remove the item from the order
            if (prev == NULL) {
                order->top = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
        }
    } else {
        printf("Insufficient quantity to remove!\n");
    }
}

int main() {
    // Creating menu items
    FoodItem *menu = createFoodItem("Chili Oil Dumplings", 280.0);
    menu->next = createFoodItem("Nasi Goreng", 300.0);
    menu->next->next = createFoodItem("Masala Fries", 110.0);
    menu->next->next->next = createFoodItem("Mandu", 200.0);
    menu->next->next->next->next = createFoodItem("Spicy Seafood Ramen", 200.0);
    menu->next->next->next->next->next = createFoodItem("Sushi", 400.0);
    menu->next->next->next->next->next->next = createFoodItem("Chowmein", 250.0);
    menu->next->next->next->next->next->next->next = createFoodItem("Seafood Noodles", 280.0);
    menu->next->next->next->next->next->next->next->next = createFoodItem("Kitkat Shake", 180.0);
    menu->next->next->next->next->next->next->next->next->next = createFoodItem("Soft Drinks", 30.0);

    // Initializing stack for order
    Stack *order = initializeStack();

    // Displaying the welcome message
    displayWelcome();

    int choice, quantity;
    do {
        printf("\n1. Show menu\n");
        printf("2. Add item to order\n");
        printf("3. Remove item from order\n");
        printf("4. Display order\n");
        printf("5. Exit and Place Order\n");
        printf("\nEnter your choice: \n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayMenu(menu);
                break;
            case 2: {
                printf("Enter the item number to add to your order: ");
                int itemNumber;
                scanf("%d", &itemNumber);
                printf("Enter the quantity: ");
                scanf("%d", &quantity);

                FoodItem *selectedItem = menu;
                for (int i = 1; i < itemNumber; i++) {
                    if (selectedItem->next == NULL) {
                        printf("Invalid item number.\n");
                        selectedItem = NULL;
                        break;
                    }
                    selectedItem = selectedItem->next;
                }
                if (selectedItem) {
                    selectedItem->quantity += quantity;
                    push(order, selectedItem);
                    printf("%d %s added to your order.\n", quantity, selectedItem->name);
                }
                break;
            }
            case 3: {
                if (order->top == NULL) {
                    printf("Order is empty!\n");
                    break;
                }
                displayOrder(order);
                printf("Enter the item number to remove from your order: ");
                int itemNumber;
                scanf("%d", &itemNumber);
                printf("Enter the quantity to remove: ");
                scanf("%d", &quantity);
                removeItem(order, itemNumber, quantity);
                break;
            }
            case 4:
                displayOrder(order);
                break;
            case 5:
                printf("\n Confirm your Checkout\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    } while (choice != 5);

    // To get user Details for their order.
    char address[100];
    char contactNumber[15];

    printf("Please enter your delivery address: ");
    getchar();
    fgets(address, sizeof(address), stdin);
    address[strcspn(address, "\n")] = 0;

    printf("Please enter your contact number: ");
    fgets(contactNumber, sizeof(contactNumber), stdin);
    contactNumber[strcspn(contactNumber, "\n")] = 0;

    // Displaying the Final Order
    displayOrder(order);
    printf("Delivery Address: %s\n", address);
    printf("Contact Number: %s\n", contactNumber);
    printf("Thank you for your order! Your food will be delivered soon.\n");

    // Making Free Memory
    while (menu != NULL) {
        FoodItem *temp = menu;
        menu = menu->next;
        free(temp);
    }
    while (order->top != NULL) {
        StackNode *temp =        order->top->next;
        free(temp);
    }
    free(order);

    return 0;
}

