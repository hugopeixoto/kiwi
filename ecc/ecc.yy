%x IN_CPP CPP_COMMENT CPP_STRING
%{
void printHtml (const char*);
void printCpp (const char*);
void printCoder ();
void printEndCoder (int v);
int i = 0;
%}
%%

<INITIAL>{
"<%="   i = 1; printCoder(); BEGIN(IN_CPP);
"<%"    BEGIN(IN_CPP);
[^<]+   printHtml(yytext);
"<"     printHtml(yytext);
}

<CPP_COMMENT>{
"*/"        BEGIN(IN_CPP);
[^*]+
"*"
}

<CPP_STRING>{
"\""        printCpp(yytext); BEGIN(IN_CPP);
\\.         printCpp(yytext);
[^\\"]+     printCpp(yytext);
}

<IN_CPP>{
"%>"        printEndCoder(i); i = 0; BEGIN(INITIAL);
"\""        printCpp(yytext); BEGIN(CPP_STRING);
"/*"        BEGIN(CPP_COMMENT);
[^%"/]+     printCpp(yytext);
"%"         printCpp(yytext);
}

%%

void print_character(char c) {
  if (c == '"' || c == '\\') printf("\\%c", c);
  else if (c == '\n') printf("%s", "\\n");
  else printf("%c", c);
}

void printHtml(const char * str) {
  printf("(*output_buffer) << \"");
  while (*str) print_character(*str++);
  printf("\";\n");
}

void printCoder () { printf("(*output_buffer) << "); }
void printEndCoder (int i) { if (i) printf(";"); }
void printCpp(const char* str) { printf("%s", str); }

int main(int argc, char * argv[])
{
  printf("%s\n", argv[2]);
  printf("void %s () {\n", argv[1]);
  yylex();
  printf("}\n");
  return 0;
}

