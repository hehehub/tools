int factorial(int n)
{
    if (n == 0 || n == 1)
        return 1;
    else
    {
        return n * factorial(n - 1);
    }
}

int cantor(int *a)
{
    int temp[8] = {0};
    int index = 0;
    for (int i = 0; i < 8; i++)
    {
        int count = 0;
        for (int j = 1; j < a[i]; j++)
        {
            if (temp[j - 1] == 0)
            {
                count++;
            }
        }
        temp[a[i] - 1] = 1;
        index += count * factorial(7 - i);
    }
    return index;
}

int *decantor(int n)
{
    int a[10] = {0};
    int assist[10] = {0};
    int counts;
    for (int i = 7; i >= 0; i--)
    {
        int temp = factorial(i);
        counts = n / temp;
        n %= temp;
        int count = 0;
        for (int j = 1; j < 9; j++)
        {
            if (assist[j] == 0)
            {
                if (count == counts)
                {
                    a[7 - i] = j;
                    assist[j] = -1;
                    break;
                }
                count++;
            }
        }
    }
    return a;
}