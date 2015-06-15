%{
#pragma warning(disable: 4068 4102)
#include "patchver.h"
#include "BuildNr.h"
#include "Product.h"
%}
%%

##VERSION##        { fputs(_MK_STR(PROD_VER_MAJOR)","_MK_STR(PROD_VER_MINOR)","_MK_STR(PROD_VER_SERVP)","_MK_STR(PROD_VER_BUILD), yyout); }
##FULL_VERSION##   { fputs(PROD_FULL_VERSION_STR, yyout); }
##SHORT_VERSION##  { fputs(PROD_SHORT_VERSION_STR, yyout); }
##PRODUCTNAME##    { fputs(PROD_PRODUCTNAME, yyout); }
##COMPANYNAME##    { fputs(PROD_COMPANYNAME, yyout); }
##COPYRIGHT##      { fputs(PROD_COPYRIGHT_SHORT, yyout); }
.                  { fputs(yytext, yyout); }

%%

int main(int argc, char **argv)
{
  if (argc!=3)
    {
      fprintf(stderr,"error: invalid arguments\n");
      fprintf(stderr,"usage: patchver [input] [output]\n");
      return 1;
    }
  FILE *infile = fopen(argv[1], "r");
  if (infile == NULL)
    {
      fprintf(stderr,"error: cannot open '%s'\n", argv[1]);
      return 1;
    }
  FILE *outfile = fopen(argv[2], "w");
  if (outfile == NULL)
    {
      fprintf(stderr,"error: cannot create '%s'\n", argv[2]);
      return 1;
    }

  printf("patchver: version=%s\n",PROD_FULL_VERSION_STR);

  yy_scan scanner;
  scanner.setinput(infile);
  scanner.setoutput(outfile);
  scanner.yylex();
  scanner.setinput(NULL);
  scanner.setoutput(NULL);
  fclose(infile);
  fclose(outfile);
  return 0;
}
