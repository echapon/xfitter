#ifndef pdf2yaml_h
#define pdf2yaml_h

#include <yaml.h>
#include "c2yaml.h"
#include "list.h"
#include <assert.h>

// Info
typedef struct info_node_s {
        enum {STRING, DARRAY} node_type;
        char* key;
        union {
                char* string;
                struct {
                        double *vals;
                        double size;
                } darray;
        } value;
} Info_Node;

typedef List Info;

void info_free(Info *info);
Info *info_add_node_str(Info *info, char* key, char *value);
Info *info_add_node_darray(Info *info, char* key, double *darray, int size);
void info_save(const Info *info, FILE *output);
Info *info_dup(const Info *info);
int info_cmp(Info *info1, Info *info2);

Info_Node *info_node_where(Info *info, char *key);
int info_node_update_str(Info_Node *node, const char *new_value);
Info_Node *info_node_dup(Info_Node *node);
int info_node_cmp(Info_Node *node1, Info_Node *node2);
Info *info_load(FILE *input);

// Pdf member
typedef struct Pdf_s {
Info *info;

int nx;
double *x;

int nq;
double *q;

int n_pdf_flavours;
int *pdf_flavours;

double ***val; /* over x, q, pdf_flavour */
} Pdf;

typedef struct PdfSet_s {
        Info *info;
        int n_members;
        Pdf *members;
} PdfSet;

void pdf_initialize(Pdf *pdf, int nx, int nq, int n_pdf_flavours, Info *info);
int load_lhapdf6_member(Pdf *pdf, char *path);
int save_lhapdf6_member(Pdf *pdf, char *path);
Pdf *pdf_dup(Pdf *pdf);
int pdf_cpy(Pdf *dest, const Pdf *src);
int pdf_cmp(Pdf *pdf1, Pdf *pdf2);
void pdf_free(Pdf *pdf);

int load_lhapdf6_set(PdfSet *pdf_set, char *path);
int save_lhapdf6_set(PdfSet *pdf_set, char *path);
void pdf_set_initialize(PdfSet *pdf_set, int n_members, int nx, int nq, int n_pdf_flavours, Info *info, Info *minfo);
PdfSet *pdf_set_dup(PdfSet *pdf_set);
int pdf_set_cmp(PdfSet *pdf_set1, PdfSet *pdf_set2);
void pdf_set_free(PdfSet *pdf_set);

//utility function for int/double convertion to char*
char *n2str(char *s, double n );
#endif
