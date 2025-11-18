#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "converter.h"

// Função para escapar caracteres especiais HTML
static char* html_escape(const char* s) {
    size_t len = 0;
    for (const char *p = s; *p; p++) {
        switch (*p) {
            case '&': len += 5; break; // &amp;
            case '<': len += 4; break; // &lt;
            case '>': len += 4; break; // &gt;
            case '\"': len += 6; break; // &quot;
            case '\'': len += 5; break; // &#39;
            default: len += 1; break;
        }
    }
    char *out = malloc(len + 1);
    if (!out) exit(1);
    char *q = out;
    for (const char *p = s; *p; p++) {
        switch (*p) {
            case '&': memcpy(q, "&amp;", 5); q+=5; break;
            case '<': memcpy(q, "&lt;", 4); q+=4; break;
            case '>': memcpy(q, "&gt;", 4); q+=4; break;
            case '\"': memcpy(q, "&quot;", 6); q+=6; break;
            case '\'': memcpy(q, "&#39;", 5); q+=5; break;
            default: *q++ = *p; break;
        }
    }
    *q = 0;
    return out;
}

void converter_csv_para_html(const char *csv_path, const char *html_path) {
    FILE *fin = fopen(csv_path, "r");
    if (!fin) {
        fprintf(stderr, "Erro: não foi possível abrir CSV: %s\n", csv_path);
        return;
    }

    FILE *fout = fopen(html_path, "w");
    if (!fout) {
        fprintf(stderr, "Erro: não foi possível criar HTML: %s\n", html_path);
        fclose(fin);
        return;
    }

    fprintf(fout,
        "<!DOCTYPE html>\n"
        "<html lang=\"pt-br\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
        "  <title>CSV para HTML</title>\n"
        "  <link rel=\"stylesheet\" href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.2/dist/css/bootstrap.min.css\">\n"
        "</head>\n<body>\n"
        "<div class=\"container mt-4\">\n"
        "<h2 class=\"mb-3\">Tabela gerada a partir do CSV</h2>\n"
        "<table class=\"table table-striped table-bordered\">\n<tbody>\n");

    char linha[1024];
    while (fgets(linha, sizeof(linha), fin)) {
        linha[strcspn(linha, "\r\n")] = 0; // remove CRLF/ LF
        fprintf(fout, "<tr>");
        char *token = strtok(linha, ",");
        while (token) {
            char *esc = html_escape(token);
            fprintf(fout, "<td>%s</td>", esc);
            free(esc);
            token = strtok(NULL, ",");
        }
        fprintf(fout, "</tr>\n");
    }

    fprintf(fout, "</tbody>\n</table>\n</div>\n</body>\n</html>\n");

    fclose(fin);
    fclose(fout);

    printf("HTML gerado com sucesso: %s\n", html_path);
}
