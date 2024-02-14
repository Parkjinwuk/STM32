int main()
{
    printf("Main - Start\n");
    func1();
    printf("Main - End\n");
    return 0;
}
void func1()
{
    printf("func1 - Start\n");
    func2();
    printf("func1 - End\n");
}
void func2()
{
    printf("func2 - Start\n");
    func3();
    printf("func2 - End\n");
}

void func3()
{
    printf("func3 - Start\n");
    func4();
    printf("func3 - End\n");
}
void func4()
{
    printf("func4 - Start\n");
    printf("func4 - End\n");
}

