#include "facile.h"

GHashTable *table;
char *class_name;
char *file_name_input;
FILE *stream;

void declare_var(char *key, long value)
{
	fprintf(stream, "\t.var %ld is %s J from BEGIN to END\n", (value - 1) * 2, key);
}

void begin_code()
{
	fprintf(stream, ".source\t%s\n", file_name_input);
	fprintf(stream, ".class\t%s\n", class_name);
	fprintf(stream, ".super\tjava/lang/Object\n\n");
	fprintf(stream, ".method\t<init>()V\n");
	fprintf(stream, "\t.limit stack\t1\n");
	fprintf(stream, "\t.limit locals\t1\n");
	fprintf(stream, "\t.line\t1\n");
	fprintf(stream, "\taload_0\n");
	fprintf(stream, "\tinvokespecial\tjava/lang/Object/<init>()V\n");
	fprintf(stream, "\treturn\n");
	fprintf(stream, ".end method\n\n");
	fprintf(stream, ".method\tpublic static main([Ljava/lang/String;)V\n");
	fprintf(stream, "\t.limit stack\t100\n");
	fprintf(stream, "\t.limit locals\t%ld\n", 2 * g_hash_table_size(table));
	g_hash_table_foreach(table, (GHFunc) declare_var, NULL);
	fprintf(stream, "BEGIN:\n");
}

void end_code()
{
	fprintf(stream, "END:\n");
	fprintf(stream, "\treturn\n");
	fprintf(stream, ".end method\n");
}

int nbif=0;
int nbelif=0;
int nband=0;
int nbor=0;
int nbwhile=0;
int nbforeach=0;
int nbvalforeach=0;

void produce_code(GNode * node)
{
	if (node) {
		switch ((long)node->data) {
		case TYPE_SEQUENCE:
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\t.line\t%ld\n", (long)(g_node_nth_child(node, 1)->data));
			produce_code(g_node_nth_child(node, 2));
			break;
		case TYPE_AFFECTATION:
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlstore\t%ld\n", 2 * ((long)g_node_nth_child(g_node_nth_child(node, 0), 0)->data - 1));
			break;
		case TYPE_ADD:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tladd\n");
			break;
		case TYPE_SUB:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlsub\n");
			break;
		case TYPE_MUL:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlmul\n");
			break;
		case TYPE_DIV:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tldiv\n");
			break;
		case TYPE_MOD:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlrem\n");
			break;
		case TYPE_NUMBER:
			fprintf(stream, "\tldc2_w\t%ld\n", (long)g_node_nth_child(node, 0)->data);
			break;
		case TYPE_IDENT:
			fprintf(stream, "\tlload\t%d\n", 2 * ((long)g_node_nth_child(node, 0)->data - 1));
			break;
		case TYPE_PRINT:
			fprintf(stream, "\tgetstatic\tjava/lang/System/out Ljava/io/PrintStream;\n");
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\tinvokevirtual\tjava/io/PrintStream/println(J)V\n");
			break;
		case TYPE_READ:
			fprintf(stream, "\tnew\tjava/util/Scanner\n");
			fprintf(stream, "\tdup\n");
			fprintf(stream, "\tgetstatic\tjava/lang/System.in Ljava/io/InputStream;\n");
			fprintf(stream, "\tinvokespecial\tjava/util/Scanner/<init>(Ljava/io/InputStream;)V\n");
			fprintf(stream, "\tinvokevirtual\tjava/util/Scanner/nextInt()I\n");
			fprintf(stream, "\ti2l\n");
			fprintf(stream, "\tlstore\t%ld\n", 2 * ((long)g_node_nth_child(g_node_nth_child(node, 0), 1)->data - 1));
			break;
		case TYPE_IF:
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\tifne\tendif%d\n",nbif);
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tgoto end%d\n",nbif);
			fprintf(stream, "endif%d:\n",nbif);
			produce_code(g_node_nth_child(node, 2));
			produce_code(g_node_nth_child(node, 3));
			fprintf(stream, "end%d:\n",nbif);
			nbif++;
			break;
		case TYPE_ELSEIF:
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\tifne\tendelif%d%d\n",nbif,nbelif);
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tgoto end%d\n",nbif);
			fprintf(stream, "endelif%d%d:\n",nbif,nbelif++);
			produce_code(g_node_nth_child(node, 2));
			nbelif=0;
			break;
		case TYPE_ELSE:
			produce_code(g_node_nth_child(node, 0));
			break;
		case TYPE_EGAL:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlcmp\n");
			break;
		case TYPE_INF:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlcmp\n");
			fprintf(stream, "\tldc_w 1\n");
			fprintf(stream, "\tiadd\n");
			break;
		case TYPE_INF_EGAL:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlsub\n");
			fprintf(stream, "\tldc2_w 1\n");
			fprintf(stream, "\tlcmp\n");
			fprintf(stream, "\tldc_w 1\n");
			fprintf(stream, "\tiadd\n");
			break;
		case TYPE_SUP:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlcmp\n");
			fprintf(stream, "\tldc_w 1\n");
			fprintf(stream, "\tisub\n");
			break;
		case TYPE_SUP_EGAL:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlsub\n");
			fprintf(stream, "\tldc2_w -1\n");
			fprintf(stream, "\tlcmp\n");
			fprintf(stream, "\tldc_w 1\n");
			fprintf(stream, "\tisub\n");
			break;
		case TYPE_NOT:
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\tdup\n");
			fprintf(stream, "\timul\n");
			fprintf(stream, "\ti2l\n");
			fprintf(stream, "\tldc2_w 1\n");
			fprintf(stream, "\tlcmp\n");
			break;
		case TYPE_AND:
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\tdup\n");
			fprintf(stream, "\tifne\tand%d\n",nband);
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tiadd\n");
			fprintf(stream, "and%d:\n",nband);
			nband++;
			break;
		case TYPE_OR:
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\tdup\n");
			fprintf(stream, "\tifeq\tor%d\n",nbor);
			fprintf(stream, "\tldc_w 0\n");
			fprintf(stream, "\timul\n");
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tiadd\n");
			fprintf(stream, "or%d:\n",nbor);
			nbor++;
			break;
		case TYPE_FALSE:
			fprintf(stream, "\tldc_w\t1\n");
			break;
		case TYPE_TRUE:
			fprintf(stream, "\tldc_w\t0\n");
			break;
		case TYPE_DIFF:
			produce_code(g_node_nth_child(node, 0));
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tlcmp\n");
			fprintf(stream, "\tdup\n");
			fprintf(stream, "\timul\n");
			fprintf(stream, "\tldc_w 1\n");
			fprintf(stream, "\tisub\n");
			break;
		case TYPE_WHILE:
			fprintf(stream, "while%d:\n",nbwhile);
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\tifne\tendwhile%d\n",nbwhile);
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\tgoto while%d\n",nbwhile);
			fprintf(stream, "endwhile%d:\n",nbwhile);
			nbwhile++;
			break;
		case TYPE_FOREACH:
			fprintf(stream, "\tgoto\tinit_foreach%d\n",nbforeach);
			fprintf(stream, "init_tab_foreach%d:\n",nbforeach);
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\taload_0\n");
			fprintf(stream, "\tiload_1\n");
			fprintf(stream, "\tgoto\tforeach%d\n",nbforeach);
			fprintf(stream, "init_foreach%d:\n",nbforeach);
			fprintf(stream, "\tldc_w\t%d\n",nbvalforeach);
			fprintf(stream, "\tistore_1\n");
			fprintf(stream, "\tiload_1\n");
			fprintf(stream, "\tnewarray\tlong\n");
			fprintf(stream, "\tastore_0\n");
			fprintf(stream, "\tgoto init_tab_foreach%d\n",nbforeach);
			fprintf(stream, "foreach%d:\n",nbforeach);
			fprintf(stream, "\ticonst_1\n");
			fprintf(stream, "\tisub\n");
			fprintf(stream, "\tswap\n");
			fprintf(stream, "\tdup_x1\n");
			fprintf(stream, "\tpop\n");
			fprintf(stream, "\tdup2\n");
			fprintf(stream, "\tlaload\n");	
			fprintf(stream, "\tlstore\t%ld\n", 2 * ((long)g_node_nth_child(g_node_nth_child(node, 0), 0)->data - 1));
			produce_code(g_node_nth_child(node, 2));
			fprintf(stream, "\tdup\n");
			fprintf(stream, "\tifne foreach%d\n",nbforeach);
			nbforeach++;
			nbvalforeach=0;
			break;
		case TYPE_TAB:
			produce_code(g_node_nth_child(node, 1));
			fprintf(stream, "\taload_0\n");
			fprintf(stream, "\tldc_w\t%d\n",nbvalforeach++);
			produce_code(g_node_nth_child(node, 0));
			fprintf(stream, "\tlastore\n");
			break;
		}
	}
}

int main(int argc, char **argv)
{
	char *file_name_output;
	table = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
	file_name_input = argv[1];
	stdin = fopen(file_name_input, "r");
	class_name = strdup(argv[1]);
	*rindex(class_name, '.') = '\0';
	file_name_output = strdup(argv[1]);
	strcpy(rindex(file_name_output, '.'), ".j");
	stream = fopen(file_name_output, "w");
	yyparse();
	fclose(stream);
	fclose(stdin);
	g_hash_table_destroy(table);
	free(file_name_output);
	free(class_name);
	return 0;
}

