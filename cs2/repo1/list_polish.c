#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>

int debug = 1;

typedef struct cell {
        double item;
        struct cell *next;
} cell;

cell *stk;

void exit_with_msg(char *message) 
{
        fprintf(stderr, "%s", message);
        exit(1);
}

int is_value_left()
{
        if (stk->next != NULL)
                return 1;

        return 0;
}

int is_stack_empty() 
{
        if (stk == NULL)
                return 1;

        return 0;
}

void print_stack() 
{
        printf("<- スタックの中身:");
        /*cell *p = stk;
        while (p != NULL) {
                printf(" %.2lf", p->item);
                p = p->next;
        }*/
        for (cell *p = stk; p != NULL; p = p->next)
                printf(" %.2lf", p->item);

        printf("\n");
}

void push(double x) 
{
        cell *p = malloc(sizeof(cell));
        p->item = x;
        p->next = stk;
        stk = p;
}

double pop() 
{
        if (is_stack_empty()) 
                exit_with_msg("Error: 演算子に対して、被演算子が少なすぎます。\n");
        

        double x = stk->item;
        struct cell *next = stk->next;

        free(stk);

        stk = next; //一つ前の構造体
        return x;
}

double top() 
{
        return stk->item;
}

int main(int argc, char *argv[]) 
{
        char *input = argv[1];
        char curr_string[100];

        if (argc <= 1) 
                exit_with_msg("Error: コマンドライン引数を逆ポーランド記法を入力してください。\n");
        
        while (strlen(input) > 0) { 

                while (*input == ' ') input++; 
                int ret = sscanf(input, "%s", curr_string); 
                if (ret == EOF) 
                        break;
                input += strlen(curr_string);

                if (isdigit(curr_string[0])) { //数字かどうか
                        double num = atof(curr_string);
                        if (debug) 
                                printf("<- %.2lf (数値)\n", num);
                        push(num);
                }

                else {
                        double num1, num2, ans;
                        if (strncmp(curr_string, "+", 1) == 0) {
                                if (debug) 
                                        printf("<- '+' (演算子)\n");
                                num1 = pop();
                                num2 = pop();
                                ans = num2 + num1;
                                push(ans);
                        }

                        else if (strncmp(curr_string, "-", 1) == 0) {
                                if (debug) 
                                        printf("<- '-' (演算子)\n");
                                num1 = pop();
                                num2 = pop();
                                ans = num2 - num1;
                                push(ans);
                        }

                        else if (strncmp(curr_string, "/", 1) == 0) {
                                if (debug) printf("<- '/' (演算子)\n");
                                num1 = pop();
                                num2 = pop();
                                ans = num2 / num1;
                                push(ans);
                        }

                        else if (strncmp(curr_string, "*", 1) == 0) {
                                if (debug) 
                                        printf("<- '*' (演算子)\n");
                                num1 = pop();
                                num2 = pop();
                                ans = num2 * num1;
                                push(ans);
                        }

                        else if (strncmp(curr_string, "sqrt", 4) == 0) {
                                if (debug) 
                                        printf("<- 'sqrt' (演算子)\n");
                                num1 = pop();
                                ans = sqrt(num1);
                                push(ans);
                        }

                        else if (strncmp(curr_string, "pow", 3) == 0) {
                                if (debug) 
                                        printf("<- 'pow' (演算子)\n");
                                num1 = pop();
                                num2 = pop();
                                ans = pow(num2, num1);
                                push(ans);
                        }

                        else {
                                if (debug) 
                                        printf("<- '%s' (演算子)\n", curr_string);
                                exit_with_msg("Error: 無効な演算子が入力されました。\n");
                        }
                }
                if (debug) 
                        print_stack();
       }

        if (is_value_left()) 
                exit_with_msg("Error: 演算子に対して被演算子が多すぎます。\n");
        
        printf("答え %.2lf\n", top());
        
        return 0;
}
