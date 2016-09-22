#include <stdio.h>
#include <conio.h>
#define RECORDS 10000
#define NO_FILES 3
#define SIZE_2 2

// structure template
struct database{

	char college[11]; // changed from 4 to 11 to account for input error, if user enters a value more than 4 the terminated NULL character will be overwritten
    char firstname[16];
    char surname[21];
    char key[38];
};

// display menu colors for viewing students
#define COLOR_3 textcolor(3)
#define COLOR_5 textcolor(5)
#define COLOR_14 textcolor(14)

// function prototypes
void canonical(char*,char*,char*);
int open_file_save(FILE *,struct database*,int*,char*);

// program menu's
void program_menu(int*);
void program_sub_menu_binary_file(int*);

// sorting & searching algorithms
void insertion_sort(struct database*,int);
void merge(struct database*,struct database*,int,struct database*,int);
void merge_sort(struct database*,int);
int binary_search(struct database*,char *key,int low,int high,int by_surname);

// display functions to display text and color
void display(struct database*,int,bool,int);
void text_n_color(char*,int);

// void replace_database(struct database **);
void replace_database(struct database**,struct database*,int*,int*);
void append_database(struct database**,struct database*,int*,int*);



// program menu function
void program_menu(int *option){

    printf("================================================================================\n\n");
    printf("Please select one of the following options and press enter:\n\n\n");
    printf("1. Create sorted list from the three files\n");
    printf("2. Search students by college\n");
    printf("3. Search student surname\n");
    printf("4. Add new student to database\n");
	printf("5. Remove student from database\n");
    printf("6. Read in binary file\n\n");
    printf("7. Exit Program\n\n");
    printf("================================================================================\n\n");
	
	scanf("%d",&(*option));
	
}// end program menu function



// program sub menu function
void program_sub_menu_binary_file(int *option){

    printf("================================================================================\n\n");
    printf("Please select one of the following options and press enter:\n\n\n");
    printf("1. Replace database with binary file\n");
	printf("2. Append binary file to database\n");
	printf("3. Press any other button to go back to main menu\n\n");
    printf("4. Exit Program\n\n");
    printf("================================================================================\n\n");
    
	scanf("%d",&(*option));
	
    printf("\n");
    
}// end program menu function



// function to replace database with binary file that was just read in.   // http://www.tutorialspoint.com/cprogramming/c_pointer_to_pointer.htm
void replace_database(struct database **ptr,struct database *newdata,int *K,int *input){
    
    // new student database size
    *K = *input;
    
    *ptr = (struct database*)realloc(*ptr,(*input)*sizeof(struct database));
    
	// check if successful
    if(*ptr == NULL){
        text_n_color("Failed to reallocate memory",12);
    }
    else{
        // copying newdata to data
        memmove(*ptr,newdata,*(input)*sizeof(struct database));
    }
    
}// end replace database function



// function to append database with binary file  // using pointer to pointer to access data in main()  // http://www.tutorialspoint.com/cprogramming/c_pointer_to_pointer.htm
void append_database(struct database **ptr,struct database* newdata,int *K,int* input){
    
	int new_size = *K + *input;
	
    *ptr = (struct database *)realloc(*ptr,new_size*sizeof(struct database));
	
	// check if successful
    if(*ptr == NULL){
        text_n_color("Failed to reallocate memory",12);
    }
    else{
        
        for(register int i = *K; i<new_size; i++){
            // copying newdata to data
            memmove((*ptr + i),(newdata + (i-*K)),sizeof(struct database));
        }
		
		// new student database size
		*K = new_size;
        
		// resort the data
        merge_sort(*ptr,new_size);
        
    }// end else

}// end append database function



// canonical form function
void canonical(char* f,char* l, char* c){
    
	strcpy(c,l);
	strcat(c,".");
	strcat(c,f);
    
	int i=0;
    
	while(c[i]!=0){
        
        // change to uppercase
		c[i]=toupper(c[i]);
		i++;
        
	}// end while
    
}// end canonical function



int open_file_save(FILE* fp,struct database* data,int* K,char* filename){
    
    int flag_file = 0;
    
    // open file for reading
    if((fp=fopen(filename,"r")) !=NULL){
		
        // go through the number of records in the file    // scaning in data into data structure while there is a record
        while((fscanf(fp,"%s%s%s", (data + *K)->college,  (data + *K)->firstname,  (data + *K)->surname))>0){
			
            // calling canonical after scan in
            canonical( (data + *K)->firstname,  (data + *K)->surname,  (data + *K)->key );
			
            // incrementing K by 1       // K = Number of students
			*K = *K + 1;
			
        }// end while loop
        
        // close file
        fclose(fp);
    }
    else{
        
        textcolor(10);
        // if any file has failed to open tell user
        cprintf("\nFailed to open file: %s",filename);
        printf("\n");
        
        // failed to open a file flag
        flag_file++;
        
    }// end else
    
    // if any file has failed to open
    return (flag_file);
    
}// end open and save function



// insertion sort function
void insertion_sort(struct database *data, int k){

	// declare structure array
    struct database *value;
	
	// temp array used to hold value of structure
	value =(struct database *)malloc(sizeof(struct database));
	
	// keep track of hole in array
	int hole = 0;
	
	// index starts at index[1] as index[0] is assumed to be sorted
	for(int i=1; i<k; i++){
		
		// copying (data + i) to value
		memmove(value , (data + i),sizeof(struct database)); 
		
		// keeping track of hole 
		hole = i;
		
		// while the hole is not less then index[0] and data at index[hole-1] is not less then value
		while(hole>0 && strcmp((data + (hole-1))->key, value->key)>0){
			
			// copying (data + (hole-1)) to (data + hole)
			memmove((data + hole),(data + (hole-1)), sizeof(struct database)); 
			
			hole--;
            
		}// end while
        
		memmove((data + hole),(value + 0), sizeof(struct database));
		
	}// end for
	
	// free used up memory
	free(value);

}// end insertion sort



// changing text color 
void text_n_color(char* text,int color){

	printf("\n");
	textcolor(color);
	cprintf("%s",text);
	printf("\n\n");

}// text color change function



// merge function
void merge(struct database *A,struct database *L,int leftCount,struct database *R,int rightCount){
    
    int i,j,k;
    
    // i - to mark the index of left sub-array (L)
	// j - to mark the index of right sub-array (R)
	// k - to mark the index of merged sub-array (A)
    i = j = k = 0;
    
    while(i<leftCount && j<rightCount){
        
		//  <0 show list in descending form    so    >0  would be ascending
        if(strcmp((L+i)->key, (R+j)->key)<0){
            // (A+k) = *(L+i); i++;
            // copying (L + i++) to (A + k) 
            memmove((A + k),(L + i++),sizeof(struct database)); 
            
        }
        else{
            // *(A+k) = *(R+j); j++;
            // copying (R + j++) to (A + k)
            memmove((A + k),(R + j++),sizeof(struct database)); 
			
        }// end else
        
        k++;
        
    }// end while
    
    while(i<leftCount){
        // *(A+k++) = *(L+i++);
        // copying (L + i++) to (A + k++)
        memmove((A + k++),(L + i++),sizeof(struct database)); 
    }
    
    while(j<rightCount){
        // *(A+k++) = *(R+j++);
        // copying (R + j++) to (A + k++)
        memmove((A + k++),(R + j++),sizeof(struct database)); 
    }
    
}// end merge function



// merge_sort function
void merge_sort(struct database *A,int k){
    
    int mid,i;
    
    // declare structure arrays
    struct database *L,*R;
    
    if(k<2){return;}  // base condition
    
    mid = k/2;  // find the mid index. 
    
    // create left and right subarrays
	// mid elements (from index 0 till mid) should be part of left sub-array 
	// and (k-mid) elements (from mid to k) will be part of right sub-array
    L =(struct database *)malloc(mid*sizeof(struct database));
    R =(struct database *)malloc((k - mid)*sizeof(struct database));
    
    for(i = 0;i<mid;i++){
        //L[i] = A[i]; // creating left subarray
        // copying (A +  i) to (R + (i-mid))
        memmove((L + i),(A + i),sizeof(struct database)); 
    }
    
	for(i = mid;i<k;i++){
		//  R[i-mid] = A[i]; // creating right subarray
        // copying (A +  i) to (R + (i-mid))
        memmove((R + (i-mid)),(A + i),sizeof(struct database)); 
    }
    
    merge_sort(L,mid);  // sorting the left subarray
	merge_sort(R,k-mid);  // sorting the right subarray
    merge(A,L,mid,R,k-mid);  // Merging L and R into A as sorted list.
    free(L);
    free(R);
    
}// end merge_sort



// binary search function
int binary_search(struct database *data,char *key,int low,int high,int by_surname){
    
    if(high < low){
        return -1;
        // not found
    }
    else{
	
		int mid = (low+high) / 2;
        
		if(by_surname == 0){
            
			char temp_key[38];
            
			// copy over surname from structure data to temp surname
			strcpy(temp_key,(data + mid)->key);
			
			// change search term to uppercase letters
			strupr(temp_key);
            
			if(strcmp(key,temp_key)<0){
				return binary_search(data,key,low,mid-1,by_surname);
			}
			else if(strcmp(key,temp_key)>0){
				return binary_search(data,key,mid+1,high,by_surname);
			}
			else{
				return (mid);
			}
            
		}
		else{
            
			char surname_temp[21];
            
			// copy over surname from structure data to temp surname
			strcpy(surname_temp,(data + mid)->surname);
			
			// change search term to uppercase letters
			strupr(surname_temp);
			
			if(strcmp(key,surname_temp)<0){
				return binary_search(data,key,low,mid-1,by_surname);
			}
			else if(strcmp(key,surname_temp)>0){
				return binary_search(data,key,mid+1,high,by_surname);
			}
			else{
				return (mid);
			}
		
		}// end inner else
		
    }// end outer else
    
}// end function