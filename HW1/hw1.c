#include <stdio.h>

/*
 *Get input from stdin using while loop

 print remainding char in last line after the while loop
 * */

int main(){
        int c;
        // initialize for 80 char lines and 1 for null buffers
        char str[5];
        int count = 0;

        while (c != EOF ){
                //print line when array is full
                if(count == 80){
                        str[80] = '\0';
                        printf("%s\n", str );
                        count = 0;
                }

                c = getchar();
                if(c == '\n'){
                        str[count] = ' ';
                        count++;
                }
                else if(c == '*'){
                        //replace ** to ^
                        int temp = getchar();
                        if(temp == '*'){
                                str[count] = '^';
                                count++;

                        }
                        else if(temp == '\n'){
                                str[count] = c;
                                count++;
                                //edge case check
                                if(count == 80){
                                        str[80] = '\0';
                                        printf("%s\n", str );
                                        count = 0;
                                }
                                str[count] = ' ';
                                count++;


                        }
                        else{
                                str[count] = c;
                                count++;
                                //edge case check
                                if(count == 80){
                                        str[80] = '\0';
                                        printf("%s\n", str );
                                        count = 0;
                                }
                                str[count] = temp;
                                count++;
                        }
                }
                else{
                        str[count] = c;
                        count++;
                }


        }


        return 0;
}
