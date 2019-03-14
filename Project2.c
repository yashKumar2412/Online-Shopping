#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef enum StatusCode {
	FAILURE = 0,
	SUCCESS = 1 
} StatusCode;

struct Product {
	int productID;
	char pName[30];
	float cost;
	int qty;
	int category;
	struct Product* nextPr;
};

struct Orders {
	int productID;
	char pName[30];
	int qty;
};

struct Consumer {
	int consumerID;
	char email[20];
	char cName[50];
	char phoneNumber[10];
	char address[50];
	struct Orders prevOrders[10];
	int numOfPrevOrders;
	struct Orders favOrders[10];
	int numOfDistinctItemsBought;
	struct Orders wishlist[10];
	int numOfItemsOnWishList;
	int numOfItemsBoughtTotal;
	char prefMode[20];
	struct Consumer* nextCons;
};

void addProduct(struct Product** pList, int prID, char prName[], float cost, int qty, int ctg)
{
	struct Product* newPr = (struct Product*) malloc(sizeof(struct Product));
	newPr->productID = prID;
	strcpy(newPr->pName,prName);
	newPr->cost = cost;
	newPr->qty = qty;
	newPr->category = ctg;
	newPr->nextPr = (*pList);
	(*pList) = newPr;
}

void createUser(struct Consumer** cList, int id)
{
	int found, rep, mode_ch;
	struct Consumer* first;
	char consEmail[20];
	first = (*cList);
	do
	{
		(*cList) = first;
		printf("Enter email address: ");
		scanf("%s",consEmail);
		rep = 0;
		found = 0;
		while((*cList) != NULL && found == 0)
		{	
			if(strcmp((*cList)->email,consEmail) == 0)
				found = 1;
			(*cList) = (*cList)->nextCons;
		}
		if(found == 1)
		{	
			printf("This email ID already exists. Try again \n");
			rep = 1;
		}		
		else
		{
			rep = 0;
			(*cList) = first;
			struct Consumer* new1 = (struct Consumer*) malloc(sizeof(struct Consumer));
			char pNo[10], name[50], address[50], modeOfPayment[20];
			printf("Enter phone number: ");
			scanf("%s",pNo);
			printf("Enter name: ");
			scanf("%s",name);
			printf("Enter address: ");
			scanf("%s",address);
			printf("Enter the key to select your preferred mode of payment \n");
			printf("1. BNPL \n");
			printf("2. Cash On Delivery \n");
			printf("3. Debit Card \n");
			printf("4. Credit Card \n");
			printf("5. Net Banking \n");
			printf("6. UPI Enabled \n");
			printf("7. PhonePe \n");
			scanf("%d",&mode_ch);
			do
			{
				switch(mode_ch)
				{
					case 1:
						{
							strcpy(new1->prefMode,"Buy Now, Pay Later");
							break;	
						}
					case 2:
						{
							strcpy(new1->prefMode,"Cash On Delivery");
							break;	
						}
					case 3:
						{
							strcpy(new1->prefMode,"Debit  Card");
							break;	
						}
					case 4:
						{
							strcpy(new1->prefMode,"Credit Card");
							break;	
						}
					case 5:
						{
							strcpy(new1->prefMode,"Net Banking");
							break;	
						}
					case 6:
						{
							strcpy(new1->prefMode,"UPI Enabled");
							break;	
						}
					case 7:
						{
							strcpy(new1->prefMode,"PhonePe");
							break;	
						}	
					default:
						{
							printf("Inappropriate Choice \n");
							strcpy(new1->prefMode,"None");
						}
				};
			}while(strcmp(new1->prefMode,"None") == 0);
			strcpy(new1->address,address);
			strcpy(new1->cName,name);
			strcpy(new1->phoneNumber,pNo);
			strcpy(new1->email,consEmail);
			new1->consumerID = id;
			new1->numOfDistinctItemsBought = 0;
			new1->numOfItemsBoughtTotal = 0;
			new1->numOfItemsOnWishList = 0;
			new1->numOfPrevOrders = 0;
			new1->nextCons = (*cList);
			(*cList) = new1;
			printf("Consumer ID Generated: %d \n\n",new1->consumerID);
		}		
	}while(rep == 1);		
}

void printItemsForACategory(int ctg, struct Product* pList)
{
	while(pList != NULL)
	{
		if(pList->category == ctg)
		{
			printf("%d: %s \n",pList->productID,pList->pName);
			printf("Quantity: %d \n",pList->qty);
			printf("\n");
		}
		pList = pList->nextPr;
	}
}

StatusCode orderItem(struct Consumer* cList, struct Product* pList)
{
	StatusCode sc = SUCCESS;
	int prodID, found = 0, f, qty, rep, i, ctg;
	printf("Enter the key for the category of the item you wish to buy \n");
	printf("1. Mobile Phones \n");
	printf("2. Mobile Accessories \n");
	printf("3. Laptops & Computer Accessories & Tablets \n");
	printf("4. Electronic Gadgets \n"); 
	printf("5. Games & Gaming Console \n"); 
	printf("6. Home & Kitchen Appliances \n");
	scanf("%d",&ctg);
	printItemsForACategory(ctg, pList);
	printf("Enter ID of the product you wish to buy: ");
	scanf("%d",&prodID);
	while(pList != NULL && found == 0)
	{
		if((pList->category == ctg) && (pList->productID == prodID))
		{
			found = 1;
			rep = 1;
			do
			{
				printf("Enter the quantity: ");
				scanf("%d",&qty);
				if(qty <= 0)
				{
					printf("Invalid quantity. Try again. \n");
					rep = 1;
				}
				else if(qty > pList->qty)
				{	
					rep = 0;
					int ch;
					printf("%d %s are not available. \n",qty,pList->pName);
					printf("Press 1 if you wish to try to order this item again, else press anything else: ");
					scanf("%d",&ch);
					if(ch == 1)
						rep = 1;
					else
						sc = FAILURE;	
				}
				else
				{
					pList->qty = pList->qty - qty;
					float price;
					price = pList->cost * qty;
					printf("Cost of buying is: %.1f \n",price);
					rep = 0;
					strcpy(cList->prevOrders[cList->numOfPrevOrders].pName,pList->pName);
					cList->prevOrders[cList->numOfPrevOrders].productID = pList->productID;
					cList->prevOrders[cList->numOfPrevOrders].qty = qty;
					cList->numOfPrevOrders++;
					cList->numOfItemsBoughtTotal += qty;					 
					i = 0;
					f = 0;
					while(i < cList->numOfDistinctItemsBought && f == 0)
					{
						if(cList->favOrders[i].productID == pList->productID)
						{
							cList->favOrders[i].qty += qty;
							f = 1;
						}
						i++;
					}
					if(f == 0)
					{
						strcpy(cList->favOrders[i].pName,pList->pName);
						cList->favOrders[i].productID = pList->productID;
						cList->favOrders[i].qty = qty;
						cList->numOfDistinctItemsBought++;
					}
				}
			}while(rep == 1);
		}
		else
			pList = pList->nextPr;	
	}
	if(found == 0)
	{
		printf("Invalid ID. \n");
		sc = FAILURE;
	}
	return sc;	
} 

StatusCode addToWishList(struct Consumer* cList, struct Product* pList)
{
	StatusCode sc = SUCCESS;
	int prodID, i, found = 0, f;
	int ctg;
	printf("Enter the key for the category of the item you wish to add to your wishlist \n");
	printf("1. Mobile Phones \n"); 
	printf("2. Mobile Accessories \n");
	printf("3. Laptops & Computer Accessories & Tablets \n"); 
	printf("4. Electronic Gadgets \n"); 
	printf("5. Games & Gaming Console \n"); 
	printf("6. Home & Kitchen Appliances \n");
	scanf("%d",&ctg);
	printItemsForACategory(ctg, pList);
	printf("Enter ID of the product you wish to add to the wishlist: ");
	scanf("%d",&prodID);
	while(pList != NULL && found == 0)
	{
		if((pList->category == ctg) && (pList->productID == prodID))
		{
			found = 1;
			i = 0;
			f = 0;
			while(i < cList->numOfItemsOnWishList && f == 0)
			{
				if(cList->wishlist[i].productID == prodID)
				{
					f = 1;
					printf("Item already on wishlist. \n");
				}
				i++;
			}
			if(f == 0)
			{
				strcpy(cList->wishlist[cList->numOfItemsOnWishList].pName,pList->pName);
				cList->wishlist[cList->numOfItemsOnWishList].productID = pList->productID;
				cList->wishlist[cList->numOfItemsOnWishList].qty = 0;
				cList->numOfItemsOnWishList++;
				printf("New item successfully added. ID: %d \n",pList->productID);
			}
		}
		else
			pList = pList->nextPr;	
	}
	if(found == 0)
	{
		printf("Invalid ID. \n");
		sc = FAILURE;
	}
	printf("\n");
	return sc;
}

StatusCode returnProduct(struct Product* pList)
{
	StatusCode sc = SUCCESS;
	int prodID, qty, found = 0;
	int ctg;
	printf("Enter the key for the category of the item you wish to return \n");
	printf("1. Mobile Phones \n"); 
	printf("2. Mobile Accessories \n");
	printf("3. Laptops & Computer Accessories & Tablets \n"); 
	printf("4. Electronic Gadgets \n"); 
	printf("5. Games & Gaming Console \n"); 
	printf("6. Home & Kitchen Appliances \n");
	scanf("%d",&ctg);
	printItemsForACategory(ctg, pList);
	printf("Enter ID of the product you wish to return: ");
	scanf("%d",&prodID);
	while(pList != NULL && found == 0)
	{
		if((pList->category == ctg) && (pList->productID == prodID))
		{
			found = 1;
			printf("How many items do you wish to return? ");
			scanf("%d",&qty);
			pList->qty += qty;
		}
		else
			pList = pList->nextPr;
	}
	if(found == 0)
	{
		printf("Invalid ID. \n");
		sc = FAILURE;
	}
	printf("\n");
	return sc;
}

void printWishList(struct Consumer* cList)
{
	int i = 0;
	printf("Items on wishlist: %d \n",cList->numOfItemsOnWishList);
	while(i < cList->numOfItemsOnWishList)
	{
		printf("%d: %s \n",cList->wishlist[i].productID,cList->wishlist[i].pName);
		i++;
	}
}

void printAvailableProducts(struct Product* pList)
{
	while(pList != NULL)
	{
		if(pList->qty > 0)
		{
			printf("%d: %s \n",pList->productID,pList->pName);
			printf("Quantity: %d \n",pList->qty);
			printf("\n");
		}
		pList = pList->nextPr;
	}
}

void printCustomerInfo(struct Consumer* cList)
{
	int consID, f = 0;
	printf("Enter consumer ID: ");
	scanf("%d",&consID);
	while(cList != NULL && f == 0)
	{
		if(cList->consumerID == consID)
		{
			f = 1;
			printf("ID: %d \nName: %s \n",cList->consumerID,cList->cName);
			printf("Email: %s \nPhone Number: %s \n",cList->email,cList->phoneNumber);
			printf("Preferred Mode Of Payment: %s \n",cList->prefMode);
		}
		else
			cList = cList->nextCons;
	}
	if(f == 0)
		printf("No such customer present. \n");
	printf("\n");
}

void printItemsPurchasedByACustomerFreq(struct Consumer* cList, struct Product* pList)
{
	int consID, f = 0, i, j, max, maxPos[10];
	printf("Enter Consumer ID to search for: ");
	scanf("%d",&consID);
	while(cList != NULL && f == 0)
	{
		if(cList->consumerID == consID)
		{
			f = 1;
			i = 1;
			max = cList->favOrders[0].qty;
			maxPos[0] = 0;
			j = 1;
			while(i < cList->numOfDistinctItemsBought)
			{
				if(cList->favOrders[i].qty == max)	
				{
					maxPos[j] = i;
					j++;
				}
				else if(cList->favOrders[i].qty > max)
				{
					max = cList->favOrders[i].qty;
					maxPos[0] = i;
					j = 1;
				}
				i++;
			}		
		}
		else
			cList = cList->nextCons;
	}
	if(f == 0)
		printf("No such consumer. \n");
	else
	{
		i = 0;
		while(i < j)
		{
			printf("%d: %s \n",cList->favOrders[maxPos[i]].productID,cList->favOrders[maxPos[i]].pName);
			i++;
		}
		printf("Quantity: %d \n",max);	
	}
	printf("\n");
}

void printItemsPurchasedByAllCustomersFreq(struct Consumer* cList, struct Product* pList)
{
	int i, j, k, found, max, maxPos[10];
	struct Orders allPurchases[20];
	i = 0;
	while(cList != NULL)
	{
		j = 0;
		while(j < cList->numOfDistinctItemsBought)
		{
			k = 0;
			found = 0;
			while(k < i && found == 0)
			{
				if(allPurchases[k].productID == cList->favOrders[j].productID)
				{
					found = 1;
					allPurchases[k].qty += cList->favOrders[j].qty;	
				}	
				k++;
			}
			if(found == 0)
			{
				strcpy(allPurchases[k].pName,cList->favOrders[j].pName);
				allPurchases[k].productID = cList->favOrders[j].productID;
				allPurchases[k].qty = cList->favOrders[j].qty;
				i++;
			}
			j++;
		}		
		cList = cList->nextCons;
	}
	k = 0;
	max = allPurchases[0].qty;
	maxPos[0] = 0;
	j = 1;
	k = 1;
	while(k < i)
	{
		if(allPurchases[k].qty == max)
		{
			maxPos[j] = k;
			j++;
		}
		else if(allPurchases[k].qty > max)
		{
			max = allPurchases[k].qty;
			maxPos[0] = k;
			j = 1;
		}
		k++;
	}
	k = 0;
	while(k < j)
	{
		printf("%d: %s \n",allPurchases[maxPos[k]].productID,allPurchases[maxPos[k]].pName);
		k++;
	}
	printf("Quantity: %d \n",max);
	printf("\n");
}

void swapCustomer(struct Consumer* c1, struct Consumer* c2)
{	
	int temp, i;
	struct Orders tempOrder[10];
	char tempStr[50];
	
	//ID
	temp = c1->consumerID;
	c1->consumerID = c2->consumerID;
	c2->consumerID = temp;
	
	//Previous Orders
	for(i = 0; i < c1->numOfPrevOrders; i++)
		tempOrder[i] = c1->prevOrders[i];
	for(i = 0; i < c2->numOfPrevOrders; i++)
		c1->prevOrders[i] = c2->prevOrders[i];
	for(i = 0; i < c1->numOfPrevOrders; i++)
		c2->prevOrders[i] = tempOrder[i];
	
	//Favourite Orders
	for(i = 0; i < c1->numOfDistinctItemsBought; i++)
		tempOrder[i] = c1->favOrders[i];
	for(i = 0; i < c2->numOfDistinctItemsBought; i++)
		c1->favOrders[i] = c2->favOrders[i];
	for(i = 0; i < c1->numOfDistinctItemsBought; i++)
		c2->favOrders[i] = tempOrder[i];
		
	//Wishlist	
	for(i = 0; i < c1->numOfItemsOnWishList; i++)
		tempOrder[i] = c1->wishlist[i];
	for(i = 0; i < c2->numOfItemsOnWishList; i++)
		c1->wishlist[i] = c2->wishlist[i];
	for(i = 0; i < c1->numOfItemsOnWishList; i++)
		c2->wishlist[i] = tempOrder[i];
		
	//nums
	temp = c1->numOfDistinctItemsBought;
	c1->numOfDistinctItemsBought = c2->numOfDistinctItemsBought;
	c2->numOfDistinctItemsBought = temp;
	
	temp = c1->numOfItemsBoughtTotal;
	c1->numOfItemsBoughtTotal = c2->numOfItemsBoughtTotal;
	c2->numOfItemsBoughtTotal = temp;
	
	temp = c1->numOfItemsOnWishList;
	c1->numOfItemsOnWishList = c2->numOfItemsOnWishList;
	c2->numOfItemsOnWishList = temp;
	
	temp = c1->numOfPrevOrders;
	c1->numOfPrevOrders = c2->numOfPrevOrders;
	c2->numOfPrevOrders = temp;
	
	//email
	strcpy(tempStr,c1->email);
	strcpy(c1->email,c2->email);
	strcpy(c2->email,tempStr);
	
	//phone number
	strcpy(tempStr,c1->phoneNumber);
	strcpy(c1->phoneNumber,c2->phoneNumber);
	strcpy(c2->phoneNumber,tempStr);
	
	//name
	strcpy(tempStr,c1->cName);
	strcpy(c1->cName,c2->cName);
	strcpy(c2->cName,tempStr);
	
	//address
	strcpy(tempStr,c1->address);
	strcpy(c1->address,c2->address);
	strcpy(c2->address,tempStr);
	
	//mode of payment
	strcpy(tempStr,c1->prefMode);
	strcpy(c1->prefMode,c2->prefMode);
	strcpy(c2->prefMode,tempStr);
}

void printCustomerListDecreasingOrderOfPurchases(struct Consumer* cList)
{
	int swapped, i;
	struct Consumer* first = cList;
	struct Consumer* cptr;
	struct Consumer* temp1;
	struct Consumer* temp2;
	struct Consumer* lptr = NULL;
	do
	{
		swapped = 0;
		cptr = cList;
		while(cptr->nextCons != lptr)
		{
			if(cptr->numOfItemsBoughtTotal < cptr->nextCons->numOfItemsBoughtTotal)
			{
				swapped = 1;
				swapCustomer(cList,cList->nextCons);
			}	
			cptr = cptr->nextCons;
		}
		lptr = cptr;	
	}while(swapped);	
	cList = first;
	while(cList != NULL)
	{
		printf("%s: %d\n",cList->cName,cList->numOfItemsBoughtTotal);
		cList = cList->nextCons;
	}
	cList = first;
}	

//Driver Code
int main()
{
	StatusCode sc;
	struct Product* itemsList = (struct Product*) malloc(sizeof(struct Product));
	struct Consumer* customersList = (struct Consumer*) malloc(sizeof(struct Consumer));
	struct Consumer* first;
	
	printf("Creating list of products... \n");
	itemsList = NULL;
	//Mobile Phones
	addProduct(&itemsList,101,"Google Pixel 2",50000.0,6,1);
	addProduct(&itemsList,102,"Moto G6",16000.0,4,1);
	addProduct(&itemsList,103,"OnePlus 5",35000.0,2,1);
	addProduct(&itemsList,104,"Jio Phone",2500.0,7,1);
	
	//Mobile Accessories
	addProduct(&itemsList,201,"JBL Earphones",2000.0,3,2);
	addProduct(&itemsList,202,"Phillips Earphones",350.0,8,2);
	addProduct(&itemsList,203,"JBL Headphones",3000.0,1,2);
	addProduct(&itemsList,204,"Lenovo Power Bank",700.0,4,2);
	addProduct(&itemsList,205,"Mi Power Bank",1500.0,3,2);
	
	//Laptops & Computer Accessories & Tablets
	addProduct(&itemsList,301,"Dell Vostro 3568",50000.0,5,3);
	addProduct(&itemsList,302,"Apple Macbook Pro",100000.0,3,3);
	addProduct(&itemsList,303,"Dell Inspiron 3569",70000.0,2,3);
	addProduct(&itemsList,304,"Apple Ipad Pro",56000.0,6,3);
	addProduct(&itemsList,305,"Lenovo Tab7",21000.0,2,3);
	addProduct(&itemsList,306,"Sandisk 32 GB Pen Drive",600.0,1,3);
	
	//Electronic Gadgets
	addProduct(&itemsList,401,"Data Cable Port C",400.0,6,4);
	addProduct(&itemsList,402,"Data Cable Port B",350.0,2,4);
	
	//Games & Gaming Consoles
	addProduct(&itemsList,501,"PS4 Pro",39000.0,2,5);
	addProduct(&itemsList,502,"Xbox One",29000.0,3,5);
	addProduct(&itemsList,503,"Nintendo Switch",26000.0,1,5);
	addProduct(&itemsList,504,"Spiderman PS4",2700.0,2,5);
	addProduct(&itemsList,505,"FIFA 19",2300.0,3,5);
	addProduct(&itemsList,506,"Pokemon Sword/Pokemon Shield",3800.0,0,5);
	
	//Home & Kitchen Appliances
	addProduct(&itemsList,601,"Water Purifier",10000.0,2,6);
	addProduct(&itemsList,602,"Iron",550.0,3,6);
	addProduct(&itemsList,603,"Induction Cooker",2200.0,1,6);
	printf("List of all products created. \n");
	
	customersList = NULL;
	int consID, found, rep = 0, ch1, ch2, choice, id;
	char main_ch;
	main_ch = 'y';
	id = 101;
	do
	{
		do
		{
			printf("Are you a User or an Admin?\nPress 1 to enter User mode or 2 to enter Admin mode. Press 0 to Exit: ");
			scanf("%d",&ch1);
			if(ch1 == 1)
			{
				//User Mode
				printf("Press 1 if you're a new user or 2 if you're an existing user: ");
				scanf("%d",&ch2);
				if(ch2 == 1)
				{
					rep = 0;
					createUser(&customersList,id);
					id++;
					consID = customersList->consumerID;
					first = customersList;
				}
				else if(ch2 == 2)
				{
					printf("Enter existing Consumer ID: ");
					scanf("%d",&consID);
					found = 0;
					first = customersList;
					while(customersList != NULL && found == 0)
					{
						if(customersList->consumerID == consID)
							found = 1;
						else
							customersList = customersList->nextCons;	
					}
					if(found == 0)
					{
						rep = 1;
						printf("User not found \n");
					}
				}
				else
				{
					printf("Inappropriate choice \n");
					rep = 1;
				}
			}
			else if(ch1 == 2)
			{	
				//Admin mode
				int admID;
				char pw[8];
				printf("Enter Admin ID and password to enter this mode \n");
				printf("Admin ID: ");
				scanf("%d",&admID);
				printf("Password: ");
				scanf("%s",pw);
				if(admID == 20941 && strcmp(pw,"password") == 0)
				{
					printf("Admin Mode entered successfully \n");
					rep = 0;
				}
				else
				{
					printf("Incorrect ID or password, try again \n");
					rep = 1;
				}
			}
			else if(ch1 == 0)
			{
				rep = 0;
				printf("Exiting Process \n");
			}
			else
			{
				printf("Inappropriate Entry, try again \n");
				rep = 1;
			}
		}while(rep == 1);
		ch2 = 1;
		if(ch1 == 1)
		{
			do
			{
				choice = 1;
				printf("Press 1 to Order a product \n");
				printf("Press 2 to Add a product to wish list \n");
				printf("Press 3 to Return a product \n");
				printf("Press 4 to Print wish list for a user \n");
				printf("Press 5 to Print product list for available products \n");
				printf("Press 6 to Log out \n");
				scanf("%d",&choice);
				switch(choice)
				{
					case 1: //Order a product - works
						{
							StatusCode sc;
							sc = orderItem(customersList, itemsList);	
							if(sc == SUCCESS)
								printf("Item bought successfully \n");
							else
								printf("Purchase unsuccessful \n");
							break;	
						}
					case 2: //Add a product to wish list - works
						{
							StatusCode sc;
							sc = addToWishList(customersList, itemsList);	
							if(sc == SUCCESS)
								printf("Item added to wishlist successfully \n");
							else
								printf("Operation unsuccessful \n");
							break;	
						}
					case 3: //Return a product - works
						{
							StatusCode sc;
							sc = returnProduct(itemsList);	
							//O/P
							break;	
						}
					case 4: //Print wish list for a user - works
						{
							while(customersList != NULL && found == 0)
							{
								if(customersList->consumerID == consID)
									found = 1;
								else
									customersList = customersList->nextCons;								
							}
							if(found == 0)
								printf("Invalid ID");
							else
								printWishList(customersList);
							customersList = first;
							break;	
						}
					case 5: //Print product list for available products - works
						{
							printAvailableProducts(itemsList);
							break;	
						}		
					case 6: //Logout - works
						{
							ch2 = 0;
							customersList = first;
							break;	
						}
					default: //Inappropriate choice - works
						{
							printf("Enter appropriate choice \n");	
						}	
				};
			}while(ch2 == 1);
		}
		else if(ch1 == 2)
		{
			first = customersList;
			while(customersList != NULL)
			{
				printf("%d\n",customersList->consumerID);
				customersList = customersList->nextCons;
			}
			printf("Done \n");
			customersList = first;
			do
			{
				printf("Press 1 to Print wish list for a user \n");
				printf("Press 2 to Print product list for available products \n");
				printf("Press 3 to Print customer information for a given customer \n");
				printf("Press 4 to Print frequently purchased items by a customer \n");
				printf("Press 5 to Print frequently purchased items by all customers \n");
				printf("Press 6 to Print the customer list in decreasing order of their frequency of purchases \n");
				printf("Press 7 to Log out \n");
				scanf("%d",&choice);
				switch(choice)
				{
					case 1: //Print wish list for a user - works
						{
							int found = 0;
							printf("Enter Consumer ID you wish to search for: ");
							scanf("%d",&consID);
							while(customersList != NULL && found == 0)
							{
								if(customersList->consumerID == consID)
									found = 1;
								else
									customersList = customersList->nextCons;								
							}
							if(found == 1)
								printWishList(customersList);
							else
								printf("Invalid ID \n");
							customersList = first;
							break;
						}
					case 2: //Print product list for available products - works
						{
							printAvailableProducts(itemsList);
							break;	
						}
					case 3: //Print customer info for a given customer - works
						{
							printCustomerInfo(customersList);
							customersList = first;
							break;	
						}
					case 4: //Print frequently purchased items by a customer - works
						{
							printItemsPurchasedByACustomerFreq(customersList, itemsList);
							customersList = first;
							break;	
						}	
					case 5: //Print frequently purchased items by all customers - works
						{
							printItemsPurchasedByAllCustomersFreq(customersList, itemsList);
							customersList = first;
							break;	
						}
					case 6: //Print the customer list in decreasing order of their frequency of purchases - yet to work
						{
							printCustomerListDecreasingOrderOfPurchases(customersList);
							customersList = first;
							break;
						}
					case 7: //Logout - works
						{
							ch2 = 0;
							break;	
						}	
					default: //Inappropriate choice - works
						{
							printf("Enter appropriate choice \n");	
						}	
				};
			}while(ch2 == 1);
		}
		else if(ch1 == 0)
			main_ch = 'n';
	}while(main_ch == 'y');
	return 0;
}

