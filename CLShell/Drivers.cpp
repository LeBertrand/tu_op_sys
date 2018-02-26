//
// Created by Shmuel Jacobs on 2/25/2018.
//

//#include "main.cpp"

int main()
{
    const char* typed = "cd -lr --force -v ASM.s > log.txt";

    tokenize_input();

    int i;
    for(i = 0; i < 10; i++){
        puts(tokens[i]);
    }
    return 0;
}
