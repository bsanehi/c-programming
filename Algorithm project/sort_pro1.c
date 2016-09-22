/*
    Author: Behzad Sanehi
    
    Program to sort and merge 3 lists into 1
    
    K  =  used for number of students
	
    flag_it == 1 will be all students flag    ||   flag_it == 2 ~ dit flag    ||   flag_it == 3 ~ itt flag   ||     flag_it == 4 ~ itb flag    ||    flag_it == 5 ~ wrong input 

*/
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include "menu.h"


main(){
    
    FILE *fp,*main,*ptr_binary; // file pointers
    
    int ret; // for removing files
    
    char search[21];  // college option
    
    char filename[15]; // variable to store file name
	
    // declare structure array 
	struct database *data = (struct database*)malloc((RECORDS)*sizeof(struct database));
    
    int flag_file = 0; // failed to open a file flag
    
    bool found = false; // student found flag
    
    int flag_it = 0;  /* flag_it == 1 will be all students flag    ||   flag_it == 2 ~ dit flag    ||   flag_it == 3 ~ itt flag   ||     flag_it == 4 ~ itb flag    ||    flag_it == 5 ~ wrong input */
    
    // loop counters
    int K = 0; // used for number of students
    int i;
    
    // temp data
    char temp_key[38];
    char temp_surname[21];
    char temp_firstname[16];
    
    // switch option
    int option = 0;
	
    // i is used to go through the different files
    for(register int i=1; i<(NO_FILES+1); i++){
        
        // name file to open
        sprintf(filename,"database/database_%d.txt",i);
        
        // open and save file to database  // call function to open_file_save 
        flag_file = flag_file + (open_file_save(fp,data,&K,filename));
        
    }// end for loop
    
    if(K!=0){
        
        // deallocate wasted memory
        data = (struct database *)realloc(data,K*sizeof(struct database));
        
        // using merge sort to sort the students names, K = number of students
        merge_sort(data,K);
        
    }else{
        //allocate memory for just 1 element 
        data = (struct database *)realloc(data,sizeof(struct database));
    }
    
    // check if successful
    if(data == NULL){
        text_n_color("Failed to reallocate memory",12);
    }
    
    
    // do while
    do
    {
        
        // re-initializing variables back to default
        option = 0;
        found = false;
        flag_it = 0; 
		
        // call program menu and send option
        program_menu(&option);
        
        // switch for the option the user selects
        switch(option){
            
            // case for creating new sorted file  //  PART 1
            case 1:{
                
                // clear screen
                system("cls");
				
                // opening new file if everything went smoothly  // and checking if there is a database 
                if( (main=fopen("new_list.txt", "w"))!=NULL && (flag_file != NO_FILES || K!=0)){
                    
                    // go through the whole list of students
                    for(register int x=0; x<K ; x++){
                        
                        // check if x = number of students - 1, if so print this cause its the last student
						if(x==(K-1)){
                            
							// printing to new file
							fprintf(main,"ID: tud%d  COLLEGE: %s  SURNAME: %-7s \tFIRSTNAME: %s ",
							x+1100,(data + x)->college, (data + x)->surname, (data + x)->firstname);
						}
						else{
                            
							// printing to new file
							fprintf(main,"ID: tud%d  COLLEGE: %s  SURNAME: %-7s \tFIRSTNAME: %s \n",
							x+1100,(data + x)->college, (data + x)->surname, (data + x)->firstname);
						}
						
                    }// end for loop
                    
                    // closing main file
                    fclose(main);
                    
                    printf("\nSorted list called \"new_list.txt\" and \"new_list.bin\" was successfully created.\n\n");
                    
                }// end if
                else{
                    
					// closing main file
                    fclose(main);
                    
                    // removing file
                    ret = remove("new_list.txt");
					
                    text_n_color("Failed to open new file.",10);
                }
                
                // creating sorted list in binary format  // and checking if there is a database to work with
                if( (ptr_binary=fopen("new_list.bin","wb")) !=NULL && (flag_file != NO_FILES || K!=0)){
                    
                    for(i=0; i<K; i++){
                        
                        // writing to binary file
                        fwrite(&data[i],sizeof(struct database),1,ptr_binary);
                    }
                    
                    // close file
                    fclose(ptr_binary);
                    
                }// end if
                else{
                    
                    // close file
                    fclose(ptr_binary);
                    
                    // removing file
                    ret = remove("new_list.bin");
                }
                
                // break out of case 1
                break;
                
            }// end case 1
            
            // case for searching students from college  //  PART 2
            case 2:{
                
                // clear screen
                system("cls");
                
                // flag to check if anyone was found  F_N_S  = found number of students
                int F_N_S = 0;
                
                printf("\n\nEnter college, dit itb itt or all: ");
                
                // scan in search term
                scanf("%s",search);
                
                // change search term to lowercase letters
                strlwr(search);
                
                /* flag_it == 1 will be all students flag    ||   flag_it == 2 dit flag    ||   flag_it == 3 itt flag   ||     flag_it == 4 itb flag    ||    flag_it == 5 wrong input */
                if(strcmp(search,"all")==0){ 
                    flag_it = 1;
                }
                else if(strcmp(search,"dit")==0){
                    flag_it = 2;
                }
                else if(strcmp(search,"itt")==0){
                    flag_it = 3;
                }
                else if(strcmp(search,"itb")==0){
                    flag_it = 4;
                }
                else{
                    flag_it = 5;
                    text_n_color("Wrong input!",12);
                }
                
                if(flag_it != 5){
                    
                    printf("\n");
                    
                    // go through the whole list  K = students
                    for(i=0; i<K; i++){
                        
                        // compare to see if the search term matches with the college of that student or if all is selected
                        if(strcmp((data + i)->college,search)==0 || flag_it == 1){
                            
                            found = true;
                            
                            // found someone keep incrementing if more than 1 student is found
                            F_N_S++;
                            
                        }
                        else{
                            found = false;
                        }
                        
						// display table
                        display(data,i,found,flag_it);
                        
                    }// end for loop
                    
                    printf("\n");
                    printf(" Students: %d",F_N_S);
                    printf("\n\n");
                    
                }// end if
                
                // break out of case 2
                break;
                
            }// end case 2
            
            // case for searching student by surname   //  PART 3
            case 3:{
                
                // clear screen
				system("cls");
                
                // flag to check if anyone was found  F_N_S = found number of students
                int F_N_S = 0;
                int found_no = 0;
                
                printf("\nEnter surname: ");
                scanf("%s",search);
                
                printf("\n");
                
                // change search term to uppercase letters
                strupr(search);
                
                // using binary search to find near by student
                found_no = binary_search(data,search,0,K,1);
                
                if(found_no == -1){
                    found = false;
                }
                else{
                    
                    found = true;
                    
                    // checking backwards to see if anyone else was missed from using the binary search function
                    // using while loop to go backwards through the list
                    while(found_no>=0){
                        
                        // copy over surname from structure data to temp surname
                        strcpy(temp_surname,(data + found_no)->surname);
                        
                        // change search term to uppercase letters
                        strupr(temp_surname);
                        
                        if(strcmp(search,temp_surname)==0){
                            found_no--;
                        }
                        else{
                            break;
                        }
                        
                    }// end while loop
                    
                    // search through found number plus 1
                    for(i = (found_no+1); i<K;i++){
                        
                        // copy over surname from structure data to temp surname
                        strcpy(temp_surname,(data + i)->surname);
                        
                        // change search term to uppercase letters
                        strupr(temp_surname);
                        
                        if(strcmp(search,temp_surname)==0){
                            
                            // display table
                            display(data,i,found,flag_it);
                            
                            // increment one_more in order to find more
                            F_N_S++;
                        }
                        else{
                            // break if no more with same surname found
                            break;
                        }
                        
                    }// end for loop
                    
                    // display number of students found
                    printf("\n Students: %d ",F_N_S);
                    printf("\n");
                    
                }// end else
                
                printf("\n");
                
                // if the search term was not found
                if(found == false && F_N_S == 0){
                    
                    // change search term to lowercase
                    strlwr(search);
                    
                    // change first character to uppercase
                    search[0] = toupper(search[0]);
                    
                    printf(" %s was not found in the database.\n\n",search);
                    
                }// end else
                
                // break out of case 3
                break;
                
            }// end case 3
            
            // case for adding new student to database
            case 4:{
                
                printf("\nEnter student details:\n\n");
                
                // adding 1 more student
                K = K+1;
                
                // reallocate to get more data
                data = (struct database *)realloc(data,K*sizeof(struct database));
                
                // check if successful
                if(data == NULL){
                    text_n_color("Failed to reallocate memory",12);
                }
                
                printf("\nEnter firstname: ");
                scanf("%s",(data + (K-1))->firstname);
                
                // changing first letter of firstname to uppercase
                (data + (K-1))->firstname[0] = toupper((data + (K-1))->firstname[0]);
                
                printf("\nEnter surname: ");
                scanf("%s",(data + (K-1))->surname);
                
                // changing first letter of surname to uppercase
                (data + (K-1))->surname[0] = toupper((data + (K-1))->surname[0]);
                
                printf("\nEnter college: ");
                scanf("%s",(data + (K-1))->college);
                
                // error check for 3 letters long
                while(strlen((data + (K-1))->college)!= 3){
                    
                    printf("\nEnter college \'Hint 3 letters\': ");
                    scanf("%s",(data + (K-1))->college);
                }
                
                // changing college to lower case
                strlwr((data + (K-1))->college);
                
                printf("\n");
                
                // calling canonical after scan in
                canonical( (data + (K-1))->firstname,  (data + (K-1))->surname,  (data + (K-1))->key );
                
                // using insertion sort to sort the students, K = number of students
                insertion_sort(data,K);
                
                // break out of case 4
                break;
                
            }// end case 4
			
			// case for deleting student
			case 5:{
                
                // clear screen
                system("cls");
                
                printf("\nEnter surname: ");
                scanf("%s",temp_surname);
                
                // change search term to uppercase letters
                strupr(temp_surname);
                
                printf("\nEnter firstname: ");
                scanf("%s",temp_firstname);
                
                // change search term to uppercase letters
                strupr(temp_firstname);
                
                strcpy(temp_key,temp_surname);
                strcat(temp_key,".");
                strcat(temp_key,temp_firstname);
                
                int found = 0;
                
                // using binary search to find student with same key   
                int found_no = binary_search(data,temp_key,0,K,0);  // the 0,K,0 the zero at the end means searching not with surname
                
                // K = number of students
                for(i=found_no; i<K; i++){
                    
                    if(strcmp(temp_key,(data + i)->key)==0){
                        
                        found = 1;
                        
                        for(int j=i;j<K-1;j++){
                            
                            // copying (data + (j+1)) to (data + j)
                            memmove((data + j),(data + (j+1)),sizeof(struct database)); 
							
                        }// inner for loop
                        
                        // one student was removed
                        K--;
						
                        if(K!=0){
                            // deallocate wasted memory                              K = new size
                            data = (struct database *)realloc(data,K*sizeof(struct database));
                        }
                        else{
                            // deallocate wasted memory                              K = new size
                            data = (struct database *)realloc(data,sizeof(struct database));
                        }
                        
						// check if successful
						if(data == NULL){
							text_n_color("Failed to reallocate memory",12);
						}
                        
                        text_n_color("Removed from database",10);
                        
                    }// end if
                    
                }// outer for loop
                
                if(found == 0){
                    
                    text_n_color("No user found to delete",12);
                }
                
                // break out of case 5
                break;
                
			}// end case 5
            
            // case for reading in binary file
            case 6:{
                
                // clear screen
                system("cls");
                
                // vairbale for new size, in order to use realloc
                int new_size = SIZE_2;
                
                // file size
                float FILE_SIZE = 0;
				
				// found someone
				int input = 0;
                
                // declare structure array for binary file
                struct database *newdata;
                
                newdata=(struct database *)malloc(new_size*sizeof(struct database));
                
                if(newdata == NULL){
                    text_n_color("Failed to allocate memory for malloc",12);
                }
                
                // open up binary file for reading
                if( (ptr_binary=fopen("new_list.bin","rb")) !=NULL){
                    
                    // read in data as long as not empty
                    for(i=0; (fread(&newdata[i],sizeof(struct database),1,ptr_binary)>0); i++){
                        
                        // if the array is not big enough resize it
                        if(i>(new_size-SIZE_2)){
                            
                            // increase space for 2 more
                            new_size = new_size + SIZE_2;
                            
                            // reallocate memory                   // new size
                            newdata=(struct database *)realloc(newdata,new_size*sizeof(struct database));
                            
                            // check if failed to allocate memory
                            if(newdata == NULL){
                                
                                text_n_color("Failed to allocate memory for realloc",12);
                                
                            }// end inner if
                            
                        }//end if
                        
                        // display table
                        display(newdata,i,found,flag_it);
						
						// found someone
						input++;
                        
                    }// end for
                    
                    // deallocate any wasted memory
                    newdata=(struct database *)realloc(newdata,input*sizeof(struct database));
                    
                    // check if failed to deallocate memory
                    if(newdata == NULL){
                        
                        text_n_color("Failed to deallocate memory using realloc",12);
                        
                    }// end inner if
                    
                    // getting file size
                    FILE_SIZE = ftell(ptr_binary);
                    
                    // close file after use
                    fclose(ptr_binary);
                    
                    // display number of students and binary file size
                    printf("\n");
                    printf(" Students: %d ",input);
                    printf("\t");
                    printf(" Binary size: %.2f KB ",FILE_SIZE/1000);
                    printf("\n\n");
                    
                    // run sub menu options function
                    int selected = 0;
                    
                    // run sub menu and send address of selected
                    program_sub_menu_binary_file(&selected);
                    
                    // A pointer to pointer data       // http://www.tutorialspoint.com/cprogramming/c_pointer_to_pointer.htm
                    struct database **ptr = &data;
                    
                    if(selected == 1){
                        // replace database // send pointer to pointer address
                        replace_database(ptr,newdata,&K,&input);
                    }
                    else if(selected == 2){
                        // append database // send pointer to pointer address
                        append_database(ptr,newdata,&K,&input);
                    }
                    else if(selected == 4){
                        // option 4 on sub menu is exit program
                        option = 7;
                    }
					else{
						// clear screen
						system("cls");
					} 
                    
                }
                else{
                    
                    text_n_color("Couldn't open binary file.",10);
                    
                }// end else
                
                // free used up memory
                free(newdata);
                
                // break out of case 6
                break;
                
            }// end case 6
            
            // case for exiting program
            case 7:{
                
                // break out of case 7
                break;
                
            }// end case 7
            
            // the default case will catch any wrong inputs
            default:{
                
                // clear screen
                system("cls");
                
                text_n_color("Wrong input!",12);
                
            }// end default case
            
        }// end switch
        
        Sleep(500);
        
    }// end do while
    while(option != 7);
        
    // free used up memory
    free(data);
    
}// end main



// display table to user
void display(struct database *data,int i,bool found,int flag_it){
   
    //  flag_it == 1 will be all students flag    ||   flag_it == 2 dit flag    ||   flag_it == 3 itt flag   ||     flag_it == 4 itb flag    ||    flag_it == 5 wrong input
    // if student found, means the user searched for a student using surname
    if(found == true && flag_it != 1){
        
        COLOR_14;
        
        cprintf(" ID: ");
        
        cprintf("tud%d ",i+1100);
        
        cprintf(" COLLEGE: ");
        
        cprintf("%s ",(data + i)->college);
        
        cprintf(" SURNAME: ");
        
        cprintf("%-11s ",(data + i)->surname);
        
        cprintf(" FIRSTNAME: ");
        
        cprintf("%-11s",(data + i)->firstname);
        printf("\n");
        
    }
    else{
        
        COLOR_3;
        cprintf(" ID: ");
        
        COLOR_5;
        cprintf("tud%d ",i+1100);
        
        COLOR_3;
        cprintf(" COLLEGE: ");
        
        COLOR_5;
        cprintf("%s ",(data + i)->college);
        
        COLOR_3;
        cprintf(" SURNAME: ");
        
        COLOR_5;
        cprintf("%-11s ",(data + i)->surname);
        
        COLOR_3;
        cprintf(" FIRSTNAME: ");
        
        COLOR_5;
        cprintf("%s",(data + i)->firstname);
        printf("\n");
        
    }// end else

}// end function