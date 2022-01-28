

/***************************************************
 * password.c
 * Author: Bailey Hirota
 * Date: 1/22/2022
 * Generate password given input
 */

#include <stdio.h>
void replace(char*, char, char);

int main() {
    /// declaring string
    char str[50];

    // reading string
    printf("Enter a word: ");
    scanf("%s",str);

    // print string
    printf("Old Password: %s\n",str);
	
    replace(str, 'e', '3');
    replace (str, 'l', '1');
    replace(str, 'a', '@');

    // print string
    printf("New Password: %s\n",str);

    return 0;
}

/*
 * Replaces char in string
 * @param str string
 * @param old char to be replaced
 * @param new char to be changed to
 */
void replace (char* str, char old, char new) {
    for (int i  = 0; str[i] != 0; i++){
	    if (str[i] == old) {
		    str[i] = new;
	    }
    }
}
