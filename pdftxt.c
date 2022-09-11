/*
 * pdftxt - a mupdf-based pdf text extractor
 *
 * Copyright (C) 2012-2016 Ali Gholami Rudi
 *
 * This program is released under the Modified BSD license.
 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "mupdf/fitz.h"

static void extract_page(fz_stext_page *pg, int pnum)
{
	fz_stext_block *blk;
	fz_stext_line *ln;
	fz_stext_char *ch;
	char utf[16];
	int j;
	for (blk = pg->first_block; blk; blk = blk->next) {
		if (blk->type != FZ_STEXT_BLOCK_TEXT)
			continue;
		for (ln = blk->u.t.first_line; ln; ln = ln->next) {
			printf("%d: ", pnum + 1);
			if (ln != blk->u.t.first_line)
				printf(" ");
			for (ch = ln->first_char; ch; ch = ch->next) {
				int n = fz_runetochar(utf, ch->c);
				for (j = 0; j < n; j++)
					printf("%c", (unsigned char) utf[j]);
			}
			printf("\n");
		}
	}
}

static void extract(char *path)
{
	fz_page *page;
	fz_context *ctx;
	fz_document *pdf;
	fz_stext_page *text;
	int i;
	ctx = fz_new_context(NULL, NULL, FZ_STORE_DEFAULT);
	fz_register_document_handlers(ctx);
	pdf = fz_open_document(ctx, path);
	if (!pdf)
		return;
	for (i = 0; i < fz_count_pages(ctx, pdf); i++) {
		if (!(page = fz_load_page(ctx, pdf, i)))
			continue;
		text = fz_new_stext_page_from_page(ctx, page, NULL);
		extract_page(text, i);
		fz_drop_stext_page(ctx, text);
		fz_drop_page(ctx, page);
	}
	fz_drop_document(ctx, pdf);
	fz_drop_context(ctx);
}

int main(int argc, char *argv[])
{
	int i;
	if (argc < 2) {
		printf("usage: %s files\n", argv[0]);
		return 1;
	}
	for (i = 1; i < argc; i++)
		extract(argv[i]);
	return 0;
}
