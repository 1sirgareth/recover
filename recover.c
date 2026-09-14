#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover name!\n");
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    int count = 0;

    BYTE b[512];
    b[0] = 0;
    b[1] = 0;
    b[2] = 0;
    b[3] = 0;
    while (!(b[0] == 0xff && b[1] == 0xd8 && b[2] == 0xff && (b[3] >= 0xe0 && b[3] <= 0xef)))
    {
        fread(b, sizeof(BYTE), 512, file);
    }

    char filename[8];
    int image_count = 0;
    sprintf(filename, "%03i.jpg", image_count);
    FILE *current = fopen(filename, "a");
    fwrite(b, sizeof(BYTE), 512, current);

    while (fread(b, sizeof(BYTE), 512, file) == 512)
    {
        if (b[0] == 0xff && b[1] == 0xd8 && b[2] == 0xff && (b[3] >= 0xe0 && b[3] <= 0xef))
        {
            fclose(current);
            image_count++;
            sprintf(filename, "%03i.jpg", image_count);
            current = fopen(filename, "a");
        }
        fwrite(b, sizeof(BYTE), 512, current);
    }

    fclose(current);
    fclose(file);
}
