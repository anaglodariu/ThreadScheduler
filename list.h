typedef struct celula {
	void *info;
	struct celula *urm;
} Celula, *Lista;

Lista aloc_cell(void *info);
int insert_after_priority(Lista *L, void *inof);
void destroy(Lista *L);
void print(Lista L);
