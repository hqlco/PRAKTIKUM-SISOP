#include <stdio.h>
#include <stdlib.h>
int fileExists(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file)
    {
        fclose(file);
        return 1;
    }
    return 0;
}
int main()
{
    if (!fileExists("fifa-player-stats-database.zip"))
    {
        system("kaggle datasets download -d bryanb/fifa-player-stats-database");
    }
    if (!fileExists("FIFA23_official_data.csv"))
    {
        system("unzip fifa-player-stats-database");
    }
    system("awk -F',' 'BEGIN {OFS=\",\"; count=1} $3 < 25 && $8 > 85 && $9 != \"Manchester City\" { print count++, $2\"\\n\" $3\"\\n\" $9\"\\n\" $5\"\\n\" $8\"\\n\" $4\"\\n\"}' FIFA23_official_data.csv");
    return 0;
}
