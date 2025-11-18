#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "converter.h"

int main() {
    char csv[512];
    char html[512];

    printf("Digite o caminho do arquivo CSV (ex: dados.csv ou pasta/dados.csv): ");
    fgets(csv, sizeof(csv), stdin);
    csv[strcspn(csv, "\n")] = 0;  // Remove newline

    printf("Digite o caminho do arquivo HTML de saída (ex: tabela.html ou pasta/tabela.html): ");
    fgets(html, sizeof(html), stdin);
    html[strcspn(html, "\n")] = 0; // Remove newline

    converter_csv_para_html(csv, html);

#ifdef _WIN32
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "start \"\" \"%s\"", html);
    system(cmd);
#else
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "xdg-open \"%s\" >/dev/null 2>&1 &", html);
    system(cmd);
#endif

    return 0;
}
